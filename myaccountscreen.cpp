
#include "myaccountscreen.h"
#include <QFont>
#include <string>
#include <iostream>
#include <QHeaderView>
#include <QFileDialog>
#include <QDir>
#include <QPainter>
#include <QBitmap>
#include "parameters.h"
#include "mycalendarscreen.h"
#include "mychangepasswordscreen.h"

MyAccountScreen::MyAccountScreen(Nurse* user, MyMainScreen* main) : QWidget() {

    this->user_ = user;
    this->mainscreen_ = main;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->changevacationsbutton_ = new QPushButton;
    this->changefriendsbutton_ = new QPushButton;
    this->changeprofilepicturebutton_ = new QPushButton;
    this->auxiliarybox2_ = new QVBoxLayout;
    this->changevacationsbox_ = new QVBoxLayout;
    this->removevacationsbutton_ = new QPushButton;
    this->namebox_ = new QLabel;
    this->myaccountbox_ = new QLabel;
    this->changepasswordbutton_ = new QPushButton;
    this->logoutbutton_ = new QPushButton;
    this->layout_ = new QGridLayout;
    this->friendsbox_ = new QVBoxLayout;

    this->friend1_ = new QComboBox();
    this->friend2_ = new QComboBox();
    this->friend3_ = new QComboBox();
    int friendid1 = -1, friendid2 = -1, friendid3 = -1;
    if (user->GetFirstFriend() != NULL){
        this->friend1_->addItem(QString::fromStdString(user->GetFirstFriend()->GetFirstName() +
                                                       " " + user->GetFirstFriend()->GetSurname()));
        friendid1 = user->GetFirstFriend()->GetId();
    } else{
        this->friend1_->addItem("-----AUCUN-----");
    }
    if (user->GetSecondFriend() != NULL){
        this->friend2_->addItem(QString::fromStdString(user->GetSecondFriend()->GetFirstName() +
                                                       " " + user->GetSecondFriend()->GetSurname()));
        friendid2 = user->GetSecondFriend()->GetId();
    } else {
        this->friend2_->addItem("-----AUCUN-----");
    }
    if (user->GetThirdFriend() != NULL){
        this->friend3_->addItem(QString::fromStdString(user->GetThirdFriend()->GetFirstName() +
                                                       " " + user->GetThirdFriend()->GetSurname()));
        friendid3 = user->GetThirdFriend()->GetId();
    } else {
        this->friend3_->addItem("-----AUCUN-----");
    }
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (i != friendid1 && i != friendid2 && i != friendid3 && i != user->GetId()){
            this->friend1_->addItem(QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName() + " "
                                                           + this->mainscreen_->engine_->staff_[i].GetSurname()));
        }
        if (i != friendid1 && i != friendid2 && i != friendid3 && i != user->GetId()){
            this->friend2_->addItem(QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName() + " "
                                                           + this->mainscreen_->engine_->staff_[i].GetSurname()));
        }
        if (i != friendid1 && i != friendid2 && i != friendid3 && i != user->GetId()){
            this->friend3_->addItem(QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName() + " "
                                                           + this->mainscreen_->engine_->staff_[i].GetSurname()));
        }
    }
    if (friendid1 >= 0){
        this->friend1_->addItem("-----AUCUN-----");
    }
    if (friendid2 >= 0){
        this->friend2_->addItem("-----AUCUN-----");
    }
    if (friendid3 >= 0){
        this->friend3_->addItem("-----AUCUN-----");
    }



    QLabel* myfriends = new QLabel(this);
    myfriends->setText("  Mes amis");
    this->layout_->addWidget(myfriends, 3, 2, 1, 1);
    myfriends->setMaximumSize(130, 300);
    myfriends->setFont(QFont("Arial", 20));//, QFont::Bold));
    QLabel* myvacations = new QLabel(this);
    myvacations->setText("Mes jours de \n   vacances");
    this->layout_->addWidget(myvacations, 5, 2, 1, 1);
    myvacations->setMaximumSize(150, 320);
    myvacations->setFont(QFont("Arial", 20));//, QFont::Bold));

    this->changefriendsbutton_->setText("Faire un bisou à mes amis");

    this->changevacationsbutton_->setText("Ajouter un jour de vacances");
    this->changevacationsbutton_->setMaximumHeight(50);
    this->removevacationsbutton_->setText("Supprimer des jours de vacances");
    this->removevacationsbutton_->setMaximumHeight(50);
    this->changevacationsbox_->addWidget(this->changevacationsbutton_);
    this->changevacationsbox_->addWidget(this->removevacationsbutton_);

    std::string job = "IDE";
    if (user->IsAs()){
        job = "AS";
    }
    this->namebox_->setText(QString::fromStdString(std::string(user->GetFirstName() +std::string(1, '\n') +
                                                               user->GetSurname() + std::string(1, '\n') + job)));
    this->namebox_->setAlignment(Qt::AlignCenter);
    this->layout_->addWidget(this->namebox_, 1, 1, 1, 1);
    this->changepasswordbutton_->setText("Modifier mon mot de passe");
    this->changepasswordbutton_->setMaximumHeight(50);
    this->auxiliarybox2_->addWidget(this->changepasswordbutton_);
    this->changeprofilepicturebutton_ ->setText("Modifier ma photo de profil");
    this->changeprofilepicturebutton_ ->setMaximumHeight(50);
    this->auxiliarybox2_->addWidget(this->changeprofilepicturebutton_);
    this->logoutbutton_->setText("Se déconnecter");
    this->logoutbutton_->setMaximumHeight(50);
    this->auxiliarybox2_->addWidget(this->logoutbutton_);
    this->layout_->addLayout(this->auxiliarybox2_, 5, 1, 1, 1);
    this->layout_->addWidget(myaccountbox_, 1, 3, 1, 1);
    this->layout_->addWidget(changefriendsbutton_, 3, 4, 1, 1);
    this->layout_->addLayout(changevacationsbox_, 5, 4, 1, 1);
    this->friendsbox_->addWidget(this->friend1_);//, 2, 3, 1, 1);
    this->friendsbox_->addWidget(this->friend2_);//, 3, 3, 1, 1);
    this->friendsbox_->addWidget(this->friend3_);//, 4, 3, 1, 1);
    this->layout_->addLayout(this->friendsbox_, 3, 3, 1, 1);
    this->myaccountbox_->setText("Mon compte");
    this->myaccountbox_->setAlignment(Qt::AlignCenter);
    this->myaccountbox_->setMaximumSize(250, 50);
    this->myaccountbox_->setFont(QFont("Arial", 40));//, QFont::Bold));
    this->namebox_->setFont(QFont("Arial", 22));
    this->namebox_->setMaximumSize(200, 100);
    //this->changevacationsbutton_->setMaximumSize(400, 100);
    this->changefriendsbutton_->setMaximumSize(400, 100);

    QLabel* imagecontainer = new QLabel;
    imagecontainer->setAlignment(Qt::AlignRight);
    QPixmap* profilepicture;
    if (this->user_->GetPathToProfilePic().length() == 0){
        profilepicture = new QPixmap(PATH_TO_DEFAULT_PROFILE_PIC);
    } else {
        QPixmap original(QString::fromStdString(this->user_->GetPathToProfilePic()));

        QRect crop(0, (original.height() - original.width())/5,
                   original.width(), original.width());
        original = original.copy(crop);

        //profilepicture = new QPixmap(QString::fromStdString(this->user_->GetPathToProfilePic()));
        // Draw the mask.
        QBitmap  mask(original.size());
        QPainter painter(&mask);
        mask.fill(Qt::white);
        painter.setBrush(Qt::black);
        painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), mask.width()/2, mask.height()/2);
        // Draw the final image.
        original.setMask(mask);

        original = original.scaled(ACCOUNT_PROFILE_PICTURE_SIZE, ACCOUNT_PROFILE_PICTURE_SIZE,
                                   Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        profilepicture = new QPixmap(original);
    }
    imagecontainer->setPixmap((*profilepicture));//.scaled(200, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    this->layout_->addWidget(imagecontainer, 1, 4);

    //Remove any vacation day that is earlier than today
    for (int n = 0; n < this->mainscreen_->engine_->staff_.size(); ++n){
        for (int i = this->mainscreen_->engine_->staff_[n].vacationdays_.size() - 1; i >= 0; --i){
            if (this->mainscreen_->engine_->staff_[n].vacationdays_[i] < QDate::currentDate()){
                this->mainscreen_->engine_->staff_[n].vacationdays_.erase(
                            this->mainscreen_->engine_->staff_[n].vacationdays_.begin() + i);
            }
        }
    }
    //Build the vacations table
    this->vacationstable_ = new QTableWidget(user->vacationdays_.size(), 2, this);
    this->vacationstable_->setHorizontalHeaderItem(0, new QTableWidgetItem("Date"));
    this->vacationstable_->setHorizontalHeaderItem(1, new QTableWidgetItem("Jour de la semaine"));
    for (int c = 0; c < this->vacationstable_->horizontalHeader()->count(); ++c){
        this->vacationstable_->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    for (int i = 0; i < user->vacationdays_.size(); ++i){
        QTableWidgetItem* item0 = new QTableWidgetItem(user->vacationdays_[i].toString("dd.MM.yyyy"));
        item0->setFlags(item0->flags() ^ Qt::ItemIsEditable);
        this->vacationstable_->setItem(i, 0, item0);
        QString jour;
        if (user->vacationdays_[i].dayOfWeek() == 1){
            jour = "Lundi";
        }
        if (user->vacationdays_[i].dayOfWeek() == 2){
            jour = "Mardi";
        }
        if (user->vacationdays_[i].dayOfWeek() == 3){
            jour = "Mercredi";
        }
        if (user->vacationdays_[i].dayOfWeek() == 4){
            jour = "Jeudi";
        }
        if (user->vacationdays_[i].dayOfWeek() == 5){
            jour = "Vendredi";
        }
        if (user->vacationdays_[i].dayOfWeek() == 6){
            jour = "Samedi";
        }
        if (user->vacationdays_[i].dayOfWeek() == 7){
            jour = "Dimanche";
        }
        QTableWidgetItem* item1 = new QTableWidgetItem(jour);
        item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
        this->vacationstable_->setItem(i, 1, item1);
    }
    for (int i = 0; i < this->vacationstable_->verticalHeader()->count(); ++i){
        this->vacationstable_->setVerticalHeaderItem(i, new QTableWidgetItem(""));
    }
    this->vacationstable_->setMaximumHeight(150);
    this->vacationstable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->layout_->addWidget(this->vacationstable_, 5, 3, 1, 1);

    this->setLayout(this->layout_);

    QObject::connect(this->logoutbutton_, SIGNAL(clicked()), this, SLOT(Logout()));
    QObject::connect(this->changevacationsbutton_, SIGNAL(clicked()), this, SLOT(ChangeVacations()));
    QObject::connect(this->removevacationsbutton_, SIGNAL(clicked()), this, SLOT(RemoveVacations()));
    QObject::connect(this->changepasswordbutton_, SIGNAL(clicked()), this, SLOT(ChangePassword()));
    QObject::connect(this->changeprofilepicturebutton_, SIGNAL(clicked()), this, SLOT(ChangeProfilePicture()));
    QObject::connect(this->friend1_, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFriendship1(int)));
    QObject::connect(this->friend2_, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFriendship2(int)));
    QObject::connect(this->friend3_, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFriendship3(int)));

}

