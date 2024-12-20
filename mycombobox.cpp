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
	//���������б�
	QStringList comPort;
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		QString serialPortInfo = info.portName() + ": " + info.description();// �����豸��Ϣ��оƬ/��������
		comPort << serialPortInfo;
	}
	this->addItems(comPort);
}