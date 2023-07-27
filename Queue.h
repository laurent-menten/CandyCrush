#pragma once

#define DEFAULT_QUEUE_SIZE 100

enum ActionType
{
	CALCUL,
	AFFICHAGE,
	SUPRESSION_V,
	SUPRESSION_H,
	VERIFICATION,
	LECTURE,
	INITIALIZATION,
	DEPLACEMENT,
	FIN_NIVEAU
};

typedef struct Action
{
	ActionType Type;
	int X1, Y1;
	int X2, Y2;
} Action;

// ============================================================================
// === 
// ============================================================================

Action* CreeActionInitialize();
Action* CreeActionAffichage();
Action* CreeActionDeplacement( int X1, int Y1, int X2, int Y2 );
Action* CreeActionCalcul();
Action* CreeActionLecture();

// ============================================================================
// === 
// ============================================================================

typedef struct ActionQueue
{
	size_t size;		// Taille de la queue
	int top;			// Emplacement du premier élément disponible
	int next;			// Prochain emplacement libre

	Action** Actions;	// Contenu
} ActionQueue;

// Vérifie si queue est vide
bool QueueVide( ActionQueue* Queue );

// Vérifie sur la queue est pleine
bool QueuePleine( ActionQueue* Queue );

// Initialize la queue
// Retourne 0 si OK, -1 si problème d'allocation mémoire
int InitializeQueue( ActionQueue* queue, size_t size = DEFAULT_QUEUE_SIZE );

// Ajoute un élément à la queue.
// Retourne 0 si OK, -1 sur la queue est pleine
int AddToQueue( ActionQueue* Queue, Action* Action );

// Retire un élément de la queue
// Retourne un nullptr si la queue est vide
Action* GetFromQueue( ActionQueue* Queue );
