
#include "Log.h"
#include "Queue.h"
#include "Matrice.h"
#include "Affichage.h"
#include "Lecture.h"
#include "Utils.h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

/*
 * Cet exercice va élaborer le jeu Candy Crush dans une version très simplifiée
 * et qui se focalisera sur l’algorithmique.
 * La partie visuelle ne sera pas fort élaborée.
 * 
 * Comme indiqué, on va travailler sur une version simplifiée mais qui devra
 * pouvoir être complexifiée sans problèmes.
 *
 * Les hypothèses simplificatrices suivantes seront d’application:
 * 
 * - Bien que le vrai jeu comporte plusieurs milliers de niveaux, on se
 *   limitera à trois niveaux.
 * 
 * - L’objectif de chaque grille consistera à éliminer la gélatine.La gélatine
 *   sera affichée à l’écran sous la forme d’une seconde grille.Une case
 *   comportant de la gélatine sera affichée par la lettre « G ».
 * - Pour chaque niveau, la gélatine sera positionnée de manière aléatoire.
 * 
 * - Chaque niveau se jouera sur une grille de 20 x 20.
 * 
 * - Les pions seront représentés par la première lettre de leur couleur. Nous
 *   aurons donc les pions suivants :
 *   - J : pion Jaune
 *   - V : pion Vert
 *   - B : pion Bleu
 *   - R : pion Rouge
 *   - M : pion Mauve
 * 
 * - Pour inverser deux pions, le joueur doit introduire les coordonnées (X, Y) de
 *   chacun des deux pions à inverser.
 * 
 * - On va se focaliser sur les séries de trois pions identiques qui se suivent
 *   soit en Vertical, soit en Horizontal.
 * 
 * Pour la seconde session, l’énoncé reste le même mais avec les ajoutes suivantes:
 * 
 * - On doit aussi pouvoir détecter les séries de 4 Pions.
 *   Quand une série de 4 pions de même couleur est trouvée sur une colonne
 *   alors on génère une action de type "Suppression-Colonne" avec comme paramètre
 *   le numéro de la colonne. Cette action va provoquer la suppression de tous les
 *   pions de la colonne avec leur gélatine éventuelle.
 *   Si ce sont 4 pions qui se trouvent sur une ligne, alors, on va générer une
 *   action de type « SuppressionLigne » avec le numéro de ligne comme paramètre.
 *   Cette action va provoquer la suppression de tous les pions de la ligne et des
 *   gélatines éventuelles qui s’y trouvent.
 * 
 * - Le joueur dispose d’un nombre déterminé pour supprimer toutes les gélatines.
 *   Le total de coups à jouer doit être affiché.
 */

/*
 * PROBLEME DE GAMEPLAY...
 * 
 * Dans l'action CALCUL, le fait de vérifier toujours les colonnes avant les
 * lignes (ce serait la même chose si on verifie toujours les lignes
 * avant les colonnes) fait parfois disparaitre des alignements qui auraient
 * pu supprimer des gélatines. .
 * 
 * Solution à possible:
 * Partir de chaque pion, rendre vide les cases en alignement dans les 4 
 * directions puis tout faire tomber quand il n'y a plus aucun alignement.
 */

// ****************************************************************************
// * Configuration ************************************************************
// ****************************************************************************

static ParametresNiveau niveaux[]
{
	{	10,			10,		1,		5 },

	//	colonnes	lignes	coups	jellies
	{	20,			20,		40,		10 },
	{	20,			20,		35,		15 },
	{	20,			20,		30,		20 },
};

// ***************************************************************************
// *
// ****************************************************************************

// Fonction de gestion des évènement liés à la fenêtre console.
// Sera appelée automatiquement.

BOOL WINAPI consoleHandler(_In_ DWORD signal )
{
	if (signal == CTRL_C_EVENT)
	{
		AfficheAvertissement("Interception de Ctrl-C ...");

		// Pour provoquer l'appel des fonctions enregistrée avec atexit();

		exit(-1);
	}

	return false;
}

// ----------------------------------------------------------------------------

static ActionQueue queue;

void DeinitialiseQueue()
{
	FinaliseQueue(&queue);
}

// ****************************************************************************
// * Point d'entrée ***********************************************************
// ****************************************************************************

