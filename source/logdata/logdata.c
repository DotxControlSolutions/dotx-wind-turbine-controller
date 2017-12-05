/* ---------------------------------------------------------------------------------
 *          file : logdata.c                                                      *
 *   description : C-source file, log functions for the MCU signals               *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./logdata.h"


/* ---------------------------------------------------------------------------------
    Main call of the log functionaility. 
--------------------------------------------------------------------------------- */
int logdata( 

		const REAL   * pData      ,
		const int      iStatus    ,
		const int      iDevice    ,
		char   * cMessage   ,
		const char   * cSimID     ,
		const char   * cOutputDir ,
		const int      iActive

) {
// 	JUAN'S VERSION OF LOGGING, EASIER TO USE, JUST ADD NEW VARIABLES TO SIGNALS_DEFINITIONS_EXTERNAL.H
//  IN THE SAME FORMAT AS THE OTHER VARIABLES IN X_NUMBERS, THEN THE CODE HANDLES EVERYTHING FOR YOU
//  NEW LOG FILE CAN BE OPENED IN EXCEL AS TAB SEPARATED AND ALSO IMPORTED EASILY IN MATLAB. ENJOY!
	int k, i;
	int iError = MCU_OK;

	static char cLog[200];
	static FILE * fidLog = NULL;

	// If not active, return
	if ( !iActive ) return iError;

	// If first call, create file path and name
	if (iStatus == MCU_STATUS_INIT)
	{
		strcpy( cLog, cOutputDir );
		if (iDevice != BLADED) strcat( cLog, cSimID );
		strcat( cLog, ".tsv" );
	}

	// If file not open, then open
	if ( fidLog == NULL  )
	{
		fidLog = fopen( cLog, "a+"); // TODO: [JGB] is "a+" OK? I think so.
	}

	// If file failed to open, return
	if ( fidLog == NULL  )
	{
		return MCU_ERR;
	}

	// If first call, create header
	if (iStatus == MCU_STATUS_INIT)
	{
		for ( k = 0; k < MAXLOG; ++k )
		{
			if ( dotxLogflags[k] == I_ENABLE )
			{
				fprintf( fidLog, "%s\t", tableNames[k] );
			}
		}
		fprintf( fidLog, "\n" );
		fflush(fidLog);
	}

	// Write log line
	for ( k = 0; k < MAXLOG; ++k )
	{
		if ( dotxLogflags[k] == I_ENABLE )
		{
			fprintf( fidLog, "%.7e\t", (REAL)pData[k] );
		}
	}
	fprintf( fidLog, "\n" );
	fflush(fidLog);

	// If exit, close file
	if (iStatus == MCU_STATUS_EXIT)
	{
		fclose( fidLog );
	}

	return iError;

} /* end logdata() */
