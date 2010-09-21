#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include "crc16.h"

static int	isAscii;
static int	verbose;

static void
process(
	FILE *		fyle
)
{
	int		c;
	Crc16		crc;
	unsigned char	lsb;
	unsigned char	msb;

	crc16_init( &crc );
	if( isAscii )	{
		/* For ASCII input, collect hex character pairs		 */
		int	leftCh;
		int	rightCh;
		while( !feof( fyle ) )	{
			while( ((leftCh = fgetc( fyle )) != EOF ) &&
				!isxdigit( leftCh ) );
			if( leftCh == EOF )	{
				break;
			}
			leftCh = toupper( leftCh );
			while( ((rightCh = fgetc( fyle )) != EOF ) &&
				!isxdigit( rightCh ) );
			if( rightCh == EOF )	{
				break;
			}
			rightCh = toupper( rightCh );
			leftCh = isdigit( leftCh ) ? (leftCh - '0') :
				(leftCh - 'A' + 10);
			rightCh = isdigit( rightCh ) ? (rightCh - '0') :
				(rightCh - 'A' + 10);
			c = (leftCh << 4) | rightCh;
			crc16_process( (Crc16) c, &crc );
		}
	} else	{
		/* Assume it's a binary message image, less the CRC	 */
		while( (c = fgetc( fyle )) != EOF )	{
			if( verbose )	{
				printf( "%02X ", c & 0xFF );
			}
			crc16_process( (Crc16) c, &crc );
		}
	}
	crc16_parts( &crc, &lsb, &msb );
	printf( "%02X %02X\n",  lsb, msb );
}

int
main(
	int		argc,
	char * * 	argv
)
{
	int		c;
	int		nonfatal;

	nonfatal = 0;
	while( (c = getopt( argc, argv, "Dav" )) != EOF )	{
		switch( c )	{
		default:
			fprintf( stderr, "Huh?\n" );
			exit( 1 );
		case 'D':		/* More debug info!		 */
			break;
		case 'a':		/* Ascii hex input		 */
			++isAscii;
			break;
		case 'v':
			++verbose;	/* More chatter while working	 */
			break;
		}
	}
	if( argc > optind )	{
		int const	multi = ((argc - optind) > 1);
		while( argc > optind )	{
			FILE *	fyle;

			optarg = argv[ optind++ ];
			if( (fyle = fopen( optarg, "rb" )) == NULL )	{
				fprintf( stderr, "where's %s?\n", optarg );
				++nonfatal;
				continue;
			}
			if( multi )	{
				printf( "%s ", optarg );
			}
			process( fyle );
			fclose( fyle );
		}
	} else	{
		/* No args, use STDIN					 */
		process( stdin );
	}
	return( nonfatal ? 1 : 0 );
}
