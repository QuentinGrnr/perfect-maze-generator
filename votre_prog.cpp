#include <ncurses.h>
#include "labIO.h"
#include<iostream>
#include<fstream>
#include <cstdlib>
#include<unistd.h> // librairie permettant d'utiliser la fonction sleep necessaire a l'affichage progressif de la recherche du chemin parfait du labyrinthe
#include <stdio.h>

using namespace std;

void init(Case & c)
{
	c.N=c.S=c.E=c.W=false;
	c.etat=false;
}

void init(laby & L, int p, int q)
{
	L.p=p; L.q=q;
	L.tab=new Case *[p];
	for(int i=0;i<p;i++) L.tab[i]=new Case[q];
	for(int i=0;i<p;i++)
	{
		for(int j=0;j<q;j++) init(L.tab[i][j]);
	}
}

laby *lire_Laby(const char  *filename)
{
	ifstream my_cin;

	my_cin.open(filename);

	laby *L;
	L=new laby;
	my_cin>>(*L).p>>(*L).q;
	init((*L),(*L).p,(*L).q);

	for(int i=0;i<(*L).p;i++)
	{
		for(int j=0;j<(*L).q;j++)
		{
			my_cin>>(*L).tab[i][j].W>>(*L).tab[i][j].N>>(*L).tab[i][j].S>>(*L).tab[i][j].E;
		}
	}
	return L;
}
//____________________________________II.2______________________________________//
void ecrire_Laby(laby & L,const char *fn){
    ofstream my_cout; //on crée notre flux
    my_cout.open(fn);
    my_cout<<(L).p<<" "<<(L).q<<" ";//on écrit le nbr de ligne et le nbr de colone
    for(int i=0;i<(L).p;i++) // on écrit les valeurs bouléenne de chaques portes réspectivment WNSE de chaque case de gauche a droite et de haut en bas
    {
        for(int j=0;j<(L).q;j++)
        {
            my_cout<<(L).tab[i][j].W<<" "<<(L).tab[i][j].N<<" "<<(L).tab[i][j].S<<" "<<(L).tab[i][j].E<<" ";
        }
    }
}
//____________________________________III______________________________________//
void deplacement(laby & L, bool *abandon){
    int i = 0;
    int j = 0;
    bool game = true; // booléen permettant de savoir si la partie est toujours en cours
    Mark_Case(i,j); // on marque la case 0 0
    while ((i != L.p-1 || j != L.q-1) && game){  // on effectue le code tant que la case d'arrivé n'est pas atteinte et que la partie est en cours
        switch (LireCommande()) {
            case 9 : if (L.tab[i][j].W){ // on enléve la marque précédente puis on marque la case a gauche
                    UMark_Case(i,j);
                    j--;
                    Mark_Case(i,j);
            }// gauche
                break;
            case 3 : if (L.tab[i][j].E){ // on enléve la marque précédente puis on marque la case a droite
                    UMark_Case(i,j);
                    j++;
                    Mark_Case(i,j);
                }// droite
                break;
            case 12 : if (L.tab[i][j].N){// on enléve la marque précédente puis on marque la case en haut
                    UMark_Case(i,j);
                    i--;
                    Mark_Case(i,j);
                }// haut
                break;
            case 6 : if (L.tab[i][j].S){ // on enléve la marque précédente puis on marque la case en bas
                    UMark_Case(i,j);
                    i++;
                    Mark_Case(i,j);
                } // bas
                break;
            case -1 : // F1 = abandon
                clear();
                *abandon = true;
                game = false;
                break;
            default: // cas ou une autre touche est pressé : on clear le laby et on affiche un message de fin de jeu
                clear();
                game = false;
                mvprintw (0,0,"echec : commande invalide, appuyez sur une touche pour mettre fin a la partie");
                switch (LireCommande()) { // permet d'attendre une commande du joueur afin qu'il puisse lire les messages avant la fin de la partie
                    default :
                        EndCurses();
                        break;
                }
                break;
        }
    }
    if ( i == L.p-1 && j == L.q-1){ //on vérifie que le while est terminé car il a atteint la case finale
        clear();
        mvprintw(0, 0, "Bien joué ! tu as finis le labyrinthe !, appuie sur une touche pour mettre fin a la partie");
        switch (LireCommande()) { // permet d'attendre une commande du joueur afin qu'il puisse lire les messages avant la fin de la partie
            default :
                EndCurses();
                break;
        }
    }
}

