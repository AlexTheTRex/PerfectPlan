
#include "mymainscreen.h"
#include <iostream>

#include <QString>
#include <QHeaderView>
#include <string>
#include <QInputDialog>
#include <QLabel>
#include <QPixmap>
#include <unordered_set>
#include <string>
#include <QProgressBar>
#include "mycreateaccountscreen.h"
#include "myloginscreen.h"
#include "myoptionsscreen.h"
#include "myconfirmplanningscreen.h"

MyMainScreen::MyMainScreen(QDate monday) : QWidget() {

  this->setWindowTitle("Perfect Plan");
  this->layout_ = new QGridLayout;
  this->setLayout(this->layout_);
  this->engine_ = new Engine();
  this->engine_->LoadStaff();
  this->engine_->LoadPlannings();
  this->engine_->LoadHistory();
  for (int i = 0; i < this->engine_->staff_.size(); ++i){
    this->engine_->friendships_.push_back(std::vector<int>(this->engine_->staff_.size(), 0));
  }
  this->mondayofcurrentweek_ = monday;//QDate::currentDate();
  while (this->mondayofcurrentweek_.dayOfWeek() > 1){
    this->mondayofcurrentweek_ = this->mondayofcurrentweek_.addDays(-1);
  }
  this->planningtable_ = new QTableWidget(6, 7);
  if (this->engine_->finalplannings_.count(this->mondayofcurrentweek_.toString().toStdString()) > 0 &&
      engine_->finalplannings_[this->mondayofcurrentweek_.toString().toStdString()].errormessage_.size() == 0){
      std::unordered_set<std::string> daysteamA;
      daysteamA.insert(mondayofcurrentweek_.toString().toStdString());
      daysteamA.insert(mondayofcurrentweek_.addDays(1).toString().toStdString());
      daysteamA.insert(mondayofcurrentweek_.addDays(4).toString().toStdString());
      daysteamA.insert(mondayofcurrentweek_.addDays(5).toString().toStdString());
      daysteamA.insert(mondayofcurrentweek_.addDays(6).toString().toStdString());
      this->DisplayPlanning(this->engine_->finalplannings_[this->mondayofcurrentweek_.toString().toStdString()]);
  } else {
      this->imagecontainer1_ = new QLabel;
      this->image1_ = new QPixmap (PATH_TO_NO_PLANNING);
      this->imagecontainer1_->setAlignment(Qt::AlignCenter);
      float coeff = 1.4;
      this->imagecontainer1_->setPixmap((*this->image1_).scaled(coeff * 562, coeff * 825,
                                                  Qt::KeepAspectRatio, Qt::SmoothTransformation));
      this->layout_->addWidget(this->imagecontainer1_, 1, 1, 3, 3);
  }

  this->createaccountbutton_ = new QPushButton("Créer un compte");
  this->loginbutton_ = new QPushButton("Se connecter");
  this->generateplanningbutton_ = new QPushButton("Générer un planning");
  //this->generateplanningbutton_->setMinimumSize(20, 80);
  this->optionsbutton_ = new QPushButton("Options");
  this->quitbutton_ = new QPushButton("Quitter");

  this->previousbutton_ = new QPushButton("Semaine précédente");
  this->nextbutton_ = new QPushButton("Semaine suivante");


  this->imagecontainer2_ = new QLabel;
  std::string weekstring = "Semaine du ";
  std::string day1 = this->mondayofcurrentweek_.toString("dddd").toStdString();
  std::string month1 = this->mondayofcurrentweek_.toString("MMMM").toStdString();
  std::string dday1 = this->mondayofcurrentweek_.toString("d").toStdString();
  std::string year1 = this->mondayofcurrentweek_.toString("yyyy").toStdString();
  std::string day2 = this->mondayofcurrentweek_.addDays(6).toString("dddd").toStdString();
  std::string month2 = this->mondayofcurrentweek_.addDays(6).toString("MMMM").toStdString();
  std::string dday2 = this->mondayofcurrentweek_.addDays(6).toString("d").toStdString();
  std::string year2 = this->mondayofcurrentweek_.addDays(6).toString("yyyy").toStdString();
  weekstring += this->engine_->translateDays[day1] + " " + dday1 + " " +
          this->engine_->translateMonths[month1] + " " + year1;
  weekstring += " au ";
  weekstring += this->engine_->translateDays[day2] + " " + dday2 + " " +
          this->engine_->translateMonths[month2] + " " + year2;
  //weekstring += this->mondayofcurrentweek_.addDays(6).toString("dddd d MMMM yyyy").toStdString();
  this->imagecontainer2_->setText(QString::fromStdString(weekstring));
  this->imagecontainer2_->setFont(QFont("Arial", 16, QFont::Bold));
  this->imagecontainer2_->setAlignment(Qt::AlignTop);
  this->imagecontainer2_->setMaximumHeight(30);
  this->imagecontainer2_->setMaximumWidth(1000);
  this->imagecontainer2_->setMinimumWidth(500);
  this->layout_->addWidget(this->imagecontainer2_, 0, 2);//, 1, 1);

  //this->image2_ = new QPixmap ("");//"/Users/Alexandre/Downloads/unicorn.jpg");
  //this->imagecontainer2_->setAlignment(Qt::AlignTop);
  //this->imagecontainer2_->setPixmap((*this->image2_).scaled(400, 72));

  this->auxiliarybox1_ = new QVBoxLayout;
  this->auxiliarybox2_ = new QVBoxLayout;

  this->auxiliarybox1_->addWidget(this->loginbutton_);
  this->auxiliarybox1_->addWidget(this->createaccountbutton_);
  this->auxiliarybox2_->addWidget(this->optionsbutton_);
  this->auxiliarybox2_->addWidget(this->quitbutton_);
  this->layout_->addLayout(this->auxiliarybox1_, 4, 0);
  this->layout_->addLayout(this->auxiliarybox2_, 4, 4);

  this->layout_->addWidget(this->generateplanningbutton_, 4, 2);
  this->layout_->addWidget(this->previousbutton_, 0, 0);
  this->layout_->addWidget(this->nextbutton_, 0, 4);



  QObject::connect(createaccountbutton_, SIGNAL(clicked()), this, SLOT(OpenCreateAccountWindow()));
  QObject::connect(loginbutton_, SIGNAL(clicked()), this, SLOT(OpenLoginWindow()));
  QObject::connect(optionsbutton_, SIGNAL(clicked()), this, SLOT(OpenOptionsWindow()));
  QObject::connect(quitbutton_, SIGNAL(clicked()), qApp, SLOT(quit()));
  QObject::connect(generateplanningbutton_, SIGNAL(clicked()), this, SLOT(GeneratePlanning()));
  QObject::connect(nextbutton_, SIGNAL(clicked()), this, SLOT(NextPlanning()));
  QObject::connect(previousbutton_, SIGNAL(clicked()), this, SLOT(PreviousPlanning()));

}

