#ifndef MYLOGINSCREEN_H
#define MYLOGINSCREEN_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QRadioButton>
#include "mymainscreen.h"

class MyLoginScreen : public QWidget
{
    Q_OBJECT

    public:
    MyLoginScreen(MyMainScreen* main);
    int CheckFormValidity();

    public slots:
    void CloseLoginWindow();
    void GoToAccountScreen();

    private:
    MyMainScreen* mainscreen_;

    QPushButton* okbutton_;
    QPushButton* cancelbutton_;
    QHBoxLayout* buttonslayout_;

    QLineEdit* firstname_;
    QLineEdit* familyname_;
    QLineEdit* password_;

    QFormLayout* myform_;
    QVBoxLayout* biglayout_;

};

#endif // MYLOGINSCREEN_H
