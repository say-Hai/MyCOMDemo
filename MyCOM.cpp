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

	checkBoxes = {
		ui.checkBoxMuti_1,
		ui.checkBoxMuti_2,
		ui.checkBoxMuti_3,
		ui.checkBoxMuti_4,
		ui.checkBoxMuti_5,
		ui.checkBoxMuti_6,
		ui.checkBoxMuti_7,
		ui.checkBoxMuti_8,
		ui.checkBoxMuti_9,
		ui.checkBoxMuti_10
	};

	//创建定时器
	PriecSendTimer = new QTimer;
	PriecSendTimer->setInterval(1000);

	//创建底部状态栏及其相关部件
	QStatusBar* STABar = statusBar();

	qlbSendSum = new QLabel(this);
	qlbRevSum = new QLabel(this);
	myLink = new QLabel(this);
	MySource = new QLabel(this);
	myLink->setMinimumSize(90, 20);// 设置标签最小大小
	MySource->setMinimumSize(90, 20);
	qlbSendSum->setMinimumSize(100, 20);
	qlbRevSum->setMinimumSize(100, 20);
	ComSendSum = 0;
	ComRevSum = 0;

	setNumOnLabel(qlbSendSum, "Tx: ", ComSendSum);
	setNumOnLabel(qlbRevSum, "Rx: ", ComRevSum);

	STABar->addPermanentWidget(qlbSendSum);// 从右往左依次添加
	STABar->addPermanentWidget(qlbRevSum);
	STABar->addWidget(myLink);// 从左往右依次添加
	STABar->addWidget(MySource);

	myLink->setOpenExternalLinks(true);//状态栏显示官网、源码链接
	myLink->setText("<style> a {text-decoration: none} </style> <a href=\"http://8.134.156.7/\">--个人博客--");
	MySource->setOpenExternalLinks(true);
	MySource->setText("<style> a {text-decoration: none} </style> <a href=\"https://github.com/say-Hai/MyCOMDemo\">--源代码--");

	//隐藏多行发送功能
	ui.groupBoxMutiSend->hide();
	ui.groupBoxRev->setFixedWidth(541);//设置接收组的大小
	ui.TextRev_2->setFixedWidth(521);//设置接收窗口的大小

	//注册定时器超时信号槽机制，循环调用发送指令
	connect(PriecSendTimer, &QTimer::timeout, this, [=]() {Pre_on_pushButtonSend_clicked(); });

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
	if (ui.pushButtonOpen_2->text() == " 打开串口 ")
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

	if (temp)
	{
		ComSendSum++;
		setNumOnLabel(qlbSendSum, "Tx: ", ComSendSum);
	}
}

/// @brief 周期发送
///	1. 未选中周期发送，则关闭定时器并开启输入文本框
///	2. 选中状态下，获取周期并开启定时器
/// @param arg1 选择框的状态变化
void MyCOM::on_checkBoxPeriodicSend_stateChanged(int arg1)
{
	if (arg1 == false)
	{
		PriecSendTimer->stop();
		ui.lineEditTime->setEnabled(true);
	}
	else
	{
		PriecSendTimer->start(ui.lineEditTime->text().toInt());
		ui.lineEditTime->setEnabled(false);
	}
}
/// @brief 清除接收区数据和状态栏数据
void MyCOM::on_pushButtonClearRev_clicked()
{
	ui.TextRev_2->clear();
	ComSendSum = 0;
	ComRevSum = 0;

	setNumOnLabel(qlbSendSum, "Tx: ", ComSendSum);
	setNumOnLabel(qlbRevSum, "Rx: ", ComRevSum);
}
/// @brief 清除发送区数据和状态栏数据
void MyCOM::on_pushButtonClearSend_clicked()
{
	ui.TextSend_2->clear();
	ComSendSum = 0;
	ComRevSum = 0;

	setNumOnLabel(qlbSendSum, "Tx: ", ComSendSum);
	setNumOnLabel(qlbRevSum, "Rx: ", ComRevSum);
}
/// @brief 读取文件数据作为发送区指令
void MyCOM::on_pushButtonRdFile_clicked()
{
	QString curPath = QDir::currentPath();
	QString dlgTitle = "打开一个文件"; //对话框标题
	QString filter = "文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
	QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
	if (aFileName.isEmpty())
		return;
	openTextByIODevice(aFileName);
}
/// @brief 通过文件保存接收区的指令
void MyCOM::on_pushButtonSaveRev_clicked()
{
	QString curFile = QDir::currentPath();
	QString dlgTitle = " 另存为一个文件 "; //对话框标题
	QString filter = " 文本文件(*.txt);;所有文件(*.*);;h文件(*.h);;c++文件(*.cpp) "; //文件过滤器
	QString aFileName = QFileDialog::getSaveFileName(this, dlgTitle, curFile, filter);
	if (aFileName.isEmpty())
		return;
	saveTextByIODevice(aFileName);
}
/// @brief 显示或隐藏多行发送
void MyCOM::on_radioButton_clicked()
{
	if (ui.radioButton->isChecked() == false)
	{
		ui.checkBoxPeriodicMutiSend_2->setChecked(false);
		ui.groupBoxMutiSend->hide();//隐藏多行发送界面
		ui.groupBoxRev->setFixedWidth(541);
		ui.TextRev_2->setFixedWidth(521);
	}
	else
	{
		ui.groupBoxRev->setFixedWidth(341);
		ui.TextRev_2->setFixedWidth(321);
		ui.groupBoxMutiSend->show();//显示多行发送界面
	}
}

