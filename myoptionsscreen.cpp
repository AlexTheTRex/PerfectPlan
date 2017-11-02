#include "myoptionsscreen.h"
#include "mystaffscreen.h"
#include "mydeleteaccountscreen.h"

MyOptionsScreen::MyOptionsScreen(MyMainScreen* main) : QWidget()
{
    this->setWindowTitle("Options");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->mainscreen_ = main;
    this->layout_ = new QVBoxLayout;
    this->seehistorybutton_ = new QPushButton("Voir l'historique des plannings");
    this->seestaffbutton_ = new QPushButton("Voir la liste du personnel");
    this->deleteanaccountbuton_ = new QPushButton("Supprimer un compte");
    this->cancelbutton_ = new QPushButton("Annuler");

    this->layout_->addWidget(this->seehistorybutton_);
    this->layout_->addWidget(this->seestaffbutton_);
    this->layout_->addWidget(this->deleteanaccountbuton_);
    this->layout_->addWidget(this->cancelbutton_);

    this->setLayout(this->layout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseOptionsWindow()));
    QObject::connect(this->seestaffbutton_, SIGNAL(clicked()), this, SLOT(SeeStaff()));
    QObject::connect(this->deleteanaccountbuton_, SIGNAL(clicked()), this, SLOT(DeleteAccount()));
}

void MyOptionsScreen::CloseOptionsWindow(){
    this->close();
    this->mainscreen_->setEnabled(true);
    delete this;
}

void MyOptionsScreen::SeeStaff(){
    this->close();
    this->mainscreen_->setEnabled(true);
    MyStaffScreen* staffscreen = new MyStaffScreen(this->mainscreen_);
    staffscreen->showMaximized();
    this->mainscreen_->hide();
    delete this;
}

void MyOptionsScreen::DeleteAccount(){
    MyDeleteAccountScreen* deleteaccountscreen = new MyDeleteAccountScreen(this->mainscreen_);
    this->close();
    deleteaccountscreen->show();
    delete this;
}
