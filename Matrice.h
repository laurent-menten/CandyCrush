#pragma once

#define TAILLE_PLATEAU_COLONNES 20
#define TAILLE_PLATEAU_LIGNES 20
#define NB_COUPS 20;

enum TypePion
{
	EMPTY,
	JAUNE,
	VERT,
	BLEU,
	ROUGE,
	MAUVE
};

typedef struct Case
{
	TypePion type;
	bool jelly;
} Case;

typedef struct PrototypePlateau
{
	int colonnes;
	int lignes;
	int coups;
	int jellies;

} PrototypePlateau;

typedef struct Plateau
{
	int niveau;
	int colonnes;
	int lignes;
	int coups;

	Case* matrice;
} Plateau;

int InitializePlateau( Plateau* plateau, int niveau );


void SuppressionV( int X1, int Y1, int X2, int Y2 );
void SuppressionH( int X1, int Y1, int X2, int Y2 );
