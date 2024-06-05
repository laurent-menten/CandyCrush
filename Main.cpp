
#include "Queue.h"
#include "Matrice.h"
#include "Affichage.h"
#include "Lecture.h"
#include "Utils.h"

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

	InitialiseJeu(niveaux, (sizeof(niveaux) / sizeof(ParametresNiveau)));

	InitializeQueue( &queue );
	AddToQueue( &queue, CreeActionInitialize() );

	// --- Boucle de jeu -------------------------------------------------------

	Action* action;

	bool finJeu = false;

	while( !finJeu && ((action = GetFromQueue( &queue )) != nullptr) )
	{
		switch( action->type )
		{
			case INITIALIZATION:	// -----------------------------------------
			{
				if( InitialisePlateau( &plateau, niveau ) != 0 )
				{
					AfficheErreur( "Erreur de création du niveau %d", niveau ); // noreturn !
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
				int x1;
				int y1;
				bool origineValide = false;

				int x2;
				int y2;
				bool destinationValide = false;

				do 
				{
					AffichePlateau(&plateau);

					if (origineValide)
					{
						// affiche origine
					}
					else
					{
						LectureCoordonnees( "Origine [x y]: ", &x1, &y1);
						origineValide = VerifieCoordonnees(&plateau, x1, y1);
					}

					if ( !destinationValide )
					{
						LectureCoordonnees( "Destination [x y]: ", &x2, &y2);
						destinationValide = VerifieCoordonnees(&plateau, x2, y2);
					}

					if (! VerifieEchange(&plateau, x1, y1, x2, y2))
					{
						origineValide = destinationValide = false;
					}

				} while ( !(origineValide && destinationValide) );

				AddToQueue( &queue, CreeActionDeplacement( x1, y1, x2, y2 ) );

				break;
			}

			case DEPLACEMENT:	// ---------------------------------------------
			{
				ActionDeplacement* actionDeplacement = (ActionDeplacement*)action;

				break;
			}

			case CALCUL:	// -------------------------------------------------
			{
				AddToQueue( &queue, CreeActionLecture() );

				break;
			}

			case VERIFICATION:	// ---------------------------------------------
			{
				break;
			}

			case SUPRESSION_V:	// ---------------------------------------------
			{
				ActionSupressionV* actionSupression = (ActionSupressionV*)action;

				SuppressionV( &plateau, actionSupression->index, actionSupression->debut, actionSupression->fin );

				break;
			}

			case SUPRESSION_H:	// ---------------------------------------------
			{
				ActionSupressionH* actionSupression = (ActionSupressionH*)action;

				SuppressionH( &plateau, actionSupression->index, actionSupression->debut, actionSupression->fin );

				break;
			}

			case SUPRESSION_COLONNE:
			{
				ActionSupressionColonne* actionSupression = (ActionSupressionColonne*)action;
				break;
			}

			case SUPRESSION_LIGNE:
			{
				ActionSupressionLigne* actionSupression = (ActionSupressionLigne*)action;
				break;
			}

			case FIN_NIVEAU:	// ---------------------------------------------
			{
				if (++niveau <= 3)
				{
					AddToQueue(&queue, CreeActionInitialize());
				}
				else
				{
					finJeu = true;
				}

				break;
			}

			default:	// -----------------------------------------------------
			{
				// Ne devrait normalement jamais arriver...

				AfficheErreur("Action de type %d non gérée", action->type);
			}
		}

		free( action );
	}

	FinaliseQueue(&queue);

	return 0;
}
