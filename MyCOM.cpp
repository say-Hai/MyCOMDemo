#pragma execution_character_set("utf-8")

#include "MyCOM.h"

#include <iostream>
#include <QDebug>
#include <QMap>
// 初始化波特率映射表
QMap<QString, QSerialPort::BaudRate> baudRateMap = {
	{"1200", QSerialPort::Baud1200},
	{"2400", QSerialPort::Baud2400},
	{"4800", QSerialPort::Baud4800},
	{"9600", QSerialPort::Baud9600},
	{"19200", QSerialPort::Baud19200},
	{"38400", QSerialPort::Baud38400},
	{"57600", QSerialPort::Baud57600},
	{"115200", QSerialPort::Baud115200},
};
// 初始化数据位映射表
QMap<QString, QSerialPort::DataBits> dataBitsMap = {
	{"5", QSerialPort::Data5},
	{"6", QSerialPort::Data6},
	{"7", QSerialPort::Data7},
	{"8", QSerialPort::Data8},
};
// 初始化停止位映射表
QMap<QString, QSerialPort::StopBits> stopBitsMap = {
	{"1", QSerialPort::OneStop},
	{"1.5", QSerialPort::OneAndHalfStop},
	{"2", QSerialPort::TwoStop},
};
// 初始化校验方式映射表
QMap<QString, QSerialPort::Parity> parityMap = {
	{"None", QSerialPort::NoParity},
	{"Even", QSerialPort::EvenParity},
	{"Odd", QSerialPort::OddParity},
	{"Mark", QSerialPort::MarkParity},
	{"Space", QSerialPort::SpaceParity},
};

MyCOM::MyCOM(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//创建串口列表
	QStringList comPort;
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		comPort << info.portName();
	}
	ui.comboBoxNo_2->addItems(comPort);

	//通过信号槽机制来处理数据发送和接收逻辑
	connect(&MyCom, &QIODevice::readyRead, this, &MyCOM::MyComRevSlot);
}

MyCOM::~MyCOM()
{}

/// @brief 打开/关闭串口的槽函数
///	1. 从控件读取串口初始化信息
///	2. 初始化串口
///	3. 根据串口的text文本分辨执行打开还是关闭串口的逻辑
///		3.1 打开串口
///		3.2 关闭串口
void MyCOM::on_pushButtonOpen_clicked()
{
	QSerialPort::BaudRate CombaudRate;
	QSerialPort::DataBits ComdataBits;
	QSerialPort::StopBits ComstopBits;
	QSerialPort::Parity   ComParity;
	QString selectedBaudRate = ui.comboBoxComBaud_2->currentText();
	std::cout << selectedBaudRate.toStdString() << "\n";

	if (baudRateMap.contains(selectedBaudRate)) {
		CombaudRate = baudRateMap[selectedBaudRate];
	}
	else {
		// 如果用户选择了一个未知的波特率，可以设置默认值或提示错误
		CombaudRate = QSerialPort::Baud9600; // 默认值
		qWarning("Invalid baud rate selected. Defaulting to 9600.");
	}
	// 根据用户选择设置数据位
	QString selectedDataBits = ui.comboBoxData_2->currentText();
	if (dataBitsMap.contains(selectedDataBits)) {
		ComdataBits = dataBitsMap[selectedDataBits];
	}
	else {
		// 如果用户选择了一个未知的数据位，可以设置默认值或提示错误
		ComdataBits = QSerialPort::Data8; // 默认值
		qWarning("Invalid data bits selected. Defaulting to 8 bits.");
	}
	// 根据用户选择设置停止位
	QString selectedStopBits = ui.comboBoxStop_2->currentText();
	if (stopBitsMap.contains(selectedStopBits)) {
		ComstopBits = stopBitsMap[selectedStopBits];
	}
	else {
		// 如果用户选择了未知的停止位，可以设置默认值或提示错误
		ComstopBits = QSerialPort::OneStop; // 默认值
		qWarning("Invalid stop bits selected. Defaulting to 1 stop bit.");
	}
	// 根据用户选择设置校验方式
	QString selectedParity = ui.comboBoxCheck_2->currentText();
	if (parityMap.contains(selectedParity)) {
		ComParity = parityMap[selectedParity];
	}
	else {
		// 如果用户选择了未知的校验方式，可以设置默认值或提示错误
		ComParity = QSerialPort::NoParity; // 默认值
		qWarning("Invalid parity selected. Defaulting to No Parity.");
	}
	QString spTxt = ui.comboBoxNo_2->currentText();
	spTxt = spTxt.section(":", 0, 0); //过滤串口信息，只保留COMX
	//初始化串口
	MyCom.setBaudRate(CombaudRate);
	MyCom.setDataBits(ComdataBits);
	MyCom.setStopBits(ComstopBits);
	MyCom.setParity(ComParity);
	MyCom.setPortName(spTxt);   //MyCom.setPortName(ui->comboBoxNo->currentText());
	//打开串口
	if (ui.pushButtonOpen_2->text() == "打开串口")
	{
		bool ComFlag;
		ComFlag = MyCom.open(QIODevice::ReadWrite);
		if (ComFlag == true)//串口打开成功
		{
			//串口下拉框设置为不可选
			ui.comboBoxCheck_2->setEnabled(false);
			ui.comboBoxComBaud_2->setEnabled(false);
			ui.comboBoxData_2->setEnabled(false);
			ui.comboBoxNo_2->setEnabled(false);
			ui.comboBoxStop_2->setEnabled(false);

			//使能相应按钮等
			ui.pushButtonSend_2->setEnabled(true);
			ui.checkBoxPeriodicSend->setEnabled(true);
			ui.checkBoxPeriodicMutiSend_2->setEnabled(true);
			ui.lineEditTime->setEnabled(true);
			ui.checkBoxAddNewShift_2->setEnabled(true);
			ui.checkBoxSendHex_2->setEnabled(true);

			ui.pushButtonOpen_2->setText(" 关闭串口 ");
		}
		else
		{
			QMessageBox::critical(this, "错误提示", "串口打开失败，该端口可能被占用或不存在！rnLinux系统可能为当前用户无串口访问权限！");
		}
	}
	else
	{
		MyCom.close();
		ui.pushButtonOpen_2->setText(" 打开串口 ");
		ui.comboBoxCheck_2->setEnabled(true);
		ui.comboBoxComBaud_2->setEnabled(true);
		ui.comboBoxData_2->setEnabled(true);
		ui.comboBoxNo_2->setEnabled(true);
		ui.comboBoxStop_2->setEnabled(true);

		//使相应的按钮不可用
		ui.pushButtonSend_2->setEnabled(false);
		ui.checkBoxPeriodicSend->setChecked(false);//取消选中
		ui.checkBoxPeriodicSend->setEnabled(false);
		ui.checkBoxPeriodicMutiSend_2->setChecked(false);//取消选中
		ui.checkBoxPeriodicMutiSend_2->setEnabled(false);
		ui.lineEditTime->setEnabled(false);
		ui.checkBoxAddNewShift_2->setEnabled(false);
		ui.checkBoxSendHex_2->setEnabled(false);
	}
}

