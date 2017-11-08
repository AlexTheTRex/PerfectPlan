#include "mystaffscreen.h"
#include "parameters.h"
#include <QHeaderView>
#include <QPixmap>
#include <QIcon>
#include <QBitmap>
#include <QPainter>

MyStaffScreen::MyStaffScreen(MyMainScreen* main) : QWidget()
{
    this->setWindowTitle("Liste du personnel");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window);

    this->mainscreen_ = main;
    this->layout_ = new QVBoxLayout(this);
    this->table_ = new QTableWidget(this->mainscreen_->engine_->staff_.size(), 5, this);
    this->table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->gobackbutton_ = new QPushButton("Retour");
    this->gobackbutton_->setMaximumWidth(200);
    this->table_->verticalHeader()->setDefaultSectionSize(60);
    for (int i = 0; i < this->table_->verticalHeader()->count(); ++i){
        this->table_->setVerticalHeaderItem(i, new QTableWidgetItem(""));
    }

    for (int i = 0; i < this->mainscreen_->engine_->staff_.size(); ++i){
        QString path;
        if (this->mainscreen_->engine_->staff_[i].GetPathToProfilePic().length() > 0){
            path = QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetPathToProfilePic());
        } else {
            path = PATH_TO_DEFAULT_PROFILE_PIC;
        }
        QPixmap original(path);
        QRect crop(0, (original.height() - original.width())/5, original.width(), original.width());
        original = original.copy(crop);

        QBitmap  mask(original.size());
        QPainter painter(&mask);
        mask.fill(Qt::white);
        painter.setBrush(Qt::black);
        painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), mask.width()/2, mask.height()/2);
        original.setMask(mask);
        original = original.scaled(STAFF_PROFILE_PIC_SIZE, STAFF_PROFILE_PIC_SIZE,
                                   Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPixmap* image = new QPixmap(original);

        QTableWidgetItem* item0 = new QTableWidgetItem("");
        item0->setTextAlignment(Qt::AlignCenter);
        item0->setData(Qt::DecorationRole, (*image));//.scaled(57, 57, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        item0->setFlags(item0->flags() ^ Qt::ItemIsEditable);
        this->table_->setItem(i, 0, item0);

        QTableWidgetItem* item1 = new QTableWidgetItem(
                    QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetSurname()));
        item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
        this->table_->setItem(i, 1, item1);

        QTableWidgetItem* item2 = new QTableWidgetItem(
                    QString::fromStdString(this->mainscreen_->engine_->staff_[i].GetFirstName()));
        item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);
        this->table_->setItem(i, 2, item2);



        std::string job;
        if (this->mainscreen_->engine_->staff_[i].IsIde()){
            job = "IDE";
        } else {
            job = "AS";
        }
        QTableWidgetItem* item3 = new QTableWidgetItem(QString::fromStdString(job));
        item3->setFlags(item3->flags() ^ Qt::ItemIsEditable);
        this->table_->setItem(i, 3, item3);

        std::string team;
        if (this->mainscreen_->engine_->staff_[i].GetTeam() == 0){
            team = "A";
        } else {
            team = "B";
        }
        QTableWidgetItem* item4 = new QTableWidgetItem(
                    QString::fromStdString(team));
        item4->setFlags(item4->flags() ^ Qt::ItemIsEditable);
        this->table_->setItem(i, 4, item4);

    }
    this->table_->setHorizontalHeaderItem(0, new QTableWidgetItem(" "));
    this->table_->setHorizontalHeaderItem(1, new QTableWidgetItem("NOM"));
    this->table_->setHorizontalHeaderItem(2, new QTableWidgetItem("PRENOM"));
    this->table_->setHorizontalHeaderItem(3, new QTableWidgetItem("POSTE"));
    this->table_->setHorizontalHeaderItem(4, new QTableWidgetItem("EQUIPE"));

    this->table_->horizontalHeader()->setDefaultSectionSize(70);
    for (int c = 1; c < this->table_->horizontalHeader()->count(); ++c){
        this->table_->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
    this->table_->sortItems(1);
    this->layout_->addWidget(this->table_);
    this->layout_->addWidget(this->gobackbutton_);
    this->setLayout(this->layout_);

    QObject::connect(this->gobackbutton_, SIGNAL(clicked()), this, SLOT(GoBack()));
}

void MyStaffScreen::GoBack(){
    this->close();
    this->mainscreen_->showMaximized();
    delete this;
}
