#include "mycombobox.h"

mycombobox::mycombobox(QWidget* parent) : QComboBox(parent)
{
	scanActivatePort();
}

void mycombobox::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		scanActivatePort();
		showPopup();
	}
}

void mycombobox::scanActivatePort()
{
	clear();
	//创建串口列表
	QStringList comPort;
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		QString serialPortInfo = info.portName() + ": " + info.description();// 串口设备信息，芯片/驱动名称
		comPort << serialPortInfo;
	}
	this->addItems(comPort);
}