//____________________________________IV______________________________________//
//**************fonction pilles*************//
void init(pile & p, int taille)
{
    p.taille=taille;
    p.nbe=0;
    p.T=new couple[taille];
}

bool empiler(pile & p, couple c)
{
    if((p.nbe)<p.taille)
    {
        p.T[p.nbe]=c;
        (p.nbe)++;
        return true;
    }
    return false;
}

couple depiler(pile & p)
{
    if(p.nbe>0)
    {
        p.nbe--;
        return(p.T[p.nbe]);
    }
    else
    {
        cout<<"la pile est vide"<<endl;
        exit(1);
    }
}

bool vide(pile & P)
{
    if(P.nbe==0) return true;
    else return false;
}
//***************************************//

bool adjacent_visite (laby &L, couple C){ //fonction qui renvoie true si toute les case adjacentes sont visités
    for (int i = 1; i <=4; i++){
        switch (i){
            case 1 : if (C.i > 0) {
                    if (!L.tab[C.i - 1][C.j].etat) {
                        return false;
                    }
                }
                break;
            case 2 : if (C.j < L.q-1){
                    if(!L.tab[C.i][C.j+1].etat){
                        return false;
                    }
                }
                break;
            case 3 : if (C.i < L.p-1) {
                    if (!L.tab[C.i + 1][C.j].etat) {
                        return false;
                    }
                }
                break;
            case 4 : if (C.j > 0){
                    if(!L.tab[C.i][C.j-1].etat){
                        return false;
                    }
                }
                break;

            default:
                return true;
        }
    }
    return true;
}


laby * My_Creation(int p, int q){
    laby *perfect_laby = new laby;
    //on ajoute un attribu à la structure "case" qui est un bouléen "etat"
    init(*perfect_laby,p,q);
    // on initialise le nouvel attribu a false
    for (int i = 0; i < perfect_laby->p; i++) {
        for (int j = 0; j < perfect_laby->q; j++) {
            perfect_laby->tab[i][j].etat = false;
        }
    }
    couple *current_cell = new couple;
    // on crée la pile de cellules
    pile *pile_cell = new pile;
    init(*pile_cell,p*q);
    int porte = 0 ;
    //on initialise
    current_cell->i = 0; //ligne
    current_cell->j = 0; // collone
    empiler(*pile_cell,*current_cell);
    perfect_laby->tab[current_cell->i][current_cell->j].etat = true;
    while (!vide(*pile_cell)){
        while(!adjacent_visite(*perfect_laby, *current_cell)){
            porte = rand () % 4 +1;

            switch (porte){
                case 1 ://porte N
                    if (current_cell->i > 0){//on verifie que l'on est pas du coté nord
                        if (!perfect_laby->tab[current_cell->i-1][current_cell->j].etat){
                            perfect_laby->tab[current_cell->i][current_cell->j].N = true;
                            perfect_laby->tab[current_cell->i-1][current_cell->j].S = true;
                            perfect_laby->tab[current_cell->i-1][current_cell->j].etat = true;
                            empiler(*pile_cell, *current_cell);
                            current_cell->i--;
                        }
                    }
                    break;

                case 2 :// porte E
                    if (current_cell->j < q-1){//on verifie que l'on est pas du coté Est
                        if (!perfect_laby->tab[current_cell->i][current_cell->j+1].etat){
                            perfect_laby->tab[current_cell->i][current_cell->j].E = true;
                            perfect_laby->tab[current_cell->i][current_cell->j+1].W = true;
                            perfect_laby->tab[current_cell->i][current_cell->j+1].etat = true;
                            empiler(*pile_cell, *current_cell);
                            current_cell->j++;
                        }
                    }
                    break;

                case 3 :
                    if (current_cell->i < p-1){//on verifie que l'on est pas du coté South
                        if (!perfect_laby->tab[current_cell->i+1][current_cell->j].etat){
                            perfect_laby->tab[current_cell->i][current_cell->j].S = true;
                            perfect_laby->tab[current_cell->i+1][current_cell->j].N = true;
                            perfect_laby->tab[current_cell->i+1][current_cell->j].etat = true;
                            empiler(*pile_cell, *current_cell);
                            current_cell->i++;
                        }
                    }
                    break;

                case 4 :
                    if (current_cell->j > 0){//on verifie que l'on est pas du coté West
                        if (!perfect_laby->tab[current_cell->i][current_cell->j-1].etat){
                            perfect_laby->tab[current_cell->i][current_cell->j].W = true;
                            perfect_laby->tab[current_cell->i][current_cell->j-1].E = true;
                            perfect_laby->tab[current_cell->i][current_cell->j-1].etat = true;
                            empiler(*pile_cell, *current_cell);
                            current_cell->j--;
                        }
                    }
                    break;

                default:
                    cout << "null" << endl;
            }
        }
        couple *c2 = new couple;
        *c2= depiler(*pile_cell);
        current_cell->i=c2->i;
        current_cell->j=c2->j;
        delete c2;
    }
    return perfect_laby;
}

