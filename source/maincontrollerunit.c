/* ---------------------------------------------------------------------------------
 *          file : maincontrollerunit.c                                           *
 *   description : C-source file, implementation of the DotX MCU                  *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


//#include <windows.h>

#include "./signals/signal_definitions_internal.h"
#include "./signals/signal_definitions_custom.h"
#include "./signals/signal_definitions_external.h"
#include "./signals/signal_gui_commands.h"

#include "./suplib/suplib.h"

#include "./turbine/mcudata.h"

#ifdef _SUP
#include "./supervisor/supdata.h"
#include "./supervisor/supervisory.h"
#endif

#ifdef _SIM
#include "./simulation/evmdata.h"
#include "./simulation/simdata.h"
#include "./simulation/simulation.h"
#include "./simulation/eventsmodule.h"
#endif

#include "./basecontrol/basecontroller.h"
#include "./operation/operation.h"
#include "./logdata/logdata.h"

#include "./maincontrollerunit.h"


// Place the two line below where you want to access the global pointers.
//extern mcu_data_static   * g_MCUS;
//extern mcu_data_dynamic  * g_MCUD;
mcu_data_static  * g_MCUS = NULL;
mcu_data_dynamic * g_MCUD = NULL;

/* ---------------------------------------------------------------------------------
 Run the MCU to generate output based on the given measurement input
--------------------------------------------------------------------------------- */
int mcu_run( 

		const REAL   * pInputs        , /* [in]     Measurement input */
			  REAL   * pOutputs       , /* [out]    Controller output */
			  REAL   * pDebug         , /* [in/out] Debug array */
			  REAL   * pLogdata       , /* [in/out] Logdata array */
		const int      iStatus        , /* [in]     Simulation status, see defines */
		const int      iDevice        , /* [in]     System which is running the simulation */
			  char   * cMessage       , /* [in/out] System message */
			  char   * cRunname         /* [in/out] Optional runname definition (bladed only) */

) {

	int iError = MCU_OK, iSuperState = STATE_OFF;

	int k;

	/* Data structs for the MCU, note how they are declared static, which enables
    subsequent calls to use the same allocated memory */
	static mcu_data_static  * MCUS  ;
	static mcu_data_dynamic * MCUD  ;

#ifdef _SUP
	static sup_data_static  * SUPS  ;
	static sup_data_dynamic * SUPD  ;
#else
	void * SUPS, * SUPD;
#endif


#ifdef _SIM
	static sim_data_static  * SIMS  ;
	static sim_data_dynamic * SIMD  ;
	static evm_data_static  * EVMS  ;
	static evm_data_dynamic * EVMD  ;
#else
	void * SIMS, * SIMD, * EVMS, * EVMD ;
#endif

	/* Par file en simulation settings */
	char cParFile[200], cTmp2[200] ;
	static char cSimID[200];

	/* In all cases initialize the output to zero */
	for ( k = 0; k < MCU_NR_OUTPUTS; ++k ) pOutputs[k] = 0.0;

	/*-------------------------------------------------------------------------------------*/
	/* Initialize controller */

	if ( iStatus == MCU_STATUS_INIT )
	{

		/* Initialize all parameters and structs used by the controller */

		MCUS = init_mcudatastatic  ( pInputs        );
		MCUD = init_mcudatadynamic ( MCUS, pInputs  );
		g_MCUS = MCUS;
		g_MCUD = MCUD;

#ifdef _SUP
		SUPS = init_supdatastatic  ( pInputs        );
		SUPD = init_supdatadynamic ( SUPS, pInputs  );
#endif

#ifdef _EEC
		EECS = init_eecdatastatic  ( pInputs        );
		EECD = init_eecdatadynamic ( EECS, pInputs  );
#endif

#ifdef _SIM
		SIMS = init_simdatastatic  ( pInputs        );
		SIMD = init_simdatadynamic ( SIMS           );
		EVMS = init_evmdatastatic  ( pInputs        );
		EVMD = init_evmdatadynamic ( EVMS           );
#endif
		/* Read configuration in controller.ini */

		iError += readconfiguration ( iDevice, cSimID, cMessage, cRunname, MCUS, SUPS, SIMS, EVMS );
		/* Load parameter files */

		iError += mcu_readfile( cSimID, iDevice, cMessage, MCUS, MCUD );

#ifdef _SUP
		iError += sup_readfile( cSimID, iDevice, cMessage, SUPS, SUPD );
#endif

#ifdef _SIM
		iError += sim_readfile( cSimID, iDevice, cMessage, SIMS, SIMD );
		iError += evm_readfile( cSimID, iDevice, cMessage, EVMS, EVMD );
#endif

	}
	else {

		/* Empty message */
		strcpy( cMessage, "\0");

	} /* end of initialization */

	/*-------------------------------------------------------------------------------------*/
	/* Call the controller modules */

	/* Call the supervisory module */
#ifdef _SUP
	iError += supervisory ( pInputs, pOutputs, pLogdata, iStatus, MCUS, MCUD, SUPS, SUPD );
	iSuperState = SUPD->STATE_INTINDEX ;
#endif

	/* Call the operation module, note that without supervisory control, the state index always
        remains at its initialized value (=OFF) */
	iError += operation ( pInputs, pOutputs, pDebug, pLogdata, iStatus, cMessage, MCUS, MCUD, iSuperState );


	/* Call the supervisory module -> post procedure */
#ifdef _SUP
	iError += postprocedure ( pInputs, pOutputs, pLogdata, iStatus, SUPS, SUPD );
#endif

	/* Call the simulation module */
#ifdef _SIM
	iError += simulation ( pInputs, pOutputs, pLogdata, iStatus, MCUS, MCUD, SUPS, SUPD, SIMS, SIMD, EVMS, EVMD );
#endif


	/* Store all logged signals to the disc */
	iError += logdata ( pLogdata, iStatus, iDevice, cMessage, cSimID, MCUS->LogDir, MCUS->Log_ON );

	/* Exit the controller */

	if ( iStatus == MCU_STATUS_EXIT ) {

		strcat( cMessage, "\n" );

		/* Release allocated memory */
		int iErrorTMP = iError;
		iError += free_mcudatastatic(  MCUS );
		iError += free_mcudatadynamic( MCUD );

#ifdef _SUP
		iError += free_supdatastatic(  SUPS );
		iError += free_supdatadynamic( SUPD );
#endif

#ifdef _EEC
		iError += free_eecdatastatic(  EECS );
		iError += free_eecdatadynamic( EECD );
#endif

#ifdef _SIM
		iError += free_simdatastatic(  SIMS );
		iError += free_simdatadynamic( SIMD );
		iError += free_evmdatastatic(  EVMS );
		iError += free_evmdatadynamic( EVMD );
#endif

		if ( iError-iErrorTMP > MCU_OK )
			strcat( cMessage, "[mcu]  <err> Failed to release allocated memory\t\n" );
		else
			strcat( cMessage, "[mcu]  Allocated memory released\t\n" );

		strcat( cMessage, "\t\n" );

	}



	/* Cosmetic empty line in message */

	if ( iStatus == MCU_STATUS_INIT ) strcat( cMessage, "\t\n" );

	/* Report error code */

	return iError;

}

/* ---------------------------------------------------------------------------------
 end maincontrollerunit.c
--------------------------------------------------------------------------------- */






