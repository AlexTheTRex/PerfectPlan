#ifndef MYCREATEACCOUNTSCREEN_H
#define MYCREATEACCOUNTSCREEN_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include "mymainscreen.h"
#include "myaccountscreen.h"

class MyCreateAccountScreen : public QWidget
{
    Q_OBJECT

    public:
    MyCreateAccountScreen(MyMainScreen* mainscreen);
    int CheckFormValidity();

    public slots:
    void CloseCreateAccountWindow();
    void GoToAccountScreen();

    private:
    MyMainScreen* mainscreen_;

    QPushButton* okbutton_;
    QPushButton* cancelbutton_;
    QHBoxLayout* buttonslayout_;

    QLineEdit* firstname_;
    QLineEdit* familyname_;
    QLineEdit* password_;
    QLineEdit* confirmpassword_;

    QVBoxLayout* radiobuttons_;
    QRadioButton* ide_;
    QRadioButton* as_;
    QButtonGroup* jobgroup_;

    QVBoxLayout* teamradiobuttons_;
    QRadioButton* zero_;
    QRadioButton* one_;
    QButtonGroup* teamgroup_;

    QFormLayout* myform_;
    QVBoxLayout* biglayout_;

};




#endif // MYCREATEACCOUNTSCREEN_H
