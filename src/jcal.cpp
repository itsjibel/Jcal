#include "jcal.h"
#include "./ui_jcal.h"
#include <string.h>
#include "stack.hpp"

Jcal::Jcal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Jcal)
{
    ui->setupUi(this);

    ui->display->setText(QString::fromStdString(calcVal));

    connect(ui->add, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->minus, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->multiply, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->divide, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button0, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button1, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button2, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button3, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button4, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button5, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button6, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button7, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button8, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->Button9, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->equal, SIGNAL(released()), this, SLOT(EqualButton()));
}

Jcal::~Jcal()
{
    delete ui;
}

void Jcal::ButtonPressed()
{
    QString displayVal = ui->display->text();
    calcVal = displayVal.toStdString();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if (QString::compare(butVal, "/", Qt::CaseInsensitive) == 0)
        calcVal += '/';
    else if (QString::compare(butVal, "*", Qt::CaseInsensitive) == 0)
        calcVal += '*';
    else if (QString::compare(butVal, "+", Qt::CaseInsensitive) == 0)
        calcVal += '+';
    else if (QString::compare(butVal, "-", Qt::CaseInsensitive) == 0)
        calcVal += '-';
    else if (QString::compare(butVal, "0", Qt::CaseInsensitive) == 0)
        calcVal += '0';
    else if (QString::compare(butVal, "1", Qt::CaseInsensitive) == 0)
        calcVal += '1';
    else if (QString::compare(butVal, "2", Qt::CaseInsensitive) == 0)
        calcVal += '2';
    else if (QString::compare(butVal, "3", Qt::CaseInsensitive) == 0)
        calcVal += '3';
    else if (QString::compare(butVal, "4", Qt::CaseInsensitive) == 0)
        calcVal += '4';
    else if (QString::compare(butVal, "5", Qt::CaseInsensitive) == 0)
        calcVal += '5';
    else if (QString::compare(butVal, "6", Qt::CaseInsensitive) == 0)
        calcVal += '6';
    else if (QString::compare(butVal, "7", Qt::CaseInsensitive) == 0)
        calcVal += '7';
    else if (QString::compare(butVal, "8", Qt::CaseInsensitive) == 0)
        calcVal += '8';
    else if (QString::compare(butVal, "9", Qt::CaseInsensitive) == 0)
        calcVal += '9';
    ui->display->setText(QString::fromStdString(calcVal));
}

void Jcal::EqualButton()
{
    std::string postfix_result = stack::infix2postfix(calcVal);
    std::string calcVal = stack::calculate_postfix(postfix_result);
    ui->display->setText(QString::fromStdString(calcVal));
    calcVal.clear();
}
