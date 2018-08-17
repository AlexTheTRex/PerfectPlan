#ifndef ENGINE_H
#define ENGINE_H

#include <unordered_map>
#include <string>
#include <unordered_set>
#include "nurse.h"
#include "planning.h"
#include "parameters.h"
#include "planningentry.h"
//#include <opencv2/core/core.hpp>

class Engine {
 public:
  Engine();
  bool IsLegal(const Planning& planning) const;
  float ComputeObjective(const Planning& planning) const;
  void UpdateFriendships();
  Planning GetRandomPlanning(QDate monday, const std::unordered_set<std::string>& daysTeamA);
  bool PerturbatePlanning(Planning& myplanning);
  void GeneratePlanning(QDate monday, const std::unordered_set<std::string>& daysTeamA);
  bool SwitchSlots(int id1, QDate date1, int id2, QDate date2);

  void CreateAccount(std::string firstname, std::string surname, std::string password, int team, bool iside, bool isas);
  void DeleteAccount(int id);
  void AddVacationDay(int id, QDate day);
  void AddFriend(int friendnumber, int idadder, int idadded);
  void ChangePassword(int id, std::string newpassword);
  void DeleteVacationDay(int id, int rownumber);
  bool SaveStaff() const;
  bool LoadStaff();
  bool SavePlannings() const;
  bool LoadPlannings();
  bool SaveHistory() const;
  bool LoadHistory();
  void CleanPastVacationDays(int id);
  static std::string encrypt(std::string clear); //We just add i to the i-th character modulo 256
  static std::string decrypt(std::string coded); //Reverse operation



 public:
  std::vector<Nurse> staff_;
  std::unordered_map<std::string, Planning> finalplannings_;
  std::vector < std::vector < int > > friendships_;
  std::vector<PlanningEntry> planninghistory_;

  std::unordered_map<std::string, std::string> translateDays;
  std::unordered_map<std::string, std::string> translateMonths;
};

#endif // ENGINE_H
