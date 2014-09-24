#include "pch.h"
#include <strsafe.h>
#include "odprint.h"

void debugOut( LPCTSTR sFormat, ... )
{
    va_list argptr;      
    va_start( argptr, sFormat ); 
    TCHAR buffer[ 2000 ];
    HRESULT hr = StringCbVPrintf( buffer, sizeof( buffer ), sFormat, argptr );
    if ( STRSAFE_E_INSUFFICIENT_BUFFER == hr || S_OK == hr )
        OutputDebugString( buffer );
    else
		OutputDebugString( LPCWSTR("StringCbVPrintf error.") );
}