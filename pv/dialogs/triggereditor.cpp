/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "triggereditor.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFormLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QLocale>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <libsigrokcxx/libsigrokcxx.hpp>

namespace pv {
namespace dialogs {

namespace {
QString condition_name(int type)
{
	switch (type) {
	case SR_TRIGGER_ZERO: return TriggerEditor::tr("Low");
	case SR_TRIGGER_ONE: return TriggerEditor::tr("High");
	case SR_TRIGGER_RISING: return TriggerEditor::tr("Rising edge");
	case SR_TRIGGER_FALLING: return TriggerEditor::tr("Falling edge");
	case SR_TRIGGER_EDGE: return TriggerEditor::tr("Either edge");
	case SR_TRIGGER_OVER: return TriggerEditor::tr("Above threshold");
	case SR_TRIGGER_UNDER: return TriggerEditor::tr("Below threshold");
	default: return TriggerEditor::tr("Unsupported (%1)").arg(type);
	}
}
}

TriggerEditor::TriggerEditor(const std::shared_ptr<sigrok::Context> &context,
	const std::shared_ptr<sigrok::Device> &device,
	const std::shared_ptr<sigrok::Trigger> &initial, QWidget *parent) :
	QDialog(parent), context_(context), device_(device),
	enabled_(new QCheckBox(tr("Enable trigger"), this)),
	name_(new QLineEdit(this)), stage_list_(new QListWidget(this)),
	conditions_(new QTableWidget(this)), error_(new QLabel(this)),
	buttons_(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
	setWindowTitle(tr("Trigger sequence"));
	resize(760, 470);
	enabled_->setObjectName("triggerEnabled");
	name_->setObjectName("triggerName");
	stage_list_->setObjectName("triggerStages");
	conditions_->setObjectName("triggerConditions");
	error_->setObjectName("triggerError");
	error_->setWordWrap(true);
	buttons_->setObjectName("triggerButtons");
	if (!context_ || !device_)
		throw std::runtime_error("Select a capture device before editing its trigger.");
	if (initial) {
		const auto channels = device_->channels();
		for (const auto &stage : initial->stages())
			for (const auto &match : stage->matches())
				if (std::find(channels.begin(), channels.end(), match->channel()) == channels.end())
					throw std::runtime_error("The trigger belongs to another device. Clear or restore it before editing.");
	}
	supported_ = trigger_match_types(device_);
	hardware_ = hardware_trigger_supported(device_);
	const auto options = read_hardware_trigger(device_);
	const auto sequence = read_trigger_sequence(initial);
	enabled_->setChecked(sequence.enabled);
	name_->setText(QString::fromStdString(sequence.name));
	for (const auto &stage : sequence.stages) {
		stages_.emplace_back();
		for (const auto &match : stage)
			stages_.back().push_back({match.channel, match.type,
				std::isnan(match.value) ? QString() : QString::number(match.value, 'g',
					std::numeric_limits<float>::max_digits10)});
	}
	if (options.mode == "parallel") {
		if (options.stages.size() != stages_.size())
			throw std::runtime_error("Trigger stage qualifiers do not match the sequence.");
		for (size_t i = 0; i < stages_.size(); ++i) stages_[i].qualifier = options.stages[i];
	}
	if (options.mode == "serial") enabled_->setChecked(true);
	mode_ = new QComboBox(this);
	mode_->setObjectName("triggerMode");
	mode_->addItems({tr("Parallel sequence"), tr("Serial word")});
	mode_->setCurrentIndex(options.mode == "serial" ? 1 : 0);
	mode_->setVisible(hardware_);
	serial_panel_ = new QWidget(this);
	auto *serial_form = new QFormLayout(serial_panel_);
	serial_clock_ = new QComboBox(serial_panel_);
	serial_data_ = new QComboBox(serial_panel_);
	serial_edge_ = new QComboBox(serial_panel_);
	serial_bits_ = new QSpinBox(serial_panel_);
	serial_value_ = new QLineEdit(serial_panel_);
	serial_clock_->setObjectName("serialClock"); serial_data_->setObjectName("serialData");
	serial_edge_->setObjectName("serialEdge"); serial_bits_->setObjectName("serialBits");
	serial_value_->setObjectName("serialValue");
	for (const auto &channel : device_->channels()) {
		if (channel->type()->id() != SR_CHANNEL_LOGIC) continue;
		const QString label = QString::fromStdString(channel->name()) +
			(channel->enabled() ? QString() : tr(" (disabled)"));
		serial_clock_->addItem(label, channel->index());
		serial_data_->addItem(label, channel->index());
	}
	serial_clock_->setCurrentIndex(serial_clock_->findData(options.clock));
	serial_data_->setCurrentIndex(serial_data_->findData(options.data));
	serial_edge_->addItems({tr("Rising edge"), tr("Falling edge")});
	serial_edge_->setCurrentIndex(options.falling ? 1 : 0);
	serial_bits_->setRange(1, 16); serial_bits_->setValue(options.bits);
	serial_value_->setText(QString("0x%1").arg(options.value, 0, 16));
	serial_value_->setToolTip(tr("Unsigned decimal or hexadecimal (0x). The value must fit the selected width."));
	serial_form->addRow(tr("Clock channel"), serial_clock_);
	serial_form->addRow(tr("Data channel"), serial_data_);
	serial_form->addRow(tr("Sample on"), serial_edge_);
	serial_form->addRow(tr("Word width (bits)"), serial_bits_);
	serial_form->addRow(tr("Word value"), serial_value_);
	auto *serial_help = new QLabel(tr("Match the most recent sampled bits, MSB first. "
		"This compares a clocked word; it does not decode a protocol or use chip select. Buffer mode is required."), serial_panel_);
	serial_help->setWordWrap(true); serial_form->addRow(serial_help);
	conditions_->setColumnCount(3);
	conditions_->setHorizontalHeaderLabels({tr("Channel"), tr("Condition"), tr("Threshold")});
	conditions_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	conditions_->setSelectionBehavior(QAbstractItemView::SelectRows);
	conditions_->setSelectionMode(QAbstractItemView::SingleSelection);
	conditions_->verticalHeader()->hide();
	auto *layout = new QVBoxLayout(this);
	auto *heading = new QHBoxLayout;
	heading->addWidget(enabled_);
	heading->addWidget(mode_);
	heading->addWidget(new QLabel(tr("Name:"), this));
	heading->addWidget(name_);
	layout->addLayout(heading);
	auto *help = new QLabel(tr("Match all conditions in each stage, then advance to the next stage. "
		"The connected device may limit the number of stages and conditions."), this);
	help->setWordWrap(true);
	layout->addWidget(help);
	auto *content = new QHBoxLayout;
	auto *left = new QVBoxLayout;
	left->addWidget(new QLabel(tr("Stages"), this));
	left->addWidget(stage_list_);
	auto button = [this](const QString &label, const char *name) {
		auto *result = new QPushButton(label, this);
		result->setObjectName(name);
		return result;
	};
	add_stage_ = button(tr("Add stage"), "addTriggerStage");
	duplicate_stage_ = button(tr("Duplicate"), "duplicateTriggerStage");
	remove_stage_ = button(tr("Remove stage"), "removeTriggerStage");
	up_ = button(tr("Move up"), "moveTriggerStageUp");
	down_ = button(tr("Move down"), "moveTriggerStageDown");
	auto *stage_buttons = new QHBoxLayout;
	stage_buttons->addWidget(add_stage_);
	stage_buttons->addWidget(duplicate_stage_);
	stage_buttons->addWidget(remove_stage_);
	left->addLayout(stage_buttons);
	auto *order_buttons = new QHBoxLayout;
	order_buttons->addWidget(up_);
	order_buttons->addWidget(down_);
	left->addLayout(order_buttons);
	content->addLayout(left, 1);
	auto *right = new QVBoxLayout;
	right->addWidget(new QLabel(tr("Conditions in selected stage"), this));
	right->addWidget(conditions_);
	auto *match_buttons = new QHBoxLayout;
	add_match_ = button(tr("Add condition"), "addTriggerCondition");
	remove_match_ = button(tr("Remove condition"), "removeTriggerCondition");
	match_buttons->addWidget(add_match_);
	match_buttons->addWidget(remove_match_);
	match_buttons->addStretch();
	right->addLayout(match_buttons);
	qualifier_panel_ = new QWidget(this);
	auto *qualifiers = new QFormLayout(qualifier_panel_);
	count_ = new QSpinBox(qualifier_panel_); count_->setRange(1, 255);
	count_->setObjectName("triggerCount");
	continuous_ = new QCheckBox(tr("Continuous"), qualifier_panel_);
	continuous_->setObjectName("triggerContinuous");
	qualifiers->addRow(tr("Count"), count_); qualifiers->addRow(continuous_);
	auto *count_help = new QLabel(tr("For edges, Count selects the occurrence. For levels, Count is nominal time in ns. "
		"Continuous requires uninterrupted matching time; otherwise matching time accumulates. "
		"The timing of mixed edge/level conditions is experimental."), qualifier_panel_);
	count_help->setWordWrap(true); qualifiers->addRow(count_help);
	qualifier_panel_->setVisible(hardware_); right->addWidget(qualifier_panel_);
	content->addLayout(right, 2);
	parallel_panel_ = new QWidget(this); parallel_panel_->setLayout(content);
	layout->addWidget(parallel_panel_);
	layout->addWidget(serial_panel_);
	layout->addWidget(error_);
	layout->addWidget(buttons_);
	connect(buttons_, &QDialogButtonBox::accepted, this, &TriggerEditor::accept);
	connect(buttons_, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(enabled_, &QCheckBox::toggled, this, [this] {
		if (enabled_->isChecked() && stages_.empty()) stages_.emplace_back();
		rebuild(stage_list_->currentRow());
	});
	connect(name_, &QLineEdit::textChanged, this, [this] { update_validation(); });
	connect(stage_list_, &QListWidget::currentRowChanged, this, [this] {
		if (!rebuilding_) populate_conditions();
	});
	connect(conditions_, &QTableWidget::itemSelectionChanged, this, [this] { update_validation(); });
	connect(add_stage_, &QPushButton::clicked, this, [this] {
		stages_.emplace_back();
		enabled_->setChecked(true);
		rebuild(static_cast<int>(stages_.size()) - 1);
	});
	connect(duplicate_stage_, &QPushButton::clicked, this, [this] {
		const int i = stage_list_->currentRow();
		if (i < 0) return;
		const auto copy = stages_.at(i);
		stages_.insert(stages_.begin() + i + 1, copy);
		rebuild(i + 1);
	});
	connect(remove_stage_, &QPushButton::clicked, this, [this] {
		const int i = stage_list_->currentRow();
		if (i < 0) return;
		stages_.erase(stages_.begin() + i);
		if (stages_.empty()) enabled_->setChecked(false);
		rebuild(std::min(i, static_cast<int>(stages_.size()) - 1));
	});
	connect(up_, &QPushButton::clicked, this, [this] { move_stage(-1); });
	connect(down_, &QPushButton::clicked, this, [this] { move_stage(1); });
	connect(add_match_, &QPushButton::clicked, this, &TriggerEditor::add_condition);
	connect(remove_match_, &QPushButton::clicked, this, &TriggerEditor::remove_condition);
	connect(mode_, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
		[this] { update_validation(); });
	for (auto *field : {serial_clock_, serial_data_, serial_edge_})
		connect(field, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
			[this] { update_validation(); });
	connect(serial_bits_, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
		[this] { update_validation(); });
	connect(serial_value_, &QLineEdit::textChanged, this, [this] { update_validation(); });
	connect(count_, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int count) {
		const int selected = stage_list_->currentRow();
		if (!rebuilding_ && selected >= 0) { stages_[selected].qualifier.count = count; update_validation(); }
	});
	connect(continuous_, &QCheckBox::toggled, this, [this](bool continuous) {
		const int selected = stage_list_->currentRow();
		if (!rebuilding_ && selected >= 0) { stages_[selected].qualifier.continuous = continuous; update_validation(); }
	});
	rebuild(stages_.empty() ? -1 : 0);
}

bool TriggerEditor::analog(unsigned int index) const
{
	for (const auto &channel : device_->channels())
		if (channel->index() == index) return channel->type()->id() == SR_CHANNEL_ANALOG;
	return false;
}

std::vector<int> TriggerEditor::allowed_types(unsigned int channel) const
{
	std::vector<int> result;
	for (const auto &ch : device_->channels())
		if (ch->index() == channel)
			for (int type : supported_)
				if (trigger_match_allowed(ch->type()->id(), type)) result.push_back(type);
	return result;
}

void TriggerEditor::rebuild(int selected)
{
	rebuilding_ = true;
	stage_list_->clear();
	for (size_t i = 0; i < stages_.size(); ++i)
		stage_list_->addItem(tr("Stage %1 (%2 conditions)").arg(static_cast<qulonglong>(i + 1))
			.arg(static_cast<qulonglong>(stages_[i].size())));
	if (!stages_.empty()) stage_list_->setCurrentRow(std::max(0, std::min(selected,
		static_cast<int>(stages_.size()) - 1)));
	rebuilding_ = false;
	populate_conditions();
}

void TriggerEditor::populate_conditions()
{
	rebuilding_ = true;
	conditions_->setRowCount(0);
	const int selected = stage_list_->currentRow();
	if (selected >= 0) {
		const auto &stage = stages_.at(selected);
		count_->setValue(stage.qualifier.count);
		continuous_->setChecked(stage.qualifier.continuous);
		conditions_->setRowCount(static_cast<int>(stage.size()));
		for (size_t i = 0; i < stage.size(); ++i) {
			const auto &match = stage[i];
			auto *channels = new QComboBox(conditions_);
			for (const auto &channel : device_->channels()) {
				if (allowed_types(channel->index()).empty()) continue;
				if (channel->enabled() || channel->index() == match.channel)
					channels->addItem(QString::number(channel->index()) + ": " +
						QString::fromStdString(channel->name()) +
						(channel->enabled() ? QString() : tr(" (disabled)")), channel->index());
			}
			if (channels->findData(match.channel) < 0)
				channels->addItem(tr("Unavailable channel %1").arg(match.channel), match.channel);
			channels->setCurrentIndex(channels->findData(match.channel));
			auto *types = new QComboBox(conditions_);
			for (int type : allowed_types(match.channel)) types->addItem(condition_name(type), type);
			if (types->findData(match.type) < 0) types->addItem(condition_name(match.type), match.type);
			types->setCurrentIndex(types->findData(match.type));
			auto *value = new QLineEdit(match.value, conditions_);
			value->setPlaceholderText(tr("Not set"));
			value->setToolTip(tr("Analog threshold; use a decimal point. Leave blank when no threshold is required."));
			value->setEnabled(analog(match.channel));
			conditions_->setCellWidget(static_cast<int>(i), 0, channels);
			conditions_->setCellWidget(static_cast<int>(i), 1, types);
			conditions_->setCellWidget(static_cast<int>(i), 2, value);
			for (QWidget *field : {static_cast<QWidget *>(channels), static_cast<QWidget *>(types),
					static_cast<QWidget *>(value)}) {
				field->setProperty("triggerRow", static_cast<int>(i));
				field->installEventFilter(this);
			}
			connect(channels, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
				[this, selected, i, channels] {
					if (rebuilding_) return;
					auto &match = stages_.at(selected).at(i);
					match.channel = channels->currentData().toUInt();
					const auto types = allowed_types(match.channel);
					if (std::find(types.begin(), types.end(), match.type) == types.end() && !types.empty())
						match.type = types.front();
					if (!analog(match.channel)) match.value.clear();
					populate_conditions();
				});
			connect(types, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
				[this, selected, i, types] {
					if (!rebuilding_) {
						stages_.at(selected).at(i).type = types->currentData().toInt();
						update_validation();
					}
				});
			connect(value, &QLineEdit::textChanged, this, [this, selected, i](const QString &text) {
				if (!rebuilding_) { stages_.at(selected).at(i).value = text; update_validation(); }
			});
		}
	}
	rebuilding_ = false;
	update_validation();
}

void TriggerEditor::move_stage(int delta)
{
	const int i = stage_list_->currentRow(), next = i + delta;
	if (i < 0 || next < 0 || next >= static_cast<int>(stages_.size())) return;
	std::swap(stages_[i], stages_[next]);
	rebuild(next);
}

bool TriggerEditor::eventFilter(QObject *object, QEvent *event)
{
	if (!rebuilding_ && event->type() == QEvent::FocusIn && object->property("triggerRow").isValid())
		conditions_->setCurrentCell(object->property("triggerRow").toInt(), 0);
	return QDialog::eventFilter(object, event);
}

void TriggerEditor::add_condition()
{
	const int selected = stage_list_->currentRow();
	if (selected < 0) return;
	auto &stage = stages_.at(selected);
	for (const auto &channel : device_->channels()) {
		if (!channel->enabled() || std::any_of(stage.begin(), stage.end(),
			[&](const Match &m) { return m.channel == channel->index(); })) continue;
		const auto types = allowed_types(channel->index());
		if (types.empty()) continue;
		stage.push_back({channel->index(), types.front(), QString()});
		rebuild(selected);
		return;
	}
}

void TriggerEditor::remove_condition()
{
	const int selected = stage_list_->currentRow(), row = conditions_->currentRow();
	if (selected < 0 || row < 0) return;
	auto &stage = stages_.at(selected);
	stage.erase(stage.begin() + row);
	rebuild(selected);
}

std::shared_ptr<sigrok::Trigger> TriggerEditor::trigger() const
{
	TriggerSequence sequence;
	sequence.enabled = enabled_->isChecked();
	sequence.name = name_->text().toStdString();
	if (!sequence.enabled || (hardware_ && mode_->currentIndex() == 1)) return nullptr;
	for (const auto &stage : stages_) {
		sequence.stages.emplace_back();
		for (const auto &match : stage) {
			float value = std::numeric_limits<float>::quiet_NaN();
			if (!match.value.trimmed().isEmpty()) {
				bool ok;
				const double number = QLocale::c().toDouble(match.value.trimmed(), &ok);
				if (!ok || !std::isfinite(number) || std::abs(number) > std::numeric_limits<float>::max())
					throw std::runtime_error("Enter a finite trigger threshold using a decimal point.");
				value = static_cast<float>(number);
			}
			sequence.stages.back().push_back({match.channel, match.type, value});
		}
	}
	return build_trigger_sequence(context_, device_, sequence);
}

HardwareTriggerOptions TriggerEditor::hardware_options() const
{
	HardwareTriggerOptions options;
	if (!hardware_ || !enabled_->isChecked()) return options;
	if (mode_->currentIndex() == 1) {
		options.mode = "serial";
		options.clock = serial_clock_->currentData().toUInt();
		options.data = serial_data_->currentData().toUInt();
		options.falling = serial_edge_->currentIndex() == 1;
		options.bits = serial_bits_->value();
		bool ok;
		const auto text = serial_value_->text().trimmed();
		options.value = text.toUInt(&ok, text.startsWith("0x", Qt::CaseInsensitive) ? 16 : 10);
		if (!ok) throw std::runtime_error("Enter an unsigned decimal or hexadecimal serial word.");
	} else {
		bool qualified = false;
		for (const auto &stage : stages_) {
			options.stages.push_back(stage.qualifier);
			qualified |= stage.qualifier.count != 1 || stage.qualifier.continuous;
		}
		if (qualified) options.mode = "parallel";
		else options.stages.clear();
		if (stages_.size() > 16) throw std::runtime_error("Use no more than sixteen trigger stages.");
	}
	validate_hardware_trigger(device_, trigger(), options);
	return options;
}

void TriggerEditor::update_validation()
{
	if (rebuilding_) return;
	bool valid = true;
	try { trigger(); hardware_options(); error_->clear(); }
	catch (const std::exception &e) { valid = false; error_->setText(QString::fromUtf8(e.what())); }
	const bool enabled = enabled_->isChecked();
	const bool serial = hardware_ && mode_->currentIndex() == 1;
	parallel_panel_->setVisible(!serial); serial_panel_->setVisible(serial);
	serial_panel_->setEnabled(enabled); mode_->setEnabled(enabled);
	const int selected = stage_list_->currentRow();
	qualifier_panel_->setEnabled(enabled && selected >= 0);
	add_stage_->setEnabled(!hardware_ || stages_.size() < 16);
	name_->setEnabled(enabled && !serial);
	stage_list_->setEnabled(enabled);
	conditions_->setEnabled(enabled);
	duplicate_stage_->setEnabled(enabled && selected >= 0 && (!hardware_ || stages_.size() < 16));
	remove_stage_->setEnabled(enabled && selected >= 0);
	up_->setEnabled(enabled && selected > 0);
	down_->setEnabled(enabled && selected >= 0 && selected + 1 < static_cast<int>(stages_.size()));
	add_match_->setEnabled(enabled && selected >= 0);
	remove_match_->setEnabled(enabled && selected >= 0 && conditions_->currentRow() >= 0);
	buttons_->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void TriggerEditor::accept()
{
	try { trigger(); hardware_options(); QDialog::accept(); }
	catch (const std::exception &e) { error_->setText(QString::fromUtf8(e.what())); }
}

}
}
