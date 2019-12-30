#include "516_input.h"

#define DELAY(X) {unsigned int _i = X; while(_i--);}


/*
description:	get the pressed key and stores the result into the given variable
				and return the result of the read reaction.
parameters:		record <unsigned char *>:	the address of the variable to store the
											key code;
return:			0 for no key pressed, 1 for key pressed.


*/
int get_key( unsigned char * record  )
{
	unsigned char counter;
	unsigned char characters[6] = {'.', 'C', '+', '-', '*', '/'};
	// matrix key detection
	MATRIX_PIN = 0x0f;
	if ( MATRIX_PIN != 0x0f )
	{
		DELAY( 1000 );
		// check again to avoid the shake
		if ( MATRIX_PIN != 0x0f )
		{
			// inits the counter variable with 0 for later use
			counter = 0;
			MATRIX_PIN = 0x0f;
			switch( MATRIX_PIN )
			{
				case( 0x07 ):
					*record = 0;
					break;
				case( 0x0b ):
					*record = 1;
					break;
				case( 0x0d ):
					*record = 2;
					break;
				case( 0x0e ):
					*record = 3;
					break;
				default:
					// ignore the others
					return 0;
			}
			
			MATRIX_PIN = 0xf0;
			switch( MATRIX_PIN )
			{
				case( 0x70 ):
					// *record = *record;
					break;
				case( 0xb0 ):
					*record += 4;
					break;
				case( 0xd0 ):
					*record += 8;
					break;
				case( 0xe0 ):
					*record += 12;
					break;
				default:
					// ignore the others
					return 0;
			}

			// ignore the release action ( 
			while ( MATRIX_PIN != 0xf0);
			
			if ( *record < 10 )
				*record += '0';
			else
				*record = characters[*record % 10];
			// notify success
			return 1;
		}
	}
	
	// detect the separate keys
	if ( K1 == 0)
	{
		DELAY( 1000 );
		if ( K1 == 0 )
		{
			*record = '(';
			while ( !K1 );
			return 1;
		}	
	}

	if ( K2 == 0)
	{
		DELAY( 1000 );
		if ( K2 == 0 )
		{
			*record = ')';
			while ( !K2 );
			return 1;
		}	
	}

	if ( K3 == 0)
	{
		DELAY( 1000 );
		if ( K3 == 0 )
		{
			*record = '$';
			while ( !K3 );
			return 1;
		}	
	}

	if ( K4 == 0)
	{
		DELAY( 1000 );
		if ( K4 == 0 )
		{
			*record = '=';
			while ( !K4 );
			return 1;
		}	
	}
	
	if ( K5 == 0)
	{
		DELAY( 1000 );
		if ( K5 == 0 )
		{
			*record = 'u';
			while ( !K5 );
			return 1;
		}	
	}


	/*
	for (counter = 0; counter < 8; counter++)
	{
		SEPARATE_PIN = 0xff;
		if ( SEPARATE_PIN^counter == 0 )
		{
			DELAY( 1000 );
			if ( SEPARATE_PIN^counter == 0 )
			{
				*record = 16 + counter;
				while ( !SEPARATE_PIN^counter );

				// notify success
				return 1;
			}
		}
	}
	
	*/

	// notify nothing found
	return 0;
}