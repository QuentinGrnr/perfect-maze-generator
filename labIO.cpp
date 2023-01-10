#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lab.h"

WINDOW *boite;
WINDOW *win;


//Affiche les bords de la case 
void Show_Case(int i , int j, Case c)
{
	    boite= subwin(stdscr, 3, 5, i*2, j*4);
	    wborder(boite,(not c.W)?'|':' ',(not c.E)?'|':' ',(not c.N)?'-':' ',(not c.S)?'-':' ', '+', '+', '+', '+');
}

//Affiche le labyrinthe
void Show_Lab(laby &L)
{
int i, j; 
for (i = 0; i < L.p; i++)
	for (j=0; j < L.q; j++)
	{
		Show_Case(i, j , L.tab[i][j]);
	}

}
//Affiche les bords de la case 
void Verif_Case(int i , int j, Case c)
{
	    boite= subwin(stdscr, 3, 5, i*3, j*5);
	    wborder(boite,not c.E?'|':' ',not c.W?'|':' ',not c.N?'-':' ',not c.S?'-':' ', '+', '+', '+', '+');
}

//Affiche le labyrinthe
void Verif_Lab(laby &L)
{
int i, j; 
for (i = 0; i < L.p; i++)
	for (j=0; j < L.q; j++)
	{
		Verif_Case(i, j , L.tab[i][j]);
	}

}

// Affiche une etoile sur dans la case
void Mark_Case(int li, int col)
{
	attron(A_REVERSE);
	mvprintw(li*2+1 , (col+1)*4-3," * ");
	mvprintw(li*2+1 , (col+1)*4-3," ");
	attroff(A_REVERSE);
}

void Mark_Case_Pile( pile & p)
{
	for(int i=0;i<p.nbe;i++) Mark_Case(p.T[i].i, p.T[i].j);
}

// Efface la case
void UMark_Case(int li, int col)
{
//	attron(A_STANDOUT);
	mvprintw(li*2+1 , (col+1)*4-3,"   ");
	mvprintw(li*2+1 , (col+1)*4-3," ");
//	attroff(A_STANDOUT);
}


// retourne 
// 0 si pas valide
// -1 si On demande de sortir (touche F1)
// 3 touche W
// 12 touche N
// 6 touche S
// 9 touche E

int LireCommande()
{
	MEVENT event ;
	int ch;
	int CC, L; 
	ch = wgetch(stdscr) ;


	switch(ch)
	{
		case KEY_F(1): /*Pour quitter le jeu*/
			return -1;
		case KEY_DOWN:
			return 6;
		case KEY_UP:
			return 12;
		case KEY_RIGHT:
			return 3;
		case KEY_LEFT:
			return 9;
	}
	return 0;

}
void InitCurses()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	//mousemask(ALL_MOUSE_EVENTS, NULL);
	wrefresh(win);
}

void EndCurses()
{
	endwin();
}