void MyMainScreen::PreviousPlanning(){
    QDate newmonday = mondayofcurrentweek_.addDays(-7);
    MyMainScreen* newmainscreen = new MyMainScreen(newmonday);
    newmainscreen->showMaximized();
    this->close();
    delete this;
}

void MyMainScreen::NextPlanning(){
    QDate newmonday = mondayofcurrentweek_.addDays(7);
    MyMainScreen* newmainscreen = new MyMainScreen(newmonday);
    newmainscreen->showMaximized();
    this->close();
    delete this;
//    mondayofcurrentweek_ = mondayofcurrentweek_.addDays(7);
//    std::string weekstring = "Semaine du ";
//    std::string day1 = this->mondayofcurrentweek_.toString("dddd").toStdString();
//    std::string month1 = this->mondayofcurrentweek_.toString("MMMM").toStdString();
//    std::string dday1 = this->mondayofcurrentweek_.toString("d").toStdString();
//    std::string year1 = this->mondayofcurrentweek_.toString("yyyy").toStdString();
//    std::string day2 = this->mondayofcurrentweek_.addDays(6).toString("dddd").toStdString();
//    std::string month2 = this->mondayofcurrentweek_.addDays(6).toString("MMMM").toStdString();
//    std::string dday2 = this->mondayofcurrentweek_.addDays(6).toString("d").toStdString();
//    std::string year2 = this->mondayofcurrentweek_.addDays(6).toString("yyyy").toStdString();
//    weekstring += this->engine_->translateDays[day1] + " " + dday1 + " " +
//            this->engine_->translateMonths[month1] + " " + year1;
//    weekstring += " au ";
//    weekstring += this->engine_->translateDays[day2] + " " + dday2 + " " +
//            this->engine_->translateMonths[month2] + " " + year2;
//    //weekstring += this->mondayofcurrentweek_.addDays(6).toString("dddd d MMMM yyyy").toStdString();
//    this->imagecontainer2_->setText(QString::fromStdString(weekstring));
//    if (engine_->finalplannings_.count(mondayofcurrentweek_.toString().toStdString()) == 0){
//        this->layout_->removeWidget(this->planningtable_);
//        this->layout_->addWidget(this->imagecontainer1_, 1, 1, 3, 3);
//        return;
//    }

//    this->DisplayPlanning(engine_->finalplannings_[
//                                           mondayofcurrentweek_.toString().toStdString()]);
}

