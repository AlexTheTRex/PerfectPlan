#ifndef MYDELETEACCOUNTSCREEN_H
#define MYDELETEACCOUNTSCREEN_H


#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QRadioButton>
#include "mymainscreen.h"
#include "myaccountscreen.h"

class MyDeleteAccountScreen : public QWidget
{
    Q_OBJECT

    public:
    MyDeleteAccountScreen(MyMainScreen* mainscreen_);
    int CheckFormValidity();

    public slots:
    void CloseWindow();
    void GoToMainScreen();

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

#endif // MYDELETEACCOUNTSCREEN_H