void MyAccountScreen::Logout(){
    this->mainscreen_->engine_->SaveStaff();
    this->close();
    this->mainscreen_->showMaximized();
    delete this;
}

void MyAccountScreen::ChangeProfilePicture(){
    QString fichier = QFileDialog::getOpenFileName(this, "Sélectionner une image",
                                                   QDir::home().absolutePath(), "Images (*.png *.gif *.jpg *.jpeg)");
    if (fichier.length() > 0){
        this->user_->pathtoprofilepic_ = fichier.toStdString();
        MyAccountScreen* newaccountscreen = new MyAccountScreen(this->user_, this->mainscreen_);
        newaccountscreen->showMaximized();
        this->close();
        delete this;
    }
}

void MyAccountScreen::ChangeVacations(){
    MyCalendarScreen* calendarscreen = new MyCalendarScreen(this);
    this->setEnabled(false);
    calendarscreen->show();
}

void MyAccountScreen::RemoveVacations(){
    QModelIndexList selection = this->vacationstable_->selectionModel()->selectedRows();
    if (selection.count() == 0){
        QMessageBox::information(this, "Sélection vide", "Pour supprimer des jours de congés,"
                                  "veuillez sélectionner un ou plusieurs jours dans le tableau des jours de vacances.");
        return;
    }
    for (int i = 0; i < selection.count(); ++i){
        this->mainscreen_->engine_->DeleteVacationDay(this->user_->GetId(), selection.at(i).row() - i);
    }
    MyAccountScreen* newaccountscreen_ = new MyAccountScreen(this->user_, this->mainscreen_);
    newaccountscreen_->showMaximized();
    this->close();
    delete this;
}

