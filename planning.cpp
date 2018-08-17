#include "planning.h"

using namespace std;

Planning::Planning(QDate monday) {
  monday_ = monday;
  hasbeenmodified_ = false;
  usic1_ = vector < vector < int > >(7, vector<int>(3, 0));
  usic2_ = vector < vector < int > >(7, vector<int>(1, 0));
  uca1_ = vector < vector < int > >(7, vector<int>(3, 0));
  uca2_ = vector < vector < int > >(7, vector<int>(1, 0));
  rythmo_ = vector < vector < int > >(7, vector<int>(4, 0));
  hds_ = vector < vector < int > >(4, vector<int>(2, 0));
}

//Planning::Planning(QDate monday, string creator, string dateofcreation, string motive) {
//  monday_ = monday;
//  hasbeenmodified_ = false;
//  creator_ = creator;
//  dateofcreation_ = dateofcreation;
//  motive_ = motive;
//  usic1_ = vector < vector < int > >(7, vector<int>(3, 0));
//  usic2_ = vector < vector < int > >(7, vector<int>(1, 0));
//  uca1_ = vector < vector < int > >(7, vector<int>(3, 0));
//  uca2_ = vector < vector < int > >(7, vector<int>(1, 0));
//  rythmo_ = vector < vector < int > >(7, vector<int>(4, 0));
//  hds_ = vector < vector < int > >(4, vector<int>(2, 0));
//}

Planning::Planning(){};

Planning::Planning(string s){
    errormessage_ = s;
}
