#pragma once

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
	int niveau;
	int colonnes;
	int lignes;
	int coups;
	int jellies;

	Case* matrice;
} Plateau;

void InitializeJeu(ParametresNiveau* niveaux, int niveauMax);
int InitializePlateau( Plateau* plateau, int niveau );

// Verifie la présence de gélatine sur le plateau
// true si gélatines présentes, false sinon. 
bool VerifieGelatine(Plateau* plateau);

bool VerifieEchange(int X1, int Y1, int X2, int Y2);


void SuppressionV( int X1, int Y1, int X2, int Y2 );
void SuppressionH( int X1, int Y1, int X2, int Y2 );