//____________________________________V______________________________________//



//____________________________________VI______________________________________//


void affiche_chemin(pile & P, laby & L) {
    for(int i = 0; i < P.taille; i++){
        couple mark_cell = depiler(P);
        Mark_Case(mark_cell.i,mark_cell.j);
    }
}

bool adjacent_accessible (laby &L, couple *C){ // renvoie false si toutes les cases
    for (int i = 1; i <=4; i++){ // on verfie chaque porte de la case
        switch (i){
            case 1 :if (C->i > 0) { //N ==> est ce que la valeur de i ne représente pas une extrémitée (ici coté nord) du labyrinthe
                    if (!L.tab[C->i - 1][C->j].marq) { // est ce que la case adjacente est marqué ? ici la case au nord
                        if (L.tab[C->i][C->j].N) {//est ce que le mur qui sépare les deux case est bien ouvert ?
                            return true;
                        }
                    }
                }
                break;
            case 2 : if (C->j < L.q-1){//E
                    if (!L.tab[C->i][C->j+ 1].marq) {
                        if (L.tab[C->i][C->j].E) {
                            return true;
                        }
                    }
                }
                break;
            case 3 : if (C->i < L.p-1) {//S
                    if (!L.tab[C->i + 1][C->j].marq) {
                        if (L.tab[C->i][C->j].S) {
                            return true;
                        }
                    }
                }
                break;
            case 4 : if (C->j > 0) {//W
                    if (!L.tab[C->i][C->j- 1].marq) {
                        if (L.tab[C->i][C->j].W) {
                            return true;
                        }
                    }
                }
                break;

            default:
                return false;
        }
    }
    return false;
}


