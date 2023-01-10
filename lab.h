#ifndef lab
#define lab
struct Case
{
	bool N,W,S,E;
	bool marq;
	bool etat;
};

struct laby
{
	int p;
	int q;
	Case **tab;
};

struct couple
{
	int i;
	int j;
};

struct pile
{
	int nbe;
	int taille;
	couple *T;
};




#endif
