
#include "Matrice.h"

#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>

static ParametresNiveau* niveaux;
static int niveauMax;

void InitializeJeu(ParametresNiveau* _prototypesNiveau, int _niveauMax)
{
	niveaux = _prototypesNiveau;
	niveauMax = _niveauMax;

	// pour éviter que rand() ne donne toujours les mêmes valeurs.
	srand((unsigned int)GetTickCount64());
}

int InitializePlateau( Plateau* plateau, int niveau )
{
	if( (niveau < 1) || (niveau > niveauMax) )
	{
		return -1;
	}

	if (niveau > 1)
	{
		free(plateau->matrice);
	}

	plateau->niveau = niveau;
	plateau->colonnes = niveaux[niveau - 1].colonnes;
	plateau->lignes = niveaux[niveau - 1].lignes;
	plateau->coups = niveaux[niveau - 1].coups;
	plateau->jellies = niveaux[niveau - 1].jellies;

	plateau->matrice = (Case*)malloc( plateau->colonnes * plateau->lignes * sizeof( Case ) );
	if (!plateau->matrice)
	{
		return -1;
	}

	for( int l = 0; l < plateau->lignes; l++ )
	{
		for( int c = 0; c < plateau->colonnes; c++ )
		{
			plateau->matrice[(l * plateau->colonnes) + c].type = (enum TypePion) ((rand() % NB_TYPE_PION));
			plateau->matrice[(l * plateau->colonnes) + c].jelly = false;
			plateau->matrice[(l * plateau->colonnes) + c].vide = false;
		}
	}

	for( int j = 0; j < plateau->jellies; j++ )
	{
		int x;
		int y;

		do
		{
			x = (rand() + 1) % plateau->colonnes;
			y = (rand() + 1) % plateau->lignes;
		}
		while( plateau->matrice[(y * plateau->lignes) + x].jelly == true );

		plateau->matrice[(y * plateau->lignes) + x].jelly = true;
	} 

	return 0;
}

bool VerifieGelatine(Plateau* plateau)
{
	return plateau->jellies > 0;
}

bool VerifieEchange(int X1, int Y1, int X2, int Y2)
{
	return false;
}

void SuppressionV(int X1, int Y1, int X2, int Y2)
{
}

void SuppressionH(int X1, int Y1, int X2, int Y2)
{
}
