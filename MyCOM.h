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

private:
	Ui::MyCOMClass ui;

	//添加自定义变量
	QSerialPort MyCom;

	//记录当前时间
	QDateTime curDateTime;
};
