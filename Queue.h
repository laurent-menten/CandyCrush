#pragma once

// ============================================================================
// === 
// ============================================================================

#define DEFAULT_QUEUE_SIZE 100

// ============================================================================
// === 
// ============================================================================

enum ActionType
{
	INITIALIZATION,		// 0
	AFFICHAGE,			// 1
	LECTURE,			// 2
	DEPLACEMENT,		// 3
	CALCUL,				// 4
	VERIFICATION,		// 5
	SUPRESSION_V,		// 6
	SUPRESSION_H,		// 7
	SUPRESSION_COLONNE,	// 8
	SUPRESSION_LIGNE,	// 9
	FIN_NIVEAU			// 10
};

// ============================================================================
// === 
// ============================================================================

// La structure de base Action ne contient que le type de l'action. Cette
// structure sert pour les actions qui n'ont pas de données supplémentaires.
// 
// Pour les actions nécessitant des données supplémentaire, on définit une
// structure contenant en premier la structure de base puis les données. Ainsi
// un pointeur vers nimporte laquelle de ces structures permettra toujours
// d'accéder au type et il suffira de faire un cast du pointeur vers le
// type de structure adéquat pour obtenir les données.
//
// ex:
//	Action* action = GetFromQueue();
//	if( action->type == DEPLACEMENT )
//	{
//		ActionDeplacement* actionDeplacement = (ActionDeplacement*)action;
//		actionDeplacement->action.type;
//		actionDeplacement->x1;
//	}

typedef struct Action
{
	ActionType type;

} Action;

typedef struct ActionDeplacement
{
	Action action;

	int x1;
	int y1;
	int x2;
	int y2;

} ActionDeplacement;

typedef struct ActionSupressionVH
{
	Action action;

	int index;
	int debut;
	int fin;

} ActionSupressionV, ActionSupressionH;

typedef struct ActionSupressionColonneLigne
{
	Action action;

	int index;

} ActionSupressionColonne, ActionSupressionLigne;

// ============================================================================
// === 
// ============================================================================

Action* CreeActionInitialize();
Action* CreeActionAffichage();
Action* CreeActionLecture();
Action* CreeActionDeplacement( int x1, int y1, int x2, int y2 );
Action* CreeActionCalcul();
Action* CreeActionVerification();
Action* CreeActionSupressionV( int collone, int ligneDebut, int ligneFin );
Action* CreeActionSupressionH( int ligne, int colloneDebut, int colloneFin );
Action* CreeActionSupressionColonne( int collone );
Action* CreeActionSupressionLigne( int ligne );
Action* CreeActionFinNiveau();

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

