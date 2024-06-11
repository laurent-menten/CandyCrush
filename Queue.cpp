
#include "Queue.h"
#include "Log.h"
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

	queue->actions = (Action**) malloc( size * sizeof( Action* ) );
	if( queue->actions == nullptr )
	{
		LOG(ERROR_FATAL, "Allocation mémoire pour le tableau d'actions de la queue" ); // noreturn
	}

	return 0;
}

void FinaliseQueue(ActionQueue* queue)
{
	for (int i = 0; i < queue->size; i++)
	{
		if (queue->actions[i] != nullptr)
		{
			free(queue->actions[i]);
		}
	}

	free(queue->actions);

	queue->actions = nullptr;
}

int AddToQueue( ActionQueue* queue, Action* action )
{
	if( QueuePleine( queue ) )
	{
		LOG(ERROR_FATAL, "La queue est pleine" ); // noreturn
	}


	// Premier élément dans la queue

	if( queue->top == -1 ) // Queue->Next vaut aussi -1
	{
		queue->top = 0;
	}

	// Ajoute à la prochaine place libre

	queue->next = (queue->next + 1) % queue->size;
	queue->actions[queue->next] = action;

	return 0;
}

Action* GetFromQueue( ActionQueue* queue )
{
	if( QueueVide( queue ) )
	{
		return nullptr;
	}

	Action* action = queue->actions[queue->top];
	queue->actions[queue->top] = nullptr;

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

bool QueueVide(ActionQueue* Queue)
{
	return (Queue->top == -1);
}

bool QueuePleine(ActionQueue* Queue)
{
	return (Queue->top == Queue->next + 1)
		|| ((Queue->top == 0) && (Queue->next == Queue->size - 1));
}
