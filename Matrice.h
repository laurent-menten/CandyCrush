#pragma once

#include "Action.h"

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

// Note: j'ai chang� la repr�sentation du plateau d'un array de Case un array de 
// pointeurs de Case, ce qui simplifie le d�placement d'un Case. En effect plut�t
// que devoir copier chaque info de Case individuellement on copie juste le pointeur.

typedef struct Plateau
{
	int niveau;			// niveau de jeu
	int colonnes;		// nombre de colonnes
	int lignes;			// nombre de lignes
	int coups;			// nombre de coups restants
	int jellies;		// nombre de jellies restantes

	Case** matrice;		// plateau de jeu
} Plateau;

// Renvois l'index d'une entr�e sur le plateau de jeu en prenant en compte
// que les index sont bas� sur 0 et non 1.
//
// fonction � utiliser chaque fois qu'il faut acc�der � une entr�e du plateau
// d�clar�e inline donc est "coll�e" dans le code plut�t que g�n�rer un appel

inline int GetNormalizedIndex(Plateau* plateau, int l, int c)
{
	return ((l - 1) * plateau->colonnes) + (c - 1);
}

void InitialiseJeu(ParametresNiveau* niveaux, int niveauMax);
int InitialisePlateau( Plateau* plateau, int niveau );
Case* CreeCaseAleatoire();

bool Verifie(Plateau* plateau);
bool VerifieCoordonnees(Plateau* plateau, int l, int c);
bool VerifieDeplacement(Plateau* plateau, int l1, int c1, int l2, int c2);
bool VerifieCoups(Plateau* plateau);
bool VerifieJellies(Plateau* plateau);
int VerifieColonnes(Plateau* plateau, int* colonne, int* ligneDebut, int* ligneFin);
int VerifieLignes(Plateau* plateau, int* ligne, int* colonneDebut, int* colonneFin);

Action* Calcul(Plateau* plateau);
void Deplacement(Plateau* plateau, int l1, int c1, int l2, int c2);

void SuppressionVerticale(Plateau* plateau, int colonne, int ligneDebut, int ligneFin );
void SuppressionHorizontale(Plateau* plateau, int ligne, int colnoneDebut, int colonneFin );
void SuppressionColonne(Plateau* plateau, int colonne);
void SuppressionLigne(Plateau* plateau, int ligne);
