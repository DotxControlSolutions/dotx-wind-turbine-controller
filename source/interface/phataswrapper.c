/* ---------------------------------------------------------------------------------
 *          file : phataswrapper.c                                                *
 *   description : C-source file, entry point of DLL                              *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
 *     reference : E.C.N. - D.A.J. Wouters, 23-04-2007, "Linking a controller to  *
 *                   Phatas" and "IO_list.pdf"                                    *
--------------------------------------------------------------------------------- */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>  

#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"
#include "./../signals/signal_definitions_external.h"

#include "./../maincontrollerunit.h"

#include "./phataswrapper.h"


/* ---------------------------------------------------------------------------------
 * Main entry point of the DLL for phatas. This function must rename this name.
--------------------------------------------------------------------------------- */

int __declspec(dllexport) _fullxs2control (

/* The first 'const' column states that the value at the address pointed to will
 * not be changed by this function (this is true for DLL inputs).
 * The second 'const' column -which is redundant- indicates that the pointer
 * itself will not be modified by this function (this is true for all arguments).
 */
          double        * const pdThdtset1,           /* pitch rate setpoint 1      [deg/s]     */
          double        * const pdThdtset2,           /* pitch rate setpoint 2      [deg/s]     */     
          double        * const pdThdtset3,           /* pitch rate setpoint 3      [deg/s]     */     
          double        * const pTeset,               /* gen torque setpoint        [Nm]        */     
          long          * const pBrakeCmd,            /* dummy - no longer used     [-]         */     
          long          * const pYawAct,              /* yaw actuation              [-]         */     
          double        * const pYawRate,             /* yaw rate                   [deg/s]     */     
          long          * const pCtrlId,              /* controller type ID         [-]         */     
          unsigned long * const pOutputFlags,         /* bitfield for out-flags     [-]         */     
    const double        * const pOmega,               /* measured rotor speed       [rad/s]     */     
    const double        * const pTh1,                 /* measured pitch angle 1     [deg]       */     
    const double        * const pTh2,                 /* measured pitch angle 2     [deg]       */     
    const double        * const pTh3,                 /* measured pitch angle 3     [deg]       */     
    const double        * const pPe,                  /* measured gen power         [MW]        */     
    const long          * const pEnable,              /* en-/disable controller     [-]         */     
    const double        * const pTowerTopAccelX,      /* for-aft tower top acc      [m/s^2]     */     
    const double        * const pTowerTopAccelY,      /* sideward tower top acc     [m/s^2]     */     
    const double        * const pYawAngle,            /* yaw angle                  [deg]       */     
    const double        * const pWindDir,             /* wind direction             [deg]       */     
    const double        * const pWindSpeed,           /* wind speed                 [m/s]       */     
    const double        * const pBlade1FlapMom,       /* bl root flapwise mom 1     [Nm]        */     
    const double        * const pBlade2FlapMom,       /* bl root flapwise mom 2     [Nm]        */     
    const double        * const pBlade3FlapMom,       /* bl root flapwise mom 3     [Nm]        */     
    const double        * const pBlade1LeadMom,       /* bl root leadwise mom 1     [Nm]        */     
    const double        * const pBlade2LeadMom,       /* bl root leadwise mom 2     [Nm]        */     
    const double        * const pBlade3LeadMom,       /* bl root leadwise mom 3     [Nm]        */     
    const double        * const pDriveShaftBendMomY,  /* dr-train sft ben mom Y     [Nm]        */     
    const double        * const pDriveShaftBendMomZ,  /* dr-train sft ben mom Z     [Nm]        */     
    const double        * const pYawBearMomX,         /* yaw bearing moment X       [Nm]        */     
    const double        * const pYawBearMomY,         /* yaw bearing moment Y       [Nm]        */     
    const double        * const pYawBearMomZ,         /* yaw bearing moment Z       [Nm]        */     
    const double        * const pAzimuth,             /* rotor azimuth              [deg]       */     
    const unsigned long * const pInputFlags,          /* bitfield for in-flags      [-]         */     
    const double        * const pt,                   /* simulation time            [s]         */     
    const double        * const pH,                   /* communication interval     [s]         */     
          long          * const pCallStatus,          /* flag for call-type         [-]         */     
          char          * const Message,              /* text for logging           [-]         */     
          double        * const dummy1,               /* dummy - future use         [-]         */     
          double        * const dummy2,               /* dummy - future use         [-]         */     
          double        * const dummy3,               /* dummy - future use         [-]         */
          double        * const dummy4                /* dummy - future use         [-]         */
    )
{
    /* Initialize */
    FILE * fid;
    int iStatus, iError;
    REAL pInputs[ MCU_NR_INPUTS ], pOutputs[ MCU_NR_OUTPUTS ], pDebug[ 20000 ];
    REAL pLogdata[ MAXLOG ] = { R_(0.0) };
    char cMessage[ 2000 ];
    
    /* Obtain the inputs from the correct Phatas Variables */
    pInputs[ I_MCU_IN_MEAS_GENSPEED       ] = (REAL)*pOmega              ;    
    pInputs[ I_MCU_IN_MEAS_ROTORAZIANGLE  ] = deg2rad( (REAL)*pAzimuth ) ; 
    pInputs[ I_MCU_IN_MEAS_TOWFAACC       ] = (REAL)*pTowerTopAccelX     ; 
    pInputs[ I_MCU_IN_MEAS_TOWSIDEACC     ] = (REAL)*pTowerTopAccelY     ;
    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM1  ] = -((REAL)*pBlade1FlapMom)   ;    
    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM2  ] = -((REAL)*pBlade2FlapMom)   ;
    
    #if NR_BLADES >= 3
    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM2  ] = -((REAL)*pBlade3FlapMom)   ;
    pInputs[ I_MCU_IN_MEAS_ROOTOUTBENDM3  ] = -((REAL)*pBlade2FlapMom)   ;
    #endif
    
    /* Generator torque unavailable, performing simple conversion 
     * without iGB (presumed iGB = 1) */
    pInputs[ I_MCU_IN_MEAS_GENTORQUE      ] = (REAL)*pPe * (REAL)1.0e6 / (REAL)*pOmega ; 
    pInputs[ I_MCU_IN_MEAS_PITCHANGLE1    ] = deg2rad((REAL)*pTh1)       ;
    pInputs[ I_MCU_IN_MEAS_PITCHANGLE2    ] = deg2rad((REAL)*pTh2)       ;
    
    /* Note the reversed order for phatas */
    #if NR_BLADES >= 3
    pInputs[ I_MCU_IN_MEAS_PITCHANGLE2    ] = deg2rad((REAL)*pTh3)       ;
    pInputs[ I_MCU_IN_MEAS_PITCHANGLE3    ] = deg2rad((REAL)*pTh2)       ;
    #endif
    
    pInputs[ I_MCU_IN_CURRENTTIME         ] = (REAL)*pt                  ;
    pInputs[ I_MCU_IN_MEAS_ELECPOWEROUT   ] = (REAL)*pPe                 ;
    pInputs[ I_MCU_IN_MEAS_YAWERROR       ] = deg2rad((REAL)*pWindDir-(REAL)*pYawAngle) ;
    pInputs[ I_MCU_IN_TIMESTEP            ] = (REAL)*pH                  ;
    pInputs[ I_MCU_IN_DEM_GRIDCONTACTOR   ] = (REAL)1.0                  ;
    
    /* Phatas only handles pitch rate */ 
    pInputs[ I_MCU_IN_ACTUATORTYPE        ] = (REAL)1.0                  ;
    pInputs[ I_MCU_IN_WINDSPEED           ] = (REAL)*pWindSpeed             ;
    
    /* The pCallStatus will indicate whether this is the first call (initialize), 
     * last call (terminate) or normal call (compute update) */
    if      ( *pCallStatus > 0 ) iStatus = MCU_STATUS_INIT ;
    else if ( *pCallStatus < 0 ) iStatus = MCU_STATUS_EXIT ;
    else                         iStatus = MCU_STATUS_RUN  ;
    
    /* Set configuration file */
    strcpy( cMessage, "./controller.ini" );
    
    /* Call the Main Controller Unit (MCU) */
    iError = mcu_run( pInputs, pOutputs, pDebug, pLogdata, iStatus, PHATAS, cMessage, NULL );
    
