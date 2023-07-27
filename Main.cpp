
#include "Queue.h"
#include "Matrice.h"
#include "Affichage.h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

bool MaximizeConsole( void );

int main()
{
	ActionQueue queue;
	Plateau plateau;
	int niveau = 1;

	// pour éviter que rand() ne donne toujours les mêmes valeurs.
	srand( (unsigned int)GetTickCount64() );

	// Console plein ecran (merci Laurent)
	MaximizeConsole();

	for( int i = 0; i < 3; i++ )
	{
		InitializePlateau( &plateau, niveau++ );
		AffichePlateau( &plateau );
		getchar();
	}

	return 0;

	InitializeQueue( &queue );

	AddToQueue( &queue, CreeActionInitialize() );

	Action* action;
	while( (action = GetFromQueue( &queue )) != nullptr )
	{
		switch( action->Type )
		{
			case INITIALIZATION:
			{
				if( InitializePlateau( &plateau, niveau ) != 0 )
				{
					AfficheError( "Failed to create level" );
					break;
				}

				AddToQueue( &queue, CreeActionAffichage() );
				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			case AFFICHAGE:
			{
				AffichePlateau( &plateau );

				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			case LECTURE:
			{
				AddToQueue( &queue, CreeActionDeplacement( 0, 0, 0, 0 ) );

				break;
			}

			case FIN_NIVEAU:
			{
				if( ++niveau <= 3 )
				{
					AddToQueue( &queue, CreeActionInitialize() );
				}

				break;
			}

			case CALCUL:
			{
				AddToQueue( &queue, CreeActionLecture() );

				break;
			}
		}

		free( action );
	}
}
