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
    //Perform decrement : any ID that is greater than the deleted one must be decremented
    for (int i = 0; i < this->staff_.size(); ++i){
        if (this->staff_[i].GetFirstFriend() != NULL){
            if (this->staff_[i].GetFirstFriend()->GetId() > id){
                this->staff_[i].SetFirstFriend(&this->staff_[this->staff_[i].GetFirstFriend()->GetId()-1]);
            }
        }
        if (this->staff_[i].GetSecondFriend() != NULL){
            if (this->staff_[i].GetSecondFriend()->GetId() > id){
                this->staff_[i].SetSecondFriend(&this->staff_[this->staff_[i].GetSecondFriend()->GetId()-1]);
            }
        }
        if (this->staff_[i].GetThirdFriend() != NULL){
            if (this->staff_[i].GetThirdFriend()->GetId() > id){
                this->staff_[i].SetThirdFriend(&this->staff_[this->staff_[i].GetThirdFriend()->GetId()-1]);
            }
        }
    }
    //Now perform the deletion and update IDs
    this->staff_.erase(this->staff_.begin() + id);
    for (int i = 0; i < this->staff_.size(); ++i){
        this->staff_[i].id_ = i;
    }
}

void Engine::AddVacationDay(int id, QDate day) { //TODO : clean passed vacation days

    this->staff_[id].vacationdays_.push_back(day);

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

//            myNurse["id_"] = nurse;
            int id = myObject["id_"].toDouble();
//            myNurse["firstname_"] = QString::fromStdString(this->staff_[nurse].firstname_);
            string firstname = myObject["firstname_"].toString().toStdString();
//            myNurse["surname_"] = QString::fromStdString(this->staff_[nurse].surname_);
            string surname = myObject["surname_"].toString().toStdString();
//            myNurse["team_"] = this->staff_[nurse].team_;
            int team = myObject["team_"].toDouble();
//            myNurse["iside_"] = this->staff_[nurse].iside_;
            bool iside = myObject["iside_"].toBool();
//            myNurse["isas_"] = this->staff_[nurse].isas_;
            bool isas = myObject["isas_"].toBool();
//            myNurse["numberofvacationdays_"] = this->staff_[nurse].numberofvacationdays_;
            int nvd = myObject["team_"].toDouble();

//            myNurse["friendID1"] = this->staff_[nurse].GetFirstFriendID();
//            myNurse["friendID2"] = this->staff_[nurse].GetSecondFriendID();
//            myNurse["friendID3"] = this->staff_[nurse].GetThirdFriendID();
//            myNurse["password_"] = QString::fromStdString(this->staff_[nurse].password_);
            string password = Engine::decrypt(myObject["password_"].toString().toStdString());
//            myNurse["pathtoprofilepic_"] = QString::fromStdString(this->staff_[nurse].pathtoprofilepic_);
            string pathtoprofilepic = myObject["pathtoprofilepic_"].toString().toStdString();

            Nurse myNurse = Nurse(id, firstname, surname, password, team, iside, isas);
            myNurse.numberofvacationdays_ = nvd;
            myNurse.pathtoprofilepic_ = pathtoprofilepic;

//            staffarray.append(QJsonValue(myNurse));
            this->staff_.push_back(myNurse);
            //            QJsonArray vacationsarray;
            for (int i = 0; i < myObject["vacationdaysarray"].toArray().size(); ++i){
            //                QString datestring = this->staff_[nurse].vacationdays_[i].toString();
                this->AddVacationDay(id, QDate::fromString(myObject["vacationdaysarray"].toArray()[i].toString()));
            //                vacationsarray.append(QJsonValue(datestring));
            }
            //            myNurse["vacationdaysarray"] = QJsonValue(vacationsarray);

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
//    cout << "aze" << endl;

//    for (auto it = affectations.begin(); it != affectations.end(); ++it){
//        cout << it->first << " ";
//        for (int i = 0; i < (it->second).size(); ++i){
//            cout << (it->second)[i] << " ";
//        }
//        cout << endl;
//    }
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
//    cout << "Swapping "  << staff_[(*p1)[slot1]].GetFirstName() << " " << staff_[(*p1)[slot1]].GetSurname() <<
//            " with " << staff_[(*p2)[slot2]].GetFirstName() << " " << staff_[(*p2)[slot2]].GetSurname() << endl;
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
        //Fill the rest of the slots with IDEs
        int count = 0;
        myplanning.usic1_[dayint][0] = availableIDE[count];
        ++count;
//        std::cout << count << std::endl;
        myplanning.usic1_[dayint][1] = availableIDE[count];
        ++count;
//        std::cout << count << std::endl;
        myplanning.usic1_[dayint][2] = availableIDE[count];
        ++count;
//        std::cout << count << std::endl;
        myplanning.usic2_[dayint][0] = availableIDE[count];
        ++count;
        // std::cout << count << std::endl;
        myplanning.uca1_[dayint][0] = availableIDE[count];
        ++count;
        // std::cout << count << std::endl;
        myplanning.uca1_[dayint][1] = availableIDE[count];
        ++count;
        // std::cout << count << std::endl;
        if (availableAS.size() <= 0){
            myplanning.uca1_[dayint][2] = availableIDE[count];
            ++count;
            // std::cout << "r" << count << std::endl;
        }
        myplanning.uca2_[dayint][0] = availableIDE[count];
        ++count;
        // std::cout << count << std::endl;
        myplanning.rythmo_[dayint][0] = availableIDE[count];
        ++count;
        // std::cout << count << std::endl;
        myplanning.rythmo_[dayint][1] = availableIDE[count];
        ++count;
        // std::cout << count << std::endl;
        myplanning.rythmo_[dayint][2] = availableIDE[count];
        ++count;
        // std::cout << count << "e" << std::endl;
        if (availableAS.size() <= 1){
            myplanning.rythmo_[dayint][3] = availableIDE[count];
            ++count;
            // std::cout << "r" << count << std::endl;
        }
        if (dayint < 4){
            myplanning.hds_[dayint][0] = availableIDE[count];
            ++count;
            // std::cout << count << std::endl;
            if (availableAS.size() <= 2){
                myplanning.hds_[dayint][1] = availableIDE[count];
                ++count;
                // std::cout << "r" << count << std::endl;
            }
        }
        // std::cout << "e" << std::endl;
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
    //this->finalplannings_[monday.toString().toStdString()]
    this->SavePlannings();
}
