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

//Attention les vacationdays ne doivent pas être enregistrés en cas d'annulation

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

public slots:
    void CloseConfirmPlanningWindow();
    void GeneratePlanning();

private:
    MyMainScreen* mainscreen_;
    QVBoxLayout* biglayout_;

    QHBoxLayout* teamslayout_;
    QLabel* teamsexplanationlabel_;
    QLabel* signatureexplanationlabel_;
    std::vector<QVBoxLayout*> dayslayouts_;
    std::vector<QButtonGroup*> groupboxes_;
    std::vector<QLabel*> dayslabels_;
    std::vector<QRadioButton*> As_;
    std::vector<QRadioButton*> Bs_;

    QFormLayout* form_;
    QLineEdit* firstname_;
    QLineEdit* familyname_;
    QLineEdit* password_;


    QHBoxLayout* buttonslayout_;
    QPushButton* okbutton_;
    QPushButton* cancelbutton_;//remember to erase added vacation days, but wait for Laeti's opinion
};

#endif // MYCONFIRMPLANNINGSCREEN_H
