#include "engine.h"
#include "parameters.h"
#include <QFile>
#include <QDir>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <cmath>
#include <iostream>
using namespace std;

//If there are not 3 friends segfault!
//At any time

Engine::Engine() {
    staff_ = vector<Nurse>();
    //friendships_ = vector< vector <int> >();

    translateDays["Monday"] = "Lundi";
    translateDays["Tuesday"] = "Mardi";
    translateDays["Wednesday"] = "Mercredi";
    translateDays["Thursday"] = "Jeudi";
    translateDays["Friday"] = "Vendredi";
    translateDays["Saturday"] = "Samedi";
    translateDays["Sunday"] = "Dimanche";

    translateMonths["January"] = "Janvier";
    translateMonths["February"] = "Février";
    translateMonths["March"] = "Mars";
    translateMonths["April"] = "Avril";
    translateMonths["May"] = "Mai";
    translateMonths["June"] = "Juin";
    translateMonths["July"] = "Juillet";
    translateMonths["August"] = "Août";
    translateMonths["September"] = "Septembre";
    translateMonths["October"] = "Octobre";
    translateMonths["November"] = "Novembre";
    translateMonths["December"] = "Décembre";
}

void Engine::CreateAccount(std::string firstname, std::string surname, string password, int team, bool iside, bool isas){
    this->staff_.push_back(Nurse::Nurse(staff_.size(), firstname, surname, password, team, iside, isas));// /!\ size()
}

void Engine::DeleteAccount(int id){
    //First remove the account to delete from any friend pointer pointing to it
    for (int i = 0; i < this->staff_.size(); ++i){
        if (this->staff_[i].GetFirstFriend() != NULL){
            if (this->staff_[i].GetFirstFriend()->GetId() == id){
                this->staff_[i].SetFirstFriend(NULL);
            }
        }
        if (this->staff_[i].GetSecondFriend() != NULL){
            if (this->staff_[i].GetSecondFriend()->GetId() == id){
                this->staff_[i].SetSecondFriend(NULL);
            }
        }
        if (this->staff_[i].GetThirdFriend() != NULL){
            if (this->staff_[i].GetThirdFriend()->GetId() == id){
                this->staff_[i].SetThirdFriend(NULL);
            }
        }
    }
    // Send to ghost team
    this->staff_[id].team_ = 2;

    // Delete any planning containing that id somewhere

    for (auto it = this->finalplannings_.begin();
         it != this->finalplannings_.end(); ++it){
        bool idInPlanning = false;
        for (auto it2 = it->second.usic1_.begin(); it2 != it->second.usic1_.end(); ++it2){
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3){
                if (*it3 == id){
                    idInPlanning = true;
                }
            }
        }
        for (auto it2 = it->second.usic2_.begin(); it2 != it->second.usic2_.end(); ++it2){
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3){
                if (*it3 == id){
                    idInPlanning = true;
                }
            }
        }
        for (auto it2 = it->second.uca1_.begin(); it2 != it->second.uca1_.end(); ++it2){
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3){
                if (*it3 == id){
                    idInPlanning = true;
                }
            }
        }
        for (auto it2 = it->second.uca2_.begin(); it2 != it->second.uca2_.end(); ++it2){
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3){
                if (*it3 == id){
                    idInPlanning = true;
                }
            }
        }
        for (auto it2 = it->second.hds_.begin(); it2 != it->second.hds_.end(); ++it2){
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3){
                if (*it3 == id){
                    idInPlanning = true;
                }
            }
        }
        for (auto it2 = it->second.rythmo_.begin(); it2 != it->second.rythmo_.end(); ++it2){
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3){
                if (*it3 == id){
                    idInPlanning = true;
                }
            }
        }
        if (idInPlanning){
            this->finalplannings_.erase(it->first);
        }
    }
}

void Engine::AddVacationDay(int id, QDate day) { //TODO : clean passed vacation days

    this->staff_[id].vacationdays_.push_back(day);

}

void Engine::CleanPastVacationDays(int id){
    for (int i = 0; i < this->staff_[id].vacationdays_.size(); ++i){
        if (this->staff_[id].vacationdays_[this->staff_[id].vacationdays_.size() - 1 - i] < QDate::currentDate()){
            this->staff_[id].vacationdays_.erase(this->staff_[id].vacationdays_.end() - 1 - i);
        }
    }
}


void Engine::DeleteVacationDay(int id, int rownumber){
    this->staff_[id].vacationdays_.erase(this->staff_[id].vacationdays_.begin() + rownumber);
}

void Engine::AddFriend(int friendnumber, int idadder, int idadded){
    if (friendnumber == 1) {
       this->staff_[idadder].friend1_ = &this->staff_[idadded];
    }
    if (friendnumber == 2) {
       this->staff_[idadder].friend2_ = &this->staff_[idadded];
    }
    if (friendnumber == 3) {
       this->staff_[idadder].friend3_ = &this->staff_[idadded];
    }
}

void Engine::ChangePassword(int id, string newpassword){
    this->staff_[id].password_ = newpassword;
}

