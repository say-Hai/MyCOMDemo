#pragma once
#include <QComboBox>
#include <QMouseEvent>
#include <QSerialPort>
#include <QSerialPortInfo>

class mycombobox : public QComboBox
{
	Q_OBJECT
public:
	explicit mycombobox(QWidget* parent = nullptr);

	void mousePressEvent(QMouseEvent* event) override;
signals:
private:
	void scanActivatePort();
};
