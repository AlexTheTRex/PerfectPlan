#include "myplanninghistorywindow.h"
#include <QHeaderView>
#include <iostream>

MyPlanningHistoryWindow::MyPlanningHistoryWindow(MyMainScreen* main) : QWidget()
{
    this->setWindowTitle("Historique de modification des plannings");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->mainscreen_ = main;
    this->mondayofcurrentweek_ = this->mainscreen_->mondayofcurrentweek_;
    this->layout_ = new QGridLayout;
    this->previousbutton_ = new QPushButton("Semaine précédente");
    this->nextbutton_ = new QPushButton("Semaine suivante");
    this->quitbutton_ = new QPushButton("Retour");
    this->table_ = new QTableWidget();


    this->textcontainer_ = new QLabel;
    std::string weekstring = "Semaine du ";
    std::string day1 = this->mondayofcurrentweek_.toString("dddd").toStdString();
    std::string month1 = this->mondayofcurrentweek_.toString("MMMM").toStdString();
    std::string dday1 = this->mondayofcurrentweek_.toString("d").toStdString();
    std::string year1 = this->mondayofcurrentweek_.toString("yyyy").toStdString();
    std::string day2 = this->mondayofcurrentweek_.addDays(6).toString("dddd").toStdString();
    std::string month2 = this->mondayofcurrentweek_.addDays(6).toString("MMMM").toStdString();
    std::string dday2 = this->mondayofcurrentweek_.addDays(6).toString("d").toStdString();
    std::string year2 = this->mondayofcurrentweek_.addDays(6).toString("yyyy").toStdString();
    weekstring += this->mainscreen_->engine_->translateDays[day1] + " " + dday1 + " " +
            this->mainscreen_->engine_->translateMonths[month1] + " " + year1;
    weekstring += " au ";
    weekstring += this->mainscreen_->engine_->translateDays[day2] + " " + dday2 + " " +
            this->mainscreen_->engine_->translateMonths[month2] + " " + year2;
    this->textcontainer_->setText(QString::fromStdString(weekstring));
    //this->textcontainer_->setFont(QFont("Arial", 16, QFont::Bold));
    this->textcontainer_->setAlignment(Qt::AlignCenter);
    this->textcontainer_->setMaximumHeight(30);
    this->textcontainer_->setMaximumWidth(1000);
    this->textcontainer_->setMinimumWidth(500);
    this->layout_->addWidget(this->textcontainer_, 0, 1);
    this->layout_->addWidget(this->previousbutton_, 0, 0);
    this->layout_->addWidget(this->nextbutton_, 0, 2);

    this->Display();

    this->layout_->addWidget(this->quitbutton_, 4, 1);
    this->layout_->addWidget(this->table_, 1, 0, 3, 3);
    this->setLayout(this->layout_);
    QObject::connect(this->quitbutton_, SIGNAL(clicked()), this, SLOT(GoBack()));
    QObject::connect(this->nextbutton_, SIGNAL(clicked()), this, SLOT(NextWeek()));
    QObject::connect(this->previousbutton_, SIGNAL(clicked()), this, SLOT(PreviousWeek()));
}

void MyPlanningHistoryWindow::GoBack(){
    this->mainscreen_->setEnabled(true);
    this->close();
    delete this;
}

void MyPlanningHistoryWindow::Display(){
    delete this->table_;
    std::vector<PlanningEntry> entries;
    for (auto it = this->mainscreen_->engine_->planninghistory_.begin();
         it != this->mainscreen_->engine_->planninghistory_.end(); ++it){
        if (it->planningmonday_ == this->mondayofcurrentweek_){
            entries.push_back(*it);
        }
    }
    if (entries.size() == 0){
        this->table_ = new QTableWidget(1, 1);
        this->table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        this->table_->verticalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        this->table_->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
        this->table_->setVerticalHeaderItem(0, new QTableWidgetItem(""));
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(
                                          "Ce planning n'a jamais été modifié"));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        this->table_->setItem(0, 0, item);
    } else {
        this->table_ = new QTableWidget(entries.size(), 3);
        this->table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        for (int c = 0; c < this->table_->horizontalHeader()->count(); ++c){
            this->table_->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
        }
        for (int c = 0; c < this->table_->verticalHeader()->count(); ++c){
            this->table_->verticalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
            this->table_->setVerticalHeaderItem(c, new QTableWidgetItem(""));
        }
        this->table_->setHorizontalHeaderItem(0, new QTableWidgetItem("Usager"));
        this->table_->setHorizontalHeaderItem(1, new QTableWidgetItem("Date de modification"));
        this->table_->setHorizontalHeaderItem(2, new QTableWidgetItem("Motif"));
    }