void MyMainScreen::OpenCreateAccountWindow(){
    MyCreateAccountScreen* createaccountscreen = new MyCreateAccountScreen(this);
    this->setEnabled(false);
    createaccountscreen->show();
}

void MyMainScreen::OpenOptionsWindow(){
    MyOptionsScreen* optionsscreen = new MyOptionsScreen(this);
    this->setEnabled(false);
    optionsscreen->show();
}

void MyMainScreen::OpenLoginWindow(){
    MyLoginScreen* createaccountscreen = new MyLoginScreen
            (this);
    this->setEnabled(false);
    createaccountscreen->show();
}

void MyMainScreen::GeneratePlanning(){
    MyConfirmPlanningScreen* cps = new MyConfirmPlanningScreen(this);
    this->setEnabled(false);
    cps->show();
    return;
//    std::unordered_set<std::string> daysteamA;
//    daysteamA.insert(mondayofcurrentweek_.toString().toStdString());
//    daysteamA.insert(mondayofcurrentweek_.addDays(1).toString().toStdString());
//    daysteamA.insert(mondayofcurrentweek_.addDays(4).toString().toStdString());
//    daysteamA.insert(mondayofcurrentweek_.addDays(5).toString().toStdString());
//    daysteamA.insert(mondayofcurrentweek_.addDays(6).toString().toStdString());
//    Planning myPlan = this->engine_->GetRandomPlanning(this->mondayofcurrentweek_, daysteamA);
//    //Mise en scène
//    QWidget* window = new QWidget;
//    QVBoxLayout* lay = new QVBoxLayout;
//    QLabel* blabla = new QLabel;
//    blabla->setText("Calcul du planning en cours...");
//    QProgressBar* pgbar = new QProgressBar;
//    window->setWindowTitle("Patientez");
//    lay->addWidget(blabla);
//    lay->addWidget(pgbar);
//    window->setLayout(lay);
//    window->show();
//    clock_t begin = clock();
//    while (double(clock() - begin) / CLOCKS_PER_SEC < 5){
//        pgbar->setValue(int((20 * double(clock() - begin)) / CLOCKS_PER_SEC));
//        window->show();
//    }
//    window->close();
//    this->engine_->GeneratePlanning(this->mondayofcurrentweek_, daysteamA);
//    this->DisplayPlanning(myPlan);
}

