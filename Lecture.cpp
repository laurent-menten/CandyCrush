
#include "Lecture.h"
#include "Affichage.h"
#include "Utils.h"

#include <Windows.h>
#include <process.h>
#include <stdio.h>

/*
 * Fonction Lecture : fonction qui retourne les coordonnées ligne et colonne des deux
 * pions qui doivent être déplacés. 
 */
bool LectureCoordonnees(const char* prompt, int* l, int* c)
{
	// Note: malloc nécessaire pour la fonction readLineFromConsole().

	char* line = (char*)malloc(2);
	if (!line)
	{
		return false;
	}


	int inL = 0;
	int inC = 0;
	int rc;

	do 
	{
		printf( prompt );

		rc = readLineFromConsole(line);

		rc = sscanf_s( line, "%d %d", &inL, &inC);
		if (rc != 2)
		{
			AfficheAvertissement("Format d'entree invalide");
		}
	}
	while (rc != 2);

	*l = inL;
	*c = inC;

	free(line);

	return true;
}

