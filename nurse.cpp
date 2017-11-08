#include "nurse.h"

using namespace std;

Nurse::Nurse(int id, std::string firstname, std::string surname, string pw, int team, bool iside, bool isas) {
  id_ = id;
  firstname_ = firstname;
  surname_ = surname;
  team_ = team;
  iside_ = iside;
  isas_ = isas;
  numberofvacationdays_ = 0;
  vacationdays_ = vector<QDate>();
  friend1_ = NULL;
  friend2_ = NULL;
  friend3_ = NULL;
  password_ = pw;
  pathtoprofilepic_ = "";
}

string Nurse::GetFirstName() const{
    return this->firstname_;
}

string Nurse::GetSurname() const{
    return this->surname_;
}

bool Nurse::IsAs() const{
    return this->isas_;
}

bool Nurse::IsIde() const{
    return this->iside_;
}

string Nurse::GetPassword() const{
    return this->password_;
}

int Nurse::GetId() const{
    return this->id_;
}

Nurse* Nurse::GetFirstFriend() const{
    return this->friend1_;
}

Nurse* Nurse::GetSecondFriend() const{
    return this->friend2_;
}

Nurse* Nurse::GetThirdFriend() const{
    return this->friend3_;
}

int Nurse::GetFirstFriendID() const{
    if (this->friend1_ != NULL){
      return this->friend1_->GetId();
    }
    return -1;
}

int Nurse::GetSecondFriendID() const{
    if (this->friend2_ != NULL){
      return this->friend2_->GetId();
    }
    return -1;
}

int Nurse::GetThirdFriendID() const{
    if (this->friend3_ != NULL){
      return this->friend3_->GetId();
    }
    return -1;
}

int Nurse::GetTeam() const{
    return this->team_;
}

void Nurse::SetFirstFriend(Nurse* myfriend){
    this->friend1_ = myfriend;
}

void Nurse::SetSecondFriend(Nurse* myfriend){
    this->friend2_ = myfriend;
}

void Nurse::SetThirdFriend(Nurse* myfriend){
    this->friend3_ = myfriend;
}

string Nurse::GetPathToProfilePic() const{
    return this->pathtoprofilepic_;
}
