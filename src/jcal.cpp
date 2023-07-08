#include "jcal.h"
#include "./ui_jcal.h"
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
    connect(ui->AC, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->open_parenthesis, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->closed_parenthesis, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->dot, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->power, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->squred, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->cube, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->sqrt, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->mod, SIGNAL(released()), this, SLOT(ButtonPressed()));
    connect(ui->equal, SIGNAL(released()), this, SLOT(EqualButton()));
}

Jcal::~Jcal()
{
    delete ui;
}

void Jcal::ButtonPressed()
{
    if (ui->display->text() == "Malformed expression")
        ui->display->setText(QString::fromStdString(""));
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
    else if (QString::compare(butVal, "(", Qt::CaseInsensitive) == 0)
        calcVal += '(';
    else if (QString::compare(butVal, ")", Qt::CaseInsensitive) == 0)
        calcVal += ')';
    else if (QString::compare(butVal, ".", Qt::CaseInsensitive) == 0)
        calcVal += '.';
    else if (QString::compare(butVal, "%", Qt::CaseInsensitive) == 0)
        calcVal += '%';
    else if (QString::compare(butVal, "X^y", Qt::CaseInsensitive) == 0)
        calcVal += '^';
    else if (QString::compare(butVal, "X^2", Qt::CaseInsensitive) == 0)
        calcVal += "^2";
    else if (QString::compare(butVal, "X^3", Qt::CaseInsensitive) == 0)
        calcVal += "^3";
    else if (QString::compare(butVal, "sqrt", Qt::CaseInsensitive) == 0)
        calcVal += 's';
    else if (QString::compare(butVal, "AC", Qt::CaseInsensitive) == 0)
        calcVal.clear();

    ui->display->setText(QString::fromStdString(calcVal));
}

void Jcal::EqualButton()
{
    if (ui->display->text() == "Malformed expression")
        ui->display->setText(QString::fromStdString(""));
    QString displayVal = ui->display->text();
    calcVal = displayVal.toStdString();
    if (validToCal(calcVal))
    {
        std::string postfix_result = stack::infix2postfix(calcVal);
        calcVal = stack::calculate_postfix(postfix_result);
        ui->display->setText(QString::fromStdString(calcVal));
    } else
        ui->display->setText(QString::fromStdString("Malformed expression"));
}

bool Jcal::validToCal(std::string& expression)
{
    bool allof_characters_are_valid = true, locationof_characters_are_valid = true;
    long int numof_diffof_parenthesis {0};

    for (long int i{0}; i<expression.size(); i++)
    {
        if (expression[i] != '(' && expression[i] != ')' && expression[i] != '.' && !(expression[i] >= '0' && expression[i] <= '9') && expression[i] != '-' && expression[i] != '+' && expression[i] != '*' && expression[i] != '/' && expression[i] != '%' && expression[i] != '^' && expression[i] != 's') {
            allof_characters_are_valid = false;
            break;
        } else if (expression[i] == '(') {
            numof_diffof_parenthesis++;
            if (i > 0)
                if ((expression[i-1] >= '0' && expression[i-1] <= '9') || expression[i-1] == ')' || expression[i+1] == '*' || expression[i+1] == '/' || expression[i+1] == '%' || expression[i+1] == ')' || expression[i+1] == '^')
                {
                    locationof_characters_are_valid = false;
                    break;
                }
        } else if (expression[i] == ')') {
            numof_diffof_parenthesis--;
            if (i > 0)
                if ((expression[i+1] >= '0' && expression[i+1] <= '9') || expression[i+1] == '(' || expression[i-1] == '+' || expression[i-1] == '-' || expression[i-1] == '*' || expression[i-1] == '/' || expression[i-1] == '%' || expression[i-1] == '(' || expression[i-1] == '^' || expression[i-1] == 's')
                {
                    locationof_characters_are_valid = false;
                    break;
                }
        } else if (expression[i] == '.') {
            if (i > 0)
                if (!(expression[i+1] >= '0' && expression[i+1] <= '9') && (expression[i-1] >= '0' && expression[i-1] <= '9'))
                {
                    locationof_characters_are_valid = false;
                    break;
                }
        } else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '%' || expression[i] == '^') {
            if (i > 0)
                if (expression[i-1] == '+' || expression[i-1] == '-' || expression[i-1] == '*' || expression[i-1] == '/' || expression[i-1] == '%' || expression[i-1] == '^' || expression[i+1] == '+' || expression[i+1] == '-' || expression[i+1] == '*' || expression[i+1] == '/' || expression[i+1] == '%' || expression[i+1] == '^')
                {
                    locationof_characters_are_valid = false;
                    break;
                }
        } else if (expression[i] == 's') {
            if (i > 0)
                if ((expression[i-1] >= '0' && expression[i-1] <= '9') || expression[i+1] == '+' || expression[i+1] == '-' || expression[i+1] == '*' || expression[i+1] == '/' || expression[i+1] == '%' || expression[i+1] == '^')
                {
                    locationof_characters_are_valid = false;
                    break;
                }
        }
    }

    if (expression.back() == '+' || expression.back() == '-' || expression.back() == '*' || expression.back() == '/' || expression.back() == '%' || expression.back() == '^' || expression.back() == '.' || expression.back() == 's' ||
        expression.front() == '*' || expression.front() == '/' || expression.front() == '%' || expression.front() == '^')
        locationof_characters_are_valid = false;

    return numof_diffof_parenthesis == 0 && allof_characters_are_valid && locationof_characters_are_valid;
}
