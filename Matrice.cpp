
#include "Log.h"
#include "Affichage.h"
#include "Matrice.h"

#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>

static ParametresNiveau* niveaux;
static int niveauMax;

void InitialiseJeu(ParametresNiveau* _prototypesNiveau, int _niveauMax)
{
	LOG(INFO, "%s( %p, %d )", __func__, _prototypesNiveau, _niveauMax);

	niveaux = _prototypesNiveau;
	niveauMax = _niveauMax;

	// pour éviter que rand() ne donne toujours les mêmes valeurs.
	srand((unsigned int)GetTickCount64());
}

/*
 * L’initialisation de la Matrice qui doit recevoir la Matrice en paramètre 
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

	plateau->matrice = (Case*)malloc( plateau->colonnes * plateau->lignes * sizeof( Case ) );
	if (!plateau->matrice)
	{
		AfficheErreur("Impossible d'allouer la mémoire pour le plateau");

		LOG(ERROR_FATAL, "Impossible d'allouer la mémoire pour le plateau");
		return -1;
	}

	for( int l = 0; l < plateau->lignes; l++ )
	{
		for( int c = 0; c < plateau->colonnes; c++ )
		{
			plateau->matrice[(l * plateau->colonnes) + c].type = (enum TypePion) ((rand() % NB_TYPE_PION));
			plateau->matrice[(l * plateau->colonnes) + c].jelly = false;
			plateau->matrice[(l * plateau->colonnes) + c].vide = false;
		}
	}

	for( int j = 0; j < plateau->jellies; j++ )
	{
		int x;
		int y;

		do
		{
			x = (rand() + 1) % plateau->colonnes;
			y = (rand() + 1) % plateau->lignes;
		}
		while( plateau->matrice[(y * plateau->lignes) + x].jelly == true );

		plateau->matrice[(y * plateau->lignes) + x].jelly = true;
	} 

	int rc = 0;
	LOG(RET, "%d", rc);

	return rc;
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

bool VerifieCoordonnees(Plateau* plateau, int x, int y)
{
	LOG(INFO, "%s( %p, %d, %d )", __func__, plateau, x, y);

	bool rc = true;
	LOG(RET, "%d", rc);

	return rc;
}

// manque la vérification de l'environement de l'échange

bool VerifieDeplacement(Plateau* plateau, int x1, int y1, int x2, int y2)
{
	LOG(INFO, "%s( %p, %d, %d, %d, %d )", __func__, plateau, x1, y1, x2, y2 );

	int x = abs(x1 - x2);
	int y = abs(y1 - y2);

	bool rc = ((x == 0) && (y == 1)) || ((x == 1) && (y == 0));
	LOG(RET, "%d", rc);

	return rc;
}

bool VerifieCoups(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	bool rc = plateau->coups > 0;
	LOG(RET, "%d", rc);

	return rc;
}

bool VerifieJellies(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	bool rc = plateau->jellies > 0;
	LOG(RET, "%d", rc);

	return rc;
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
void Deplacement(Plateau* plateau, int x1, int y1, int x2, int y2)
{
	LOG(INFO, "%s( %p, %d, %d, %d, %d )", __func__, plateau, x1, y1, x2, y2);
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
