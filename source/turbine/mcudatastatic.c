/* ---------------------------------------------------------------------------------
 *          file : mcudatastatic.c                                                *
 *   description : C-source file, functions for the mcu_data_static struct        *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./mcudata.h"


/* ---------------------------------------------------------------------------------
 Constructor of mcu_data_static struct. Here all memory for data arrays needs to
 be allocated.
--------------------------------------------------------------------------------- */
mcu_data_static * init_mcudatastatic( const REAL * pInputs )
{

    /* Allocate memory for the struct */
    
    mcu_data_static * MCUS = (mcu_data_static*) calloc( 1, sizeof(mcu_data_static) );
    
    /* Set values from input array */
    
    MCUS->Ts = pInputs[ I_MCU_IN_TIMESTEP ] ;
    
    /* General settings */
    
    MCUS->Log_ON                    = 0 ;
    MCUS->iGB                       = R_(1.0);
    MCUS->ToptCoef                  = R_(1.0); 
    MCUS->Wmax                      = R_(1.0);
    MCUS->Wmin                      = R_(1.0);
    MCUS->OmgRat                    = R_(1.0);
    MCUS->Prat                      = R_(1.0);
    MCUS->Trat                      = R_(1.0);
    MCUS->DTdamp_ON                 = 0 ;
    MCUS->FAdamp_ON                 = 0 ;
    MCUS->Yaw_ON                    = 0 ;
    MCUS->PitFollow_ON              = 0 ;
    MCUS->StepResponse_Mode         = MCU_STEP_NONE;

    MCUS->pitchOffset[3]			    = R_(0.0);

    /* Return the pointer to the created struct */
    
    return MCUS;

} /* end of constructer for the mcu_data_static struct */


/* ---------------------------------------------------------------------------------
 Destructor of mcu_data_static struct. Here all memory for data arrays needs to
 be released.
--------------------------------------------------------------------------------- */
int free_mcudatastatic( mcu_data_static * MCUS )
{
    /* Release memory of members of the struct */
    
    /* The static struct defines not other variables besides standard types */

    /* Release memory of the struct itself */
    free( MCUS );
    
    return MCU_OK;
    
} /* end of destructor for the mcu_data_static struct */


/* ---------------------------------------------------------------------------------
 end mcudatastatic.c
--------------------------------------------------------------------------------- */
