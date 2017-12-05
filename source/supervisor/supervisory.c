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
 Supervisory controller
--------------------------------------------------------------------------------- */
int supervisory (    

    const   REAL                * pInputs   , 
            REAL                * pOutputs  , 
            REAL                * pLogdata  , 
    const   int                   iStatus   , 
    const   mcu_data_static     * MCUS      , 
            mcu_data_dynamic    * MCUD      , 
    const   sup_data_static     * SUPS      , 
            sup_data_dynamic    * SUPD      

) {

    /* Local variables */
    int i, iError = MCU_OK;
  
    /* -------------------------------------------------------------------------- */
    /* SET TRIGGERS */
  
    iError += detect_triggers( pInputs, pOutputs, pLogdata, iStatus, 
                                    MCUS, MCUD, SUPS, SUPD );
                                    
    /* -------------------------------------------------------------------------- */
    /* DETERMINE STATE CHANGE */
    
    /* Initialize some temporary variables */
    int detected_trigger_index     = -1 ;
    int destination_state_index    = -1 ;
    int destination_state_priority =  0 ;
    
    /* Loop over all triggers to see which one could cause a state change. */
    for( i = 0; i < N_TRIGGERS; i++ ) {
    
        /*  - Check if a trigger is actived,  
            - Check if the trigger is on, and
            - Check if this trigger can be actived in this state. 
        */
        if( ( SUPD->TRIGGERLIST[i] > 0 ) && 
            ( SUPS->trigger_table[i].on ) &&
            ( SUPS->trigger_table[i].stateMask[ SUPD->STATE_INTINDEX ] > 0 ) 
          ) {
        
            /* Check to see if the state change is valid and more in portant than 
                    previously detected triggers. */
        
            /* First find the state index from the desired destination state id. */
            int j;
            int optional_state_index = -1;
            for( j = 0; j < N_STATES; j++ )
            { 
                if( SUPS->state_table[j].id == SUPS->trigger_table[i].stateMask[SUPD->STATE_INTINDEX] )
                {
                    optional_state_index = j;
                    break;
                }
            };
            
            /* If an index was found */
            if( optional_state_index >= 0 )
            {
                /* Now check the validity and if it has a higher priority over the previously desired state change. */
                if( SUPS->state_table[SUPD->STATE_INTINDEX].priorityTo[optional_state_index] > destination_state_priority )
                {
                    detected_trigger_index = i;
                    destination_state_index = optional_state_index;
                    destination_state_priority = SUPS->state_table[SUPD->STATE_INTINDEX].priorityTo[optional_state_index];
                }
            }
            else
            {
                ++iError;
            }
        }
    }
    
    /* If a valid trigger was found, change the state and update the trigger data. */
    if( detected_trigger_index >= 0  ) {
        SUPD->TRIGGER           = SUPS->trigger_table[ detected_trigger_index ].id;
        SUPD->TRIGGER_INTINDEX  = detected_trigger_index;        
        SUPD->STATE             = SUPS->state_table[ destination_state_index ].id;
        SUPD->STATE_INTINDEX    = destination_state_index;
    }
    
    /* -------------------------------------------------------------------------- */
    /* LOG DATA */
    
    pLogdata[ SUP_TRIGGER           ]   = SUPD->TRIGGER           ;
    pLogdata[ SUP_TRIGGER_INDEX     ]   = SUPD->TRIGGER_INTINDEX  ;
    pLogdata[ SUP_STATE             ]   = SUPD->STATE             ;
    pLogdata[ SUP_STATE_INDEX       ]   = SUPD->STATE_INTINDEX    ;
    pLogdata[ SUP_OVERPOWER         ]   = SUPS->OverpowerLimit    ; 
    pLogdata[ SUP_OVERSPEED         ]   = SUPS->OverspeedLimit    ;
    
    /* Report error */
    return iError;
} 

/* ---------------------------------------------------------------------------------
 end supervisory.c
--------------------------------------------------------------------------------- */
#endif