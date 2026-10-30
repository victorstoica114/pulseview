/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "capturecollection.hpp"
#include "util.hpp"
#include <QCryptographicHash>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QSaveFile>
#include <QTemporaryDir>
#include <zip.h>
#include <memory>
#include <stdexcept>

namespace pv {
namespace {
constexpr auto Manifest = "pulseview-frames.json";
constexpr zip_uint64_t MaxManifest = 8 * 1024 * 1024;
constexpr uint64_t MaxMember = UINT64_C(128) * 1024 * 1024 * 1024;
using Zip = std::unique_ptr<zip_t, decltype(&zip_discard)>;
using Entry = std::unique_ptr<zip_file_t, decltype(&zip_fclose)>;

void require(bool condition, const char* reason)
{
	if (!condition) throw std::runtime_error(reason);
}

Zip open_zip(const QString& path, int flags)
{
	int error = 0;
	Zip zip(zip_open(path.toUtf8().constData(), flags, &error), zip_discard);
	require(bool(zip), "Cannot open capture collection ZIP.");
	return zip;
}

QString member_name(size_t index)
{
	return QString("frames/%1.sr").arg(qulonglong(index + 1), 6, 10, QChar('0'));
}

uint64_t number(const QJsonObject& obj, const char* key)
{
	const auto value = obj.value(key);
	bool ok = false;
	const auto text = value.toString();
	const auto n = text.toULongLong(&ok);
	require(value.isString() && ok && text == QString::number(n),
		"Invalid integer in capture collection manifest.");
	return n;
}

QString outcome(const QJsonObject& obj)
{
	const auto text = obj.value("status").toString();
	require(QStringList{"complete", "stopped", "error", "unknown", "disconnected", "timeout", "unstable"}.contains(text),
		"Invalid capture collection outcome.");
	return text;
}

void check_cancel(const std::atomic_bool& cancel)
{
	if (cancel.load()) throw std::runtime_error("Saving/loading cancelled.");
}
} // namespace

bool CaptureCollection::is_collection(const QString& path)
{
	if (path.endsWith(".srcollection", Qt::CaseInsensitive)) return true;
	try {
		auto zip = open_zip(path, ZIP_RDONLY);
		return zip_name_locate(zip.get(), Manifest, 0) >= 0;
	} catch (const std::exception&) { return false; }
}

CaptureCollection CaptureCollection::read(const QString& path)
{
	auto zip = open_zip(path, ZIP_RDONLY);
	zip_stat_t stat;
	require(zip_stat(zip.get(), Manifest, 0, &stat) == 0 && stat.size <= MaxManifest,
		"Missing or oversized capture collection manifest.");
	Entry file(zip_fopen(zip.get(), Manifest, 0), zip_fclose);
	require(bool(file), "Cannot read capture collection manifest.");
	QByteArray text(int(stat.size), Qt::Uninitialized);
	require(zip_fread(file.get(), text.data(), text.size()) == text.size(),
		"Truncated capture collection manifest.");
	QJsonParseError error;
	const auto doc = QJsonDocument::fromJson(text, &error);
	require(error.error == QJsonParseError::NoError && doc.isObject(),
		"Invalid capture collection JSON.");
	const auto obj = doc.object();
	require(obj.value("kind") == "pulseview-capture-collection" &&
		obj.value("version") == 1 && obj.value("inter_frame_timing") == "unknown",
		"Unsupported capture collection format.");
	require(obj.value("frames").isArray(), "Missing capture collection frames.");
	const auto items = obj.value("frames").toArray();
	require(!items.isEmpty() && items.size() <= 10000 &&
		zip_get_num_entries(zip.get(), 0) == items.size() + 1,
		"Invalid capture collection frame count.");
	CaptureCollection result;
	result.status = outcome(obj);
	result.error = obj.value("error").toString();
	for (const auto& value : items) {
		require(value.isObject(), "Invalid capture collection frame.");
		const auto item = value.toObject();
		CollectionFrame frame;
		frame.samples = number(item, "samples");
		frame.samplerate = number(item, "samplerate");
		frame.bytes = number(item, "bytes");
		frame.status = outcome(item);
		frame.error = item.value("error").toString();
		frame.sha256 = item.value("sha256").toString();
		require(frame.samples && frame.samplerate && frame.bytes && frame.bytes <= MaxMember &&
			QRegularExpression("^[0-9a-f]{64}$").match(frame.sha256).hasMatch(),
			"Invalid capture collection frame metadata.");
		require(item.value("member") == member_name(result.frames.size()),
			"Invalid capture collection member name.");
		require(item.value("triggers").isArray(), "Invalid collection trigger list.");
		const auto triggers = item.value("triggers").toArray();
		require(triggers.size() <= 10000, "Too many trigger positions.");
		for (const auto& trigger : triggers) {
			require(trigger.isString() && trigger.toString().size() <= 128 &&
				QRegularExpression("^[+-]?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?$")
				.match(trigger.toString()).hasMatch(), "Invalid collection trigger position.");
			const util::Timestamp time(trigger.toString().toStdString());
			require(time >= 0 && time <= util::Timestamp(frame.samples) / frame.samplerate,
				"Collection trigger outside frame.");
			frame.triggers.append(trigger.toString());
		}
		const auto name = member_name(result.frames.size()).toUtf8();
		require(zip_stat(zip.get(), name.constData(), 0, &stat) == 0 && stat.size == frame.bytes,
			"Missing capture frame or incorrect member size.");
		result.frames.push_back(std::move(frame));
	}
	return result;
}

void CaptureCollection::extract(const QString& path, size_t index,
	const QString& destination, const std::atomic_bool& cancel) const
{
	const auto& frame = frames.at(index);
	auto zip = open_zip(path, ZIP_RDONLY);
	const auto name = member_name(index).toUtf8();
	Entry input(zip_fopen(zip.get(), name.constData(), 0), zip_fclose);
	require(bool(input), "Cannot read capture collection frame.");
	QSaveFile output(destination);
	require(output.open(QIODevice::WriteOnly), "Cannot create temporary capture frame.");
	QCryptographicHash hash(QCryptographicHash::Sha256);
	QByteArray block(1024 * 1024, Qt::Uninitialized);
	uint64_t count = 0;
	while (true) {
		check_cancel(cancel);
		const auto n = zip_fread(input.get(), block.data(), block.size());
		require(n >= 0, "Corrupt capture collection member.");
		if (!n) break;
		count += n;
		require(count <= frame.bytes && output.write(block.constData(), n) == n,
			"Cannot extract capture collection frame.");
		hash.addData(block.constData(), n);
	}
	require(count == frame.bytes && hash.result().toHex() == frame.sha256.toLatin1(),
		"Capture collection checksum mismatch.");
	check_cancel(cancel);
	require(output.commit(), "Cannot commit temporary capture frame.");
}

void CaptureCollection::write(const QString& path, const QStringList& members,
	const std::atomic_bool& cancel, const std::function<void(int, int)>& progress) const
{
	require(!frames.empty() && frames.size() <= 10000 && int(frames.size()) == members.size(),
		"Invalid collection save request.");
	QTemporaryDir directory;
	require(directory.isValid(), "Cannot create collection staging directory.");
	const auto temporary = directory.filePath("collection.zip");
	auto zip = open_zip(temporary, ZIP_CREATE | ZIP_TRUNCATE);
	QJsonArray entries;
	for (size_t i = 0; i < frames.size(); ++i) {
		check_cancel(cancel);
		QFile file(members.at(i));
		require(file.open(QIODevice::ReadOnly) && file.size() > 0 && uint64_t(file.size()) <= MaxMember,
			"Cannot read staged capture frame.");
		QCryptographicHash hash(QCryptographicHash::Sha256);
		while (!file.atEnd()) {
			check_cancel(cancel);
			const auto block = file.read(1024 * 1024);
			require(!block.isEmpty(), "Cannot checksum staged capture frame.");
			hash.addData(block);
		}
		const auto& frame = frames[i];
		QJsonArray triggers;
		for (const auto& t : frame.triggers) triggers.append(t);
		entries.append(QJsonObject{{"member", member_name(i)},
			{"samples", QString::number(frame.samples)}, {"samplerate", QString::number(frame.samplerate)},
			{"bytes", QString::number(file.size())}, {"sha256", QString::fromLatin1(hash.result().toHex())},
			{"status", frame.status.isEmpty() ? "unknown" : frame.status}, {"error", frame.error},
			{"triggers", triggers}});
		auto source = zip_source_file(zip.get(), members.at(i).toUtf8().constData(), 0, 0);
		require(source, "Cannot stage capture frame ZIP member.");
		const auto entry = zip_file_add(zip.get(), member_name(i).toUtf8().constData(), source, ZIP_FL_ENC_UTF_8);
		if (entry < 0) { zip_source_free(source); throw std::runtime_error("Cannot add capture frame."); }
		require(zip_set_file_compression(zip.get(), entry, ZIP_CM_STORE, 0) == 0,
			"Cannot configure capture collection compression.");
		progress(int(i + 1), int(frames.size()) + 1);
	}
	const auto manifest = QJsonDocument(QJsonObject{{"kind", "pulseview-capture-collection"},
		{"version", 1}, {"inter_frame_timing", "unknown"}, {"status", status.isEmpty() ? "unknown" : status},
		{"error", error}, {"frames", entries}}).toJson();
	require(uint64_t(manifest.size()) <= MaxManifest, "Collection manifest too large.");
	auto source = zip_source_buffer(zip.get(), manifest.constData(), manifest.size(), 0);
	require(source, "Cannot stage capture collection manifest.");
	if (zip_file_add(zip.get(), Manifest, source, ZIP_FL_ENC_UTF_8) < 0) {
		zip_source_free(source); throw std::runtime_error("Cannot add collection manifest.");
	}
	// Sources and manifest must outlive zip_close; no destination is changed yet.
	check_cancel(cancel);
	require(zip_close(zip.get()) == 0, "Cannot finish capture collection ZIP.");
	zip.release();
	QFile input(temporary);
	QSaveFile output(path);
	require(input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly),
		"Cannot open capture collection destination.");
	while (!input.atEnd()) {
		check_cancel(cancel);
		const auto block = input.read(1024 * 1024);
		require(!block.isEmpty() && output.write(block) == block.size(),
			"Cannot write capture collection destination.");
	}
	check_cancel(cancel);
	require(output.commit(), "Cannot atomically replace capture collection destination.");
	progress(int(frames.size()) + 1, int(frames.size()) + 1);
}
} // namespace pv
