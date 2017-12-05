/* ---------------------------------------------------------------------------------
 *          file : supervisory.c                                                  *
 *   description : C-source file, functions for reading the supervisory           *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"
#include "./../signals/signal_definitions_external.h"
#include "./../signals/signal_gui_commands.h"

#include "./../suplib/suplib.h"

#include "./../turbine/mcudata.h"

#include "./supdata.h"

#include "./../maincontrollerunit.h"

#include "./supervisory.h"


#ifdef _SUP
/* ---------------------------------------------------------------------------------
 A function which holds the IF statements of the different triggers.
--------------------------------------------------------------------------------- */
int detect_triggers (     

    const   REAL                * pInputs   , 
            REAL                * pOutputs  , 
            REAL                * pLogdata  , 
    const   int                   iStatus   , 
    const   mcu_data_static     * MCUS      , 
            mcu_data_dynamic    * MCUD      , 
    const   sup_data_static     * SUPS      , 
            sup_data_dynamic    * SUPD      
            
)  {
    
    /* Local variables */
    int i, iError = MCU_OK;
    
    /* Clear the previously detected triggers. */
    for( i = 0; i < N_TRIGGERS; i++ ) SUPD->TRIGGERLIST[i] = 0;
    
    /* Controller hasnt settled yet */
    if ( pInputs[ I_MCU_IN_CURRENTTIME ] > SUPS->SettlingTime )
        SUPD->TRIGGERLIST[ TRIGGER_SUP_ON  ] = 1 ;
    else
        SUPD->TRIGGERLIST[ TRIGGER_SUP_OFF ] = 1 ;
    
    /* Detect overpower */
    if ( pInputs[ I_MCU_IN_MEAS_ELECPOWEROUT ] > (SUPS->OverpowerLimit / 1e6) )
        SUPD->TRIGGERLIST[ TRIGGER_OVERPOWER ] = 1;

    /* Detect overspeed */
    if ( pInputs[ I_MCU_IN_MEAS_GENSPEED ] > SUPS->OverspeedLimit )
        SUPD->TRIGGERLIST[ TRIGGER_OVERSPEED ] = 1;
   
    /* Detect extreme event */
    if ( SUPD->EEC_flag )
        SUPD->TRIGGERLIST[ TRIGGER_EXTREMEEVENT ] = 1;
   
    /* Detect grid-loss */
    if ( pInputs[ I_MCU_IN_MEAS_ELECPOWEROUT ] < EPS ) 
        SUPD->TRIGGERLIST[ TRIGGER_GRIDLOSS ] = 1;
    
    return iError;
    
} /* End detect_triggers() */

#endif
    