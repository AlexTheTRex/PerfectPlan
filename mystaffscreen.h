#ifndef MYSTAFFSCREEN_H
#define MYSTAFFSCREEN_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTableWidget>
#include "mymainscreen.h"
#include "myaccountscreen.h"

class MyStaffScreen : public QWidget
{
    Q_OBJECT

public:
    MyStaffScreen(MyMainScreen* main);

    MyMainScreen* mainscreen_;
    QVBoxLayout* layout_;
    QTableWidget* table_;
    QPushButton* gobackbutton_;

public slots:
    void GoBack();
};

#endif // MYSTAFFSCREEN_H
