
#include "Matrice.h"

#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>

static ParametresNiveau* niveaux;
static int niveauMax;

void InitialiseJeu(ParametresNiveau* _prototypesNiveau, int _niveauMax)
{
	niveaux = _prototypesNiveau;
	niveauMax = _niveauMax;

	// pour éviter que rand() ne donne toujours les mêmes valeurs.
	srand((unsigned int)GetTickCount64());
}

int InitialisePlateau( Plateau* plateau, int niveau )
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

bool VerifieCoordonnees(Plateau* plateau, int x, int y)
{
	return true;
}

bool VerifieCoups(Plateau* plateau)
{
	return plateau->coups > 0;
}

bool VerifieJellies(Plateau* plateau)
{
	return plateau->jellies > 0;
}

bool VerifieEchange( Plateau* plateau, int x1, int y1, int x2, int y2 )
{
	int x = abs(x1 - x2);
	int y = abs(y1 - y2);

	return ((x == 0) && (y == 1)) || ((x == 1) && (y == 0));
}

void SuppressionV(Plateau* plateau, int collone, int ligneDebut, int ligneFin)
{
}

void SuppressionH(Plateau* plateau, int ligne, int colonneDebut, int colonneFin)
{
}

void SuppressionColonne(Plateau* plateau, int colonne)
{
}

void SuppressionLigne(Plateau* plateau, int ligne)
{
}
