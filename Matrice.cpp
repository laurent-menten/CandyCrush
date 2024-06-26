
#include "Log.h"
#include "Affichage.h"
#include "Matrice.h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

static ParametresNiveau* parametresNiveaux;
static int niveauMax;

/*
 * initialize les param�tres du jeu 
 */
void InitialiseJeu(ParametresNiveau* _prototypesNiveau, int _niveauMax)
{
	LOG(INFO, "%s( %p, %d )", __func__, _prototypesNiveau, _niveauMax);

	parametresNiveaux = _prototypesNiveau;
	niveauMax = _niveauMax;

	// pour �viter que rand() ne donne toujours les m�mes valeurs.

	srand((unsigned int)GetTickCount64());
}

/*
 * L�initialisation de la Matrice en fonction du niveau
 */

int InitialisePlateau( Plateau* plateau, int niveau )
{
	LOG(INFO, "%s( %p, %d )", __func__, plateau, niveau);

	if( (niveau < 1) || (niveau > niveauMax) )
	{
		return -1;
	}

	if (niveau > 1)
	{
		// free des cases
		free(plateau->matrice);
	}

	plateau->niveau = niveau;

	plateau->colonnes = parametresNiveaux[niveau - 1].colonnes;
	plateau->lignes = parametresNiveaux[niveau - 1].lignes;

	plateau->coups = parametresNiveaux[niveau - 1].coups;
	plateau->coupsMax = parametresNiveaux[niveau - 1].coups;

	plateau->jellies = parametresNiveaux[niveau - 1].jellies;
	plateau->jelliesMax = parametresNiveaux[niveau - 1].jellies;

	plateau->matrice = (Case**)malloc( plateau->colonnes * plateau->lignes * sizeof( Case* ) );
	if (!plateau->matrice)
	{
		AfficheErreur("Impossible d'allouer la m�moire pour le plateau");

		LOG(ERROR_FATAL, "Impossible d'allouer la m�moire pour le plateau"); // noreturn
	}

	for( int l = 1; l <= plateau->lignes; l++ )
	{
		for (int c = 1; c <= plateau->colonnes; c++)
		{
			Case* NewCase = CreeCaseAleatoire();
			if (!NewCase)
			{
				AfficheErreur("Impossible d'allouer la m�moire pour la case (%d-%d) du plateau", l, c);

				LOG(ERROR_FATAL, "Impossible d'allouer la m�moire pour la case (%d-%d) du plateau", l, c); // noreturn
			}

			plateau->matrice[GetNormalizedIndex(plateau,l,c)] = NewCase;
		}
	}

	for( int j = 0; j < plateau->jellies; j++ )
	{
		int l;
		int c;

		do
		{
			l = 1 + (rand() % plateau->colonnes);
			c = 1 + (rand() % plateau->lignes);
		}
		while( plateau->matrice[GetNormalizedIndex(plateau, l, c)]->jelly == true );

		plateau->matrice[GetNormalizedIndex(plateau, l, c)]->jelly = true;
	} 

	int rc = 0;
	LOG(RET, "%d", rc);

	return rc;
}

Case* CreeCaseAleatoire()
{
	Case* NewCase = (Case*)malloc(sizeof(Case));
	if (NewCase)
	{
		NewCase->type = (enum TypePion)((rand() % NB_TYPE_PION));
		NewCase->jelly = false;
		NewCase->vide = false;
	}

	return NewCase;
}

// true si OK
bool VerifieCoordonnees(Plateau* plateau, int l, int c)
{
	LOG(INFO, "%s( %p, %d, %d )", __func__, plateau, l, c);

	bool rc =	((l >= 1) && (l <= plateau->colonnes))
					&&
				((c >= 1) && (c <= plateau->lignes));

	LOG(RET, "%d", rc);

	return rc;
}

// manque la v�rification de l'environement de l'�change
// mais ce n'est pas explicitment indiqu� dans l'�nonc�

// true si OK
bool VerifieDeplacement(Plateau* plateau, int l1, int c1, int l2, int c2)
{
	LOG(INFO, "%s( %p, %d, %d, %d, %d )", __func__, plateau, l1, c1, l2, c2 );

	int delta_l = abs(l1 - l2);
	int delta_c = abs(c1 - c2);

	bool rc = (delta_l + delta_c) == 1;

	LOG(RET, "%d", rc);

	return rc;
}

