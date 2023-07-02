#include "jcal.h"
#include "./ui_jcal.h"

double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;


Jcal::Jcal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Jcal)
{
    ui->setupUi(this);

    ui->display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];
    for (int i=0; i<10; ++i)
    {
        QString butName = "Button" + QString::number(i);
        numButtons[i] = Jcal::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->minus, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->equal, SIGNAL(released()), this, SLOT(EqualButton()));
}

Jcal::~Jcal()
{
    delete ui;
}

void Jcal::NumPressed()
{
    QPushButton *button = (QPushButton *)sender();
    QString butval = button->text();
    QString displayVal = ui->display->text();
    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0))
    {
        ui->display->setText(butval);
    } else {
        QString newVal = displayVal + butval;
        double dblNewVal = newVal.toDouble();
        ui->display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Jcal::MathButtonPressed()
{
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    QString displayVal = ui->display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if (QString::compare(butVal, "/", Qt::CaseInsensitive) == 0)
    {
        divTrigger = true;
    } else if (QString::compare(butVal, "*", Qt::CaseInsensitive) == 0) {
        multTrigger = true;
    } else if (QString::compare(butVal, "+", Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    } else {
        subTrigger = true;
    }
    ui->display->setText("");
}

void Jcal::EqualButton()
{
    double solution = 0.0;
    QString displayVal = ui->display->text();
    double dblDisplayVal = displayVal.toDouble();
    if (addTrigger || subTrigger || multTrigger || divTrigger)
    {
        if (addTrigger)
        {
            solution = calcVal + dblDisplayVal;
        } else if (subTrigger) {
            solution = calcVal - dblDisplayVal;
        } else if (multTrigger) {
            solution = calcVal * dblDisplayVal;
        } else {
            solution = calcVal / dblDisplayVal;
        }
    }
    ui->display->setText(QString::number(solution));
}
