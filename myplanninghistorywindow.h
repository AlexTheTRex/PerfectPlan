#ifndef MYPLANNINGHISTORYWINDOW_H
#define MYPLANNINGHISTORYWINDOW_H

#include "mymainscreen.h"
#include "planningentry.h"

class MyPlanningHistoryWindow : public QWidget
{
    Q_OBJECT

public:
    MyPlanningHistoryWindow(MyMainScreen* main);
    void Display();

public slots:
    void GoBack();
    void NextWeek();
    void PreviousWeek();

private:
    MyMainScreen* mainscreen_;
    QGridLayout* layout_;
    QTableWidget* table_;
    QPushButton* nextbutton_;
    QPushButton* previousbutton_;
    QPushButton* quitbutton_;
    QDate mondayofcurrentweek_;
    QLabel* textcontainer_;

};

#endif // MYPLANNINGHISTORYWINDOW_H
