#ifndef JCAL_H
#define JCAL_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Jcal; }
QT_END_NAMESPACE

class Jcal : public QMainWindow
{
    Q_OBJECT

public:
    Jcal(QWidget *parent = nullptr);
    ~Jcal();

private:
    Ui::Jcal *ui;
};
#endif // JCAL_H