//    // Test if DNPC can run without load on the generator (for Hempenius tests)
//    pOutputs[ I_MCU_OUT_DEM_GENTORQUE   ] = 0.0;

    /* Return the outputs to Phatas */
    *pdThdtset1 = rad2deg( (double)pOutputs[ I_MCU_OUT_DEM_PITCHANGLE1 ] ); 
    *pdThdtset2 = rad2deg( (double)pOutputs[ I_MCU_OUT_DEM_PITCHANGLE2 ] );
    
    /* Note the reversed order for phatas */
    #if NR_BLADES >= 3
    *pdThdtset3 = rad2deg( (double)pOutputs[ I_MCU_OUT_DEM_PITCHANGLE2 ] );
    *pdThdtset2 = rad2deg( (double)pOutputs[ I_MCU_OUT_DEM_PITCHANGLE3 ] ); 
    #endif 
    
    /* Phatas needs gross torque */
    *pTeset     =          (double)pOutputs[ I_MCU_OUT_DEM_GENTORQUE   ]   ; 
    *pYawRate   = rad2deg( (double)pOutputs[ I_MCU_OUT_DEM_YAWRATE     ] ) ;
    
    /* Phatas whatchdog */
    *pCtrlId      =     -1;
    *pOutputFlags =    0.0;
    
    /* Try not to get empty lines over the screen. */
    sprintf( Message, "\0" );
    
    /* Directly print message to screen (only works in phatas) */
    printf( cMessage );
    
    /* Return value not used in current Phatas versions */    
    *pCallStatus = -(iError);
    return -(iError);
}

/* ---------------------------------------------------------------------------------
 * End of phatascontroller.c
--------------------------------------------------------------------------------- */