void MyMainScreen::DisplayPlanning(const Planning &plan){

    delete this->planningtable_;
    this->planningtable_ = new QTableWidget(6, 7);
    this->planningtable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int c = 0; c < this->planningtable_->horizontalHeader()->count(); ++c){
        this->planningtable_->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    for (int c = 0; c < this->planningtable_->verticalHeader()->count(); ++c){
        this->planningtable_->verticalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    this->planningtable_->setHorizontalHeaderItem(0, new QTableWidgetItem("Lundi"));
    this->planningtable_->setHorizontalHeaderItem(1, new QTableWidgetItem("Mardi"));
    this->planningtable_->setHorizontalHeaderItem(2, new QTableWidgetItem("Mercredi"));
    this->planningtable_->setHorizontalHeaderItem(3, new QTableWidgetItem("Jeudi"));
    this->planningtable_->setHorizontalHeaderItem(4, new QTableWidgetItem("Vendredi"));
    this->planningtable_->setHorizontalHeaderItem(5, new QTableWidgetItem("Samedi"));
    this->planningtable_->setHorizontalHeaderItem(6, new QTableWidgetItem("Dimanche"));
    this->planningtable_->setVerticalHeaderItem(0, new QTableWidgetItem("USIC"));
    this->planningtable_->setVerticalHeaderItem(1, new QTableWidgetItem("CORO"));
    this->planningtable_->setVerticalHeaderItem(2, new QTableWidgetItem("MONITORE"));
    this->planningtable_->setVerticalHeaderItem(3, new QTableWidgetItem("USIC 2\nMONITORE 2"));
    this->planningtable_->setVerticalHeaderItem(4, new QTableWidgetItem("HDS"));
    this->planningtable_->setVerticalHeaderItem(5, new QTableWidgetItem("RYTHMO"));
    this->layout_->addWidget(this->planningtable_, 1, 0, 3, 5);

    std::string names;
    for (int day = 0; day < 7; ++day){
        //Usic1
        names = "";
        names += this->engine_->staff_[plan.usic1_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.usic1_[day][0]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.usic1_[day][1]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.usic1_[day][1]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.usic1_[day][2]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.usic1_[day][2]].GetSurname();
        QTableWidgetItem* item = new QTableWidgetItem(
                    QString::fromStdString(names));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        this->planningtable_->setItem(0, day, item);
        //Uca1
        names = "";
        names += this->engine_->staff_[plan.uca1_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.uca1_[day][0]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.uca1_[day][1]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.uca1_[day][1]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.uca1_[day][2]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.uca1_[day][2]].GetSurname();
        QTableWidgetItem* item2 = new QTableWidgetItem(
                    QString::fromStdString(names));
        item->setFlags(item2->flags() ^ Qt::ItemIsEditable);
        this->planningtable_->setItem(2, day, item2);
        //Usic2, Uca2
        names = "";
        names += this->engine_->staff_[plan.usic2_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.usic2_[day][0]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.uca2_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.uca2_[day][0]].GetSurname();
        QTableWidgetItem* item3 = new QTableWidgetItem(
                    QString::fromStdString(names));
        item->setFlags(item3->flags() ^ Qt::ItemIsEditable);
        this->planningtable_->setItem(3, day, item3);
        //Hds
        if (day < 4){
            names = "";
            names += this->engine_->staff_[plan.hds_[day][0]].GetFirstName();
            names += " ";
            names += this->engine_->staff_[plan.hds_[day][0]].GetSurname();
            names += "\n";
            names += this->engine_->staff_[plan.hds_[day][1]].GetFirstName();
            names += " ";
            names += this->engine_->staff_[plan.hds_[day][1]].GetSurname();
            QTableWidgetItem* item4 = new QTableWidgetItem(
                        QString::fromStdString(names));
            item->setFlags(item4->flags() ^ Qt::ItemIsEditable);
            this->planningtable_->setItem(4, day, item4);
        }
        //Rythmo
        names = "";
        names += this->engine_->staff_[plan.rythmo_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.rythmo_[day][0]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.rythmo_[day][1]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.rythmo_[day][1]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.rythmo_[day][2]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.rythmo_[day][2]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.rythmo_[day][3]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.rythmo_[day][3]].GetSurname();
        QTableWidgetItem* item5 = new QTableWidgetItem(
                    QString::fromStdString(names));
        item->setFlags(item5->flags() ^ Qt::ItemIsEditable);
        this->planningtable_->setItem(5, day, item5);
        //Coro
        names = "";
        names += this->engine_->staff_[plan.usic1_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.usic1_[day][0]].GetSurname();
        names += "\n";
        names += this->engine_->staff_[plan.uca1_[day][0]].GetFirstName();
        names += " ";
        names += this->engine_->staff_[plan.uca1_[day][0]].GetSurname();
        QTableWidgetItem* item6 = new QTableWidgetItem(
                    QString::fromStdString(names));
        item->setFlags(item6->flags() ^ Qt::ItemIsEditable);
        this->planningtable_->setItem(1, day, item6);
    }

}
