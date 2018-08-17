#ifndef MYOPTIONSSCREEN_H
#define MYOPTIONSSCREEN_H
#include <QPushButton>
#include <QWidget>
#include <QLayout>
#include "mymainscreen.h"
#include "myplanninghistorywindow.h"

class MyOptionsScreen : public QWidget
{
    Q_OBJECT

public:
    MyOptionsScreen(MyMainScreen* main);
public slots:
    void CloseOptionsWindow();
    void SeeStaff();
    void DeleteAccount();
    void SeeHistory();
    void SwitchSlots();
    void ExportPlanning();
private:
    MyMainScreen* mainscreen_;
    QVBoxLayout* layout_;
    QPushButton* seestaffbutton_;
    QPushButton* switchslotsbutton_;
    QPushButton* seehistorybutton_;
    QPushButton* deleteanaccountbuton_;
    QPushButton* cancelbutton_;
    QPushButton* printplanningbutton_;
};

#endif // MYOPTIONSSCREEN_H