/// @brief 数据发送槽函数
/// 1. 读取发送文本框的数据
/// 2. 根据选择框判断数据格式化逻辑（是否按照16进制发送）
/// 3. 发送串口数据
void MyCOM::on_pushButtonSend_clicked()
{
	QByteArray comSendData;
	QString SendTemp;
	int temp;

	//读取发送窗口数据
	SendTemp = ui.TextSend_2->toPlainText();

	//判断发送格式，并格式化数据
	if (ui.checkBoxSendHex_2->checkState() != false)//16进制发送
	{
		if (ui.checkBoxAddNewShift_2->checkState() != false)
		{
			SendTemp.append("0D0A");// 后面添加换行
		}
		comSendData = QByteArray::fromHex(SendTemp.toUtf8()).data();//获取字符串
	}
	else //字符串形式发送
	{
		if (ui.checkBoxAddNewShift_2->checkState() != false)
		{
			SendTemp.append("\r\n");// 后面添加换行
		}
		comSendData = SendTemp.toLocal8Bit().data();//获取字符串
	}

	temp = MyCom.write(comSendData);
}

/// @brief 接收串口数据槽函数
/// 1. 处理接收数据格式（是否十六进制）
/// 2. 接收数据并显示在接收文本框
void MyCOM::MyComRevSlot()
{
	QByteArray MyComRevBUff;//接收数据缓存
	QString StrTemp, StrTimeDate, StrTemp1;

	//读取串口接收到的数据，并格式化数据
	MyComRevBUff = MyCom.readAll();
	StrTemp = QString::fromLocal8Bit(MyComRevBUff);

	curDateTime = QDateTime::currentDateTime();
	StrTimeDate = curDateTime.toString("[yyyy-MM-dd hh:mm:ss.zzz]");

	if (ui.checkBoxRevHex_2->checkState() != true) //以十六进制接收
	{
		StrTemp = MyComRevBUff.toHex().toUpper();//转换为16进制数，并大写
		for (int i = 0; i < StrTemp.length(); i += 2)//整理字符串，即添加空格
		{
			StrTemp1 += StrTemp.mid(i, 2);
			StrTemp1 += " ";
		}
		//添加时间头
		StrTemp1.prepend(StrTimeDate);
		StrTemp1.append("\r\n");//后面添加换行
		ui.TextRev_2->insertPlainText(StrTemp1);//显示数据
		ui.TextRev_2->moveCursor(QTextCursor::End);//光标移动到文本末尾
	}
	else //显示原始数据
	{
		ui.TextRev_2->insertPlainText(StrTemp);//显示数据
		ui.TextRev_2->moveCursor(QTextCursor::End);//光标移动到文本末尾
	}
}