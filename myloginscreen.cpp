#include "myloginscreen.h"
#include "nurse.h"
#include "myaccountscreen.h"
#include <iostream>

using namespace std;

MyLoginScreen::MyLoginScreen(MyMainScreen* main) : QWidget() {
  if (true){
    this->setWindowTitle("Se connecter");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->mainscreen_ = main;

    this->firstname_ = new QLineEdit;
    this->familyname_ = new QLineEdit;
    this->password_ = new QLineEdit;
    this->password_->setEchoMode(QLineEdit::Password);
    this->myform_ = new QFormLayout;
    this->myform_->addRow("Votre prénom", this->firstname_);
    this->myform_->addRow("Votre nom de famille", this->familyname_);
    this->myform_->addRow("Votre mot de passe", this->password_);

    this->buttonslayout_ = new QHBoxLayout;
    this->okbutton_ = new QPushButton("Se connecter");
    this->cancelbutton_ = new QPushButton("Annuler");
    this->buttonslayout_->addWidget(this->okbutton_);
    this->buttonslayout_->addWidget(this->cancelbutton_);

    this->biglayout_ = new QVBoxLayout;
    this->biglayout_->addLayout(this->myform_);
    this->biglayout_->addLayout(this->buttonslayout_);
    this->setLayout(biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseLoginWindow()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(GoToAccountScreen()));
  }
}

void MyLoginScreen::CloseLoginWindow(){
    this->close();
    this->mainscreen_->setEnabled(true);
    delete this;
}

int MyLoginScreen::CheckFormValidity(){
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower().toStdString() ==
                this->firstname_->text().toLower().toStdString()){
            if (this->mainscreen_->engine_->staff_[i].GetSurname() == this->familyname_->text().toUpper().toStdString()){
                if (this->mainscreen_->engine_->staff_[i].GetPassword() == this->password_->text().toStdString()){
                    if (this->mainscreen_->engine_->staff_[i].team_ < 2){
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

void MyLoginScreen::GoToAccountScreen(){
    int id = this->CheckFormValidity();
    if (id == -1){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; combinaison nom/prénom/mot de passe inconnue.");
        return;
    }
    this->close();
    this->mainscreen_->setEnabled(true);

    MyAccountScreen* accountscreen = new MyAccountScreen(&(this->mainscreen_->engine_->staff_[id]), this->mainscreen_);

    accountscreen->showMaximized();
    this->mainscreen_->hide();
    delete this;
}

