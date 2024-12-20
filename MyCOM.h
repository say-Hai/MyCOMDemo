#pragma once

#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QScreen>
#include "ui_MyCOM.h"

class MyCOM : public QMainWindow
{
	Q_OBJECT

public:
	MyCOM(QWidget* parent = nullptr);
	~MyCOM();

private slots:
	void on_pushButtonOpen_clicked();
	void on_pushButtonSend_clicked();
	void MyComRevSlot();
	void on_checkBoxPeriodicSend_stateChanged(int arg1);
	void on_pushButtonClearRev_clicked();
	void on_pushButtonClearSend_clicked();
	void on_pushButtonRdFile_clicked();
	void on_pushButtonSaveRev_clicked();
	void on_radioButton_clicked();
	void on_pushButtonMuti_1_clicked();
	void on_pushButtonMuti_2_clicked();
	void on_pushButtonMuti_3_clicked();
	void on_pushButtonMuti_4_clicked();
	void on_pushButtonMuti_5_clicked();
	void on_pushButtonMuti_6_clicked();
	void on_pushButtonMuti_7_clicked();
	void on_pushButtonMuti_8_clicked();
	void on_pushButtonMuti_9_clicked();
	void on_pushButtonMuti_10_clicked();
	void on_pushButtonMuti_clicked(int lineEditIndex);
	void on_pushButtonMutiReset_clicked();//清除多行文本
	void on_checkBoxMuti_stateChanged(int);
	void Pre_on_pushButtonSend_clicked();
private:
	Ui::MyCOMClass ui;

	//添加自定义变量
	QSerialPort MyCom;

	//记录当前时间
	QDateTime curDateTime;

	//创建定时器，控制周期性指令发送
	QTimer* PriecSendTimer;

	//添加自定义变量
	long ComSendSum, ComRevSum;//发送和接收流量统计变量
	QLabel* qlbSendSum, * qlbRevSum;//发送接收流量label对象
	QLabel* myLink, * MySource;
	// 使用QList存储所有复选框的指针
	QList<QCheckBox*> checkBoxes;

	int LastSend = 0;//多行循环发送起始位置
	void setNumOnLabel(QLabel* lbl, QString strS, long num);
	QString byteArrayToUnicode(const QByteArray& array);
	bool openTextByIODevice(const QString& chars);
	bool saveTextByIODevice(const QString& chars);
};