void MyAccountScreen::ChangePassword(){
    MyChangePasswordScreen* changepwscreen = new MyChangePasswordScreen(this);
    this->setEnabled(false);
    changepwscreen->show();
}


//void MyAccountScreen::UpdateFriendship1(int newindex){
//    if (this->friend1_->currentText().toStdString() == "-----AUCUN-----"){
//        std::cout << "NULL" << std::endl;
//        this->user_->SetFirstFriend(NULL);
//        return;
//    }
//    int newfriendid = newindex;
//    //Have to infer the actual new friend ID from the selected index.
//    //Is slighty tricky because the user and its current friend are not in the list.
//    if (this->user_->GetFirstFriend() == NULL){
//        --newfriendid;
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//    } else {
//        if (newfriendid <= this->user_->GetFirstFriend()->GetId()){
//            --newfriendid;
//        }
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//    }
//    this->user_->SetFirstFriend(&(this->mainscreen_->engine_->staff_[newfriendid]));
//}
void MyAccountScreen::UpdateFriendsWidgets(){

    this->friend1_->disconnect();
    this->friend2_->disconnect();
    this->friend3_->disconnect();
    this->friend1_->clear();
    this->friend2_->clear();
    this->friend3_->clear();
    int friendid1 = -1, friendid2 = -1, friendid3 = -1;
    if (user_->GetFirstFriend() != NULL){
        this->friend1_->addItem(QString::fromStdString(user_->GetFirstFriend()->GetFirstName() +
                                                       " " + user_->GetFirstFriend()->GetSurname()));
        friendid1 = user_->GetFirstFriend()->GetId();
    } else{
        this->friend1_->addItem("-----AUCUN-----");
    }
    if (user_->GetSecondFriend() != NULL){
        this->friend2_->addItem(QString::fromStdString(user_->GetSecondFriend()->GetFirstName() +
                                                       " " + user_->GetSecondFriend()->GetSurname()));
        friendid2 = user_->GetSecondFriend()->GetId();
    } else {
        this->friend2_->addItem("-----AUCUN-----");
    }
    if (user_->GetThirdFriend() != NULL){
        this->friend3_->addItem(QString::fromStdString(user_->GetThirdFriend()->GetFirstName() +
                                                       " " + user_->GetThirdFriend()->GetSurname()));
        friendid3 = user_->GetThirdFriend()->GetId();
    } else {
        this->friend3_->addItem("-----AUCUN-----");
    }
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (i != friendid1 && i != friendid2 && i != friendid3 && i != user_->GetId()){
            this->friend1_->addItem(QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName() + " "
                                                           + this->mainscreen_->engine_->staff_[i].GetSurname()));
        }
        if (i != friendid1 && i != friendid2 && i != friendid3 && i != user_->GetId()){
            this->friend2_->addItem(QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName() + " "
                                                           + this->mainscreen_->engine_->staff_[i].GetSurname()));
        }
        if (i != friendid1 && i != friendid2 && i != friendid3 && i != user_->GetId()){
            this->friend3_->addItem(QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName() + " "
                                                           + this->mainscreen_->engine_->staff_[i].GetSurname()));
        }
    }
    if (friendid1 >= 0){
        this->friend1_->addItem("-----AUCUN-----");
    }
    if (friendid2 >= 0){
        this->friend2_->addItem("-----AUCUN-----");
    }
    if (friendid3 >= 0){
        this->friend3_->addItem("-----AUCUN-----");
    }

    QObject::connect(this->friend1_, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFriendship1(int)));
    QObject::connect(this->friend2_, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFriendship2(int)));
    QObject::connect(this->friend3_, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFriendship3(int)));
}


