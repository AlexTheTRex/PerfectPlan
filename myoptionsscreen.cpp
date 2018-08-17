#include "myoptionsscreen.h"
#include "mystaffscreen.h"
#include "mydeleteaccountscreen.h"
#include "myswitchslotscreen.h"
#include <QFile>
#include <iostream>

MyOptionsScreen::MyOptionsScreen(MyMainScreen* main) : QWidget()
{
    this->setWindowTitle("Options");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->mainscreen_ = main;
    this->layout_ = new QVBoxLayout;
    this->seehistorybutton_ = new QPushButton("Voir l'historique de modification des plannings");
    this->seestaffbutton_ = new QPushButton("Voir la liste du personnel");
    this->switchslotsbutton_ = new QPushButton("Echanger deux créneaux");
    this->deleteanaccountbuton_ = new QPushButton("Supprimer un compte");
    this->cancelbutton_ = new QPushButton("Annuler");
    this->printplanningbutton_ = new QPushButton("Exporter ce planning");

    this->layout_->addWidget(this->printplanningbutton_);
    this->layout_->addWidget(this->seehistorybutton_);
    this->layout_->addWidget(this->seestaffbutton_);
    this->layout_->addWidget(this->switchslotsbutton_);
    this->layout_->addWidget(this->deleteanaccountbuton_);
    this->layout_->addWidget(this->cancelbutton_);

    this->setLayout(this->layout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(CloseOptionsWindow()));
    QObject::connect(this->seestaffbutton_, SIGNAL(clicked()), this, SLOT(SeeStaff()));
    QObject::connect(this->deleteanaccountbuton_, SIGNAL(clicked()), this, SLOT(DeleteAccount()));
    QObject::connect(this->seehistorybutton_, SIGNAL(clicked()), this, SLOT(SeeHistory()));
    QObject::connect(this->switchslotsbutton_, SIGNAL(clicked()), this, SLOT(SwitchSlots()));
    QObject::connect(this->printplanningbutton_, SIGNAL(clicked()), this, SLOT(ExportPlanning()));
}

void MyOptionsScreen::ExportPlanning(){
    if (this->mainscreen_->engine_->finalplannings_.count(this->mainscreen_->mondayofcurrentweek_.toString().toStdString()) == 0){
        QMessageBox::warning(this, "Erreur", "Il n'y a pas encore de planning généré cette semaine!");
        return;
    }
    std::string path = PATH_TO_EXPORT_PLANNING + this->mainscreen_->mondayofcurrentweek_.toString(
                "dd_MM_yyyy").toStdString()
            + ".jpg";
    std::cout << path << std::endl;
    QPixmap pixmap(this->mainscreen_->planningtable_->size());
    this->mainscreen_->planningtable_->render(&pixmap);
    QFile myfile(QString::fromStdString(path));
    myfile.open(QIODevice::WriteOnly);
    pixmap.save(&myfile, "JPG");
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

void MyOptionsScreen::SeeHistory(){
    this->close();
    MyPlanningHistoryWindow* historyscreen = new MyPlanningHistoryWindow(this->mainscreen_);
    historyscreen->show();
    delete this;
}

void MyOptionsScreen::DeleteAccount(){
    MyDeleteAccountScreen* deleteaccountscreen = new MyDeleteAccountScreen(this->mainscreen_);
    this->close();
    deleteaccountscreen->show();
    delete this;
}

void MyOptionsScreen::SwitchSlots(){
    MySwitchSlotScreen* switchslots = new MySwitchSlotScreen(this->mainscreen_);
    this->close();
    switchslots->show();
    delete this;
}
