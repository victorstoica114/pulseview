/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef PULSEVIEW_CAPTURECOLLECTION_HPP
#define PULSEVIEW_CAPTURECOLLECTION_HPP

#include <atomic>
#include <cstdint>
#include <functional>
#include <vector>
#include <QString>
#include <QStringList>

namespace pv {

// Each member remains a standard single-frame sigrok session file. Origins
// are present only for windows derived from the same uninterrupted input.
struct CollectionFrame {
	uint64_t samples = 0, samplerate = 0, bytes = 0;
	uint64_t origin = 0;
	bool has_origin = false;
	QString status, error, sha256;
	QStringList triggers;
};

class CaptureCollection {
public:
	std::vector<CollectionFrame> frames;
	QString status, error;
	static bool is_collection(const QString& path);
	static CaptureCollection read(const QString& path);
	void extract(const QString& path, size_t frame, const QString& destination,
		const std::atomic_bool& cancel) const;
	void write(const QString& path, const QStringList& members,
		const std::atomic_bool& cancel,
		const std::function<void(int, int)>& progress) const;
};

} // namespace pv
#endif