// true si encore des coups
bool VerifieCoups(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	bool rc = plateau->coups > 0;
	LOG(RET, "%d", rc);

	return rc;
}

// true si encore des jellies
bool VerifieJellies(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	bool rc = plateau->jellies > 0;
	LOG(RET, "%d", rc);

	return rc;
}

// retourne 0 si rien trouv�
// le nombre de pions successifs et les infos si trouv�.
int VerifieColonnes(Plateau* plateau, int* colonne, int* ligneDebut, int* ligneFin)
{
	int c;
	int l;
	int lz;

	for (c = 1; c <= plateau->colonnes; c++)
	{
		Case* pionDebut = plateau->matrice[GetNormalizedIndex(plateau, 1, c)];

		for (l = 1, lz = 1; l+lz <= plateau->lignes; )
		{
			Case* pionFin = plateau->matrice[GetNormalizedIndex(plateau, l+lz, c)];

			if (pionFin->type == pionDebut->type)
			{
				lz++;
			}
			else
			{
				if (lz > 2)
				{
					break;
				}

				pionDebut = pionFin;
				l += lz;
				lz = 1;
			}
		}

		if (lz > 2)
		{
			*colonne = c;
			*ligneDebut = l;
			*ligneFin = l + lz - 1;

			return lz;
		}
	}

	return 0;
}

int VerifieLignes(Plateau* plateau, int* ligne, int* colonneDebut, int* colonneFin)
{
	int l;
	int c;
	int cz;

	for (l = 1; l <= plateau->lignes; l++)
	{
		Case* pionDebut = plateau->matrice[GetNormalizedIndex(plateau, l, 1)];

		for (c = 1, cz = 1; c + cz <= plateau->colonnes; )
		{
			Case* pionFin = plateau->matrice[GetNormalizedIndex(plateau, l, c + cz)];

			if (pionFin->type == pionDebut->type)
			{
				cz++;
			}
			else
			{
				if (cz > 2)
				{
					break;
				}

				pionDebut = pionFin;
				c += cz;
				cz = 1;
			}
		}

		if (cz > 2)
		{
			*ligne = l;
			*colonneDebut = c;
			*colonneFin = c + cz - 1;

			return cz;
		}
	}

	return 0;
}

/*
 * Calcul : action g�n�r�e lorsque l�utilisateur � intervertit deux cases. Il s�agit de
 * calculer si trois pions se suivent en Vertical ou en Horizontal. Si trois pions se
 * suivent en vertical, la fonction devra ajouter une action � Suppression V � sur la
 * Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une action
 * � Suppression H � sur la Queue.
 */
Action* Calcul(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	// --- block "vertical"

	{
		int colonne = 0;
		int ligneDebut = 0;
		int ligneFin = 0;
		int taille;

		taille = VerifieColonnes(plateau, &colonne, &ligneDebut, &ligneFin);
		if (taille == 3)
		{
			LOG( RET, "colonne: %d, lignes: %d-%d", colonne, ligneDebut, ligneFin );

			return CreeActionSupressionV(colonne, ligneDebut, ligneFin);
		}
		else if (taille > 3)
		{
			LOG(RET, "colonne: %d", colonne, ligneDebut, ligneFin);

			return CreeActionSupressionColonne(colonne) ;
		}
	}

	// -- block "horizontal"

	{
		int ligne;
		int colonneDebut = 0;
		int colonneFin = 0;
		int taille;

		taille = VerifieLignes(plateau, &ligne, &colonneDebut, &colonneFin);
		if (taille == 3)
		{
			LOG(RET, "ligne: %d, colonnes: %d-%d", ligne, colonneDebut, colonneFin);

			return CreeActionSupressionH(ligne, colonneDebut, colonneFin);
		}
		else if (taille > 3)
		{
			LOG(RET, "ligne: %d", ligne );

			return CreeActionSupressionLigne(ligne);
		}

	}

	LOG(RET, "verification");

	return CreeActionVerification();
}

/*
 * D�placement : Cette fonction re�oit en coordonn�e les deux pions qui doivent �tre
 * intervertis. La fonction va effectivement les intervertir.
 */
