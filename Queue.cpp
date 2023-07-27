
#include "Queue.h"
#include "Affichage.h"

#include <stdlib.h>
#include <malloc.h>

// ============================================================================
// =
// ============================================================================

int InitializeQueue( ActionQueue* queue, size_t size )
{
	queue->size = size;
	queue->top = -1;
	queue->next = -1;

	queue->Actions = (Action**) malloc( size * sizeof( Action* ) );
	if( queue->Actions == nullptr )
	{
		AfficheError( "Allocation mémoire pour le tableau d'actions de la queue" );

		return -1;
	}

	return 0;
}

bool QueueVide( ActionQueue* Queue )
{
	return (Queue->top == -1);
}

bool QueuePleine( ActionQueue* Queue )
{
	return (Queue->top == Queue->next + 1)
		|| ((Queue->top == 0) && (Queue->next == Queue->size - 1));
}


int AddToQueue( ActionQueue* queue, Action* action )
{
	if( QueuePleine( queue ) )
	{
		AfficheError( "La queue est pleine" );
		return -1;
	}


	// Premier élément dans la queue

	if( queue->top == -1 ) // Queue->Next vaut aussi -1
	{
		queue->top = 0;
	}

	// Ajoute à la prochaine place libre

	queue->next = (queue->next + 1) % queue->size;
	queue->Actions[queue->next] = action;

	return 0;
}

Action* GetFromQueue( ActionQueue* queue )
{
	if( QueueVide( queue ) )
	{
		AfficheError( "La queue est vide" );
		return nullptr;
	}

	Action* action = queue->Actions[queue->top];
//	queue->Actions[queue->top] = nullptr;

	if( queue->top == queue->next )
	{
		queue->top = -1;
		queue->next = -1;
	}
	else
	{
		queue->top = (queue->top + 1) % queue->size;
	}

	return action;
}

// ============================================================================
// =
// ============================================================================

Action* CreeActionInitialize()
{
	Action* action = (Action*) malloc( sizeof( Action ) );
	if( action == nullptr )
	{
		AfficheError( "Allocation d'une action INITIALIZE" );
		return nullptr;
	}

	action->Type = INITIALIZATION;

	return action;
}

Action* CreeActionAffichage()
{
	Action* action = (Action*)malloc( sizeof( Action ) );
	if( action == nullptr )
	{
		AfficheError( "Allocation d'une action AFFICHAGE" );
		return nullptr;
	}

	action->Type = AFFICHAGE;

	return action;
}

Action* CreeActionDeplacement( int X1, int Y1, int X2, int Y2 )
{
	Action* action = (Action*)malloc( sizeof( Action ) );
	if( action == nullptr )
	{
		AfficheError( "Allocation d'une action DEPALCEMENT" );
		return nullptr;
	}

	action->Type = DEPLACEMENT;
	action->X1 = X1;
	action->Y1 = Y1;
	action->X2 = X2;
	action->Y2 = Y2;

	return action;

}

Action* CreeActionCalcul()
{
	Action* action = (Action*) malloc( sizeof( Action ) );
	if( action == nullptr )
	{
		AfficheError( "Allocation d'une action CALCUL" );
		return nullptr;
	}

	action->Type = CALCUL;

	return action;
}

Action* CreeActionLecture()
{
	Action* action = (Action*)malloc( sizeof( Action ) );
	if( action == nullptr )
	{
		AfficheError( "Allocation d'une action LECTURE" );
		return nullptr;
	}

	action->Type = LECTURE;

	return action;
}
