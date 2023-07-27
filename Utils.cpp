#include <Windows.h>
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

	return false;
}