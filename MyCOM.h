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
	void on_pushButtonMutiReset_clicked();//��������ı�
	void on_checkBoxMuti_stateChanged(int);
	void Pre_on_pushButtonSend_clicked();
private:
	Ui::MyCOMClass ui;

	//����Զ������
	QSerialPort MyCom;

	//��¼��ǰʱ��
	QDateTime curDateTime;

	//������ʱ��������������ָ���
	QTimer* PriecSendTimer;

	//����Զ������
	long ComSendSum, ComRevSum;//���ͺͽ�������ͳ�Ʊ���
	QLabel* qlbSendSum, * qlbRevSum;//���ͽ�������label����
	QLabel* myLink, * MySource;
	// ʹ��QList�洢���и�ѡ���ָ��
	QList<QCheckBox*> checkBoxes;

	int LastSend = 0;//����ѭ��������ʼλ��
	void setNumOnLabel(QLabel* lbl, QString strS, long num);
	QString byteArrayToUnicode(const QByteArray& array);
	bool openTextByIODevice(const QString& chars);
	bool saveTextByIODevice(const QString& chars);
};
