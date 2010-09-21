#ifndef	crc16_h
#define	crc16_h

typedef unsigned short	Crc16;

static void inline
crc16_init(
	Crc16 * const	val
)
{
	*val = 0xFFFF;
}

static void inline
crc16_process(
	Crc16		data,
	Crc16 * const	crc
)
{
	Crc16		k;

	*crc ^= data;
	for(k=0; k<8; k++) {
		if(*crc%2) { 
			*crc >>= 1; 
			*crc ^= 0xA001; 
		} else {
			*crc >>= 1;
		}
	}
}

static void inline
crc16_parts(
	Crc16 const * const	crc,
	unsigned char * const	lsb,
	unsigned char * const	msb
)
{
	*lsb = (*crc >> 0) & 0xFF;
	*msb = (*crc >> 8) & 0xFF;
}

#endif	/* crc16_h */
