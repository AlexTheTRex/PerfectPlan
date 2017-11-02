#ifndef MYOPTIONSSCREEN_H
#define MYOPTIONSSCREEN_H
#include <QPushButton>
#include <QWidget>
#include <QLayout>
#include "mymainscreen.h"

class MyOptionsScreen : public QWidget
{
    Q_OBJECT

public:
    MyOptionsScreen(MyMainScreen* main);
public slots:
    void CloseOptionsWindow();
    void SeeStaff();
    void DeleteAccount();
private:
    MyMainScreen* mainscreen_;
    QVBoxLayout* layout_;
    QPushButton* seestaffbutton_;
    QPushButton* seehistorybutton_;
    QPushButton* deleteanaccountbuton_;
    QPushButton* cancelbutton_;
};

#endif // MYOPTIONSSCREEN_H
