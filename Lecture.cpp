
#include "Lecture.h"

#include <Windows.h>
#include <process.h>
#include <stdio.h>

//the function to read lines of variable length

int readLineFromConsole(char* line)
{
	int ch;             // as getchar() returns `int`
	long capacity = 0;  // capacity of the buffer
	long length = 0;    // maintains the length of the string
	char* temp = NULL;  // use additional pointer to perform allocations in order to avoid memory leaks

	while (((ch = getchar()) != '\n') && (ch != EOF))
	{
		if ((length + 1) >= capacity)
		{
			// resetting capacity
			if (capacity == 0)
				capacity = 2; // some initial fixed length 
			else
				capacity *= 2; // double the size

			// try reallocating the memory
			if ((temp = (char*)realloc(line, capacity * sizeof(char))) == NULL) //allocating memory
			{
				printf("ERROR: unsuccessful allocation");
				// return line; or you can exit
				exit(1);
			}

			line = temp;
		}

		line[length++] = (char)ch; //type casting `int` to `char`
	}

	line[length] = '\0'; //inserting null character at the end

	// remove additionally allocated memory
	if ((temp = (char*)realloc(line, (length + 1) * sizeof(char))) == NULL)
	{
		printf("ERROR: unsuccessful allocation");
		// return line; or you can exit
		exit(1);
	}

	line = temp;

	return length;
}

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

