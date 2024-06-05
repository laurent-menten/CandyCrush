
#include "Queue.h"
#include "Matrice.h"
#include "Affichage.h"
#include "Utils..h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

// ****************************************************************************
// * Configuration ************************************************************
// ****************************************************************************

static ParametresNiveau niveaux[]
{
	//	colonnes	lignes	coups	jellies
	{	20,			20,		40,		10 },
	{	20,			20,		35,		15 },
	{	20,			20,		30,		20 },
};

// ****************************************************************************
// * Point d'entrée ***********************************************************
// ****************************************************************************

int main()
{
	ActionQueue queue;
	Plateau plateau;
	int niveau = 1;

	// --- Console en plein écran (simule Alt-Enter) --------------------------

	MaximizeConsole();
	
	// --- Initialisation -----------------------------------------------------

	InitializeJeu(niveaux, (sizeof(niveaux) / sizeof(ParametresNiveau)));

	InitializeQueue( &queue );
	AddToQueue( &queue, CreeActionInitialize() );

	// --- Boucle de jeu -------------------------------------------------------

	Action* action;
	while( (action = GetFromQueue( &queue )) != nullptr )
	{
		switch( action->Type )
		{
			case INITIALIZATION:	// -----------------------------------------
			{
				if( InitializePlateau( &plateau, niveau ) != 0 )
				{
					AfficheErreur( "Failed to create level" );
					break;
				}

				AddToQueue( &queue, CreeActionAffichage() );
				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			case AFFICHAGE:	// -------------------------------------------------
			{
				AffichePlateau( &plateau );

				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			case LECTURE:	// -------------------------------------------------
			{
				AddToQueue( &queue, CreeActionDeplacement( 0, 0, 0, 0 ) );

				break;
			}

			case DEPLACEMENT:	// ---------------------------------------------
			{
				break;
			}

			case FIN_NIVEAU:	// ---------------------------------------------
			{
				if( ++niveau <= 3 )
				{
					AddToQueue( &queue, CreeActionInitialize() );
				}

				break;
			}

			case CALCUL:	// -------------------------------------------------
			{
				AddToQueue( &queue, CreeActionLecture() );

				break;
			}

			case SUPRESSION_V:	// ---------------------------------------------
			{
				break;
			}

			case SUPRESSION_H:	// ---------------------------------------------
			{
				break;
			}

			default:	// -----------------------------------------------------
			{
				// Ne devrait normalement jamais arriver...

				AfficheErreur("Action de type %d non reconnue", action->Type);
			}
		}

		free( action );
	}
}
