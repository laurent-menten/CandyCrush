
#include "Lecture.h"
#include "Utils.h"

#include <Windows.h>
#include <process.h>
#include <stdio.h>

/*
 * Fonction Lecture : fonction qui retourne les coordonnées X et Y des deux
 * pions qui doivent être déplacés. 
 */
bool LectureCoordonnees( const char* prompt, int* x, int* y)
{
	char* line = (char*)malloc(100);

	int inX = 0;
	int inY = 0;
	int rc;

	do 
	{
		printf( prompt );

		rc = readLineFromConsole(line);

		rc = sscanf_s( line, "%d %d", &inX, &inY);
		if (rc != 2)
		{
			printf("erreur...\n");
		}
	} while (rc != 2);

	*x = inX;
	*y = inY;

	return true;
}

