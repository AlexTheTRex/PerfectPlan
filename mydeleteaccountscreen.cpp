#include "mydeleteaccountscreen.h"

using namespace std;

MyDeleteAccountScreen::MyDeleteAccountScreen(MyMainScreen* main) : QWidget() {
  if (true){
    this->setWindowTitle("Se connecter");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->mainscreen_ = main;

    this->firstname_ = new QLineEdit;
    this->familyname_ = new QLineEdit;
    this->password_ = new QLineEdit;
    this->password_->setEchoMode(QLineEdit::Password);
    this->myform_ = new QFormLayout;
    this->myform_->addRow("Prénom du compte à supprimer", this->firstname_);
    this->myform_->addRow("Nom du compte à supprimer", this->familyname_);
    this->myform_->addRow("Mot de passe du compte à supprimer", this->password_);

    this->buttonslayout_ = new QHBoxLayout;
    this->okbutton_ = new QPushButton("Supprimer le compte");
    this->cancelbutton_ = new QPushButton("Annuler");
    this->buttonslayout_->addWidget(this->okbutton_);
    this->buttonslayout_->addWidget(this->cancelbutton_);

    this->biglayout_ = new QVBoxLayout;
    this->biglayout_->addLayout(this->myform_);
    this->biglayout_->addLayout(this->buttonslayout_);
    this->setLayout(biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseWindow()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(GoToMainScreen()));
  }
}

void MyDeleteAccountScreen::CloseWindow(){
    this->close();
    this->mainscreen_->setEnabled(true);
    delete this;
}

int MyDeleteAccountScreen::CheckFormValidity(){
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (this->mainscreen_->engine_->staff_[i].GetFirstName() == this->firstname_->text().toStdString()){
            if (this->mainscreen_->engine_->staff_[i].GetSurname() == this->familyname_->text().toUpper().toStdString()){
                if (this->mainscreen_->engine_->staff_[i].GetPassword() == this->password_->text().toStdString()){
                    return i;
                }
            }
        }
    }
    return -1;
}

void MyDeleteAccountScreen::GoToMainScreen(){
    int id = this->CheckFormValidity();
    if (id == -1){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; nom inconnu ou mot de passe incorrect.");
        return;
    }
    int answer = QMessageBox::question(this, "Confirmer la suppression",
                                       "Êtes-vous sûr de vouloir supprimer ce compte?",
                                        QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes) {
        this->mainscreen_->engine_->DeleteAccount(id);
        this->mainscreen_->engine_->SaveStaff();
        this->close();
        this->mainscreen_->setEnabled(true);
        delete this;
    } else {
        this->CloseWindow();
    }
}
