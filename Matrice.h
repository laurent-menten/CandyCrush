#pragma once

#include "Action.h"

#define TAILLE_PLATEAU_COLONNES 20
#define TAILLE_PLATEAU_LIGNES 20
#define NB_COUPS 20;

// Enum représantant les couleurs possible d'un bonbon.

enum TypePion
{
	JAUNE,			// 0
	VERT,			// 1
	BLEU,			// 2
	ROUGE,			// 3
	MAUVE,			// 4

	NB_TYPE_PION	// = 5
};

// Structure représentant une case du plateau.

typedef struct Case
{
	TypePion type;
	bool jelly;
	bool vide;
} Case;

// Structure représentant les paramètres d'un niveau.

typedef struct ParametresNiveau
{
	int colonnes;
	int lignes;
	int coups;
	int jellies;

} PrototypePlateau;

// Structure représentant le plateau de jeu.

typedef struct Plateau
{
	int niveau;			// niveau de jeu
	int colonnes;		// nombre de colonnes
	int lignes;			// nombre de lignes
	int coups;			// nombre de coups restants
	int jellies;		// nombre de jellies restantes

	Case* matrice;		// plateau de jeu
} Plateau;

void InitialiseJeu(ParametresNiveau* niveaux, int niveauMax);
int InitialisePlateau( Plateau* plateau, int niveau );

bool Verifie(Plateau* plateau);
bool VerifieCoordonnees(Plateau* plateau, int x, int y);
bool VerifieDeplacement(Plateau* plateau, int x1, int y1, int x2, int y2);
bool VerifieCoups(Plateau* plateau);
bool VerifieJellies(Plateau* plateau);

Action* Calcul(Plateau* plateau);
void Deplacement(Plateau* plateau, int x1, int y1, int x2, int y2);

void SuppressionVerticale(Plateau* plateau, int collone, int ligneDebut, int ligneFin );
void SuppressionHorizontale(Plateau* plateau, int ligne, int colnoneDebut, int colonneFin );
void SuppressionColonne(Plateau* plateau, int colonne);
void SuppressionLigne(Plateau* plateau, int ligne);