void MyAccountScreen::UpdateFriendship1(int newindex){
    if (this->friend1_->currentText().toStdString() == "-----AUCUN-----"){
        this->user_->SetFirstFriend(NULL);
        this->UpdateFriendsWidgets();
        return;
    }
    std::string txt = this->friend1_->currentText().toStdString();
    int seconduppercase = 0;
    int firstuppercase = 0;
    bool found = false;
    for (int i = 1; i < txt.length() && found == false; ++i){
        if (txt.at(i) < 91 && txt.at(i) >= 65){
            if (firstuppercase == 0){
                firstuppercase = i;
            } else {
                found = true;
                seconduppercase = i;
            }
        }
        if (txt.at(i) < 123 && txt.at(i) >= 97){
            firstuppercase = 0;
        }
    }
    std::string familyname = txt.substr(firstuppercase, txt.length() - firstuppercase);
    std::string firstname = txt.substr(0, firstuppercase - 1);
    int newid;
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower() ==
                QString::fromStdString(firstname).toLower() &&
                QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetSurname()).toUpper() ==
                QString::fromStdString(familyname).toUpper()){
            newid = i;
        }
    }
//    int newfriendid = newindex;
//    //Have to infer the actual new friend ID from the selected index.
//    //Is slighty tricky because the user and its current friend are not in the list.
//    if (this->user_->GetFirstFriend() == NULL){
//        --newfriendid;
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//        if (this->user_->GetSecondFriend() != NULL){
//            if (newfriendid >= this->user_->GetSecondFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//        if (this->user_->GetThirdFriend() != NULL){
//            if (newfriendid >= this->user_->GetThirdFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//    } else {
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//        if (newfriendid < this->user_->GetFirstFriend()->GetId()){
//            --newfriendid;
//        }
//        if (this->user_->GetSecondFriend() != NULL){
//            if (newfriendid >= this->user_->GetSecondFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//        if (this->user_->GetThirdFriend() != NULL){
//            if (newfriendid >= this->user_->GetThirdFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//    }
////    std::cout << newfriendid << std::endl;
    this->user_->SetFirstFriend(&(this->mainscreen_->engine_->staff_[newid]));
