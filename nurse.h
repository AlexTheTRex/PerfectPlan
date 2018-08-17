#ifndef NURSE_H
#define NURSE_H

#include <vector>
#include <string>
#include <QDate>

class Nurse {

  friend class Engine;

 public:
  Nurse(int id, std::string firstname, std::string surname, std::string pw, int team, bool iside, bool isas);

  int GetId() const;
  std::string GetFirstName() const;
  std::string GetSurname() const;
  std::string GetPassword() const;
  int GetTeam() const;
  bool IsIde() const;
  bool IsAs() const;
  int GetNumberOfVacationDays() const;
  int GetVacationDayByIndex(int index) const;
  Nurse* GetFirstFriend() const;
  Nurse* GetSecondFriend() const;
  Nurse* GetThirdFriend() const;
  int GetFirstFriendID() const;
  int GetSecondFriendID() const;
  int GetThirdFriendID() const;
  std::string GetPathToProfilePic() const;

  void AddVacationDay(int day);
  void DeleteVacationDay(int day);
  void CleanPastVacationDays();
  void ClearVacations();
  void SetFirstFriend(Nurse* myfriend);
  void SetSecondFriend(Nurse* myfriend);
  void SetThirdFriend(Nurse* myfriend);
  void SetPassword(std::string newpassword);

 public:
  int id_;
  std::string firstname_;
  std::string surname_;
  int team_; //0, 1 or 2 if deleted
  bool iside_;
  bool isas_;
  int numberofvacationdays_;
  std::vector<QDate> vacationdays_;
  Nurse* friend1_;
  Nurse* friend2_;
  Nurse* friend3_;
  std::string password_;
  std::string pathtoprofilepic_;
};

#endif // NURSE_H
