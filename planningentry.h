#ifndef PLANNINGENTRY_H
#define PLANNINGENTRY_H

#include <QDate>
#include <QTime>
#include <string>

class PlanningEntry{
  public:
    QDate planningmonday_;
    QDate modifdate_;
    QTime modiftime_;
    std::string modiffirstname_;
    std::string modifsurname_;
    std::string motive_;
};



#endif // PLANNINGENTRY_H