////    std::cout << "friend set" << std::endl;
    this->UpdateFriendsWidgets();
////    std::cout << "screen updated" << std::endl;
////    MyAccountScreen* newaccountscreen_ = new MyAccountScreen(this->user_, this->mainscreen_);
////    std::cout << "bew account screen created" << std::endl;
////    newaccountscreen_->showMaximized();
////    std::cout << "screen maximized" << std::endl;
////    this->close();
////    std::cout << "screen closed" << std::endl;
//    delete this;
//    std::cout << "screen deleted" << std::endl;
}

//void MyAccountScreen::UpdateFriendship2(int newindex){
//    if (this->friend2_->currentText().toStdString() == "-----AUCUN-----"){
//        this->user_->SetSecondFriend(NULL);
//        return;
//    }
//    int newfriendid = newindex;
//    //Have to infer the actual new friend ID from the selected index.
//    //Is slighty tricky because the user and its current friend are not in the list.
//    if (this->user_->GetSecondFriend() == NULL){
//        --newfriendid;
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//    } else {
//        if (newfriendid <= this->user_->GetSecondFriend()->GetId()){
//            --newfriendid;
//        }
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//    }
//    this->user_->SetSecondFriend(&(this->mainscreen_->engine_->staff_[newfriendid]));
//}

void MyAccountScreen::UpdateFriendship2(int newindex){
    if (this->friend2_->currentText().toStdString() == "-----AUCUN-----"){
        this->user_->SetSecondFriend(NULL);
        this->UpdateFriendsWidgets();
        return;
    }
    std::string txt = this->friend2_->currentText().toStdString();
    int seconduppercase = 0;
    int firstuppercase = 0;
    bool found = false;
    for (int i = 1; i < txt.length() && found == false; ++i){
        if (txt.at(i) < 91 && txt.at(i) >= 65){
            if (firstuppercase == 0){
                firstuppercase = i;
            } else {
                found = true;
                seconduppercase = i;
            }
        }
        if (txt.at(i) < 123 && txt.at(i) >= 97){
            firstuppercase = 0;
        }
    }
    std::string familyname = txt.substr(firstuppercase, txt.length() - firstuppercase);
    std::string firstname = txt.substr(0, firstuppercase - 1);
    int newid;
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower() ==
                QString::fromStdString(firstname).toLower() &&
                QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetSurname()).toUpper() ==
                QString::fromStdString(familyname).toUpper()){
            newid = i;
        }
    }
//    int newfriendid = newindex;
//    //Have to infer the actual new friend ID from the selected index.
//    //Is slighty tricky because the user and its current friend are not in the list.
//    if (this->user_->GetFirstFriend() == NULL){
//        --newfriendid;
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//        if (this->user_->GetSecondFriend() != NULL){
//            if (newfriendid >= this->user_->GetSecondFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//        if (this->user_->GetThirdFriend() != NULL){
//            if (newfriendid >= this->user_->GetThirdFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//    } else {
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//        if (newfriendid < this->user_->GetFirstFriend()->GetId()){
//            --newfriendid;
//        }
//        if (this->user_->GetSecondFriend() != NULL){
//            if (newfriendid >= this->user_->GetSecondFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//        if (this->user_->GetThirdFriend() != NULL){
//            if (newfriendid >= this->user_->GetThirdFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//    }
////    std::cout << newfriendid << std::endl;
    this->user_->SetSecondFriend(&(this->mainscreen_->engine_->staff_[newid]));