void Engine::UpdateFriendships() {

    for (int nn1 = 0; nn1 < this->staff_.size(); ++nn1){
        for (int nn2 = 0; nn2 < this->staff_.size(); ++nn2){
            vector<int> nnf1, nnf2;
            if (staff_[nn1].friend1_ != NULL){
                nnf1.push_back(staff_[nn1].friend1_->GetId());
            }
            if (staff_[nn1].friend2_ != NULL){
                nnf1.push_back(staff_[nn1].friend2_->GetId());
            }
            if (staff_[nn1].friend3_ != NULL){
                nnf1.push_back(staff_[nn1].friend3_->GetId());
            }
            if (staff_[nn2].friend1_ != NULL){
                nnf2.push_back(staff_[nn2].friend1_->GetId());
            }
            if (staff_[nn2].friend2_ != NULL){
                nnf2.push_back(staff_[nn2].friend2_->GetId());
            }
            if (staff_[nn2].friend3_ != NULL){
                nnf2.push_back(staff_[nn2].friend3_->GetId());
            }
            if (find(nnf1.begin(), nnf1.end(), nn2) != nnf1.end() &&
                    find(nnf2.begin(), nnf2.end(), nn1) != nnf2.end()){
                this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_DOUBLE;
            } else if (find(nnf1.begin(), nnf1.end(), nn2) != nnf1.end()){
                this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_SINGLE;
            } else if (find(nnf2.begin(), nnf2.end(), nn1) != nnf2.end()){
                this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_SINGLE;
            } else {
                this->friendships_[nn1][nn2] = 0;
            }

//            /////
//            if (staff_[nn1].friend1_->id_ == staff_[nn2].id_ ||
//                    staff_[nn1].friend2_->id_ == staff_[nn2].id_ ||
//                    staff_[nn1].friend3_->id_ == staff_[nn2].id_){
//                if (staff_[nn2].friend1_->id_ == staff_[nn1].id_ ||
//                        staff_[nn2].friend2_->id_ == staff_[nn1].id_ ||
//                        staff_[nn2].friend3_->id_ == staff_[nn1].id_){
//                    this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_DOUBLE;
//                } else {
//                    this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_SINGLE;
//                }
//            } else {
//                if (staff_[nn2].friend1_->id_ == staff_[nn1].id_ ||
//                        staff_[nn2].friend2_->id_ == staff_[nn1].id_ ||
//                        staff_[nn2].friend3_->id_ == staff_[nn1].id_){
//                    this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_SINGLE;
//                } else {
//                    this->friendships_[nn1][nn2] = 0;
//                }
//            }
//            /////
        }
    }
    cout << "Friendships updated" << endl;
}

bool Engine::SaveStaff() const{

        QFile savefile(QStringLiteral(PATH_TO_STAFF_DATA));
        if (!savefile.open(QIODevice::WriteOnly)) {
            return false;
        }
        QJsonArray staffarray;
        for (int nurse = 0; nurse < this->staff_.size(); ++nurse){
            QJsonObject myNurse;
            myNurse["id_"] = nurse;
            myNurse["firstname_"] = QString::fromStdString(this->staff_[nurse].firstname_);
            myNurse["surname_"] = QString::fromStdString(this->staff_[nurse].surname_);
            myNurse["team_"] = this->staff_[nurse].team_;
            myNurse["iside_"] = this->staff_[nurse].iside_;
            myNurse["isas_"] = this->staff_[nurse].isas_;
            myNurse["numberofvacationdays_"] = this->staff_[nurse].numberofvacationdays_;
            QJsonArray vacationsarray;
            for (int i = 0; i < this->staff_[nurse].vacationdays_.size(); ++i){
                QString datestring = this->staff_[nurse].vacationdays_[i].toString();
                vacationsarray.append(QJsonValue(datestring));
            }
            myNurse["vacationdaysarray"] = QJsonValue(vacationsarray);
            myNurse["friendID1"] = this->staff_[nurse].GetFirstFriendID();
            myNurse["friendID2"] = this->staff_[nurse].GetSecondFriendID();
            myNurse["friendID3"] = this->staff_[nurse].GetThirdFriendID();
            myNurse["password_"] = QString::fromStdString(Engine::encrypt(this->staff_[nurse].password_));
            myNurse["pathtoprofilepic_"] = QString::fromStdString(this->staff_[nurse].pathtoprofilepic_);

            staffarray.append(QJsonValue(myNurse));
        }


        QJsonDocument saveDoc(staffarray);
        savefile.write(saveDoc.toJson());

        return true;
}