/// @brief 每行单独发送按钮
void MyCOM::on_pushButtonMuti_1_clicked()
{
	on_pushButtonMuti_clicked(1);
}
void MyCOM::on_pushButtonMuti_2_clicked()
{
	on_pushButtonMuti_clicked(2);
}
void MyCOM::on_pushButtonMuti_3_clicked()
{
	on_pushButtonMuti_clicked(3);
}
void MyCOM::on_pushButtonMuti_4_clicked()
{
	on_pushButtonMuti_clicked(4);
}
void MyCOM::on_pushButtonMuti_5_clicked()
{
	on_pushButtonMuti_clicked(5);
}
void MyCOM::on_pushButtonMuti_6_clicked()
{
	on_pushButtonMuti_clicked(6);
}
void MyCOM::on_pushButtonMuti_7_clicked()
{
	on_pushButtonMuti_clicked(7);
}
void MyCOM::on_pushButtonMuti_8_clicked()
{
	on_pushButtonMuti_clicked(8);
}
void MyCOM::on_pushButtonMuti_9_clicked()
{
	on_pushButtonMuti_clicked(9);
}
void MyCOM::on_pushButtonMuti_10_clicked()
{
	on_pushButtonMuti_clicked(10);
}

/// @brief 通用槽函数，通过传递不同参数来完成多行发送
/// @param lineEditIndex 每行的索引
void MyCOM::on_pushButtonMuti_clicked(int lineEditIndex)
{
	QString Strtemp;
	switch (lineEditIndex) {
	case 1:
		Strtemp = ui.lineEditMuti1_2->text();
		break;
	case 2:
		Strtemp = ui.lineEditMuti2_2->text();
		break;
	case 3:
		Strtemp = ui.lineEditMuti3_2->text();
		break;
	case 4:
		Strtemp = ui.lineEditMuti4_2->text();
		break;
	case 5:
		Strtemp = ui.lineEditMuti5_2->text();
		break;
	case 6:
		Strtemp = ui.lineEditMuti6_2->text();
		break;
	case 7:
		Strtemp = ui.lineEditMuti7_2->text();
		break;
	case 8:
		Strtemp = ui.lineEditMuti8_2->text();
		break;
	case 9:
		Strtemp = ui.lineEditMuti9_2->text();
		break;
	case 10:
		Strtemp = ui.lineEditMuti10_2->text();
		break;
	default:
		return;  // 默认情况下不做任何操作
	}
	ui.TextSend_2->clear();
	ui.TextSend_2->insertPlainText(Strtemp);
	ui.TextSend_2->moveCursor(QTextCursor::End);
	MyCOM::on_pushButtonSend_clicked();
}
/// @brief 清除多行文本
void MyCOM::on_pushButtonMutiReset_clicked()
{
	ui.lineEditMuti1_2->clear();
	ui.lineEditMuti2_2->clear();
	ui.lineEditMuti3_2->clear();
	ui.lineEditMuti4_2->clear();
	ui.lineEditMuti5_2->clear();
	ui.lineEditMuti6_2->clear();
	ui.lineEditMuti7_2->clear();
	ui.lineEditMuti8_2->clear();
	ui.lineEditMuti9_2->clear();
	ui.lineEditMuti10_2->clear();
}
/// @brief 通过选择框的状态变化来打开/关闭定时器发送
/// @param arg
void MyCOM::on_checkBoxMuti_stateChanged(int arg)
{
	if (!arg)
	{
		PriecSendTimer->stop();//关闭定时器
		ui.lineEditTime->setEnabled(true);//使能对话框编辑
	}
	else
	{
		LastSend = 0;//从第一行开始发送
		ui.checkBoxPeriodicSend->setChecked(false);
		PriecSendTimer->start(ui.lineEditTime->text().toInt());
		ui.lineEditTime->setEnabled(false);//关闭对话框编辑
	}
}
/// @brief 重构定时器超时响应函数，适配多行重复发送功能
void MyCOM::Pre_on_pushButtonSend_clicked()
{
	if (ui.checkBoxPeriodicMutiSend_2->isChecked() == true)
	{
		//每次循环检查下一行所要发送的数据的行索引
		while (LastSend < 10)
		{
			if (checkBoxes[LastSend]->isChecked())
			{
				on_pushButtonMuti_clicked(++LastSend);
				break;
			}
			LastSend++;
		}
		// 防止地址越界
		if (LastSend == 10)
		{
			LastSend = 0;
		}
	}
	else
	{
		on_pushButtonSend_clicked();
	}
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
	ComRevSum++;
	setNumOnLabel(qlbRevSum, "Rx: ", ComRevSum);

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
/// @brief 状态栏显示计数值
/// @param lbl 所要显示的标签
/// @param strS 字符串
/// @param num 计数值
void MyCOM::setNumOnLabel(QLabel* lbl, QString strS, long num)
{
	QString strN = QString("%1").arg(num);
	QString str = strS + strN;
	lbl->setText(str);
}
/// @brief 对字符串进行编码格式转换
/// @param array 字符串
/// @return 转换后的格式（UTF-8/GBK）
QString MyCOM::byteArrayToUnicode(const QByteArray& array)
{
	// state用于保存转换状态，它的成员invalidChars，可用来判断是否转换成功
	// 如果转换成功，则值为0，如果值大于0，则说明转换失败
	QTextCodec::ConverterState state;
	// 先尝试使用utf-8的方式把QByteArray转换成QString
	QString text = QTextCodec::codecForName("UTF-8")->toUnicode(array.constData(), array.size(), &state);
	// 如果转换时无效字符数量大于0，说明编码格式不对
	if (state.invalidChars > 0)
	{
		// 再尝试使用GBK的方式进行转换，一般就能转换正确(当然也可能是其它格式，但比较少见了)
		text = QTextCodec::codecForName("GBK")->toUnicode(array);
	}
	return text;
}
/// @brief 对选择的文件进行数据读取
/// @param aFileName 文件路径
/// @return 文件是否打开成功
bool MyCOM::openTextByIODevice(const QString& aFileName)
{
	QFile aFile(aFileName);
	if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QByteArray text = aFile.readAll();
	QString strText = byteArrayToUnicode(text);//编码格式转换，防止GBK中文乱码
	ui.TextSend_2->setPlainText(strText);
	aFile.close();
	return  true;
}
/// @brief 对选择的文件进行数据保存
/// @param aFileName 文件路径
bool MyCOM::saveTextByIODevice(const QString& aFileName) {
	QFile aFile(aFileName);
	if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	QString str = ui.TextRev_2->toPlainText();//整个内容作为字符串
	QByteArray  strBytes = str.toUtf8();//转换为字节数组
	aFile.write(strBytes, strBytes.length());  //写入文件
	aFile.close();

	return true;
}