#ifndef MYACCOUNTSCREEN_H
#define MYACCOUNTSCREEN_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QComboBox>
#include "Nurse.h"
#include "mymainscreen.h"
#include <QTableWidget>

class MyAccountScreen : public QWidget
{
    Q_OBJECT

    public:
    MyAccountScreen(Nurse* user, MyMainScreen* main);
    void UpdateFriendsWidgets();

    public slots:
    void Logout();
    void ChangeVacations();
    void UpdateFriendship1(int newindex);
    void UpdateFriendship2(int newindex);
    void UpdateFriendship3(int newindex);
    void RemoveVacations();
    void ChangePassword();
    void ChangeProfilePicture();

    public:

    Nurse* user_;

    QTableWidget* vacationstable_;
    MyMainScreen* mainscreen_;
    QPushButton* changevacationsbutton_;
    QPushButton* removevacationsbutton_;//
    QPushButton* changefriendsbutton_;
    QComboBox* friend1_;
    QComboBox* friend2_;
    QComboBox* friend3_;

    QVBoxLayout* friendsbox_;
    QVBoxLayout* auxiliarybox2_;
    QVBoxLayout* changevacationsbox_;//
    QLabel* namebox_;
    QLabel* myaccountbox_;
    QPushButton* logoutbutton_;
    QPushButton* changepasswordbutton_;
    QPushButton* changeprofilepicturebutton_;//
    QGridLayout* layout_;

};

#endif // MYACCOUNTSCREEN_H
