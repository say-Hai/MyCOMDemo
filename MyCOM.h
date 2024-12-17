#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyCOM.h"

class MyCOM : public QMainWindow
{
    Q_OBJECT

public:
    MyCOM(QWidget *parent = nullptr);
    ~MyCOM();

private:
    Ui::MyCOMClass ui;
};