//    QDate planningmonday_;
//    QDate modifdate_;
//    QTime modiftime_;
//    std::string modiffirstname_;
//    std::string modifsurname_;
//    std::string motive_;
    int count = 0;
    for (auto it = this->mainscreen_->engine_->planninghistory_.begin();
         it != this->mainscreen_->engine_->planninghistory_.end(); ++it){
        if (it->planningmonday_ == this->mondayofcurrentweek_){
            QTableWidgetItem* item0 = new QTableWidgetItem(
                        QString::fromStdString(it->modiffirstname_ + " " + it->modifsurname_));
            item0->setFlags(item0->flags() ^ Qt::ItemIsEditable);
            this->table_->setItem(count, 0, item0);
            QTableWidgetItem* item1 = new QTableWidgetItem(
                        it->modifdate_.toString() + " " + it->modiftime_.toString());
            item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
            this->table_->setItem(count, 1, item1);
            QTableWidgetItem* item2 = new QTableWidgetItem(
                        QString::fromStdString(it->motive_));
            item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);
            this->table_->setItem(count, 2, item2);
            ++count;
        }
    }
}

void MyPlanningHistoryWindow::NextWeek(){
    this->mondayofcurrentweek_ = this->mondayofcurrentweek_.addDays(7);
    this->Display();
    this->layout_->addWidget(this->table_, 1, 0, 3, 3);
    std::string weekstring = "Semaine du ";
    std::string day1 = this->mondayofcurrentweek_.toString("dddd").toStdString();
    std::string month1 = this->mondayofcurrentweek_.toString("MMMM").toStdString();
    std::string dday1 = this->mondayofcurrentweek_.toString("d").toStdString();
    std::string year1 = this->mondayofcurrentweek_.toString("yyyy").toStdString();
    std::string day2 = this->mondayofcurrentweek_.addDays(6).toString("dddd").toStdString();
    std::string month2 = this->mondayofcurrentweek_.addDays(6).toString("MMMM").toStdString();
    std::string dday2 = this->mondayofcurrentweek_.addDays(6).toString("d").toStdString();
    std::string year2 = this->mondayofcurrentweek_.addDays(6).toString("yyyy").toStdString();
    weekstring += this->mainscreen_->engine_->translateDays[day1] + " " + dday1 + " " +
            this->mainscreen_->engine_->translateMonths[month1] + " " + year1;
    weekstring += " au ";
    weekstring += this->mainscreen_->engine_->translateDays[day2] + " " + dday2 + " " +
            this->mainscreen_->engine_->translateMonths[month2] + " " + year2;
    this->textcontainer_->setText(QString::fromStdString(weekstring));
}

void MyPlanningHistoryWindow::PreviousWeek(){
    this->mondayofcurrentweek_ = this->mondayofcurrentweek_.addDays(-7);
    this->Display();
    this->layout_->addWidget(this->table_, 1, 0, 3, 3);
    std::string weekstring = "Semaine du ";
    std::string day1 = this->mondayofcurrentweek_.toString("dddd").toStdString();
    std::string month1 = this->mondayofcurrentweek_.toString("MMMM").toStdString();
    std::string dday1 = this->mondayofcurrentweek_.toString("d").toStdString();
    std::string year1 = this->mondayofcurrentweek_.toString("yyyy").toStdString();
    std::string day2 = this->mondayofcurrentweek_.addDays(6).toString("dddd").toStdString();
    std::string month2 = this->mondayofcurrentweek_.addDays(6).toString("MMMM").toStdString();
    std::string dday2 = this->mondayofcurrentweek_.addDays(6).toString("d").toStdString();
    std::string year2 = this->mondayofcurrentweek_.addDays(6).toString("yyyy").toStdString();
    weekstring += this->mainscreen_->engine_->translateDays[day1] + " " + dday1 + " " +
            this->mainscreen_->engine_->translateMonths[month1] + " " + year1;
    weekstring += " au ";
    weekstring += this->mainscreen_->engine_->translateDays[day2] + " " + dday2 + " " +
            this->mainscreen_->engine_->translateMonths[month2] + " " + year2;
    this->textcontainer_->setText(QString::fromStdString(weekstring));
}