void Deplacement(Plateau* plateau, int l1, int c1, int l2, int c2)
{
	LOG(INFO, "%s( %p, %d, %d, %d, %d )", __func__, plateau, l1, c1, l2, c2);

	int i1 = GetNormalizedIndex(plateau, l1, c1);
	int i2 = GetNormalizedIndex(plateau, l2, c2);

	Case* tmp = plateau->matrice[i1];
	plateau->matrice[i1] = plateau->matrice[i2];
	plateau->matrice[i2] = tmp;

	plateau->coups--;
}

/*
 * Suppression-V : trois pions identiques ont �t� d�cel�s en position Verticale.
 * Il s�agit de les supprimer du jeu et faire glisser les pions du dessus.
 *
 * Si une case renseign�e contenait une g�latine alors elle sera supprim�e. On
 * fait descendre les pions et on fait entrer al�atoirement de nouveaux pions.
 * 
 * La fonction doit recevoir la Matrice et l�action � ex�cuter en param�tre.
 */

void SuppressionVerticale(Plateau* plateau, int colonne, int ligneDebut, int ligneFin)
{
	LOG(INFO, "%s( %p, %d, %d, %d )", __func__, plateau, colonne, ligneDebut, ligneFin );

	// --- optimization pour une colonne compl�te

	if (ligneDebut == 1 && ligneFin == plateau->lignes)
	{
		for (int l = ligneDebut; l <= ligneFin; l++)
		{
			int i = GetNormalizedIndex(plateau, l, colonne);

			if (plateau->matrice[i]->jelly)
			{
				plateau->jellies--;
			}

			free(plateau->matrice[i]);

			plateau->matrice[i] = CreeCaseAleatoire();
		}

		return;
	}

	// -- supprime les pions de la colonne

	for (int l = ligneDebut; l <= ligneFin; l++)
	{
		int i = GetNormalizedIndex(plateau, l, colonne);

		if (plateau->matrice[i]->jelly)
		{
			plateau->jellies--;
		}

		plateau->matrice[i]->vide = true;
	}

	// d�place les pions restants de la colonne vers le bas

	int compteLignes = (ligneFin - ligneDebut) + 1;

	for (int l = ligneDebut; l <= (plateau->lignes - compteLignes) ; l++ )
	{
		int i1 = GetNormalizedIndex(plateau, l, colonne);
		int i2 = i1 + (compteLignes * plateau->colonnes);

		Case* tmp = plateau->matrice[i1];
		plateau->matrice[i1] = plateau->matrice[i2];
		plateau->matrice[i2] = tmp;
	}

	// -- ins�re les nouveaux pions pour remplir la colonne

	for (int l = (plateau->lignes - compteLignes) + 1; l <= plateau->lignes; l++)
	{
		int i = GetNormalizedIndex(plateau, l, colonne);

		free(plateau->matrice[i]);

		plateau->matrice[i] = CreeCaseAleatoire();
	}
}

/*
 * Suppression-H : trois pions identiques ont �t� d�cel�s en position Horizontale.
 * Il s�agit de les supprimer du jeu et faire glisser les pions du dessus.
 * 
 * Si une case renseign�e contenait une g�latine alors elle sera supprim�e. On
 * fait glisser les pions des colonnes d�une position vers le bas et on fait entrer
 * un pion al�atoire dans chaque colonne.
 * 
 * La fonction doit recevoir la Matrice et l�action � ex�cuter en param�tre.
 */
void SuppressionHorizontale(Plateau* plateau, int ligne, int colonneDebut, int colonneFin)
{
	LOG(INFO, "%s( %p, %d, %d, %d )", __func__, plateau, ligne, colonneDebut, colonneFin );

	for (int c = colonneDebut; c <= colonneFin; c++)
	{
		SuppressionVerticale(plateau, c, ligne, ligne);
	}
}

void SuppressionColonne(Plateau* plateau, int colonne)
{
	LOG(INFO, "%s( %p, %d )", __func__, plateau, colonne);

	SuppressionVerticale(plateau, colonne, 1, plateau->lignes);
}

void SuppressionLigne(Plateau* plateau, int ligne)
{
	LOG(INFO, "%s( %p, %d )", __func__, plateau, ligne);

	SuppressionHorizontale(plateau, ligne, 1, plateau->colonnes);
}
