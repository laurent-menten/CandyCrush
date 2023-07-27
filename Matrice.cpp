
#include "Matrice.h"

#include <stdlib.h>
#include <malloc.h>

static PrototypePlateau prototypes[]
{
	//	colonnes	lignes	coups	jellies
	{	20,			20,		40,		10 },
	{	20,			20,		35,		15 },
	{	20,			20,		30,		20 },
};

int InitializePlateau( Plateau* plateau, int niveau )
{
	if( (niveau < 1) || (niveau > ( sizeof( prototypes ) / sizeof( PrototypePlateau ) )) )
	{
		return -1;
	}

	plateau->niveau = niveau;
	plateau->colonnes = prototypes[niveau-1].colonnes;
	plateau->lignes = prototypes[niveau - 1].lignes;
	plateau->coups = prototypes[niveau - 1].coups;

	plateau->matrice = (Case*)malloc( plateau->colonnes * plateau->lignes * sizeof( Case ) );

	for( int l = 0; l < plateau->lignes; l++ )
	{
		for( int c = 0; c < plateau->colonnes; c++ )
		{
//			plateau->matrice[(l * plateau->colonnes) + c].type = EMPTY;
			plateau->matrice[(l * plateau->colonnes) + c].type = (enum TypePion) ((rand() % 5) + 1);
			plateau->matrice[(l * plateau->colonnes) + c].jelly = false;
		}
	}

	for( int j = 0; j < prototypes[niveau - 1].jellies; j++ )
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