pile *explorer(laby & L){
    clear();
    Show_Lab(L);
    // on crée un nouveau couple de coordonnés
    couple *c = new couple;
    // on crée la pile de cellules
    pile *chemin = new pile;
    init(*chemin,L.p*L.q); // a noté que la taille choisi ici est la taille maximum théorique que peut prendre la pile, la vraie taille sera renvoyé a la fin de la fonction
    //on donne les coordonnées de départ et on les empile
    c->i=0;
    c->j=0;
    int taille_pile = 0;
    empiler(*chemin,*c);
    taille_pile++;
    L.tab[c->i][c->j].marq = true;
    int porte;
    bool found = false; //indique si la case accessible a été trouvé
    while (c->i != L.p-1 || c->j != L.q-1){
        if(adjacent_accessible(L,c)){ // on vérifie d'abord s'il y a une case accessible autour
            depiler(*chemin);
            taille_pile--;
            while(!found){ //on trouve aléatoirement parmi la ou les cases accessibles la prochaine que l'on va marquer
                porte = rand () % 4 +1;
                switch (porte){
                    case 1 :if (c->i > 0) { //N ==> est ce que la valeur de i ne représente pas une extrémitée (ici coté nord) du labyrinthe
                            if (!L.tab[c->i - 1][c->j].marq) { // est ce que la case adjacente est marqué ? ici la case au nord
                                if (L.tab[c->i][c->j].N) {//est ce que le mur qui sépare les deux case est bien ouvert ?
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    L.tab[c->i-1][c->j].marq = true;
                                    Mark_Case(c->i,c->j);
                                    Show_Lab(L);
                                    switch (LireCommande()) { // permet de detecter quand le joueur appui sur nimporte quelle touche
                                        default :
                                            break;
                                    }
                                    c->i--;
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    found = true;
                                }
                            }
                        }
                        break;
                    case 2 : if (c->j < L.q-1){//E ==> meme raisonnement pour chaque mur
                            if (!L.tab[c->i][c->j+1].marq) {
                                if (L.tab[c->i][c->j].E) {
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    L.tab[c->i][c->j+1].marq = true;
                                    Mark_Case(c->i,c->j);
                                    Show_Lab(L);
                                    switch (LireCommande()) { // permet de detecter quand le joueur appui sur nimporte quelle touche
                                        default :
                                            break;
                                    }
                                    c->j++;
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    found = true;
                                }
                            }
                        }
                        break;
                    case 3 : if (c->i < L.p-1) {//S
                            if (!L.tab[c->i + 1][c->j].marq) {
                                if (L.tab[c->i][c->j].S) {
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    L.tab[c->i + 1][c->j].marq = true;
                                    Mark_Case(c->i,c->j);
                                    Show_Lab(L);
                                    switch (LireCommande()) { // permet de detecter quand le joueur appui sur nimporte quelle touche
                                        default :
                                            break;
                                    }
                                    c->i++;
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    found = true;
                                }
                            }
                        }
                        break;
                    case 4 : if (c->j > 0) {//W
                            if (!L.tab[c->i][c->j-1].marq) {
                                if (L.tab[c->i][c->j].W) {
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    L.tab[c->i][c->j-1].marq = true;
                                    Mark_Case(c->i,c->j);
                                    Show_Lab(L);
                                    switch (LireCommande()) { // permet de detecter quand le joueur appui sur nimporte quelle touche
                                        default :
                                            break;
                                    }
                                    c->j--;
                                    empiler(*chemin,*c);
                                    taille_pile++;
                                    found = true;
                                }
                            }
                        }
                        break;

                    default:
                        cout << "valeur de porte non comprise entre 1 et 4";
                }
            }
            found = false;
        } else {
            *c=depiler(*chemin);
            UMark_Case(c->i,c->j);
            Show_Lab(L);
            switch (LireCommande()) { // permet de detecter quand le joueur appui sur nimporte quelle touche
                default :
                    break;
            }
            taille_pile--;
        }
    }
    chemin->taille = taille_pile;
    return chemin;
}

//____________________________________VII______________________________________//

void jeu (){
    int p,q;
    bool abandon = false; // permet de voir si le joueur a abandonné lors de sa partie
    cout << "entrez le nombre de ligne puis de collones du labyrinthe" <<endl;
    cin >> p >> q;
    laby *L1= My_Creation(p,q);
    InitCurses();
    Show_Lab(*L1);
    deplacement(*L1, &abandon);
    if (abandon){
        clear();
        mvprintw(0, 0, "Vous avez abandonné : Appuyez sur une touche pour voir la solution du labyrinthe");
        switch (LireCommande()) { // permet d'attendre une commande du joueur afin qu'il puisse lire les messages avant la fin de la partie
            default :
                pile * pilechemin = new pile;
                pilechemin = explorer(*L1);
                mvprintw(p*2+1, 0, "appuyez sur une touche pour terminer la partie"); //p*2+1 = coordonnées d'affichage du texte
                switch (LireCommande()) { // permet de detecter quand le joueur appui sur nimporte quelle touche afin de terminer la partie
                    default :
                        EndCurses();
                        break;
                }
                break;
        }
    }
}

int main()
{
    srand (time(NULL));
    jeu();
    return 0;
}