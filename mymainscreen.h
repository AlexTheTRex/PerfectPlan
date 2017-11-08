#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QTableWidget>
#include <QDate>
#include "engine.h"

class MyMainScreen : public QWidget {

  Q_OBJECT

  public:
  MyMainScreen();
  void DisplayPlanning(const Planning& plan);

  public slots:
  void OpenCreateAccountWindow();
  void OpenLoginWindow();
  void OpenOptionsWindow();
  void GeneratePlanning();
  //void PreviousPlanning();
  //void NextPlanning();


  public:
  Engine* engine_;
  QDate mondayofcurrentweek_;
  QTableWidget* planningtable_;

  QPushButton* createaccountbutton_;
  QPushButton* loginbutton_;
  QPushButton* generateplanningbutton_;
  QPushButton* optionsbutton_;
  QPushButton* quitbutton_;
  QPushButton* previousbutton_;
  QPushButton* nextbutton_;

  QVBoxLayout* auxiliarybox1_;
  QVBoxLayout* auxiliarybox2_;
  QLabel* imagecontainer1_;
  QPixmap* image1_;
  QLabel* imagecontainer2_;
  QPixmap* image2_;

  QGridLayout* layout_;
};


#endif
