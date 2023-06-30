#include "jcal.h"
#include "./ui_jcal.h"

Jcal::Jcal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Jcal)
{
    ui->setupUi(this);
}

Jcal::~Jcal()
{
    delete ui;
}

