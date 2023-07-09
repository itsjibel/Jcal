#ifndef JCAL_H
#define JCAL_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Jcal; }
QT_END_NAMESPACE

class Jcal : public QMainWindow
{
    Q_OBJECT

protected:
    bool eventFilter(QObject* object, QEvent* event);

public:
    Jcal(QWidget *parent = nullptr);
    ~Jcal();

private:
    Ui::Jcal *ui;
    std::string calcVal;
    bool validToCal(std::string& expression);
    void replaceKeywords(std::string& str);

private slots:
    void ButtonPressed();
    void EqualButton();
};
#endif
