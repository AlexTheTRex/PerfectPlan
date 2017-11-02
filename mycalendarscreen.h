#ifndef MYCALENDARSCREEN_H
#define MYCALENDARSCREEN_H


#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QRadioButton>
#include "myaccountscreen.h"
#include <QCalendarWidget>
#include <QDate>

class MyCalendarScreen : public QWidget
{
    Q_OBJECT

    public:
    MyCalendarScreen(MyAccountScreen* account);

    public slots:
    void Add();
    void Cancel();

    private:
    MyAccountScreen* accountscreen_;

    QPushButton* okbutton_;
    QPushButton* cancelbutton_;
    QHBoxLayout* buttonslayout_;

    QCalendarWidget* calendar_;

    QVBoxLayout* biglayout_;

};

#endif // MYCALENDARSCREEN_H
