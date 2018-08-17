#include "myminiloginscreen.h"

MyMiniLoginScreen::MyMiniLoginScreen(MyConfirmPlanningScreen* confirm) : QWidget()

{
    this->setWindowTitle("Choisir un personnel");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->confirmscreen_ = confirm;
    this->calendar_ = new QCalendarWidget;
    this->calendar_->setLocale(QLocale::French);

    this->firstname_ = new QLineEdit;
    this->familyname_ = new QLineEdit;
    this->myform_ = new QFormLayout;
    this->myform_->addRow("Prénom", this->firstname_);
    this->myform_->addRow("Nom de famille", this->familyname_);

    this->buttonslayout_ = new QHBoxLayout;
    this->okbutton_ = new QPushButton("Confirmer");
    this->cancelbutton_ = new QPushButton("Annuler");
    this->buttonslayout_->addWidget(this->okbutton_);
    this->buttonslayout_->addWidget(this->cancelbutton_);

    this->biglayout_ = new QVBoxLayout;
    this->biglayout_->addLayout(this->myform_);
    this->biglayout_->addWidget(this->calendar_);
    this->biglayout_->addLayout(this->buttonslayout_);
    this->setLayout(biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseWindow()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(Confirm()));

}

void MyMiniLoginScreen::CloseWindow(){
    this->close();
    this->confirmscreen_->setEnabled(true);
    delete this;
}

int MyMiniLoginScreen::CheckFormValidity(){
    for (int i = 0; i < this->confirmscreen_->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->confirmscreen_->mainscreen_->engine_->staff_[i].GetFirstName()).toLower().toStdString() ==
                this->firstname_->text().toLower().toStdString()){
            if (this->confirmscreen_->mainscreen_->engine_->staff_[i].GetSurname() == this->familyname_->text().toUpper().toStdString()){
                if (this->confirmscreen_->mainscreen_->engine_->staff_[i].team_ < 2){
                    return i;
                }
            }
        }
    }
    return -1;
}

void MyMiniLoginScreen::Confirm(){
    int id = this->CheckFormValidity();
    if (id == -1){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; combinaison nom/prénom inconnue.");
        return;
    }
    this->confirmscreen_->mainscreen_->engine_->AddVacationDay(id, this->calendar_->selectedDate());
    this->confirmscreen_->mainscreen_->engine_->SaveStaff();
    this->confirmscreen_->DisplayVacations();
    this->close();
    this->confirmscreen_->setEnabled(true);
    delete this;
}