int main()
{
	// ------------------------------------------------------------------------
	// - Pré-Initalisation ----------------------------------------------------
	// ------------------------------------------------------------------------

	// Initialisation du système de logging fichier.
	// Seulement si la configuration du projet est Debug.

#ifdef _DEBUG
	InitialiseLog("log.txt");
#endif

	// Installation dans le système de la fonction de gestion des évènements
	// liés à la fenêtre console.

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleHandler, TRUE);

	// Appel automatique de la finalisation de que Queue lors de la sortie du
	// programme avec exit()

	atexit( DeinitialiseQueue );

	// ------------------------------------------------------------------------
	// - Intialisation du jeu proprement dit ----------------------------------
	// ------------------------------------------------------------------------

	Plateau plateau;
	int niveau = 1;

	// --- Console en plein écran (simule Alt-Enter) --------------------------

	MaximizeConsole();

	// --- Initialisation -----------------------------------------------------

	InitialiseJeu(niveaux, (sizeof(niveaux) / sizeof(ParametresNiveau)));

	InitializeQueue(&queue);

	AddToQueue(&queue, CreeActionInitialize());

	// --- Boucle de jeu -------------------------------------------------------

	bool finJeu = false;

	Action* action;

	while( !finJeu && ((action = GetFromQueue( &queue )) != nullptr) )
	{
		switch( action->type )
		{
			// ---------------------------------------------------------------
			// - INITIALIZATION ----------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * Cette fonction va initialiser le début d’un nouveau niveau
			 * On ajoute l’action CALCUL dans la Queue afin de traiter les éventuelles
			 * séquences de pions qui seraient déjà existantes dans la Matrice.
			 */

			case INITIALIZATION:
			{
				LOG(ACTION, "INITIALIZATION");

				if( InitialisePlateau( &plateau, niveau ) != 0 )
				{
					AfficheErreur( "Erreur de création du niveau %d", niveau );

					LOG(ERROR_FATAL, "Erreur de création du niveau %d", niveau); // noreturn
				}

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			// ---------------------------------------------------------------
			// - AFFICHAGE ---------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * Cette action a pour effet d’effacer l’écran et de redessiner les deux
			 * matrices :
			 *		- La matrice avec l’emplacement des pions. Chaque case contient la
			 *		  première lettre du pion qui occupe la case.
			 *		- La matrice avec l’emplacement de la gélatine.
			 */

			case AFFICHAGE:
			{
				LOG(ACTION, "AFFICHAGE");

				AffichePlateau( &plateau );

				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			// ---------------------------------------------------------------
			// - LECTURE -----------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * - Lecture des coordonnées X et Y des deux cases qui doivent être
			 *   interverties
			 * On ajoute l’action DEPLACEMENT dans la Queue.
			 */

			case LECTURE:
			{
				LOG(ACTION, "LECTURE");

				int l1 = 0;
				int c1 = 0;
				bool origineValide = false;
				bool origineInvalide = false;

				int l2 = 0;
				int c2 = 0;
				bool destinationValide = false;
				bool destinationInvalide = false;

				bool deplacementInvalide = false;

				do 
				{
					AffichePlateau(&plateau);

					// --- Message d'erreur de l'itération précédente

					if (deplacementInvalide)
					{
						AfficheAvertissement("Les coordonnees (%d,%d)-(%d,%d) ne sont pas contigues.\n", l1, c1, l2, c2);
						origineValide = false;
						destinationValide = false;
					}

					if (origineInvalide)
					{
						AfficheAvertissement("L'origine doit etre comprise entre (%d-%d) et (%d-%d).\n", 1, 1, plateau.lignes, plateau.colonnes);
						origineInvalide = false;
					}

					if (destinationInvalide)
					{
						AfficheAvertissement("La destination doit etre comprise entre (%d-%d) et (%d-%d).\n", 1, 1, plateau.lignes, plateau.colonnes);
						destinationInvalide = false;
					}

					// --- Lecture (éventuelle) de l'origine (si on a pas déjà l'info)

					if (origineValide)
					{
						printf("Origine: (%d, %d)\n", l1, c1);
					}
					else
					{
						LectureCoordonnees("Origine [ligne colonne]: ", &l1, &c1);
						origineValide = VerifieCoordonnees(&plateau, l1, c1);
						if (!origineValide)
						{
							origineInvalide = true;
							continue;
						}
					}

					// --- Lecture de la destination

					LectureCoordonnees("Destination [ligne colonne]: ", &l2, &c2);
					destinationValide = VerifieCoordonnees(&plateau, l2, c2);
					if (!destinationValide)
					{
						destinationInvalide = true;
						continue;
					}

					// --- Verification de la validité du déplacement

					deplacementInvalide = !VerifieDeplacement(&plateau, l1, c1, l2, c2);

				} while ( !origineValide || !destinationValide || deplacementInvalide );

				LOG(ARGS, "(%d,%d) - (%d,%d) OK", l1, c1, l2, c2);

				AddToQueue( &queue, CreeActionDeplacement( l1, c1, l2, c2 ) );

				break;
			}

			// ---------------------------------------------------------------
			// - DEPLACEMENT -------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * Cette action va déclencher l’inversion des deux pions dont les
			 * coordonnées ont été introduites.
			 * La fonction va générer l’action CALCUL.
			 */

			case DEPLACEMENT:
			{
				LOG(ACTION, "DEPLACEMENT");

				ActionDeplacement* actionDeplacement = (ActionDeplacement*)action;

				int l1 = actionDeplacement->l1;
				int c1 = actionDeplacement->c1;
				int l2 = actionDeplacement->l2;
				int c2 = actionDeplacement->c2;
				LOG(ARGS, "(%d,%d) - (%d,%d)", l1, c1, l2, c2);

				Deplacement(&plateau, l1, c1, l2, c2);

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			// ---------------------------------------------------------------
			// - CALCUL ------------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * Cette action doit être générée dès que l’action LECTURE est terminée.
			 * 
			 * Si on détecte que au moins trois pions sont alignés en vertical, alors
			 * il faut placer une nouvelle action dans la Queue:
			 *		- SUPPRESSION V avec X et Y du pion supérieur et le X et le Y
			 *		   du pion inférieur.
			 * 
			 * Si on détecte que ai moins trois pions sont alignés en horizontal, alors
			 * il faut générer une nouvelle action dans la Queue:
			 *		- SUPPRESSION H avec X et Y du premier pion et X et Y du dernier pion.
			 * 
			 * Une seule Action peut être placée dans la Queue.
			 * 
			 * Si on ne détecte aucun alignement de pions alors on place les actions
			 * VERIFICATION dans la queue
			 */

			case CALCUL:
			{
				LOG(ACTION, "CALCUL");

				AffichePlateau(&plateau);

				{
					int colonne = 0;
					int ligneDebut = 0;
					int ligneFin = 0;
					int taille;

					taille = VerifieColonnes(&plateau, &colonne, &ligneDebut, &ligneFin);
					if (taille == 3)
					{
						AddToQueue(&queue, CreeActionSupressionV(colonne, ligneDebut, ligneFin));
						break;
					}
					else if (taille > 3)
					{
						AddToQueue(&queue, CreeActionSupressionColonne(colonne));
						break;
					}
				}

				{
					int ligne;
					int colonneDebut = 0;
					int colonneFin = 0;
					int taille;

					taille = VerifieLignes(&plateau, &ligne, &colonneDebut, &colonneFin);
					if (taille == 3)
					{
						AddToQueue(&queue, CreeActionSupressionH(ligne, colonneDebut, colonneFin));
						break;
					}
					else if (taille > 3)
					{
						AddToQueue(&queue, CreeActionSupressionLigne(ligne));
						break;
					}

				}

				AddToQueue( &queue, CreeActionVerification() );
				break;
			}

			// ---------------------------------------------------------------
			// - VERIFICATION ------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * - Il faut examiner la grille avec la gélatine en vue de vérifier s’il y
			 *   a encore de la gélatine
			 * - S’il y en a encore, on ne fait rien
			 * - S’il n’y en a plus alors il faut ajouter l’action FIN NIVEAU dans la
			 *   queue. Sinon, on place l’action LECTURE dans la Queue
			 */

			// consigne pas claire ...

			case VERIFICATION:
			{
				LOG(ACTION, "VERIFICATION");

				if (!VerifieCoups(&plateau))
				{
					printf("Vous avez perdu...\n");
					printf("Pressez une touche\n");
					(void)getchar();

					finJeu = true;
					break;
				}

				if (!VerifieJellies(&plateau))
				{
					printf("Bravo...\n");
					printf("Pressez une touche\n");
					(void) getchar();

					AddToQueue(&queue, CreeActionFinNiveau());
				}
				else
				{
					AddToQueue(&queue, CreeActionLecture());
				}

				break;
			}

			// ---------------------------------------------------------------
			// - SUPRESSION_V ------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * - Suppression des trois pions identifiés en position verticale
			 * - Suppression des éventuelles gélatines
			 * - Faire glisser les pions du dessus pour occuper les trois pions
			 *   supprimés
			 * - Pour les trois nouveaux pions à faire entrer, il faut une
			 *   génération aléatoire de pions.
			 * - Placer l’action CALCUL dans la queue
			 */

			case SUPRESSION_V:
			{
				LOG(ACTION, "SUPRESSION_V");

				ActionSupressionV* actionSupression = (ActionSupressionV*)action;

				int colonne = actionSupression->index;
				int ligneDebut = actionSupression->debut;
				int ligneFin = actionSupression->fin;
				LOG(ARGS, "c:%d %d-%d", colonne, ligneDebut, ligneFin);

				SuppressionVerticale( &plateau, colonne, ligneDebut, ligneFin );

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			// ---------------------------------------------------------------
			// - SUPRESSION_H ------------------------------------------------
			// ---------------------------------------------------------------

			/*
			 * - Suppression des trois pions identifiés en position horizontale
			 * - Suppression des éventuelles gélatines
			 * - Faire glisser les pions des trois colonnes du dessus pour occuper
			 *   les trois pions supprimés
			 * - Pour les trois nouveaux pions à faire entrer, il faut une
			 *   génération aléatoire de pions.
			 * - Placer l’action CALCUL dans la queue
			 */

			case SUPRESSION_H:
			{
				LOG(ACTION, "SUPRESSION_H");

				ActionSupressionH* actionSupression = (ActionSupressionH*)action;

				int ligne = actionSupression->index;
				int colonneDebut = actionSupression->debut;
				int colonneFin = actionSupression->fin;
				LOG(ARGS, "l:%d %d-%d", ligne, colonneDebut, colonneFin);

				SuppressionHorizontale( &plateau, ligne, colonneDebut, colonneFin );

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			// ---------------------------------------------------------------
			// - SUPRESSION_COLONNE ------------------------------------------
			// ---------------------------------------------------------------

			case SUPRESSION_COLONNE:
			{
				LOG(ACTION, "SUPRESSION_COLONNE");

				ActionSupressionColonne* actionSupression = (ActionSupressionColonne*)action;

				int colonne = actionSupression->index;
				LOG(ARGS, "c:%d", colonne );

				SuppressionColonne( &plateau, colonne );

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			// ---------------------------------------------------------------
			// - SUPRESSION_LIGNE --------------------------------------------
			// ---------------------------------------------------------------

			case SUPRESSION_LIGNE:
			{
				LOG(ACTION, "SUPRESSION_LIGNE");

				ActionSupressionLigne* actionSupression = (ActionSupressionLigne*)action;

				int ligne = actionSupression->index;
				LOG(ARGS, "l:%d", ligne);

				SuppressionLigne( &plateau, ligne );

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			// ---------------------------------------------------------------
			// - FIN_NIVEAU --------------------------------------------------
			// ---------------------------------------------------------------

			case FIN_NIVEAU:
			{
				LOG(ACTION, "FIN_NIVEAU");

				if (++niveau <= 3)
				{
					AddToQueue(&queue, CreeActionInitialize());
				}
				else
				{
					finJeu = true;
				}

				break;
			}

			// ---------------------------------------------------------------
			// - Ne devrait JAMAIS arriver... --------------------------------
			// ---------------------------------------------------------------

			default:
			{
				AfficheErreur("Action de type %d non gérée", action->type);

				LOG(ERROR_FATAL, "Action inconnue: %d", action->type); // noreturn
			}
		}

		free( action );
	}

	exit(0);
}
