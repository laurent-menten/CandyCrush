
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

// ****************************************************************************
// * Configuration ************************************************************
// ****************************************************************************

static ParametresNiveau niveaux[]
{
	//	colonnes	lignes	coups	jellies
	{	20,			20,		40,		10 },
	{	20,			20,		35,		15 },
	{	20,			20,		30,		20 },
};

// ****************************************************************************
// * Point d'entrée ***********************************************************
// ****************************************************************************

int main()
{
	ActionQueue queue;
	Plateau plateau;
	int niveau = 1;

	InitialiseLog( "log.txt" );

	// --- Console en plein écran (simule Alt-Enter) --------------------------

	MaximizeConsole();
	
	// --- Initialisation -----------------------------------------------------

	Log( INFO, "InitialiseJeu");
	InitialiseJeu(niveaux, (sizeof(niveaux) / sizeof(ParametresNiveau)));

	Log(INFO, "InitialiseQueue");
	InitializeQueue( &queue );

	AddToQueue( &queue, CreeActionInitialize() );

	// --- Boucle de jeu -------------------------------------------------------

	Action* action;

	bool finJeu = false;

	while( !finJeu && ((action = GetFromQueue( &queue )) != nullptr) )
	{
		switch( action->type )
		{
			/*
			 * Cette fonction va initialiser le début d’un nouveau niveau
			 * On ajoute l’action CALCUL dans la Queue afin de traiter les éventuelles
			 * séquences de pions qui seraient déjà existantes dans la Matrice.
			 */
			case INITIALIZATION:
			{
				Log(ACTION, "INITIALIZATION");

				if( InitialisePlateau( &plateau, niveau ) != 0 )
				{
					AfficheErreur( "Erreur de création du niveau %d", niveau );

					Log(ERROR_FATAL, "Erreur de création du niveau %d", niveau); // noreturn
					break;
				}

				AddToQueue( &queue, CreeActionAffichage() );
				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			/*
			 * Cette action a pour effet d’effacer l’écran et de redessiner les deux
			 * matrices :
			 *		- La matrice avec l’emplacement des pions. Chaque case contient la
			 *		  première lettre du pion qui occupe la case.
			 *		- La matrice avec l’emplacement de la gélatine.
			 */

			case AFFICHAGE:	// -------------------------------------------------
			{
				Log(ACTION, "AFFICHAGE");

				AffichePlateau( &plateau );

				AddToQueue( &queue, CreeActionCalcul() );

				break;
			}

			/*
			 * - Lecture des coordonnées X et Y des deux cases qui doivent être
			 *   interverties
			 * On ajoute l’action DEPLACEMENT dans la Queue.
			 */

			case LECTURE:
			{
				Log(ACTION, "LECTURE");

				int x1;
				int y1;
				bool origineValide = false;

				int x2;
				int y2;
				bool destinationValide = false;

				bool deplacementInvalide = false;

				do 
				{
					AffichePlateau(&plateau);

					if (deplacementInvalide)
					{
						AfficheAvertissement("Les coordonnées (%d,%d)-(%d,%d) ne sont pas contigues.\n", x1, y1, x2, y2);
					}

					if (origineValide)
					{
						printf("Origine: (%d, %d)\n", x1, y1);
					}
					else
					{
						LectureCoordonnees( "Origine [x y]: ", &x1, &y1);
						origineValide = VerifieCoordonnees(&plateau, x1, y1);
					}

					LectureCoordonnees( "Destination [x y]: ", &x2, &y2);
					destinationValide = VerifieCoordonnees(&plateau, x2, y2);
					if (destinationValide)
					{
						deplacementInvalide = ! VerifieDeplacement(&plateau, x1, y1, x2, y2);
						origineValide = destinationValide = !deplacementInvalide;
					}

				} while ( !(origineValide && destinationValide) );

				Log(ARGS, "(%d,%d) - (%d,%d)", x1, y1, x2, y2);

				AddToQueue( &queue, CreeActionDeplacement( x1, y1, x2, y2 ) );

				break;
			}

			/*
			 * Cette action va déclencher l’inversion des deux pions dont les
			 * coordonnées ont été introduites.
			 * La fonction va générer l’action CALCUL.
			 */
			case DEPLACEMENT:
			{
				Log(ACTION, "DEPLACEMENT");

				ActionDeplacement* actionDeplacement = (ActionDeplacement*)action;

				int x1 = actionDeplacement->x1;
				int y1 = actionDeplacement->y1;
				int x2 = actionDeplacement->x2;
				int y2 = actionDeplacement->y2;
				Log(ARGS, "(%d,%d) - (%d,%d)", x1, y1, x2, y2);

				Deplacement(&plateau, x1, y1, x2, y2);

				AddToQueue(&queue, CreeActionCalcul());


				break;
			}

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
				Log(ACTION, "CALCUL");

				AddToQueue( &queue, CreeActionLecture() );

				break;
			}

			/*
			 * - Il faut examiner la grille avec la gélatine en vue de vérifier s’il y
			 *   a encore de la gélatine
			 * - S’il y en a encore, on ne fait rien
			 * - S’il n’y en a plus alors il faut ajouter l’action FIN NIVEAU dans la
			 *   queue. Sinon, on place l’action LECTURE dans la Queue
			 */

			case VERIFICATION:
			{
				Log(ACTION, "VERIFICATION");

				break;
			}

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
				Log(ACTION, "SUPRESSION_V");

				ActionSupressionV* actionSupression = (ActionSupressionV*)action;

				int colonne = actionSupression->index;
				int ligneDebut = actionSupression->debut;
				int ligneFin = actionSupression->fin;
				Log(ARGS, "c:%d %d-%d", colonne, ligneDebut, ligneFin);

				SuppressionVerticale( &plateau, colonne, ligneDebut, ligneDebut );

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

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
				Log(ACTION, "SUPRESSION_H");

				ActionSupressionH* actionSupression = (ActionSupressionH*)action;

				int ligne = actionSupression->index;
				int colonneDebut = actionSupression->debut;
				int colonneFin = actionSupression->fin;
				Log(ARGS, "l:%d %d-%d", ligne, colonneDebut, colonneFin);

				SuppressionHorizontale( &plateau, ligne, colonneDebut, colonneFin );

				AddToQueue(&queue, CreeActionCalcul());

				break;
			}

			case SUPRESSION_COLONNE:
			{
				Log(ACTION, "SUPRESSION_COLONNE");

				ActionSupressionColonne* actionSupression = (ActionSupressionColonne*)action;

				int colonne = actionSupression->index;
				Log(ARGS, "c:%d", colonne );

				SuppressionColonne( &plateau, colonne );

				break;
			}

			case SUPRESSION_LIGNE:
			{
				Log(ACTION, "SUPRESSION_LIGNE");

				ActionSupressionLigne* actionSupression = (ActionSupressionLigne*)action;

				int ligne = actionSupression->index;
				Log(ARGS, "l:%d", ligne);

				SuppressionLigne( &plateau, ligne );

				break;
			}

			case FIN_NIVEAU:	// ---------------------------------------------
			{
				Log(ACTION, "FIN_NIVEAU");

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

			default:	// -----------------------------------------------------
			{
				Log(ERROR_SEVERE, "Action inconnue: %d", action->type);

				// Ne devrait normalement jamais arriver...

				AfficheErreur("Action de type %d non gérée", action->type);
			}
		}

		free( action );
	}

	FinaliseQueue(&queue);

	return 0;
}
