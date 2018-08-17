#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <QString>
#include <vector>
//We make sure that at any time the ID of someone matches their index in the engine array

//With a very small number of friends, the friendship term is about 8
//The pure entropy term is about 26 at most
//If everybody has a lot of friends the entropy will become negligible so we need to reweight it.


#define WEIGHT_FRIENDSHIP_SINGLE 1
#define WEIGHT_FRIENDSHIP_DOUBLE 3
#define FRIENSHIP_LAMBDA 0.1
#define NUMBER_PARENTS 10
#define NUMBER_CHILDREN 10
#define PATH_TO_DEFAULT_PROFILE_PIC "/Users/Alexandre/Documents/Development/PerfectPlan/data/default_profile_pic.gif"
#define PATH_TO_STAFF_DATA "/Users/Alexandre/Documents/Development/PerfectPlan/data/staff.json"
#define PATH_TO_PLANNING_DATA "/Users/Alexandre/Documents/Development/PerfectPlan/data/plannings.json"
#define PATH_TO_HISTORY_DATA "/Users/Alexandre/Documents/Development/PerfectPlan/data/history.json"
#define ACCOUNT_PROFILE_PICTURE_SIZE 200
#define STAFF_PROFILE_PIC_SIZE 57
#define PATH_TO_NO_PLANNING "/Users/Alexandre/Documents/Development/PerfectPlan/data/noplanning.png"
#define PATH_TO_EXPORT_PLANNING "/Users/Alexandre/Desktop/"
#define ALGORITHM_MU 20
#define ALGORITHM_LAMBDA 1
#define ALGORITHM_N 3
#define ALGORITHM_MAXITER 200



#endif // PARAMETERS_H


/*************   TODO *************/


//   Débugger les amis qui déconnent : ne pas hésiter à recharger l'écran quand un nouvel ami est sélectionné. Faire
     //en sorte que les amis n'apparaissent pas dans les autres listes
     // si le nouvel ami est aucun? IL Y A DES BEUG
     //STOP ON ARRETE AVEC LES INDICES? ON VA JUSTE CHERCHER DANS LA LISTE D'AMIS CELUI DONT LE NOM CORRESPOND


//Clean past vacation days 30 mn -> a chaque calcul de planning et a chaque ouverture de compte
//Les petits bisous 1h

//Une fois que tout ça est fini, il te reste plus que l'algo de planning!!!! et aussi l'apparence visuelle du planning


// L'algo de planning
// Affichage du planning


//Echanger des creneaux
//L'historique des plannings
//Les bisoux

//
//     Rajouter des jours de vacances pour les autres (sur l'écran de confirmation de génération proposer ça)




//Dealer avec les éventuels créneaux en trop/pas assez (donner l'option de laisser de
//blancs dans le planning qui seront remplis à la main)

//Créer l'option d'ouvrir le HDS le weekend -> est-elle également fermée le vendredi?
//En fait c'est pas sûr du tout que ça soit toujours mercredi jeudi et le reste de la semaine hein.
//faut donner l'option

//Si il y a trop ou d'AS il se passe quoi? s'il y a pas assez de personnel?
//Maybe try ghost IDEs

//const correctness

//ATTENTION ATTENTION ATTENTION ATTENTION les ids des gens changent quand on supprime un compte, mais les amis ne doivent surtout pas changer!!!

//Deal with what happens when there are too few nurses
