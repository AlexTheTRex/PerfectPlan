#ifndef PLANNING_H
#define PLANNING_H

#include <vector>
#include "nurse.h"

class Planning {
 public:
  Planning();
  Planning(QDate monday);
  void swap(int unit1, int unit2, int day1, int day2, int num1, int num2);

 public:
  QDate monday_;
  bool hasbeenmodified_;
  //Order of units is USIC1, USIC2, UCA1, UCA2, Rythmo, HDS
  //CORO is made up of the first nurses in USIC1 and UCA1, they must be IDE (index 0, or 1 if 0 is AS)
  //Order of days of the week is standard
  //First index is unit, second is day, third is nurse
  //USIC1 : 3 IDE, UCA1 : 3 IDE or 2 IDE + 1 AS
  //USIC2 : 1 IDE, UCA2 : 1 IDE
  //Rythmo : 3 IDE + 1 AS
  //HDS (closed on weekends) : 2 IDE or 1 IDE + 1 AS
  std::vector < std::vector <int> > usic1_;
  std::vector < std::vector <int> > usic2_;
  std::vector < std::vector <int> > uca1_;
  std::vector < std::vector <int> > uca2_;
  std::vector < std::vector <int> > rythmo_;
  std::vector < std::vector <int> > hds_;


};

#endif // PLANNING_H
