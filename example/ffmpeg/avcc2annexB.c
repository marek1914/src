#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char				uint8_t;
typedef unsigned int 				uint16_t;
typedef unsigned int 				uint32_t;

int	H264ConvertAVCCtoAnnexBHeader( 
		const uint8_t *		inAVCCPtr,
		size_t				inAVCCLen,
		uint8_t *			inHeaderBuf,
		size_t				inHeaderMaxLen,
		size_t *			outHeaderLen,
		size_t *			outNALSize,
		const uint8_t **	outNext )
{
	const uint8_t *				src = inAVCCPtr;
	const uint8_t * const		end = src + inAVCCLen;
	uint8_t * const				buf = inHeaderBuf;
	uint8_t *					dst = buf;
	const uint8_t * const		lim = dst + inHeaderMaxLen;
	int					err;
	size_t						nalSize;
	int							i, n;
	uint16_t					len;
	
	// AVCC Format is documented in ISO/IEC STANDARD 14496-15 (AVC file format) section 5.2.4.1.1.
	// [0x00] version = 1.
	// [0x01] AVCProfileIndication		Profile code as defined in ISO/IEC 14496-10.
	// [0x02] Profile Compatibility		Byte between profile_IDC and level_IDC in SPS from ISO/IEC 14496-10.
	// [0x03] AVCLevelIndication		Level code as defined in ISO/IEC 14496-10.
	// [0x04] LengthSizeMinusOne		0b111111xx where xx is 0, 1, or 3 mapping to 1, 2, or 4 bytes for nal_size.
	// [0x05] SPSCount					0b111xxxxx where xxxxx is the number of SPS entries that follow this field.
	// [0x06] SPS entries				Variable-length SPS array. Each entry has the following structure:
	//		uint16_t	spsLen			Number of bytes in the SPS data until the next entry or the end (big endian).
	//		uint8_t		spsData			SPS entry data.
	// [0xnn] uint8_t	PPSCount		Number of Picture Parameter Sets (PPS) that follow this field.
	// [0xnn] PPS entries				Variable-length PPS array. Each entry has the following structure:
	//		uint16_t	ppsLen			Number of bytes in the PPS data until the next entry or the end (big endian).
	//		uint8_t		ppsData			PPS entry data.
	//
	// Annex-B format is documented in the H.264 spec in Annex-B.
	// Each NAL unit is prefixed with 0x00 0x00 0x00 0x01 and the nal_size from the AVCC is removed.
	
	// Write the SPS NAL units.
	
	nalSize	= ( src[ 4 ] & 0x03 ) + 1;
	n		=   src[ 5 ] & 0x1F;
	src		=  &src[ 6 ];
	for( i = 0; i < n; ++i )
	{
		len = (uint16_t)( ( src[ 0 ] << 8 ) | src[ 1 ] );
		src += 2;
		
		if( inHeaderBuf )
		{
			dst[ 0 ] = 0x00;
			dst[ 1 ] = 0x00;
			dst[ 2 ] = 0x00;
			dst[ 3 ] = 0x01;
			memcpy( dst + 4, src, len );
		}
		src += len;
		dst += ( 4 + len );
	}
	
	// Write PPS NAL units.
	
	if( ( end - src ) >= 1 )
	{
		n = *src++;
		for( i = 0; i < n; ++i )
		{
			len = (uint16_t)( ( src[ 0 ] << 8 ) | src[ 1 ] );
			src += 2;
			
			if( inHeaderBuf )
			{
				dst[ 0 ] = 0x00;
				dst[ 1 ] = 0x00;
				dst[ 2 ] = 0x00;
				dst[ 3 ] = 0x01;
				memcpy( dst + 4, src, len );
			}
			src += len;
			dst += ( 4 + len );
		}
	}
	
	if( outHeaderLen )	*outHeaderLen	= (size_t)( dst - buf );
	if( outNALSize )	*outNALSize		= nalSize;
	if( outNext )		*outNext		= src;
	err = 0;
	
exit:
	return( err );
}

int H264GetNextNALUnit( 
		const uint8_t *		inSrc, 
		const uint8_t *		inEnd, 
		size_t				inNALSize, 
		const uint8_t * *	outDataPtr, 
		size_t *			outDataLen,
		const uint8_t **	outSrc )
{
	int		err;
	size_t			len;
	
	switch( inNALSize )
	{
		case 1:
			len = *inSrc++;
			break;
		
		case 2:
			len  = ( *inSrc++ << 8 );
			len |=   *inSrc++;
			break;
		
		case 4:
			len  = ( *inSrc++ << 24 );
			len |= ( *inSrc++ << 16 );
			len |= ( *inSrc++ <<  8 );
			len |=   *inSrc++;
			break;
		
	}
	
	*outDataPtr = inSrc;
	*outDataLen = len;
	*outSrc		= inSrc + len;
	err = 0;
	
exit:
	return( err );
}


int main(int argc, char *argv[])
{
	int ExtraData_len;	
	uint8_t *ExtraData;
	int AVCC_Len;
	uint8_t *AVCC_Data;
	FILE * outputFile;

	int annexBHeaderLen;
	uint8_t *annexBHeaderPtr;
	size_t 				nalSize;

	outputFile = fopen( "/data/annexB.264", "wb" );
	
//head	
	H264ConvertAVCCtoAnnexBHeader( ExtraData, ExtraData_len, NULL, 0, &annexBHeaderLen, NULL, NULL );
	annexBHeaderPtr = (uint8_t *) malloc( annexBHeaderLen );
	H264ConvertAVCCtoAnnexBHeader( ExtraData, ExtraData_len, annexBHeaderPtr, annexBHeaderLen, &annexBHeaderLen, 
				&nalSize, NULL );

//write the head
	fwrite( annexBHeaderPtr, 1, annexBHeaderLen, outputFile);

//data
	const uint8_t *		nalPtr;
	size_t				nalLen;
	uint8_t *			src;
	uint8_t *			end;

	src = AVCC_Data;
	end = src + AVCC_Len;

	while( H264GetNextNALUnit( src, end, 4, &nalPtr, &nalLen, &src) == 0 )
	{
		//write 0x00, 0x00, 0x00, 0x01
		fwrite( "\x00\x00\x00\x01", 1, 4, outputFile );
		//write data
		fwrite( nalPtr, 1, nalLen, outputFile );
	}

	fclose(outputFile);
	return 0;
}


