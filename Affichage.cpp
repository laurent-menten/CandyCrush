
#include "Affichage.h"

#include <stdio.h>
#include <stdarg.h>
#include <process.h>

//
// Efface l'écran
// 
void EffaceEcran()
{
	// Sequence ANSI:

	// \033[H Home
	printf( "\033[H" );

	// \033[J Clear screen
	printf( "\033[J" );
}

/*
* Fonction d’affichage qui reçoit la Matrice en paramètre et va afficher les deux
* grilles de jeu (en version simplifiée) : une avec les pions et l’autre avec la
* gélatine. En version plus complexe, un affichage couleur est possible avec la
* gélatine en arrière-fond.
*/
void AffichePlateau( Plateau* plateau, bool avecInfos )
{
	EffaceEcran();

	// Affiche les numéros de colonne

	printf( "    " );
	for( int c = 0; c < plateau->colonnes; c++ )
	{
		printf( "   %-2d ", (c + 1) );
	}
	printf( "\n" );

	// Pour chaques lignes du plateau:
	// Affiche une ligne de séparation horizontale et la ligne de contenu

	for( int l = plateau->lignes - 1; l >= 0; l-- )
	{
		printf( "    +" );
		for( int c = 0; c < plateau->colonnes; c++ )
		{
			printf( "-----+" );
		}
		printf( "\n" );

		printf( " %2d |", l+1 );
		for( int c = 0; c < plateau->colonnes; c++ )
		{
			Case* pion = &plateau->matrice[(l * plateau->colonnes) + c];

			printf( " " );

			if (pion->vide)
			{
				printf( "   " );
			}
			else
			{
				if (pion->jelly)
				{
					// bg color
					printf("\033[47m");
				}

				switch (pion->type)
				{

					// fg color; bold; reset
					case JAUNE: printf("\033[33;1m J \033[0m"); break;
					case VERT:  printf("\033[32;1m V \033[0m"); break;
					case BLEU:  printf("\033[34;1m B \033[0m"); break;
					case ROUGE: printf("\033[31;1m R \033[0m"); break;
					case MAUVE: printf("\033[35;1m M \033[0m"); break;
				}

				if (pion->jelly)
				{
					// bg color
					printf("\033[40m");
				}
			}

			printf( " |" );

		}
		printf( "\n" );
	}

	// Affiche une ligne de séparation terminale

	printf( "    +" );
	for( int c = 0; c < plateau->colonnes; c++ )
	{
		printf( "-----+" );
	}
	printf( "\n" );

	if (avecInfos)
	{
		// Affiche le nombre de coups restants

		printf("Niveau: %d\n", plateau->niveau);
		printf("Coups restants: %d\n", plateau->coups);
		printf("Jellies restantes: %d\n", plateau->jellies);
	}
}

void AfficheAvertissement(const char* error, ...)
{
	printf("\n");
	printf("\b\033[33;1mWARNING: \033[0m");

	va_list errorArgs;
	va_start(errorArgs, error);
	vprintf(error, errorArgs);
	va_end(errorArgs);

	printf("\n");
}

void AfficheErreur( const char* error, ... )
{
	printf("\n");
	printf("\b\033[31;1mERROR: \033[0m");

	va_list errorArgs;
	va_start(errorArgs, error);
	vprintf( error, errorArgs );
	va_end(errorArgs);

	printf("\n");
}

