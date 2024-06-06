
#include "Action.h"
#include "Affichage.h"

#include <malloc.h>

// ============================================================================
// =
// ============================================================================

Action* CreeActionInitialize()
{
	Action* action = (Action*)malloc(sizeof(Action));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action INITIALIZE");
		return nullptr;
	}

	action->type = INITIALIZATION;

	return action;
}

Action* CreeActionAffichage()
{
	Action* action = (Action*)malloc(sizeof(Action));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action AFFICHAGE");
		return nullptr;
	}

	action->type = AFFICHAGE;

	return action;
}

Action* CreeActionLecture()
{
	Action* action = (Action*)malloc(sizeof(Action));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action LECTURE");
		return nullptr;
	}

	action->type = LECTURE;

	return action;
}

Action* CreeActionDeplacement(int x1, int y1, int x2, int y2)
{
	ActionDeplacement* action = (ActionDeplacement*)malloc(sizeof(ActionDeplacement));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action DEPALCEMENT");
		return nullptr;
	}

	action->action.type = DEPLACEMENT;
	action->x1 = x1;
	action->y1 = y1;
	action->x2 = x2;
	action->y2 = y2;

	return (Action*)action;

}

Action* CreeActionCalcul()
{
	Action* action = (Action*)malloc(sizeof(Action));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action CALCUL");
		return nullptr;
	}

	action->type = CALCUL;

	return action;
}

Action* CreeActionVerification()
{
	Action* action = (Action*)malloc(sizeof(Action));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action VERIFICATION");
		return nullptr;
	}

	action->type = VERIFICATION;

	return action;
}

Action* CreeActionSupressionV(int colonne, int ligneDebut, int ligneFin)
{
	ActionSupressionV* action = (ActionSupressionV*)malloc(sizeof(ActionSupressionV));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action SUPRESSION_V");
		return nullptr;
	}

	action->action.type = SUPRESSION_V;
	action->index = colonne;
	action->debut = ligneDebut;
	action->fin = ligneFin;

	return (Action*)action;
}

Action* CreeActionSupressionH(int ligne, int colonneDebut, int colonneFin)
{
	ActionSupressionH* action = (ActionSupressionH*)malloc(sizeof(ActionSupressionH));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action SUPRESSION_H");
		return nullptr;
	}

	action->action.type = SUPRESSION_H;
	action->index = ligne;
	action->debut = colonneDebut;
	action->fin = colonneFin;

	return (Action*)action;
}

Action* CreeActionSupressionColonne(int colonne)
{
	ActionSupressionColonne* action = (ActionSupressionColonne*)malloc(sizeof(ActionSupressionColonne));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action SUPRESSION_COLONNE");
		return nullptr;
	}

	action->action.type = SUPRESSION_COLONNE;
	action->index = colonne;

	return (Action*)action;
}

Action* CreeActionSupressionLigne(int ligne)
{
	ActionSupressionLigne* action = (ActionSupressionLigne*)malloc(sizeof(ActionSupressionLigne));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action SUPRESSION_LIGNE");
		return nullptr;
	}

	action->action.type = SUPRESSION_LIGNE;
	action->index = ligne;

	return (Action*)action;
}

Action* CreeActionFinNiveau()
{
	Action* action = (Action*)malloc(sizeof(Action));
	if (action == nullptr)
	{
		AfficheErreur("Allocation d'une action FIN_NIVEAU");
		return nullptr;
	}

	action->type = FIN_NIVEAU;

	return action;
}
