#pragma once

#define TAILLE_PLATEAU_COLONNES 20
#define TAILLE_PLATEAU_LIGNES 20
#define NB_COUPS 20;

// Enum repr�santant les couleurs possible d'un bonbon.

enum TypePion
{
	JAUNE,			// 0
	VERT,			// 1
	BLEU,			// 2
	ROUGE,			// 3
	MAUVE,			// 4

	NB_TYPE_PION	// = 5
};

// Structure repr�sentant une case du plateau.

typedef struct Case
{
	TypePion type;
	bool jelly;
	bool vide;
} Case;

// Structure repr�sentant les param�tres d'un niveau.

typedef struct ParametresNiveau
{
	int colonnes;
	int lignes;
	int coups;
	int jellies;

} PrototypePlateau;

// Structure repr�sentant le plateau de jeu.

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

// Verifie la pr�sence de g�latine sur le plateau
// true si g�latines pr�sentes, false sinon. 
bool VerifieGelatine(Plateau* plateau);

bool VerifieEchange(int X1, int Y1, int X2, int Y2);


void SuppressionV( int X1, int Y1, int X2, int Y2 );
void SuppressionH( int X1, int Y1, int X2, int Y2 );
