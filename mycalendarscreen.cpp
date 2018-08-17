#include "mycalendarscreen.h"

MyCalendarScreen::MyCalendarScreen(MyAccountScreen* account) : QWidget()
{
    this->accountscreen_ = account;
    this->switchscreen_ = NULL;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->cancelbutton_ = new QPushButton("Annuler");
    this->okbutton_ = new QPushButton("Ajouter");
    this->buttonslayout_ = new QHBoxLayout;
    this->buttonslayout_->addWidget(this->okbutton_);
    this->buttonslayout_->addWidget(this->cancelbutton_);

    this->biglayout_ = new QVBoxLayout;

    this->calendar_ = new QCalendarWidget;
    this->calendar_->setLocale(QLocale::French);

    this->biglayout_->addWidget(this->calendar_);
    this->biglayout_->addLayout(buttonslayout_);
    this->setLayout(this->biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(Cancel()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(Add()));
}

MyCalendarScreen::MyCalendarScreen(MySwitchSlotScreen* switchslot) : QWidget()
{
    this->accountscreen_ = NULL;
    this->switchscreen_ = switchslot;
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->cancelbutton_ = new QPushButton("Annuler");
    this->okbutton_ = new QPushButton("Confirmer");
    this->buttonslayout_ = new QHBoxLayout;
    this->buttonslayout_->addWidget(this->okbutton_);
    this->buttonslayout_->addWidget(this->cancelbutton_);

    this->biglayout_ = new QVBoxLayout;

    this->calendar_ = new QCalendarWidget;
    this->calendar_->setLocale(QLocale::French);

    this->biglayout_->addWidget(this->calendar_);
    this->biglayout_->addLayout(buttonslayout_);
    this->setLayout(this->biglayout_);

    QObject::connect(this->cancelbutton_, SIGNAL(clicked()), this, SLOT(Cancel()));
    QObject::connect(this->okbutton_, SIGNAL(clicked()), this, SLOT(Add()));
}

void MyCalendarScreen::Cancel(){
    this->close();
    if (this->accountscreen_ != NULL){
        this->accountscreen_->setEnabled(true);
    } else {
        this->switchscreen_->setEnabled(true);
    }
    delete this;
}

void MyCalendarScreen::Add(){
    if (this->accountscreen_ != NULL){
        QDate date = this->calendar_->selectedDate();
        this->accountscreen_->mainscreen_->engine_->AddVacationDay(this->accountscreen_->user_->GetId(), date);
        MyAccountScreen* oldaccountscreen = this->accountscreen_;
        this->accountscreen_ = new MyAccountScreen(oldaccountscreen->user_, oldaccountscreen->mainscreen_);
        this->close();
        this->accountscreen_->showMaximized();
        oldaccountscreen->close();
        delete oldaccountscreen;
        delete this;
    } else {
        QDate date = this->calendar_->selectedDate();
        std::string s = "Date du créneau : ";
        s += date.toString().toStdString();
        if (this->switchscreen_->choosingleftdate_){
            this->switchscreen_->leftcalendarlabel_->setText(QString::fromStdString(s));
            this->switchscreen_->leftdate_ = date;
        } else {
            this->switchscreen_->rightcalendarlabel_->setText(QString::fromStdString(s));
            this->switchscreen_->rightdate_ = date;
        }
        this->switchscreen_->setEnabled(true);
        this->close();
        delete this;
    }
}
