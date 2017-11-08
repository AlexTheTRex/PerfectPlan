#include "myconfirmplanningscreen.h"
#include <iostream>
#include <unordered_set>

using namespace std;

MyConfirmPlanningScreen::MyConfirmPlanningScreen(MyMainScreen* main) : QWidget()
{
    this->setWindowTitle("Générer un planning");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);
    this->mainscreen_ = main;

    biglayout_ = new QVBoxLayout;
    teamslayout_ = new QHBoxLayout;
    teamsexplanationlabel_ = new QLabel;
    buttonslayout_ = new QHBoxLayout;
    string explanationstring = "Veuillez spécifier quelle équipe travaille quel jour.";
    teamsexplanationlabel_->setText(QString::fromStdString(explanationstring));
    teamsexplanationlabel_->setAlignment(Qt::AlignCenter);
    biglayout_->addWidget(teamsexplanationlabel_);

    for (int i = 0; i < 7; ++i){
        dayslayouts_.push_back(new QVBoxLayout);
        groupboxes_.push_back(new QButtonGroup);
        dayslabels_.push_back(new QLabel);
        As_.push_back(new QRadioButton("Équipe A"));
        Bs_.push_back(new QRadioButton("Équipe B"));
        //Give each day its proper name
        if (i == 0){
            dayslabels_[i]->setText("Lundi");
        }
        if (i == 1){
            dayslabels_[i]->setText("Mardi");
        }
        if (i == 2){
            dayslabels_[i]->setText("Mercredi");
        }
        if (i == 3){
            dayslabels_[i]->setText("Jeudi");
        }
        if (i == 4){
            dayslabels_[i]->setText("Vendredi");
        }
        if (i == 5){
            dayslabels_[i]->setText("Samedi");
        }
        if (i == 6){
            dayslabels_[i]->setText("Dimanche");
        }

        //Organize the vboxlayout
        dayslayouts_[i]->addWidget(dayslabels_[i]);
        dayslayouts_[i]->addWidget(As_[i]);
        dayslayouts_[i]->addWidget(Bs_[i]);
        groupboxes_[i]->addButton(As_[i]);
        groupboxes_[i]->addButton(Bs_[i]);


        //Add layout to the hboxlayout
        teamslayout_->addLayout(dayslayouts_[i]);
    }
    this->biglayout_->addLayout(teamslayout_);

    this->form_ = new QFormLayout;
    this->firstname_ = new QLineEdit;
    this->familyname_ = new QLineEdit;
    this->password_ = new QLineEdit;
    this->password_->setEchoMode(QLineEdit::Password);
    this->form_->addRow("Votre prénom", this->firstname_);
    this->form_->addRow("Votre nom de famille", this->familyname_);
    this->form_->addRow("Votre mot de passe", this->password_);
    signatureexplanationlabel_ = new QLabel;
    signatureexplanationlabel_->setText("Pour confirmer, veuillez rentrer vos identifiants.");
    signatureexplanationlabel_->setAlignment(Qt::AlignCenter);
    biglayout_->addWidget(signatureexplanationlabel_);
    biglayout_->addLayout(form_);
    okbutton_ = new QPushButton("Générer");
    cancelbutton_ = new QPushButton("Annuler");
    okbutton_->setMaximumWidth(100);
    cancelbutton_->setMaximumWidth(100);
    buttonslayout_->addWidget(okbutton_);
    buttonslayout_->addWidget(cancelbutton_);
    this->biglayout_->addLayout(buttonslayout_);
    this->setLayout(biglayout_);

    QObject::connect(cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseConfirmPlanningWindow()));
    QObject::connect(okbutton_, SIGNAL(clicked()), this, SLOT(GeneratePlanning()));
}

int MyConfirmPlanningScreen::CheckFormValidity(){
    /* Returns -2 if not al days are filled, user id if found, -1 otherwise */

    if ((this->As_[0]->isChecked() || this->Bs_[0]->isChecked()) == false ||
        (this->As_[1]->isChecked() || this->Bs_[1]->isChecked()) == false ||
        (this->As_[2]->isChecked() || this->Bs_[2]->isChecked()) == false ||
        (this->As_[3]->isChecked() || this->Bs_[3]->isChecked()) == false ||
        (this->As_[4]->isChecked() || this->Bs_[4]->isChecked()) == false ||
        (this->As_[5]->isChecked() || this->Bs_[5]->isChecked()) == false ||
        (this->As_[6]->isChecked() || this->Bs_[6]->isChecked()) == false){
        return -2;
    }

    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()).toLower().toStdString() ==
                this->firstname_->text().toLower().toStdString()){
            if (this->mainscreen_->engine_->staff_[i].GetSurname() == this->familyname_->text().toUpper().toStdString()){
                if (this->mainscreen_->engine_->staff_[i].GetPassword() == this->password_->text().toStdString()){
                    return i;
                }
            }
        }
    }

    return -1;

}

void MyConfirmPlanningScreen::GeneratePlanning(){
    int id = this->CheckFormValidity();
    if (id == -2){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; Vérifier que vous avez bien tout rempli.");
        return;
    }
    if (id == -1){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; Combinaison nom/prénom/mot de passe inconnue.");
        return;
    }
    unordered_set<string> daysteamA;
    for (int i = 0; i < 7; ++i){
        if (this->As_[i]->isChecked()){
            daysteamA.insert(mainscreen_->mondayofcurrentweek_.addDays(i).toString().toStdString());
        }
    }
    Planning myPlan = this->mainscreen_->engine_->GetRandomPlanning(this->mainscreen_->mondayofcurrentweek_, daysteamA);
    this->mainscreen_->engine_->GeneratePlanning(this->mainscreen_->mondayofcurrentweek_, daysteamA);
    this->mainscreen_->setEnabled(true);
    this->mainscreen_->DisplayPlanning(this->mainscreen_->engine_->finalplannings_[
                                       this->mainscreen_->mondayofcurrentweek_.toString().toStdString()]);
    this->close();
    delete this;
}

void MyConfirmPlanningScreen::CloseConfirmPlanningWindow(){
    this->mainscreen_->setEnabled(true);
    this->close();
    delete this;
}
