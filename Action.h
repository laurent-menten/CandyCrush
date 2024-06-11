#pragma once

// ============================================================================
// === Types d'actions ========================================================
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
// === Structure des différents types d'actions ===============================
// ============================================================================

// La structure de base Action ne contient que le type de l'action. Cette
// structure sert pour les actions qui n'ont pas de données supplémentaires.
// 
// Pour les actions nécessitant des données supplémentaire, on définit une
// structure contenant en premier la structure de base puis les données. Ainsi
// un pointeur vers nimporte laquelle de ces structures permettra toujours
// d'accéder au type et il suffira de faire un cast du pointeur vers le
// type de structure adéquat pour obtenir les données supplémentaires.
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

	int ligneOrigine;
	int colonneOrigine;
	int ligneDestination;
	int colonneDestination;

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
// === Fonctions de construction des différents types d'actions ===============
// ============================================================================

Action* CreeActionInitialize();
Action* CreeActionAffichage();
Action* CreeActionLecture();
Action* CreeActionDeplacement(int x1, int y1, int x2, int y2);
Action* CreeActionCalcul();
Action* CreeActionVerification();
Action* CreeActionSupressionV(int collone, int ligneDebut, int ligneFin);
Action* CreeActionSupressionH(int ligne, int colloneDebut, int colloneFin);
Action* CreeActionSupressionColonne(int collone);
Action* CreeActionSupressionLigne(int ligne);
Action* CreeActionFinNiveau();
