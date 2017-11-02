#ifndef MYCHANGEPASSWORDSCREEN_H
#define MYCHANGEPASSWORDSCREEN_H


#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QRadioButton>
#include "myaccountscreen.h"

class MyChangePasswordScreen : public QWidget
{
    Q_OBJECT

    public:
    MyChangePasswordScreen(MyAccountScreen* accountscreen_);
    int CheckFormValidity();

    public slots:
    void Cancel();
    void GoToAccountScreen();

    private:
    MyAccountScreen* accountscreen_;

    QPushButton* okbutton_;
    QPushButton* cancelbutton_;
    QHBoxLayout* buttonslayout_;

    QLineEdit* currentPassword_;
    QLineEdit* newpassword_;
    QLineEdit* confirmnewpassword_;

    QFormLayout* myform_;
    QVBoxLayout* biglayout_;

};
#endif // MYCHANGEPASSWORDSCREEN_H
