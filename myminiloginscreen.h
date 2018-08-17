#ifndef MYMINILOGINSCREEN_H
#define MYMINILOGINSCREEN_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QRadioButton>
#include <QCalendarWidget>
#include "mymainscreen.h"
#include "myconfirmplanningscreen.h"

class MyMiniLoginScreen : public QWidget
{
    Q_OBJECT

    public:
    MyMiniLoginScreen(MyConfirmPlanningScreen* confirm);
    int CheckFormValidity();

    public slots:
    void CloseWindow();
    void Confirm();

    private:
    MyConfirmPlanningScreen* confirmscreen_;

    QPushButton* okbutton_;
    QPushButton* cancelbutton_;
    QHBoxLayout* buttonslayout_;

    QLineEdit* firstname_;
    QLineEdit* familyname_;

    QFormLayout* myform_;
    QCalendarWidget* calendar_;
    QVBoxLayout* biglayout_;
};

#endif // MYMINILOGINSCREEN_H
