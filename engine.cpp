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
    friendships_ = vector< vector <int> >();

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
            if (staff_[nn1].friend1_->id_ == staff_[nn2].id_ ||
                    staff_[nn1].friend2_->id_ == staff_[nn2].id_ ||
                    staff_[nn1].friend3_->id_ == staff_[nn2].id_){
                if (staff_[nn2].friend1_->id_ == staff_[nn1].id_ ||
                        staff_[nn2].friend2_->id_ == staff_[nn1].id_ ||
                        staff_[nn2].friend3_->id_ == staff_[nn1].id_){
                    this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_DOUBLE;
                } else {
                    this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_SINGLE;
                }
            } else {
                if (staff_[nn2].friend1_->id_ == staff_[nn1].id_ ||
                        staff_[nn2].friend2_->id_ == staff_[nn1].id_ ||
                        staff_[nn2].friend3_->id_ == staff_[nn1].id_){
                    this->friendships_[nn1][nn2] = WEIGHT_FRIENDSHIP_SINGLE;
                } else {
                    this->friendships_[nn1][nn2] = 0;
                }
            }
        }
    }
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
            entropy -= (it->second[i] / sum) * std::log(it->second[i] / sum);
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




Planning Engine::GetRandomPlanning(QDate monday, const unordered_set<string>& daysTeamA){
    Planning myplanning = Planning(monday);
    //Number the slots : 3 for usic1, 1 for usic2, 3 uca1, 1 uca2, 4 rythmo, 2 hds except on the weekend+friday
    // (3+1+3+1+4+2) * 4 + (3+1+3+1+4) * 3 -> first 56 are early week, next 36 are weekend, total 82.
    //Can hold an AS : rythmo, hds and uca 1 -> slots 14 * k + (6, 11, 13) (k=0..3) et 56 + 12 * (6, 11) (k=0..2)
    std::cout << "a" << std::endl;
    //Build the list of available people everyday for the week in question
    for (QDate day = monday; day < monday.addDays(7); day = day.addDays(1)){
        std::cout << "b" << std::endl;
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
        std::cout << "c" << std::endl;
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
        std::cout << "availableAS are in number : " << availableAS.size() << std::endl;
        std::cout << "availableIDE are in number : " << availableIDE.size() << std::endl;
        for (auto it = availableIDE.begin(); it != availableIDE.end(); ++it){
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        //Fill the rest of the slots with IDEs
        int count = 0;
        myplanning.usic1_[dayint][0] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.usic1_[dayint][1] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.usic1_[dayint][2] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.usic2_[dayint][0] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.uca1_[dayint][0] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.uca1_[dayint][1] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        if (availableAS.size() <= 0){
            myplanning.uca1_[dayint][2] = availableIDE[count];
            ++count;
            std::cout << "r" << count << std::endl;
        }
        myplanning.uca2_[dayint][0] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.rythmo_[dayint][0] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.rythmo_[dayint][1] = availableIDE[count];
        ++count;
        std::cout << count << std::endl;
        myplanning.rythmo_[dayint][2] = availableIDE[count];
        ++count;
        std::cout << count << "e" << std::endl;
        if (availableAS.size() <= 1){
            myplanning.rythmo_[dayint][3] = availableIDE[count];
            ++count;
            std::cout << "r" << count << std::endl;
        }
        if (dayint < 4){
            myplanning.hds_[dayint][0] = availableIDE[count];
            ++count;
            std::cout << count << std::endl;
            if (availableAS.size() <= 2){
                myplanning.hds_[dayint][1] = availableIDE[count];
                ++count;
                std::cout << "r" << count << std::endl;
            }
        }
        std::cout << "e" << std::endl;
        //At this point the planning is filled and is legal normally
    }
    return myplanning;

}

