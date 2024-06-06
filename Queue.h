#pragma once

#include "Action.h"

// ============================================================================
// === 
// ============================================================================

#define DEFAULT_QUEUE_SIZE 100

// ============================================================================
// === 
// ============================================================================

typedef struct ActionQueue
{
	size_t size;		// Taille de la queue
	int top;			// Emplacement du premier élément disponible
	int next;			// Prochain emplacement libre

	Action** actions;	// Contenu
} ActionQueue;

// Initialize la queue
// Retourne 0 si OK, -1 si problème d'allocation mémoire
int InitializeQueue( ActionQueue* queue, size_t size = DEFAULT_QUEUE_SIZE );

// Libère les resources de la queue
void FinaliseQueue( ActionQueue* queue );

// Ajoute un élément à la queue.
// Retourne 0 si OK, -1 sur la queue est pleine
int AddToQueue( ActionQueue* Queue, Action* Action );

// Retire un élément de la queue
// Retourne un nullptr si la queue est vide
Action* GetFromQueue( ActionQueue* Queue );

// Vérifie si queue est vide
bool QueueVide(ActionQueue* Queue);

// Vérifie sur la queue est pleine
bool QueuePleine(ActionQueue* Queue);

