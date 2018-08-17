#include "myconfirmplanningscreen.h"
#include <iostream>
#include <unordered_set>
#include <QInputDialog>
#include <QHeaderView>
#include "myminiloginscreen.h"

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

    this->calendarexplanationlabel_ = new QLabel;
    this->calendarexplanationlabel_->setText("Ci-dessous les jours de congé actuellement enregistrés "
                                             "cette semaine.");
    this->calendarexplanationlabel_->setAlignment(Qt::AlignCenter);
    this->biglayout_->addWidget(this->calendarexplanationlabel_);

    //Take care of the table
    this->vacationstable_ = new QTableWidget;
    this->DisplayVacations();

    //Add the table and buttons to layout
    this->addvacationsbutton_ = new QPushButton("Ajouter un jour de congé");
    this->removevacationsbuton_ = new QPushButton("Supprimer un jour de congé");
    this->vacationslayout_ = new QHBoxLayout;
    this->vacationslayout_->addWidget(this->addvacationsbutton_);
    this->vacationslayout_->addWidget(this->removevacationsbuton_);
    this->biglayout_->addLayout(this->vacationslayout_);

    //Take care of the form
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
    //okbutton_->setMaximumWidth(100);
    //cancelbutton_->setMaximumWidth(100);
    buttonslayout_->addWidget(okbutton_);
    buttonslayout_->addWidget(cancelbutton_);
    this->biglayout_->addLayout(buttonslayout_);
    this->setLayout(biglayout_);

    QObject::connect(cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseConfirmPlanningWindow()));
    QObject::connect(okbutton_, SIGNAL(clicked()), this, SLOT(GeneratePlanning()));
    QObject::connect(addvacationsbutton_, SIGNAL(clicked()), this, SLOT(OpenVacationWindow()));
    QObject::connect(removevacationsbuton_, SIGNAL(clicked()), this, SLOT(RemoveVacations()));
}

void MyConfirmPlanningScreen::RemoveVacations(){
    QModelIndexList selection = this->vacationstable_->selectionModel()->selectedRows();
    if (selection.count() == 0){
        QMessageBox::information(this, "Sélection vide", "Pour supprimer des jours de congés,"
                                  "veuillez sélectionner un ou plusieurs jours dans le tableau des jours de vacances.");
        return;
    }
    for (int i = 0; i < selection.count(); ++i){
        QString firstn = this->vacationstable_->item(selection.at(i).row(), 1)->text();
        QString familyn = this->vacationstable_->item(selection.at(i).row(), 0)->text();
        int idd = -1;
        //Look for right id
        for (int j = 0; j < this->mainscreen_->engine_->staff_.size(); ++j){
            if (QString::fromStdString(this->mainscreen_->engine_->staff_[j].GetFirstName()).toLower().toStdString() ==
                    firstn.toLower().toStdString()){
                if (this->mainscreen_->engine_->staff_[j].GetSurname() == familyn.toUpper().toStdString()){
                    idd = j;
                }
            }
        }
        //Look for right day
        for (int k = 0; k < this->mainscreen_->engine_->staff_[idd].vacationdays_.size(); ++k){
            if (this->mainscreen_->engine_->staff_[idd].vacationdays_[k].toString("dd.MM.yyyy") ==
                    this->vacationstable_->item(selection.at(i).row(), 2)->text()){
                this->mainscreen_->engine_->DeleteVacationDay(idd, k);
            }
        }
    }
    this->mainscreen_->engine_->SaveStaff();
    this->DisplayVacations();
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
                    if (this->mainscreen_->engine_->staff_[i].team_ < 2){
                        return i;
                    }
                }
            }
        }
    }

    return -1;

}

void MyConfirmPlanningScreen::OpenVacationWindow(){
    this->setEnabled(false);
    MyMiniLoginScreen* mini = new MyMiniLoginScreen(this);
    mini->show();
}