bool Engine::SavePlannings() const{
    QFile savefile(QStringLiteral(PATH_TO_PLANNING_DATA));
    if (!savefile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray planningsarray;
    for (auto n = this->finalplannings_.begin(); n != this->finalplannings_.end(); ++n){
        QJsonObject myPlanning;
        myPlanning["monday_"] = n->second.monday_.toString();
        myPlanning["hasbeenmodified_"] = n->second.hasbeenmodified_;
        //myPlanning["creator_"] = QString::fromStdString(n->second.creator_);
        //myPlanning["dateofcreation_"] = QString::fromStdString(n->second.dateofcreation_);
        //myPlanning["motive_"] = QString::fromStdString(n->second.motive_);

        QJsonArray usic1;
        for (int day = 0; day < n->second.usic1_.size(); ++day){
            QJsonArray array;
            for (int i = 0; i < n->second.usic1_[day].size(); ++i){
                array.append(n->second.usic1_[day][i]);
            }
            usic1.append(QJsonValue(array));
        }
        myPlanning["usic1_"] = QJsonValue(usic1);

        QJsonArray usic2;
        for (int day = 0; day < n->second.usic2_.size(); ++day){
            QJsonArray array;
            for (int i = 0; i < n->second.usic2_[day].size(); ++i){
                array.append(n->second.usic2_[day][i]);
            }
            usic2.append(QJsonValue(array));
        }
        myPlanning["usic2_"] = QJsonValue(usic2);

        QJsonArray uca1;
        for (int day = 0; day < n->second.uca1_.size(); ++day){
            QJsonArray array;
            for (int i = 0; i < n->second.uca1_[day].size(); ++i){
                array.append(n->second.uca1_[day][i]);
            }
            uca1.append(QJsonValue(array));
        }
        myPlanning["uca1_"] = QJsonValue(uca1);

        QJsonArray uca2;
        for (int day = 0; day < n->second.uca2_.size(); ++day){
            QJsonArray array;
            for (int i = 0; i < n->second.uca2_[day].size(); ++i){
                array.append(n->second.uca2_[day][i]);
            }
            uca2.append(QJsonValue(array));
        }
        myPlanning["uca2_"] = QJsonValue(uca2);

        QJsonArray rythmo;
        for (int day = 0; day < n->second.rythmo_.size(); ++day){
            QJsonArray array;
            for (int i = 0; i < n->second.rythmo_[day].size(); ++i){
                array.append(n->second.rythmo_[day][i]);
            }
            rythmo.append(QJsonValue(array));
        }
        myPlanning["rythmo_"] = QJsonValue(rythmo);

        QJsonArray hds;
        for (int day = 0; day < n->second.hds_.size(); ++day){
            QJsonArray array;
            for (int i = 0; i < n->second.hds_[day].size(); ++i){
                array.append(n->second.hds_[day][i]);
            }
            hds.append(QJsonValue(array));
        }
        myPlanning["hds_"] = QJsonValue(hds);


        /*
        myNurse["vacationdaysarray"] = QJsonValue(vacationsarray);
        myNurse["friendID1"] = this->staff_[nurse].GetFirstFriendID();
        myNurse["friendID2"] = this->staff_[nurse].GetSecondFriendID();
        myNurse["friendID3"] = this->staff_[nurse].GetThirdFriendID();
        myNurse["password_"] = QString::fromStdString(Engine::encrypt(this->staff_[nurse].password_));
        myNurse["pathtoprofilepic_"] = QString::fromStdString(this->staff_[nurse].pathtoprofilepic_);

        staffarray.append(QJsonValue(myNurse));
        */
        planningsarray.append(QJsonValue(myPlanning));
    }


    QJsonDocument saveDoc(planningsarray);
    savefile.write(saveDoc.toJson());

    return true;
}


bool Engine::LoadStaff(){

        QFile loadfile(QStringLiteral(PATH_TO_STAFF_DATA));
        if (!loadfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
            return false;
        }
        QString datastring;
        datastring = loadfile.readAll();
        QJsonDocument datadoc = QJsonDocument::fromJson(datastring.toUtf8());
        QJsonArray staffarray = datadoc.array();

        for (int nurse = 0; nurse < staffarray.size(); ++nurse){

            QJsonObject myObject = staffarray[nurse].toObject();

            int id = myObject["id_"].toDouble();
            string firstname = myObject["firstname_"].toString().toStdString();
            string surname = myObject["surname_"].toString().toStdString();
            int team = myObject["team_"].toDouble();
            bool iside = myObject["iside_"].toBool();
            bool isas = myObject["isas_"].toBool();
            int nvd = myObject["team_"].toDouble();

            string password = Engine::decrypt(myObject["password_"].toString().toStdString());
            string pathtoprofilepic = myObject["pathtoprofilepic_"].toString().toStdString();

            Nurse myNurse = Nurse(id, firstname, surname, password, team, iside, isas);
            myNurse.numberofvacationdays_ = nvd;
            myNurse.pathtoprofilepic_ = pathtoprofilepic;

            this->staff_.push_back(myNurse);
            for (int i = 0; i < myObject["vacationdaysarray"].toArray().size(); ++i){
                this->AddVacationDay(id, QDate::fromString(myObject["vacationdaysarray"].toArray()[i].toString()));
            }

        }
        //Second pass to add friendships without segfaults
        for (int nurse = 0; nurse < staffarray.size(); ++nurse){
            QJsonObject myObject = staffarray[nurse].toObject();
            if (int(myObject["friendID1"].toDouble()) >= 0){
                this->staff_[nurse].SetFirstFriend(&this->staff_[int(myObject["friendID1"].toDouble())]);
            }
            if (int(myObject["friendID2"].toDouble()) >= 0){
                this->staff_[nurse].SetSecondFriend(&this->staff_[int(myObject["friendID2"].toDouble())]);
            }
            if (int(myObject["friendID3"].toDouble()) >= 0){
                this->staff_[nurse].SetThirdFriend(&this->staff_[int(myObject["friendID3"].toDouble())]);
            }
        }
        return true;
}

bool Engine::LoadPlannings(){

        QFile loadfile(QStringLiteral(PATH_TO_PLANNING_DATA));
        if (!loadfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
            return false;
        }
        QString datastring;
        datastring = loadfile.readAll();
        QJsonDocument datadoc = QJsonDocument::fromJson(datastring.toUtf8());
        QJsonArray planningsarray = datadoc.array();

        for (int plan = 0; plan < planningsarray.size(); ++plan){

            QJsonObject myObject = planningsarray[plan].toObject();

            string monday = myObject["monday_"].toString().toStdString();
            this->finalplannings_[monday] = Planning(QDate::fromString(QString::fromStdString(monday)));

            this->finalplannings_[monday].hasbeenmodified_ = myObject["hasbeenmodified_"].toBool();


            //Usic1
            QJsonArray usic1array = myObject["usic1_"].toArray();
            for (int day = 0; day < 7; ++day){
                QJsonArray arr = usic1array[day].toArray();
                for (int i = 0; i < arr.count(); ++i){
                    this->finalplannings_[monday].usic1_[day][i] = arr[i].toInt();
                }
            }
            //Usic2
            QJsonArray usic2array = myObject["usic2_"].toArray();
            for (int day = 0; day < 7; ++day){
                QJsonArray arr = usic2array[day].toArray();
                for (int i = 0; i < arr.count(); ++i){
                    this->finalplannings_[monday].usic2_[day][i] = arr[i].toInt();
                }
            }
            //Uca1
            QJsonArray uca1array = myObject["uca1_"].toArray();
            for (int day = 0; day < 7; ++day){
                QJsonArray arr = uca1array[day].toArray();
                for (int i = 0; i < arr.count(); ++i){
                    this->finalplannings_[monday].uca1_[day][i] = arr[i].toInt();
                }
            }
            //Uca2
            QJsonArray uca2array = myObject["uca2_"].toArray();
            for (int day = 0; day < 7; ++day){
                QJsonArray arr = uca2array[day].toArray();
                for (int i = 0; i < arr.count(); ++i){
                    this->finalplannings_[monday].uca2_[day][i] = arr[i].toInt();
                }
            }
            //Rythmo
            QJsonArray rythmoarray = myObject["rythmo_"].toArray();
            for (int day = 0; day < 7; ++day){
                QJsonArray arr = rythmoarray[day].toArray();
                for (int i = 0; i < arr.count(); ++i){
                    this->finalplannings_[monday].rythmo_[day][i] = arr[i].toInt();
                }
            }
            //Hds
            QJsonArray hdsarray = myObject["hds_"].toArray();
            for (int day = 0; day < 4; ++day){
                QJsonArray arr = hdsarray[day].toArray();
                for (int i = 0; i < arr.count(); ++i){
                    this->finalplannings_[monday].hds_[day][i] = arr[i].toInt();
                }
            }
        }

        return true;
}

bool Engine::SaveHistory() const{
    QFile savefile(QStringLiteral(PATH_TO_HISTORY_DATA));
    if (!savefile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray array;
    for (auto it = planninghistory_.begin(); it != planninghistory_.end(); ++it){
        QJsonObject myEntry;
        myEntry["planningmonday_"] = QJsonValue(it->planningmonday_.toString());
        myEntry["modifdate_"] = QJsonValue(it->modifdate_.toString());
        myEntry["modiffirstname_"] = QJsonValue(QString::fromStdString(it->modiffirstname_));
        myEntry["modifsurname_"] = QJsonValue(QString::fromStdString(it->modifsurname_));
        myEntry["modiftime_"] = QJsonValue(it->modiftime_.toString());
        myEntry["motive_"] = QJsonValue(QString::fromStdString(it->motive_));
        array.append(QJsonValue(myEntry));
    }
    QJsonDocument saveDoc(array);
    savefile.write(saveDoc.toJson());

    return true;
}

bool Engine::LoadHistory(){

    QFile loadfile(QStringLiteral(PATH_TO_HISTORY_DATA));
    if (!loadfile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        return false;
    }
    QString datastring;
    datastring = loadfile.readAll();
    QJsonDocument datadoc = QJsonDocument::fromJson(datastring.toUtf8());
    QJsonArray array = datadoc.array();

    for (int entry = 0; entry < array.size(); ++entry){

        QJsonObject myObject = array[entry].toObject();
        PlanningEntry myEntry;

        string planningmonday = myObject["planningmonday_"].toString().toStdString();
        myEntry.planningmonday_ = QDate::fromString(QString::fromStdString(planningmonday));
        myEntry.modiffirstname_ = myObject["modiffirstname_"].toString().toStdString();
        myEntry.modifsurname_ = myObject["modifsurname_"].toString().toStdString();
        myEntry.motive_ = myObject["motive_"].toString().toStdString();
        string modifdate = myObject["modifdate_"].toString().toStdString();
        myEntry.modifdate_ = QDate::fromString(QString::fromStdString(modifdate));
        string modiftime = myObject["modiftime_"].toString().toStdString();
        myEntry.modiftime_ = QTime::fromString(QString::fromStdString(modiftime));

        this->planninghistory_.push_back(myEntry);
    }
    return true;
}


string Engine::encrypt(string clear){
    string result = "";
    for (int i = 0; i < clear.length(); ++i){
        char c = clear.at(i);
        c = (c + i) % 256;
        result += string(1, c);
    }
    return result;
}

string Engine::decrypt(string coded){
    string result = "";
    for (int i = 0; i < coded.length(); ++i){
        char c = coded.at(i);
        c = (c - i) % 256;
        result += string(1, c);
    }
    return result;
}

float Engine::ComputeObjective(const Planning &planning) const{
    //Compute the friendship term
    float friendshipscore = 0;
    for (int day = 0; day < 7; ++day){
        //Usic1
        friendshipscore += this->friendships_[planning.usic1_[day][0]][planning.usic1_[day][1]];
        friendshipscore += this->friendships_[planning.usic1_[day][0]][planning.usic1_[day][2]];
        friendshipscore += this->friendships_[planning.usic1_[day][1]][planning.usic1_[day][2]];
        //Usic2 et Uca2 -> a confirmer qu'ils sont bien ensemble au travail
        friendshipscore += this->friendships_[planning.usic2_[day][0]][planning.uca2_[day][0]];
        //Uca1
        friendshipscore += this->friendships_[planning.uca1_[day][0]][planning.uca1_[day][1]];
        friendshipscore += this->friendships_[planning.uca1_[day][0]][planning.uca1_[day][2]];
        friendshipscore += this->friendships_[planning.uca1_[day][1]][planning.uca1_[day][2]];
        //Coro
        friendshipscore += this->friendships_[planning.usic1_[day][0]][planning.uca1_[day][0]];
        //Rythmo
        friendshipscore += this->friendships_[planning.rythmo_[day][0]][planning.rythmo_[day][1]];
        friendshipscore += this->friendships_[planning.rythmo_[day][0]][planning.rythmo_[day][2]];
        friendshipscore += this->friendships_[planning.rythmo_[day][1]][planning.rythmo_[day][2]];
        friendshipscore += this->friendships_[planning.rythmo_[day][3]][planning.rythmo_[day][0]];
        friendshipscore += this->friendships_[planning.rythmo_[day][3]][planning.rythmo_[day][1]];
        friendshipscore += this->friendships_[planning.rythmo_[day][3]][planning.rythmo_[day][2]];
        //HdS
        if (day < 4){
            friendshipscore += this->friendships_[planning.hds_[day][0]][planning.hds_[day][1]];
        }
    }
    //cout << "Friendship scores computed : " << friendshipscore << endl;
    //Compute the entropy   //Order of units is USIC1, USIC2, UCA1, UCA2, Rythmo, HDS
    unordered_map< int, vector <int> > affectations;
    int unitnumber;
    //usic1
    unitnumber = 0;
    for (int day = 0; day < 7; ++day){
        for (int i = 0; i < planning.usic1_[day].size(); ++i){
            if (affectations.count(planning.usic1_[day][i]) == 0){
                affectations[planning.usic1_[day][i]] = vector<int>(7, 0);
            }
            affectations[planning.usic1_[day][i]][unitnumber]++;
        }
    }
    //Usic2
    unitnumber = 1;
    for (int day = 0; day < 7; ++day){
        for (int i = 0; i < planning.usic2_[day].size(); ++i){
            if (affectations.count(planning.usic2_[day][i]) == 0){
                affectations[planning.usic2_[day][i]] = vector<int>(7, 0);
            }
            affectations[planning.usic2_[day][i]][unitnumber]++;
        }
    }
    //Uca1
    unitnumber = 2;
    for (int day = 0; day < 7; ++day){
        for (int i = 0; i < planning.uca1_[day].size(); ++i){
            if (affectations.count(planning.uca1_[day][i]) == 0){
                affectations[planning.uca1_[day][i]] = vector<int>(7, 0);
            }
            affectations[planning.uca1_[day][i]][unitnumber]++;
        }
    }
    //Uca2
    unitnumber = 3;
    for (int day = 0; day < 7; ++day){
        for (int i = 0; i < planning.uca2_[day].size(); ++i){
            if (affectations.count(planning.uca2_[day][i]) == 0){
                affectations[planning.uca2_[day][i]] = vector<int>(7, 0);
            }
            affectations[planning.uca2_[day][i]][unitnumber]++;
        }
    }
    //Rythmo
    unitnumber = 4;
    for (int day = 0; day < 7; ++day){
        for (int i = 0; i < planning.rythmo_[day].size(); ++i){
            if (affectations.count(planning.rythmo_[day][i]) == 0){
                affectations[planning.rythmo_[day][i]] = vector<int>(7, 0);
            }
            affectations[planning.rythmo_[day][i]][unitnumber]++;
        }
    }
    //Hds
    unitnumber = 5;
    for (int day = 0; day < 4; ++day){
        for (int i = 0; i < planning.hds_[day].size(); ++i){
            if (affectations.count(planning.hds_[day][i]) == 0){
                affectations[planning.hds_[day][i]] = vector<int>(7, 0);
            }
            affectations[planning.hds_[day][i]][unitnumber]++;
        }
    }

    //Now the actual computation
    float entropy = 0;
    for (auto it = affectations.begin(); it != affectations.end(); ++it){
        float sum = std::accumulate(it->second.begin(), it->second.end(), 0.);
        for (int i = 0; i < it->second.size(); ++i){
            float x = it->second[i] / sum;
            if (x > 0){
              entropy -= x * std::log(x);
            }
        }
    }
    return entropy + FRIENSHIP_LAMBDA * friendshipscore;
}

bool Engine::IsLegal(const Planning &planning) const{
    for (int day = 0; day < 7; ++day){
        int nasusic1 = 0, nasusic2 = 0, nasuca1 = 0, nasuca2 = 0, nashds = 0, nasrythmo = 0;
        //usic1
        for (int i = 0; i < planning.usic1_[day].size(); ++i){
            if (this->staff_[planning.usic1_[day][i]].isas_){
                ++nasusic1;
            }
        }
        //usic2
        for (int i = 0; i < planning.usic2_[day].size(); ++i){
            if (this->staff_[planning.usic2_[day][i]].isas_){
                ++nasusic2;
            }
        }
        //uca1
        for (int i = 0; i < planning.uca1_[day].size(); ++i){
            if (this->staff_[planning.uca1_[day][i]].isas_){
                ++nasuca1;
            }
        }
        //uca2
        for (int i = 0; i < planning.uca2_[day].size(); ++i){
            if (this->staff_[planning.uca2_[day][i]].isas_){
                ++nasuca2;
            }
        }
        //rythmo
        for (int i = 0; i < planning.rythmo_[day].size(); ++i){
            if (this->staff_[planning.rythmo_[day][i]].isas_){
                ++nasrythmo;
            }
        }

        //Hds
        if (day < 4){
            for (int i = 0; i < planning.hds_[day].size(); ++i){
                if (this->staff_[planning.hds_[day][i]].isas_){
                    ++nashds;
                }
            }
        }
        if (nashds > 1 || nasrythmo > 1 || nasuca1 > 1 || nasusic1 > 0 ||
                nasuca2 > 0 || nasusic2 > 0){
            return false;
        }
    }
    return true;
}

bool Engine::PerturbatePlanning(Planning &myplanning){//Return true if swapping was successful
    //Pick a day
    int day = rand() % 7;
    //Generate two random slots to swap
    int unit1 = rand() % 6;
    int slot1;
    vector<int>* p1;
    if (unit1 == 5){
        if (day < 4){
            slot1 = rand() % 2;
            p1 = &myplanning.hds_[day];
        } else {
            unit1 = rand() % 5;
        }
    }
    if (unit1 == 0){
        slot1 = rand() % 3;
        p1 = &myplanning.usic1_[day];
    }
    if (unit1 == 1){
        slot1 = 0;
        p1 = &myplanning.usic2_[day];
    }
    if (unit1 == 2){
        slot1 = rand() % 3;
        p1 = &myplanning.uca1_[day];
    }
    if (unit1 == 3){
        slot1 = 0;
        p1 = &myplanning.uca2_[day];
    }
    if (unit1 == 4){
        slot1 = rand() % 4;
        p1 = &myplanning.rythmo_[day];
    }

    int unit2;
    int slot2;
    vector<int>* p2;
    do {
        unit2 = rand() % 6;
        if (unit2 == 5){
            if (day < 4){
                slot2 = rand() % 2;
                p2 = &myplanning.hds_[day];
            } else {
                unit2 = rand() % 5;
            }
        }
        if (unit2 == 0){
            slot2 = rand() % 3;
            p2 = &myplanning.usic1_[day];
        }
        if (unit2 == 1){
            slot2 = 0;
            p2 = &myplanning.usic2_[day];
        }
        if (unit2 == 2){
            slot2 = rand() % 3;
            p2 = &myplanning.uca1_[day];
        }
        if (unit2 == 3){
            slot2 = 0;
            p2 = &myplanning.uca2_[day];
        }
        if (unit2 == 4){
            slot2 = rand() % 4;
            p2 = &myplanning.rythmo_[day];
        }
    } while (unit1 == unit2 && slot1 == slot2);
    //Swap
    int save = (*p1)[slot1];
    (*p1)[slot1] = (*p2)[slot2];
    (*p2)[slot2] = save;
    //Check for legality
    if (this->staff_[(*p1)[slot1]].iside_ != this->staff_[(*p2)[slot2]].iside_){
        if (this->IsLegal(myplanning) == false){
            save = (*p1)[slot1];
            (*p1)[slot1] = (*p2)[slot2];
            (*p2)[slot2] = save;
            return false;
        }
    }
    return true;
}


Planning Engine::GetRandomPlanning(QDate monday, const unordered_set<string>& daysTeamA){
    Planning myplanning = Planning(monday);
    //Number the slots : 3 for usic1, 1 for usic2, 3 uca1, 1 uca2, 4 rythmo, 2 hds except on the weekend+friday
    // (3+1+3+1+4+2) * 4 + (3+1+3+1+4) * 3 -> first 56 are early week, next 36 are weekend, total 82.
    //Can hold an AS : rythmo, hds and uca 1 -> slots 14 * k + (6, 11, 13) (k=0..3) et 56 + 12 * (6, 11) (k=0..2)
    //Build the list of available people everyday for the week in question
    for (QDate day = monday; day < monday.addDays(7); day = day.addDays(1)){
        int dayint = monday.daysTo(day);
        //For everyday, build the list of people available
        vector<int> availableIDE, availableAS;
        for (int n = 0; n < this->staff_.size(); ++n){
            if (this->staff_[n].team_ == 0 && daysTeamA.count(day.toString().toStdString()) > 0) {
                if (find(this->staff_[n].vacationdays_.begin(), this->staff_[n].vacationdays_.end(), day) ==
                        this->staff_[n].vacationdays_.end()){
                    if (this->staff_[n].iside_){
                        availableIDE.push_back(n);
                    } else {
                        availableAS.push_back(n);
                    }
                }
            } else if (this->staff_[n].team_ > 0 && daysTeamA.count(day.toString().toStdString()) == 0){
                if (find(this->staff_[n].vacationdays_.begin(), this->staff_[n].vacationdays_.end(), day) ==
                        this->staff_[n].vacationdays_.end()){
                    if (this->staff_[n].iside_){
                        availableIDE.push_back(n);
                    } else {
                        availableAS.push_back(n);
                    }
                }
            }
        }
//        std::cout << "c" << std::endl;
        random_shuffle(availableAS.begin(), availableAS.end());
        random_shuffle(availableIDE.begin(), availableIDE.end());
        random_shuffle(availableAS.begin(), availableAS.end());
        random_shuffle(availableIDE.begin(), availableIDE.end());
        //Fill the planning
        //The AS Slots, last ones of Rythmo, Hds and UCA1
        if (availableAS.size() > 0){
            myplanning.uca1_[dayint][2] = availableAS[0];
            if (availableAS.size() > 1){
                myplanning.rythmo_[dayint][3] = availableAS[1];
                if (availableAS.size() > 2 && day.dayOfWeek() <= 4){
                    myplanning.hds_[dayint][1] = availableAS[2];
                }
            }
        }
//        std::cout << "availableAS are in number : " << availableAS.size() << std::endl;
//        std::cout << "availableIDE are in number : " << availableIDE.size() << std::endl;
        for (auto it = availableIDE.begin(); it != availableIDE.end(); ++it){
//            std::cout << *it << " ";
        }
//        std::cout << std::endl;
        //Check that we have enough people to fill the planning
        if (std::min((int)availableAS.size(), 3) + availableIDE.size() < 14 - 2 * (dayint > 3)){
            std::string s = "Impossible de calculer le planning; trop peu de personnels disponibles le ";
            s += this->translateDays[monday.toString("dddd").toStdString()] + ".";
            return Planning(s);
        }
        //Fill the rest of the slots with IDEs
        int count = 0;
        myplanning.usic1_[dayint][0] = availableIDE[count];
        ++count;
        myplanning.usic1_[dayint][1] = availableIDE[count];
        ++count;
        myplanning.usic1_[dayint][2] = availableIDE[count];
        ++count;
        myplanning.usic2_[dayint][0] = availableIDE[count];
        ++count;
        myplanning.uca1_[dayint][0] = availableIDE[count];
        ++count;
        myplanning.uca1_[dayint][1] = availableIDE[count];
        ++count;
        if (availableAS.size() <= 0){
            myplanning.uca1_[dayint][2] = availableIDE[count];
            ++count;
        }
        myplanning.uca2_[dayint][0] = availableIDE[count];
        ++count;
        myplanning.rythmo_[dayint][0] = availableIDE[count];
        ++count;
        myplanning.rythmo_[dayint][1] = availableIDE[count];
        ++count;
        myplanning.rythmo_[dayint][2] = availableIDE[count];
        ++count;
        if (availableAS.size() <= 1){
            myplanning.rythmo_[dayint][3] = availableIDE[count];
            ++count;
        }
        if (dayint < 4){
            myplanning.hds_[dayint][0] = availableIDE[count];
            ++count;
            if (availableAS.size() <= 2){
                myplanning.hds_[dayint][1] = availableIDE[count];
                ++count;
            }
        }
        //At this point the planning is filled and is legal normally
    }
    return myplanning;

}


void Engine::GeneratePlanning(QDate monday, const unordered_set<string>& daysTeamA){
    this->UpdateFriendships();
    srand(clock());
    vector<Planning> population;
    for (int i = 0; i < ALGORITHM_MU; ++i){
        population.push_back(this->GetRandomPlanning(monday, daysTeamA));
        if (population.back().errormessage_.length() > 0){
            //std::cout << population.back().errormessage_ << std::endl;
            this->finalplannings_[monday.toString().toStdString()] = population.back();
            return;
        }
    }
    for (int iteration = 0; iteration < ALGORITHM_MAXITER; ++iteration){
        vector<Planning> newpopulation;
        newpopulation.reserve(ALGORITHM_MU);
        for (int i = 0; i < ALGORITHM_MU; ++i){
            for (int j = 0; j < ALGORITHM_LAMBDA; ++j){
                population.push_back(population[i]);
                int count = 0;
                while (count < ALGORITHM_N){
                    if (this->PerturbatePlanning(population.back())){
                        ++count;
                    }
                }
            }
        }
        vector<float> scores = vector<float>(population.size(), 0);
        for (int i = 0; i < population.size(); ++i){
            scores[i] = this->ComputeObjective(population[i]);
        }
        vector<float> tosort = scores;
        sort(tosort.begin(), tosort.end());
        float median = tosort[tosort.size() / 2];
        cout << "median is : " << median << " and max is : " << tosort.back() <<endl;
        for (int i = 0; i < scores.size(); ++i){
            if (scores[i] >= median){
                newpopulation.push_back(population[i]);
            }
        }
        population.clear();
        population = newpopulation;
    }
    float maxi = 0;
    int maxindex = 0;
    for (int i = 0; i < ALGORITHM_MU; ++i){
        float obj = this->ComputeObjective(population[i]);
        if (obj > maxi){
            maxi = obj;
            maxindex = i;
        }
    }
    this->finalplannings_[monday.toString().toStdString()] = population[maxindex];
    this->SavePlannings();
}

bool Engine::SwitchSlots(int id1, QDate date1, int id2, QDate date2){
    std::cout << id1 << " " << date1.toString().toStdString() << " " << id2 << " " <<
                 date2.toString().toStdString() << std::endl;
    QDate monday1 = date1;
    while (monday1.dayOfWeek() > 1){
      monday1 = monday1.addDays(-1);
    }
    QDate monday2 = date2;
    while (monday2.dayOfWeek() > 1){
      monday2 = monday2.addDays(-1);
    }
    if (monday1 != monday2){
        return false;
    }
    if (this->finalplannings_.count(monday1.toString().toStdString()) == 0){
        return false;
    }
    int ndays1 = monday1.daysTo(date1);
    int ndays2 = monday2.daysTo(date2);
    std::cout << ndays1 << " " << ndays2 << std::endl;
    Planning* myplanning = &this->finalplannings_[monday1.toString().toStdString()];
    int* p1 = NULL;
    int* p2 = NULL;

    //Look in usic1
    for (int i = 0; i < 1; ++i){
        for (int j = 0; j < myplanning->usic1_[ndays1].size(); ++j){
            if (myplanning->usic1_[ndays1][j] == id1){
                p1 = &(myplanning->usic1_[ndays1][j]);
            }
        }
        for (int j = 0; j < myplanning->usic1_[ndays2].size(); ++j){
            if (myplanning->usic1_[ndays2][j] == id2){
                p2 = &(myplanning->usic1_[ndays2][j]);
            }
        }
    }
    //Look in usic2
    for (int i = 0; i < 1; ++i){
        for (int j = 0; j < myplanning->usic2_[ndays1].size(); ++j){
            if (myplanning->usic2_[ndays1][j] == id1){
                p1 = &(myplanning->usic2_[ndays1][j]);
            }
        }
        for (int j = 0; j < myplanning->usic1_[ndays2].size(); ++j){
            if (myplanning->usic2_[ndays2][j] == id2){
                p2 = &(myplanning->usic2_[ndays2][j]);
            }
        }
    }
    //Look in uca1
    for (int i = 0; i < 1; ++i){
        for (int j = 0; j < myplanning->uca1_[ndays1].size(); ++j){
            if (myplanning->uca1_[ndays1][j] == id1){
                p1 = &(myplanning->uca1_[ndays1][j]);
            }
        }
        for (int j = 0; j < myplanning->usic1_[ndays2].size(); ++j){
            if (myplanning->uca1_[ndays2][j] == id2){
                p2 = &(myplanning->uca1_[ndays2][j]);
            }
        }
    }
    //Look in uca2
    for (int i = 0; i < 1; ++i){
        for (int j = 0; j < myplanning->uca2_[ndays1].size(); ++j){
            if (myplanning->uca2_[ndays1][j] == id1){
                p1 = &(myplanning->uca2_[ndays1][j]);
            }
        }
        for (int j = 0; j < myplanning->usic1_[ndays2].size(); ++j){
            if (myplanning->uca2_[ndays2][j] == id2){
                p2 = &(myplanning->uca2_[ndays2][j]);
            }
        }
    }
    //Look in rythmo
    for (int i = 0; i < 1; ++i){
        for (int j = 0; j < myplanning->rythmo_[ndays1].size(); ++j){
            if (myplanning->rythmo_[ndays1][j] == id1){
                p1 = &(myplanning->rythmo_[ndays1][j]);
            }
        }
        for (int j = 0; j < myplanning->usic1_[ndays2].size(); ++j){
            if (myplanning->rythmo_[ndays2][j] == id2){
                p2 = &(myplanning->rythmo_[ndays2][j]);
            }
        }
    }
    //Look in hds
    if (ndays1 < 4){
        for (int j = 0; j < myplanning->hds_[ndays1].size(); ++j){
            if (myplanning->hds_[ndays1][j] == id1){
                p1 = &(myplanning->hds_[ndays1][j]);
            }
        }
        for (int j = 0; j < myplanning->usic1_[ndays2].size(); ++j){
            if (myplanning->hds_[ndays2][j] == id2){
                p2 = &(myplanning->hds_[ndays2][j]);
            }
        }
    }
    if (p1 == NULL || p2 == NULL){
        return false;
    }
    std::cout << this->finalplannings_[monday1.toString().toStdString()].usic1_[5][0] << " " <<
                 this->finalplannings_[monday1.toString().toStdString()].rythmo_[5][0] << std::endl;
    int save = *p1;
    *p1 = *p2;
    *p2 = save;
    this->SavePlannings();
    std::cout << this->finalplannings_[monday1.toString().toStdString()].usic1_[5][0] << " " <<
                 this->finalplannings_[monday1.toString().toStdString()].rythmo_[5][0] << std::endl;
    return true;
}
