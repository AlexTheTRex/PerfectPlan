#include "mychangepasswordscreen.h"
#include "nurse.h"
#include "myaccountscreen.h"
#include <iostream>

using namespace std;

MyChangePasswordScreen::MyChangePasswordScreen(MyAccountScreen* account) : QWidget() {
  if (true){
    this->setWindowTitle("Modifier mon mot de passe");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->accountscreen_ = account;

    this->currentPassword_ = new QLineEdit;
    this->currentPassword_->setEchoMode(QLineEdit::Password);
    this->newpassword_ = new QLineEdit;
    this->newpassword_->setEchoMode(QLineEdit::Password);
    this->confirmnewpassword_ = new QLineEdit;
    this->confirmnewpassword_->setEchoMode(QLineEdit::Password);
    this->myform_ = new QFormLayout;
    this->myform_->addRow("Mot de passe actuel", this->currentPassword_);
    this->myform_->addRow("Nouveau mot de passe", this->newpassword_);
    this->myform_->addRow("Confirmer le nouveau mot de passe", this->confirmnewpassword_);

    this->buttonslayout_ = new QHBoxLayout;
    this->okbutton_ = new QPushButton("Modifier");
    this->cancelbutton_ = new QPushButton("Annuler");
    this->buttonslayout_->addWidget(this->okbutton_);
    this->buttonslayout_->addWidget(this->cancelbutton_);

    this->biglayout_ = new QVBoxLayout;
    this->biglayout_->addLayout(this->myform_);
    this->biglayout_->addLayout(this->buttonslayout_);
    this->setLayout(biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(Cancel()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(GoToAccountScreen()));
  }
}

void MyChangePasswordScreen::Cancel(){//slot in accountscreen
    this->close();
    this->accountscreen_->setEnabled(true);
    delete this;
}

int MyChangePasswordScreen::CheckFormValidity(){
    //Check if passwords are the same, if not return 1
    if (this->newpassword_->text() != this->confirmnewpassword_->text()){
        return 1;
    }
    //Check if password is correct, if not return 2
    if (this->accountscreen_->user_->GetPassword() != this->currentPassword_->text().toStdString()){
        return 2;
    }
    return 0;
}

void MyChangePasswordScreen::GoToAccountScreen(){
    int id = this->CheckFormValidity();
    if (id == 1){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; vérifiez que vos nouveaux mots de passe"
                                             "sont identiques.");
        return;
    }
    if (id == 2){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; mot de passe actuel incorrect.");
        return;
    }
    //Change password in database
    this->accountscreen_->user_->password_ = this->newpassword_->text().toStdString();
    this->close();
    this->accountscreen_->setEnabled(true);
    delete this;
}

