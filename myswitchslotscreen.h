#ifndef MYSWITCHSLOTSCREEN_H
#define MYSWITCHSLOTSCREEN_H

#include "mymainscreen.h"
#include <QFormLayout>
#include <QCalendarWidget>


class MySwitchSlotScreen : public QWidget
{
    Q_OBJECT

public:
    MySwitchSlotScreen(MyMainScreen* main);
    int CheckLeftFormValidity();
    int CheckRightFormValidity();

public slots:
    void CloseWindow();
    void SwitchSlots();
    void LeftCalendar();
    void RightCalendar();

public:
    MyMainScreen* mainscreen_;
    QVBoxLayout* biglayout_;
    QHBoxLayout* tophlayout_;
    QHBoxLayout* bottomhlayout_;
    QVBoxLayout* leftlayout_;
    QVBoxLayout* rightlayout_;
    QLabel* leftlabel_;
    QLabel* rightlabel_;
    QFormLayout* leftform_;
    QLineEdit* leftfirstname_;
    QLineEdit* leftfamilyname_;
    QLineEdit* leftpassword_;
    QDate leftdate_;
    QFormLayout* rightform_;
    QLineEdit* rightfirstname_;
    QLineEdit* rightfamilyname_;
    QLineEdit* rightpassword_;
    QDate rightdate_;
    QPushButton* leftcalendarbutton_;
    QPushButton* rightcalendarbutton_;
    QLabel* leftcalendarlabel_;
    QLabel* rightcalendarlabel_;
    QPushButton* okbutton_;
    QPushButton* cancelbutton_;
    bool choosingleftdate_;

};

#endif // MYSWITCHSLOTSCREEN_H
