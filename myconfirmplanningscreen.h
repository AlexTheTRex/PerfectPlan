#ifndef MYCONFIRMPLANNINGSCREEN_H
#define MYCONFIRMPLANNINGSCREEN_H

#include <QWidget>
#include <QLayout>
#include <QTabWidget>
#include <QCalendarWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <vector>
#include <mymainscreen.h>
#include <QLineEdit>
#include <QFormLayout>

//Attention les vacationdays ne doivent pas être enregistrés en cas d'annulation - vraiment?

//Choisis les jours par équipes
//Confirmation des jours de vacances
//Nom prénom et mot de passe

//Demande de confirmation si il y a déjà un planning
//Sauvegarde de l'historique des plannings


class MyConfirmPlanningScreen : public QWidget
{
    Q_OBJECT

public:
    MyConfirmPlanningScreen(MyMainScreen* main);
    int CheckFormValidity();
    void DisplayVacations();


public slots:
    void CloseConfirmPlanningWindow();
    void OpenVacationWindow();
    void GeneratePlanning();
    void RemoveVacations();

public:
    MyMainScreen* mainscreen_;
    QVBoxLayout* biglayout_;

    QHBoxLayout* teamslayout_;
    QLabel* teamsexplanationlabel_;
    QLabel* signatureexplanationlabel_;
    QLabel* calendarexplanationlabel_;
    std::vector<QVBoxLayout*> dayslayouts_;
    std::vector<QButtonGroup*> groupboxes_;
    std::vector<QLabel*> dayslabels_;
    std::vector<QRadioButton*> As_;
    std::vector<QRadioButton*> Bs_;

    QHBoxLayout* vacationslayout_;
    QPushButton* removevacationsbuton_;
    QPushButton* addvacationsbutton_;
    QTableWidget* vacationstable_;


    QFormLayout* form_;
    QLineEdit* firstname_;
    QLineEdit* familyname_;
    QLineEdit* password_;


    QHBoxLayout* buttonslayout_;
    QPushButton* okbutton_;
    QPushButton* cancelbutton_;//remember to erase added vacation days, but wait for Laeti's opinion

    int selectedid_;
    QDate selecteddate_;
};

#endif // MYCONFIRMPLANNINGSCREEN_H
