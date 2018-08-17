#include "myswitchslotscreen.h"
#include "mymainscreen.h"
#include "mycalendarscreen.h"
#include <QLineEdit>

MySwitchSlotScreen::MySwitchSlotScreen(MyMainScreen* main) : QWidget()
{
    this->setWindowTitle("Echanger des créneaux");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);
    this->mainscreen_ = main;

    //Left Side
    this->leftlayout_ = new QVBoxLayout;

    this->leftlabel_ = new QLabel;
    this->leftlabel_->setText("Premier personnel : remplissez les champs");
    this->leftlabel_->setAlignment(Qt::AlignCenter);

    this->leftfirstname_ = new QLineEdit;
    this->leftfamilyname_ = new QLineEdit;
    this->leftpassword_ = new QLineEdit;
    this->leftpassword_->setEchoMode(QLineEdit::Password);
    this->leftform_ = new QFormLayout;
    this->leftform_->addRow("Prénom", this->leftfirstname_);
    this->leftform_->addRow("Nom de famille", this->leftfamilyname_);
    this->leftform_->addRow("Mot de passe", this->leftpassword_);

    this->leftcalendarlabel_ = new QLabel;
    this->leftcalendarlabel_->setText("Sélectionnez la date du créneau à échanger");
    this->leftcalendarbutton_ = new QPushButton("Changer la date du créneau");

    this->leftlayout_->addWidget(this->leftlabel_);
    this->leftlayout_->addLayout(this->leftform_);
    this->leftlayout_->addWidget(this->leftcalendarlabel_);
    this->leftlayout_->addWidget(this->leftcalendarbutton_);


    //Right Side
    this->rightlayout_ = new QVBoxLayout;

    this->rightlabel_ = new QLabel;
    this->rightlabel_->setText("Second personnel : remplissez les champs");
    this->rightlabel_->setAlignment(Qt::AlignCenter);

    this->rightfirstname_ = new QLineEdit;
    this->rightfamilyname_ = new QLineEdit;
    this->rightpassword_ = new QLineEdit;
    this->rightpassword_->setEchoMode(QLineEdit::Password);
    this->rightform_ = new QFormLayout;
    this->rightform_->addRow("Prénom", this->rightfirstname_);
    this->rightform_->addRow("Nom de famille", this->rightfamilyname_);
    this->rightform_->addRow("Mot de passe", this->rightpassword_);

    this->rightcalendarlabel_ = new QLabel;
    this->rightcalendarlabel_->setText("Sélectionnez la date du créneau à échanger");
    this->rightcalendarbutton_ = new QPushButton("Changer la date du créneau");

    this->rightlayout_->addWidget(this->rightlabel_);
    this->rightlayout_->addLayout(this->rightform_);
    this->rightlayout_->addWidget(this->rightcalendarlabel_);
    this->rightlayout_->addWidget(this->rightcalendarbutton_);

    this->tophlayout_ = new QHBoxLayout;
    this->tophlayout_->addLayout(this->leftlayout_);
    this->tophlayout_->addLayout(this->rightlayout_);


    //Bottom
    this->okbutton_ = new QPushButton("Echanger");
    this->cancelbutton_ = new QPushButton("Annuler");
    this->bottomhlayout_ = new QHBoxLayout;
    this->bottomhlayout_->addWidget(this->okbutton_);
    this->bottomhlayout_->addWidget(this->cancelbutton_);

    //Instructions for request sending

    //Final
    this->biglayout_ = new QVBoxLayout;
    this->biglayout_->addLayout(this->tophlayout_);
    this->biglayout_->addLayout(this->bottomhlayout_);

    this->setLayout(biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseWindow()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(SwitchSlots()));
    QObject::connect(this->leftcalendarbutton_, SIGNAL(clicked()), this, SLOT(LeftCalendar()));
    QObject::connect(this->rightcalendarbutton_, SIGNAL(clicked()), this, SLOT(RightCalendar()));
}

void MySwitchSlotScreen::LeftCalendar(){
    this->choosingleftdate_ = true;
    MyCalendarScreen* calendar = new MyCalendarScreen(this);
    this->setEnabled(false);
    calendar->show();
}

void MySwitchSlotScreen::RightCalendar(){
    this->choosingleftdate_ = false;
    MyCalendarScreen* calendar = new MyCalendarScreen(this);
    this->setEnabled(false);
    calendar->show();
}

void MySwitchSlotScreen::CloseWindow(){
    this->mainscreen_->setEnabled(true);
    this->close();
    delete this;
}

int MySwitchSlotScreen::CheckLeftFormValidity(){
    //Return id if found, 0 otherwise
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower().toStdString() ==
                this->leftfirstname_->text().toLower().toStdString()){
            if (this->mainscreen_->engine_->staff_[i].GetSurname() == this->leftfamilyname_->text().toUpper().toStdString()){
                if (this->mainscreen_->engine_->staff_[i].GetPassword() == this->leftpassword_->text().toStdString()){
                    if (this->mainscreen_->engine_->staff_[i].team_ < 2){
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

int MySwitchSlotScreen::CheckRightFormValidity(){
    //Return id if found, 0 otherwise
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower().toStdString() ==
                this->rightfirstname_->text().toLower().toStdString()){
            if (this->mainscreen_->engine_->staff_[i].GetSurname() == this->rightfamilyname_->text().toUpper().toStdString()){
                if (this->mainscreen_->engine_->staff_[i].GetPassword() == this->rightpassword_->text().toStdString()){
                    if (this->mainscreen_->engine_->staff_[i].team_ < 2){
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

void MySwitchSlotScreen::SwitchSlots(){
    int left = this->CheckLeftFormValidity();
    int right = this->CheckRightFormValidity();
    bool leftdateexists = (this->leftcalendarlabel_->text() != "Sélectionnez la date du créneau à échanger");
    bool rightdateexists = (this->rightcalendarlabel_->text() != "Sélectionnez la date du créneau à échanger");
    bool hasworked;
    if ((left >= 0 && right >= 0 && leftdateexists && rightdateexists) == false){
        QMessageBox::warning(this, "Erreur", "Erreur : Veuillez sélectionner une date"
                                             " et vérifier les noms/prénoms/mot de passe.");
        return;
    } else {
        hasworked = this->mainscreen_->engine_->SwitchSlots(left, this->leftdate_, right, this->rightdate_);
    }
    if (hasworked == false){
        QMessageBox::warning(this, "Erreur", "Erreur : soit il n'y a pas encore de planning pour les créneaux indiqués, soit"
                                             " les personnes correspondantes ne travaillent pas à la date"
                                             " indiquée, soit vous avez rentrés des créneaux sur deux semaines différentes.");
        return;
    }
    this->mainscreen_->setEnabled(true);
    this->mainscreen_->DisplayPlanning(this->mainscreen_->engine_->finalplannings_[
                                       this->mainscreen_->mondayofcurrentweek_.toString().toStdString()]);
    this->close();
    delete this;
}









