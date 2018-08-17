#include "mycreateaccountscreen.h"
#include "myaccountscreen.h"
#include <ctime>
#include <random>


MyCreateAccountScreen::MyCreateAccountScreen(MyMainScreen* mainscreen) {
    if (true){

        this->setWindowTitle("Créer un compte");
        this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);//MinMaxButtonsHint);

        this->mainscreen_ = mainscreen;

        this->firstname_ = new QLineEdit;
        this->familyname_ = new QLineEdit;
        this->password_ = new QLineEdit;
        this->confirmpassword_ = new QLineEdit;
        this->password_->setEchoMode(QLineEdit::Password);
        this->confirmpassword_->setEchoMode(QLineEdit::Password);
        this->myform_ = new QFormLayout;
        this->myform_->addRow("Votre prénom", this->firstname_);
        this->myform_->addRow("Votre nom de famille", this->familyname_);
        this->myform_->addRow("Votre mot de passe", this->password_);
        this->myform_->addRow("Confirmer le mot de passe", this->confirmpassword_);

        this->radiobuttons_ = new QVBoxLayout;
        this->ide_ = new QRadioButton("Je suis IDE");
        this->as_ = new QRadioButton("Je suis AS");
        this->radiobuttons_->addWidget(this->ide_);
        this->radiobuttons_->addWidget(this->as_);

        std::string teama = "Je suis dans l'équipe A ", teamb = "Je suis dans l'équipe B ";
        std::vector<int> A, B;
        for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
            if (this->mainscreen_->engine_->staff_[i].team_ == 0){
                A.push_back(i);
            } else {
                B.push_back(i);
            }
        }
        std::srand(std::clock());
        if (A.size() > 0){
            int a = std::rand() % A.size();
            teama += "(équipe de ";
            teama += this->mainscreen_->engine_->staff_[A[a]].GetFirstName() + " ";
            teama += this->mainscreen_->engine_->staff_[A[a]].GetSurname();
            teama += ")";
        }
        if (B.size() > 0){
            int b = std::rand() % B.size();
            teamb += "(équipe de ";
            teamb += this->mainscreen_->engine_->staff_[B[b]].GetFirstName() + " ";
            teamb += this->mainscreen_->engine_->staff_[B[b]].GetSurname();
            teamb += ")";
        }

        this->teamradiobuttons_ = new QVBoxLayout;
        this->teamgroup_ = new QButtonGroup;
        this->zero_ = new QRadioButton(QString::fromStdString(teama));
        this->one_ = new QRadioButton(QString::fromStdString(teamb));
        this->teamradiobuttons_->addWidget(this->zero_);
        this->teamradiobuttons_->addWidget(this->one_);
        this->teamgroup_->addButton(this->zero_);
        this->teamgroup_->addButton(this->one_);

        this->buttonslayout_ = new QHBoxLayout;
        this->okbutton_ = new QPushButton("Créer");
        this->cancelbutton_ = new QPushButton("Annuler");
        this->buttonslayout_->addWidget(this->okbutton_);
        this->buttonslayout_->addWidget(this->cancelbutton_);

        this->biglayout_ = new QVBoxLayout;
        this->biglayout_->addLayout(this->myform_);
        this->biglayout_->addLayout(this->radiobuttons_);
        this->biglayout_->addLayout(this->teamradiobuttons_);
        this->biglayout_->addLayout(this->buttonslayout_);
        this->setLayout(biglayout_);

        //QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(CloseCreateAccountWindow()));
        QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseCreateAccountWindow()));
        QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(GoToAccountScreen()));
    }
}

void MyCreateAccountScreen::CloseCreateAccountWindow(){
    this->close();
    this->mainscreen_->setEnabled(true);
    delete this;
}

int MyCreateAccountScreen::CheckFormValidity(){
    /* Returns 1 if passwords are different or fields are not filled
     *  2 if name already exists, 0 otherwise. */

    //Check passwords are the same
    if (this->password_->text() != this->confirmpassword_->text()){
        return 1;
    }
    if (this->password_->text().size() == 0 ||
            this->confirmpassword_->text().size() == 0 ||
            this->firstname_->text().size() == 0 ||
            this->familyname_->text().size() == 0 ||
            (this->ide_->isChecked() || this->as_->isChecked()) == false ||
            (this->zero_->isChecked() || this->one_->isChecked()) == false){
        return 1;
    }
    //Search database for equal pairs of names
    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        if (this->mainscreen_->engine_->staff_[i].GetFirstName() == this->firstname_->text().toStdString()){
            if (this->mainscreen_->engine_->staff_[i].GetSurname() == this->familyname_->text().toUpper().toStdString()){
                if (this->mainscreen_->engine_->staff_[i].team_ < 2){
                    return 2;
                }
                else {
                    return -i - 1; //We are creating accoutn with same name as deleted one
                }
            }
        }
    }

    return 0;
}

void MyCreateAccountScreen::GoToAccountScreen(){
    if (this->CheckFormValidity() == 1){
        QMessageBox::warning(this, "Erreur", "Formulaire invalide; vérifiez que les mots de passe correspondent et que vous avez rempli tous les champs.");
        return;
    }
    if (this->CheckFormValidity() == 2){
        QMessageBox::warning(this, "Erreur", "Un compte avec les même noms et prénoms existe déjà.");
        return;
    }
    if (this->CheckFormValidity() < 0){
        int team;
        if (this->zero_->isChecked()){
            team = 0;
        } else {
            team = 1;
        }
        int idd = this->CheckFormValidity() * (-1) - 1;
        this->mainscreen_->engine_->staff_[idd].team_ = team;
        this->mainscreen_->engine_->staff_[idd].firstname_ = this->firstname_->text().toStdString();
        this->mainscreen_->engine_->staff_[idd].surname_ = this->familyname_->text().toUpper().toStdString();
        this->mainscreen_->engine_->staff_[idd].password_ = this->password_->text().toStdString();
        this->mainscreen_->engine_->staff_[idd].iside_ = this->ide_->isChecked();
        this->mainscreen_->engine_->staff_[idd].isas_ = this->as_->isChecked();
        this->mainscreen_->engine_->staff_[idd].vacationdays_.clear();
        this->mainscreen_->engine_->staff_[idd].SetFirstFriend(NULL);
        this->mainscreen_->engine_->staff_[idd].SetSecondFriend(NULL);
        this->mainscreen_->engine_->staff_[idd].SetThirdFriend(NULL);
        this->mainscreen_->engine_->staff_[idd].numberofvacationdays_ = 0;
        this->mainscreen_->engine_->staff_[idd].pathtoprofilepic_ = PATH_TO_DEFAULT_PROFILE_PIC;
        this->close();
        this->mainscreen_->setEnabled(true);

        MyAccountScreen* accountscreen = new MyAccountScreen(&this->mainscreen_->engine_->staff_[idd], this->mainscreen_);

        accountscreen->showMaximized();
        this->mainscreen_->hide();
        delete this;
        return;
    }
    int team;
    if (this->zero_->isChecked()){
        team = 0;
    } else {
        team = 1;
    }

    this->mainscreen_->engine_->CreateAccount(this->firstname_->text().toStdString(),
                                              this->familyname_->text().toUpper().toStdString(),
                                              this->password_->text().toStdString(),
                                              team, this->ide_->isChecked(), this->as_->isChecked());
    this->close();
    this->mainscreen_->setEnabled(true);

    MyAccountScreen* accountscreen = new MyAccountScreen(&this->mainscreen_->engine_->staff_.back(), this->mainscreen_);

    accountscreen->showMaximized();
    this->mainscreen_->hide();
    delete this;
}
