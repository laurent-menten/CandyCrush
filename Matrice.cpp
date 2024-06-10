
#include "Log.h"
#include "Affichage.h"
#include "Matrice.h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

static ParametresNiveau* niveaux;
static int niveauMax;

/*
 * initialize les paramètres du jeu 
 */
void InitialiseJeu(ParametresNiveau* _prototypesNiveau, int _niveauMax)
{
	LOG(INFO, "%s( %p, %d )", __func__, _prototypesNiveau, _niveauMax);

	niveaux = _prototypesNiveau;
	niveauMax = _niveauMax;

	// pour éviter que rand() ne donne toujours les mêmes valeurs.
	srand((unsigned int)GetTickCount64());
}

/*
 * L’initialisation de la Matrice en fonction du niveau
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
		free(plateau->matrice);
	}

	plateau->niveau = niveau;
	plateau->colonnes = niveaux[niveau - 1].colonnes;
	plateau->lignes = niveaux[niveau - 1].lignes;
	plateau->coups = niveaux[niveau - 1].coups;
	plateau->jellies = niveaux[niveau - 1].jellies;

	plateau->matrice = (Case**)malloc( plateau->colonnes * plateau->lignes * sizeof( Case* ) );
	if (!plateau->matrice)
	{
		AfficheErreur("Impossible d'allouer la mémoire pour le plateau");

		LOG(ERROR_FATAL, "Impossible d'allouer la mémoire pour le plateau");
		return -1;
	}

	for( int l = 1; l <= plateau->lignes; l++ )
	{
		for (int c = 1; c <= plateau->colonnes; c++)
		{
			Case* NewCase = CreeCaseAleatoire();
			if (!NewCase)
			{
				AfficheErreur("Impossible d'allouer la mémoire pour la case (%d-%d) du plateau", l, c);

				LOG(ERROR_FATAL, "Impossible d'allouer la mémoire pour la case (%d-%d) du plateau", l, c);
				return -1;
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

/*
 * Vérification : calcule si oui ou non l’utilisateur a gagné. Le joueur
 * a gagné s’il ne reste plus aucune gélatine.
 */
bool Verifie(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	bool rc = true;
	LOG(RET, "%d", rc);

	return rc;
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

// manque la vérification de l'environement de l'échange
// mais ce n'est pas explicitment indiqué dans l'énoncé

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

// retourne 0 si rien trouvé
// le nombre de pions successifs et les infos si trouvé.
int VerifieColonnes(Plateau* plateau, int* colonne, int* ligneDebut, int* ligneFin)
{
	int l;
	int lz;
	int c;

	for (c = 1; c <= plateau->colonnes; c++)
	{
		Case* pionDebut = plateau->matrice[GetNormalizedIndex(plateau, 1, c)];

		for (l = 1, lz = 1; l+lz < plateau->lignes; )
		{
			Case* pionFin = plateau->matrice[GetNormalizedIndex(plateau, l+lz, c)];

//			AffichePlateau(plateau);
//			printf("%d: %d +%d = %d\n", c, l, lz, l+lz);

			if (!pionDebut->vide && (pionFin->type == pionDebut->type))
			{
				lz++;
			}
			else
			{
				if (lz > 2)
				{
					*colonne = c;
					*ligneDebut = l;
					*ligneFin = l + lz - 1;

					return lz;
				}

				pionDebut = pionFin;
				l += lz;
				lz = 1;
			}

//			(void) getchar();
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

		for (c = 1, cz = 1; c + cz < plateau->colonnes; )
		{
			Case* pionFin = plateau->matrice[GetNormalizedIndex(plateau, l, c + cz)];

//			AffichePlateau(plateau);
//			printf("%d: %d +%d = %d\n", l, c, cz, c+cz);

			if (!pionDebut->vide && (pionFin->type == pionDebut->type))
			{
				cz++;
			}
			else
			{
				if (cz > 2)
				{
					*ligne = l;
					*colonneDebut = c;
					*colonneFin = c + cz - 1;

					return cz;
				}

				pionDebut = pionFin;
				c += cz;
				cz = 1;
			}

//			(void) getchar();
		}
	}

	return 0;
}

/*
 * Calcul : action générée lorsque l’utilisateur à intervertit deux cases. Il s’agit de
 * calculer si trois pions se suivent en Vertical ou en Horizontal. Si trois pions se
 * suivent en vertical, la fonction devra ajouter une action « Suppression V » sur la
 * Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une action
 * « Suppression H » sur la Queue.
 * 
  *Si la Queue est pleine, il faut afficher un message d’erreur et arrêter le programme.
 */
Action* Calcul(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	LOG(RET, "nullptr");
	return nullptr;
}

/*
 * Déplacement : Cette fonction reçoit en coordonnée les deux pions qui doivent être
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
}

/*
 * Suppression-V : trois pions identiques ont été décelés en position Verticale.
 * Il s’agit de les supprimer du jeu et faire glisser les pions du dessus.
 *
 * Si une case renseignée contenait une gélatine alors elle sera supprimée. On
 * fait descendre les pions et on fait entrer aléatoirement de nouveaux pions.
 * 
 * La fonction doit recevoir la Matrice et l’action à exécuter en paramètre.
 */
void SuppressionVerticale(Plateau* plateau, int colonne, int ligneDebut, int ligneFin)
{
	LOG(INFO, "%s( %p, %d, %d, %d )", __func__, plateau, colonne, ligneDebut, ligneFin );

	int l;
	int l2;

	// optimization pour une colonne complète

	if (ligneDebut == 1 && ligneFin == plateau->lignes)
	{
		for (l = ligneDebut; l <= ligneFin; l++)
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

	// supprime les pions de la ligne

	for (l = ligneDebut; l <= ligneFin; l++)
	{
		int i = GetNormalizedIndex(plateau, l, colonne);

		if (plateau->matrice[i]->jelly)
		{
			plateau->jellies--;
		}

		free(plateau->matrice[i]);
	}

	// déplace les pions vers le bas

	for (l = ligneDebut, l2 = ligneFin + 1; l <= (plateau->lignes - (ligneFin - ligneDebut) - 1); l++, l2++ )
	{
		int i1 = GetNormalizedIndex(plateau, l, colonne);
		int i2 = GetNormalizedIndex(plateau, l2, colonne);

		plateau->matrice[i1] = plateau->matrice[i2];
	}

	// insère les nouveaux pions

	for (l = (plateau->lignes - (ligneFin - ligneDebut) - 1) + 1; l < plateau->lignes; l++)
	{
		int i = GetNormalizedIndex(plateau, l, colonne);
		plateau->matrice[i] = CreeCaseAleatoire();
	}
}

/*
 * Suppression-H : trois pions identiques ont été décelés en position Horizontale.
 * Il s’agit de les supprimer du jeu et faire glisser les pions du dessus.
 * 
 * Si une case renseignée contenait une gélatine alors elle sera supprimée. On
 * fait glisser les pions des colonnes d’une position vers le bas et on fait entrer
 * un pion aléatoire dans chaque colonne.
 * 
 * La fonction doit recevoir la Matrice et l’action à exécuter en paramètre.
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
