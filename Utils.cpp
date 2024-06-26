#include "Utils.h"
#include "Log.h"

#include <windows.h>
#include <stdio.h>

// Simulate Alt-Enter input

bool MaximizeConsole( void )
{
	INPUT inputs[4] = {};
	ZeroMemory( inputs, sizeof( inputs ) );

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_MENU;					// Press Alt

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_RETURN;				// Press Enter

	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = VK_RETURN;				// Release Enter
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_MENU;					// Release Alt
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT uSent = SendInput( ARRAYSIZE( inputs ), inputs, sizeof( INPUT ) );
	if( uSent != ARRAYSIZE( inputs ) )
	{
		printf( "SendInput failed: 0x%x\n", HRESULT_FROM_WIN32( GetLastError() ) );
		return false;
	}

	// Delay to make sure console has maximized
	Sleep( 500 );

	return true;
}


//the function to read lines of variable length

int readLineFromConsole(char* line, long InCapacity )
{
	int ch;             // as getchar() returns `int`
	long capacity = InCapacity;  // capacity of the buffer
	long length = 0;    // maintains the length of the string
	char* temp = nullptr;  // use additional pointer to perform allocations in order to avoid memory leaks

	while (((ch = getchar()) != '\n') && (ch != EOF))
	{
		if ((length + 1) >= capacity)
		{
			// resetting capacity
			if (capacity == 0)
				capacity = 2; // some initial fixed length 
			else
				capacity *= 2; // double the size

			LOG(INFO, "Reallocation -> %d", capacity );

			// try reallocating the memory
			if ((temp = (char*)realloc(line, capacity * sizeof(char))) == NULL) //allocating memory
			{
				LOG(ERROR_FATAL, "Reallocation du buffer de chaine"); // noreturn
			}

			line = temp;
		}

		line[length++] = (char)ch; //type casting `int` to `char`
	}

	line[length] = '\0'; //inserting null character at the end

	if( temp != nullptr  )
	{
		LOG(INFO, "Reallocation %d -> %d", capacity, (length + 1));

		// remove additionally allocated memory
		if ((temp = (char*)realloc(line, (length + 1) * sizeof(char))) == NULL)
		{
			LOG(ERROR_FATAL, "Reallocation du buffer de chaine"); // noreturn
		}

		line = temp;
	}

	return length;
}