////    std::cout << "friend set" << std::endl;
    this->UpdateFriendsWidgets();
////    std::cout << "screen updated" << std::endl;
////    MyAccountScreen* newaccountscreen_ = new MyAccountScreen(this->user_, this->mainscreen_);
////    std::cout << "bew account screen created" << std::endl;
////    newaccountscreen_->showMaximized();
////    std::cout << "screen maximized" << std::endl;
////    this->close();
////    std::cout << "screen closed" << std::endl;
//    delete this;
//    std::cout << "screen deleted" << std::endl;
}

//void MyAccountScreen::UpdateFriendship3(int newindex){
//    if (this->friend3_->currentText().toStdString() == "-----AUCUN-----"){
//        this->user_->SetThirdFriend(NULL);
//        return;
//    }
//    int newfriendid = newindex;
//    //Have to infer the actual new friend ID from the selected index.
//    //Is slighty tricky because the user and its current friend are not in the list.
//    if (this->user_->GetThirdFriend() == NULL){
//        --newfriendid;
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//    } else {
//        if (newfriendid <= this->user_->GetThirdFriend()->GetId()){
//            --newfriendid;
//        }
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//    }
//    this->user_->SetThirdFriend(&(this->mainscreen_->engine_->staff_[newfriendid]));
//}

void MyAccountScreen::UpdateFriendship3(int newindex){
    if (this->friend1_->currentText().toStdString() == "-----AUCUN-----"){
        this->user_->SetThirdFriend(NULL);
        this->UpdateFriendsWidgets();
        return;
    }
    std::string txt = this->friend3_->currentText().toStdString();
    int seconduppercase = 0;
    int firstuppercase = 0;
    bool found = false;
    for (int i = 1; i < txt.length() && found == false; ++i){
        if (txt.at(i) < 91 && txt.at(i) >= 65){
            if (firstuppercase == 0){
                firstuppercase = i;
            } else {
                found = true;
                seconduppercase = i;
            }
        }
        if (txt.at(i) < 123 && txt.at(i) >= 97){
            firstuppercase = 0;
        }
    }
    std::string familyname = txt.substr(firstuppercase, txt.length() - firstuppercase);
    std::string firstname = txt.substr(0, firstuppercase - 1);
    int newid;
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower() ==
                QString::fromStdString(firstname).toLower() &&
                QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetSurname()).toUpper() ==
                QString::fromStdString(familyname).toUpper()){
            newid = i;
        }
    }
//    int newfriendid = newindex;
//    //Have to infer the actual new friend ID from the selected index.
//    //Is slighty tricky because the user and its current friend are not in the list.
//    if (this->user_->GetFirstFriend() == NULL){
//        --newfriendid;
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//        if (this->user_->GetSecondFriend() != NULL){
//            if (newfriendid >= this->user_->GetSecondFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//        if (this->user_->GetThirdFriend() != NULL){
//            if (newfriendid >= this->user_->GetThirdFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//    } else {
//        if (newfriendid >= this->user_->GetId()){
//            ++newfriendid;
//        }
//        if (newfriendid < this->user_->GetFirstFriend()->GetId()){
//            --newfriendid;
//        }
//        if (this->user_->GetSecondFriend() != NULL){
//            if (newfriendid >= this->user_->GetSecondFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//        if (this->user_->GetThirdFriend() != NULL){
//            if (newfriendid >= this->user_->GetThirdFriend()->GetId()){
//                ++newfriendid;
//            }
//        }
//    }
////    std::cout << newfriendid << std::endl;
    this->user_->SetThirdFriend(&(this->mainscreen_->engine_->staff_[newid]));
////    std::cout << "friend set" << std::endl;
    this->UpdateFriendsWidgets();
////    std::cout << "screen updated" << std::endl;
////    MyAccountScreen* newaccountscreen_ = new MyAccountScreen(this->user_, this->mainscreen_);
////    std::cout << "bew account screen created" << std::endl;
////    newaccountscreen_->showMaximized();
////    std::cout << "screen maximized" << std::endl;
////    this->close();
////    std::cout << "screen closed" << std::endl;
//    delete this;
//    std::cout << "screen deleted" << std::endl;
}


//Watch out that friends are mutually exclusive in the lists
//What if new friend is aucun, there is still a bug!!!
