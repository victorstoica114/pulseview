/* SPDX-License-Identifier: GPL-3.0-or-later */
#ifndef PULSEVIEW_PV_DIALOGS_TRIGGEREDITOR_HPP
#define PULSEVIEW_PV_DIALOGS_TRIGGEREDITOR_HPP

#include <memory>
#include <vector>
#include <QDialog>
#include <QString>
#include <pv/triggersequence.hpp>

class QCheckBox;
class QComboBox;
class QSpinBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QTableWidget;

namespace pv {
namespace dialogs {

class TriggerEditor : public QDialog
{
	Q_OBJECT
public:
	TriggerEditor(const std::shared_ptr<sigrok::Context> &context,
		const std::shared_ptr<sigrok::Device> &device,
		const std::shared_ptr<sigrok::Trigger> &initial, QWidget *parent = nullptr);
	std::shared_ptr<sigrok::Trigger> trigger() const;
	HardwareTriggerOptions hardware_options() const;

protected:
	void accept() override;

private:
	struct Match {
		unsigned int channel;
		int type;
		QString value;
	};
	struct Stage : std::vector<Match> { TriggerQualifier qualifier; };
	void rebuild(int selected);
	void populate_conditions();
	void update_validation();
	void add_condition();
	void remove_condition();
	void move_stage(int delta);
	std::vector<int> allowed_types(unsigned int channel) const;
	bool analog(unsigned int channel) const;
	const std::shared_ptr<sigrok::Context> context_;
	const std::shared_ptr<sigrok::Device> device_;
	std::vector<int> supported_;
	std::vector<Stage> stages_;
	bool rebuilding_ = false;
	QCheckBox *enabled_;
	QLineEdit *name_;
	QListWidget *stage_list_;
	QTableWidget *conditions_;
	QLabel *error_;
	QDialogButtonBox *buttons_;
	QPushButton *add_stage_, *remove_stage_, *duplicate_stage_, *up_, *down_;
	QPushButton *add_match_, *remove_match_;
	bool hardware_ = false;
	QComboBox *mode_, *serial_clock_, *serial_data_, *serial_edge_;
	QSpinBox *count_, *serial_bits_;
	QCheckBox *continuous_;
	QLineEdit *serial_value_;
	QWidget *parallel_panel_, *serial_panel_, *qualifier_panel_;
};

}
}
#endif
