#ifndef labIO

#define labIO

#include "lab.h"
//Affiche le labyrinthe
void Show_Lab(laby &L);
void Verif_Lab(laby &L);
void  Show_Case(int i , int j, Case c);

// Affiche une etoile sur dans la case
void Mark_Case(int li, int col);
void Mark_Case_Pile(pile & p);

// Efface la case
void UMark_Case(int li, int col);


// retourne 
// 0 si pas valide
// -1 si On demande de sortir (touche F1)
// 1 si click gauche l et c sont la ligne et la colonne de la case
// 2 si click droit
// 3 touche W
// 12 touche N
// 6 touche S
// 9 touche E

int LireCommande();
void InitCurses();

void EndCurses();

#endif

