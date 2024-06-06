
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

	// pour �viter que rand() ne donne toujours les m�mes valeurs.
	srand((unsigned int)GetTickCount64());
}

/*
 * L�initialisation de la Matrice qui doit recevoir la Matrice en param�tre 
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
		AfficheErreur("Impossible d'allouer la m�moire pour le plateau");

		LOG(ERROR_FATAL, "Impossible d'allouer la m�moire pour le plateau");
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
 * V�rification : calcule si oui ou non l�utilisateur a gagn�. Le joueur
 * a gagn� s�il ne reste plus aucune g�latine.
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

// manque la v�rification de l'environement de l'�change

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
 * Calcul : action g�n�r�e lorsque l�utilisateur � intervertit deux cases. Il s�agit de
 * calculer si trois pions se suivent en Vertical ou en Horizontal. Si trois pions se
 * suivent en vertical, la fonction devra ajouter une action � Suppression V � sur la
 * Queue. Si trois pions se suivent en horizontal, alors il faut ajouter une action
 * � Suppression H � sur la Queue.
 * 
  *Si la Queue est pleine, il faut afficher un message d�erreur et arr�ter le programme.
 */
Action* Calcul(Plateau* plateau)
{
	LOG(INFO, "%s( %p )", __func__, plateau);

	LOG(RET, "nullptr");
	return nullptr;
}

/*
 * D�placement : Cette fonction re�oit en coordonn�e les deux pions qui doivent �tre
 * intervertis. La fonction va effectivement les intervertir.
 */
void Deplacement(Plateau* plateau, int x1, int y1, int x2, int y2)
{
	LOG(INFO, "%s( %p, %d, %d, %d, %d )", __func__, plateau, x1, y1, x2, y2);
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