void MyConfirmPlanningScreen::DisplayVacations(){
    int numdays = 0;
    for (auto it = this->mainscreen_->engine_->staff_.begin();
         it != this->mainscreen_->engine_->staff_.end(); ++it){
        for (auto it2 = it->vacationdays_.begin(); it2 != it->vacationdays_.end(); ++it2){
            if (mainscreen_->mondayofcurrentweek_.daysTo(*it2) >= 0 &&
                    mainscreen_->mondayofcurrentweek_.daysTo(*it2) < 7){
                ++numdays;
            }
        }
    }
    //Create table
    delete this->vacationstable_;
    this->vacationstable_ = new QTableWidget(numdays, 4, this);
    this->vacationstable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->vacationstable_->setHorizontalHeaderItem(0, new QTableWidgetItem("Nom"));
    this->vacationstable_->setHorizontalHeaderItem(1, new QTableWidgetItem("Prénom"));
    this->vacationstable_->setHorizontalHeaderItem(2, new QTableWidgetItem("Date"));
    this->vacationstable_->setHorizontalHeaderItem(3, new QTableWidgetItem("Jour de la semaine"));
    for (int c = 0; c < this->vacationstable_->horizontalHeader()->count(); ++c){
        this->vacationstable_->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    for (int i = 0; i < this->vacationstable_->verticalHeader()->count(); ++i){
        this->vacationstable_->setVerticalHeaderItem(i, new QTableWidgetItem(""));
    }
    this->vacationstable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Fill table
    int i = 0;
    for (auto it = this->mainscreen_->engine_->staff_.begin();
         it != this->mainscreen_->engine_->staff_.end(); ++it){
        for (auto it2 = it->vacationdays_.begin(); it2 != it->vacationdays_.end(); ++it2){
            if (mainscreen_->mondayofcurrentweek_.daysTo(*it2) >= 0 &&
                    mainscreen_->mondayofcurrentweek_.daysTo(*it2) < 7){
                QTableWidgetItem* item2 = new QTableWidgetItem(
                            QString::fromStdString(it->GetSurname()));
                item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);
                this->vacationstable_->setItem(i, 0, item2);
                QTableWidgetItem* item3 = new QTableWidgetItem(
                            QString::fromStdString(it->GetFirstName()));
                item3->setFlags(item3->flags() ^ Qt::ItemIsEditable);
                this->vacationstable_->setItem(i, 1, item3);
                QTableWidgetItem* item0 = new QTableWidgetItem(it2->toString("dd.MM.yyyy"));
                item0->setFlags(item0->flags() ^ Qt::ItemIsEditable);
                this->vacationstable_->setItem(i, 2, item0);
                QString jour;
                if (it2->dayOfWeek() == 1){
                    jour = "Lundi";
                }
                if (it2->dayOfWeek() == 2){
                    jour = "Mardi";
                }
                if (it2->dayOfWeek() == 3){
                    jour = "Mercredi";
                }
                if (it2->dayOfWeek() == 4){
                    jour = "Jeudi";
                }
                if (it2->dayOfWeek() == 5){
                    jour = "Vendredi";
                }
                if (it2->dayOfWeek() == 6){
                    jour = "Samedi";
                }
                if (it2->dayOfWeek() == 7){
                    jour = "Dimanche";
                }
                QTableWidgetItem* item1 = new QTableWidgetItem(jour);
                item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
                this->vacationstable_->setItem(i, 3, item1);
                ++i;
            }
        }
    }
    this->biglayout_->insertWidget(3, this->vacationstable_);

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
    std::string motive = "Création du planning";
    unordered_set<string> daysteamA;
    for (int i = 0; i < 7; ++i){
        if (this->As_[i]->isChecked()){
            daysteamA.insert(mainscreen_->mondayofcurrentweek_.addDays(i).toString().toStdString());
        }
    }
    //If planning already exists, ask for a motive
    bool exists = false;
    for (auto it = this->mainscreen_->engine_->planninghistory_.begin();
         it != this->mainscreen_->engine_->planninghistory_.end(); ++it){
        if (it->planningmonday_ == this->mainscreen_->mondayofcurrentweek_){
            exists = true;
        }
    }
    if (exists){
        motive = QInputDialog::getText(this, "Planning déjà existant", "Un planning a déjà"
         "été calculé pour cette semaine. Si vous voulez remplacer le planning existant, veuillez entrer un motif "
         "(ex : erreur sur les jours de vacances).").toStdString();
    }
    //Generate the planning
    this->mainscreen_->engine_->GeneratePlanning(this->mainscreen_->mondayofcurrentweek_, daysteamA);
    //Check that everything went well
    if (this->mainscreen_->engine_->finalplannings_[
            this->mainscreen_->mondayofcurrentweek_.toString().toStdString()].errormessage_.length() > 0){
        QMessageBox::critical(this, "Erreur", QString::fromStdString(this->mainscreen_->engine_->finalplannings_[
                              this->mainscreen_->mondayofcurrentweek_.toString()
                .toStdString()].errormessage_));
    } else {
        this->mainscreen_->DisplayPlanning(this->mainscreen_->engine_->finalplannings_[
                                       this->mainscreen_->mondayofcurrentweek_.toString().toStdString()]);
        //Add the new planning to the history
        PlanningEntry myEntry;
        myEntry.modifdate_ = QDate::currentDate();
        myEntry.modiffirstname_ = this->mainscreen_->engine_->staff_[id].GetFirstName();
        myEntry.modifsurname_ = this->mainscreen_->engine_->staff_[id].GetSurname();
        myEntry.modiftime_ = QTime::currentTime();
        myEntry.planningmonday_ = this->mainscreen_->mondayofcurrentweek_;
        myEntry.motive_ = motive;
        this->mainscreen_->engine_->planninghistory_.push_back(myEntry);
        this->mainscreen_->engine_->SaveHistory();
    }
    this->mainscreen_->setEnabled(true);
    this->close();
    delete this;
}

void MyConfirmPlanningScreen::CloseConfirmPlanningWindow(){
    this->mainscreen_->setEnabled(true);
    this->close();
    delete this;
}
