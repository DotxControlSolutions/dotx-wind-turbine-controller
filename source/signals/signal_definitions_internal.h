/* ---------------------------------------------------------------------------------
 *          file : signal_definitions_internal.h                                  *
 *   description : C-header file, internal signal definitions                     *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SIGNAL_DEFINITIONS_INTERNAL_H_
#define _SIGNAL_DEFINITIONS_INTERNAL_H_

//#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "./../suplib/debugger.h"

#ifdef _DOTX_GLOG_
	extern FILE * dotxfid;
#endif

/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleMCU
 *  @{*/
 
/*!
    \file signal_definitions_internal.h
    \brief Definitions used throughout the controller.
*/

#define MCU_VERSION          "DWTC[2.0.0]"  //!<    Version identifier
#define MCU_STATUS_INIT      1              //!<    If iStatus is equal to this value, initialization procedures should be performed.
#define MCU_STATUS_RUN       0              //!<    If iStatus is equal to this value, normal operation is ongoing.
#define MCU_STATUS_EXIT     -1              //!<    If iStatus is equal to this value, prepare to shutdown the controller.
#define MCU_OK               0              //!<    No error present
#define MCU_ERR              1              //!<    Error present (any value >0 is considered erroneous).

/*! \enum DEVICES
    Different devices that can run the mcu.
*/
enum DEVICES {

    MATLAB                              ,   //!<    MATLAB Simulation environment call through mcu() )
    BLADED                              ,   //!<    GH Bladed (call through DISCON() C-style)
    PHATAS                              ,   //!<    Focus/Phatas (call through _fullxs2control() )
    FLEX5                               ,   //!<    Flex5 (not implemented yet)
    FAST                                ,   //!<    FAST (call through DISCON() F90-style)
    PLC                                 ,   //!<    PLC (not implemented yet)
    DCS                                     //!<    DCS (not implemented yet)

} DEVICES ;

/*! \enum MCU_SIGNALS_IN
    Signal indexes of the pInputs array.
*/
enum MCU_SIGNALS_IN {

    I_MCU_IN_MEAS_GENSPEED              ,   //!<    Measured generator speed [rad/s]
    I_MCU_IN_MEAS_ROTORAZIANGLE         ,   //!<    Measured azimuth angle [rad]
    I_MCU_IN_MEAS_TOWFAACC              ,   //!<    Measured tower fore-aft acceleration [m/s^2]
    I_MCU_IN_MEAS_TOWSIDEACC            ,   //!<    Measured tower sidewards acceleration [m/s^2]
    I_MCU_IN_MEAS_ROOTOUTBENDM1         ,   //!<    Measured bending moment blade 1 [Nm] (blade numbering clockwise)
    I_MCU_IN_MEAS_ROOTOUTBENDM2         ,   //!<    Measured bending moment blade 2 [Nm] (blade numbering clockwise)
    //I_MCU_IN_MEAS_ROOTFLAPBENDM1         ,   //!<    Measured bending moment blade 1 [Nm] (blade numbering clockwise)
    //I_MCU_IN_MEAS_ROOTFLAPBENDM2         ,   //!<    Measured bending moment blade 2 [Nm] (blade numbering clockwise)
#if NR_BLADES == 3          
    I_MCU_IN_MEAS_ROOTOUTBENDM3         ,   //!<    Measured bending moment blade 3 [Nm] (blade numbering clockwise)
    //I_MCU_IN_MEAS_ROOTFLAPBENDM3         ,   //!<    Measured bending moment blade 3 [Nm] (blade numbering clockwise)
    #endif
    I_MCU_IN_MEAS_GENTORQUE             ,   //!<    Measured generator torque [Nm]
    I_MCU_IN_MEAS_PITCHANGLE1           ,   //!<    Measured pitch angle blade 1 [rad]  (blade numbering clockwise)
    I_MCU_IN_MEAS_PITCHANGLE2           ,   //!<    Measured pitch angle blade 2 [rad]  (blade numbering clockwise)
#if NR_BLADES == 3          
    I_MCU_IN_MEAS_PITCHANGLE3           ,   //!<    Measured pitch angle blade 3 [rad]  (blade numbering clockwise)
#endif          
    I_MCU_IN_CURRENTTIME                ,   //!<    Measured current time [s]
    I_MCU_IN_WINDSPEED                  ,   //!<    Measured wind speed at hubheight [m/s]
    I_MCU_IN_MEAS_ELECPOWEROUT          ,   //!<    Measured electrical power output [MW]
    I_MCU_IN_MEAS_YAWERROR              ,   //!<    Measured yaw error [rad]
    I_MCU_IN_TIMESTEP                   ,   //!<    Measured time step [s]
    I_MCU_IN_DEM_GRIDCONTACTOR          ,   //!<    Measured grid contactor [-]
    I_MCU_IN_ACTUATORTYPE               ,   //!<    Measured actuator type [-] (0 = pitch angle, 1 = pitch rate)


    I_MCU_IN_MEAS_ROOTINBENDM1			,	 //!<   Measured In-Plane Bending Moments Blade 1
    I_MCU_IN_MEAS_ROOTINBENDM2			,	 //!<   Measured In-Plane Bending Moments Blade 2
#if NR_BLADES == 3
    I_MCU_IN_MEAS_ROOTINBENDM3          ,	 //!<   Measured In-Plane Bending Moments Blade 3
#endif

    MCU_NR_INPUTS_STANDARD                  //!<    Total number of standard inputs

} MCU_SIGNALS_IN;

/*! \enum MCU_SIGNALS_OUT
    Signal indexes of the pOutputs array.
*/
enum MCU_SIGNALS_OUT {

    I_MCU_OUT_DEM_GENTORQUE             ,   //!<    Demanded generator torque [Nm] 
    I_MCU_OUT_DEM_PITCHANGLE1           ,   //!<    Demanded pitch angle/rate blade 1 [rad - rad/s]
    I_MCU_OUT_DEM_PITCHANGLE2           ,   //!<    Demanded pitch angle/rate blade 2 [rad - rad/s]
#if NR_BLADES == 3      
    I_MCU_OUT_DEM_PITCHANGLE3           ,   //!<    Demanded pitch angle/rate blade 3 [rad - rad/s]
#endif      
    I_MCU_OUT_DEM_YAWRATE               ,   //!<    Demanded yaw rate [rad/s]
    I_MCU_OUT_DEM_GRIDCONTACTOR         ,   //!<    Demanded grid contactor [-]
        
    MCU_NR_OUTPUTS_STANDARD                 //!<    Total number of standard outputs

} MCU_SIGNALS_OUT;

/*! \enum MCU_STEPRESPONSES
    Possibilities for stepresponses.
*/
enum MCU_STEPRESPONSES {

    MCU_STEP_NONE                       ,   //!<    No step response
    MCU_STEP_IPC1Pd                     ,   //!<    Step on D-component in IPC at 1P
    MCU_STEP_IPC1Pq                     ,   //!<    Step on Q-component in IPC at 1P
    MCU_STEP_IPC2Pd                     ,   //!<    Step on D-component in IPC at 2P
    MCU_STEP_IPC2Pq                     ,   //!<    Step on Q-component in IPC at 2P
	MCU_STEP_COLL						    //!<    Step ont he collective pitch angle
} MCU_STEPRESPONSES;
//!@}


/* ------------------------------------------------------------------------------ */
/** \addtogroup moduleDNPC DotX Nonlinear Model Predictive Control (DNPC)
 *  @{*/

#define DNPC_VERSION     "DNPC[2.0.0]@WIND" //!< Version identifier of DNPC
#define DNPC_INIT        1                  //!< If iStatus is equal to this value, initialization procedures should be performed.
#define DNPC_RUN         0                  //!< If iStatus is equal to this value, normal operation is ongoing.
#define DNPC_EXIT       -1                  //!< If iStatus is equal to this value, prepare to shutdown the controller.
#define DNPC_OK          0                  //!< No error present
#define DNPC_ERR         1                  //!< Error present (any value >0 is considered erroneous).

/*! \enum DNPC_SIGNALS_IN
    Signal indexes of the pInputs array. Note that inputs align with MCU.
*/
enum DNPC_SIGNALS_IN {

    I_DNPC_IN_GEN_SPEED                 ,   //!<    Measured generator speed [rad/s]
    I_DNPC_IN_AZ_ANGLE                  ,   //!<    Measured azimuth angle [rad]
    I_DNPC_IN_FOREAFT_ACC               ,   //!<    Measured tower fore-aft acceleration [m/s^2]
    I_DNPC_IN_SIDEWARDS_ACC             ,   //!<    Measured tower sidewards acceleration [m/s^2]
    I_DNPC_IN_BENDINGMOM_BL1            ,   //!<    Measured bending moment blade 1 [Nm] (blade numbering clockwise)
    I_DNPC_IN_BENDINGMOM_BL2            ,   //!<    Measured bending moment blade 2 [Nm] (blade numbering clockwise)
#if NR_BLADES == 3                          
    I_DNPC_IN_BENDINGMOM_BL3            ,   //!<    Measured bending moment blade 3 [Nm] (blade numbering clockwise)
#endif                                      
    I_DNPC_IN_GEN_TORQUE                ,   //!<    Measured generator torque [Nm]
    I_DNPC_IN_PITCH_BL1                 ,   //!<    Measured pitch angle blade 1 [rad]  (blade numbering clockwise)
    I_DNPC_IN_PITCH_BL2                 ,   //!<    Measured pitch angle blade 2 [rad]  (blade numbering clockwise)
#if NR_BLADES == 3                          
    I_DNPC_IN_PITCH_BL3                 ,   //!<    Measured pitch angle blade 3 [rad]  (blade numbering clockwise)
#endif                                      
    I_DNPC_IN_TIME                      ,   //!<    Measured current time [s]
    I_DNPC_IN_WIND_HUBHEIGHT            ,   //!<    Measured wind speed at hubheight [m/s]
    I_DNPC_IN_ELECPOWER                 ,   //!<    Measured electrical power output [MW]
    I_DNPC_IN_MEAS_YAWERROR             ,   //!<    Measured yaw error [rad]
    I_DNPC_IN_TIMESTEP                  ,   //!<    Measured time step [s]
    I_DNPC_IN_DEM_GRIDCONTACTOR         ,   //!<    Measured grid contactor [-]
    I_DNPC_IN_ACTUATORTYPE              ,   //!<    Measured actuator type [-] (0 = pitch angle, 1 = pitch rate)
    I_DNPC_IN_DEMTORQUE                 ,   //!<    Demanded generator torque from previous timestep [Nm]
    I_DNPC_IN_DEMPITCH_BL1              ,   //!<    Demanded pitch angle/rate blade 1 from previous timestep [rad - rad/s]
    I_DNPC_IN_DEMPITCH_BL2              ,   //!<    Demanded pitch angle/rate blade 2 from previous timestep [rad - rad/s]
#if NR_BLADES == 3                       
    I_DNPC_IN_DEMPITCH_BL3              ,   //!<    Demanded pitch angle/rate blade 3 from previous timestep [rad - rad/s]
#endif
    
    DNPC_NR_INPUTS                          //!<    Total number of inputs
    
} DNPC_SIGNALS_IN ;

/*! \enum DNPC_SIGNALS_OUT
    Signal indexes of the pOutputs array. Note that inputs allign with MCU.
*/
enum DNPC_SIGNALS_OUT {

    I_DNPC_OUT_GEN_TORQUE               ,   //!<    Demanded generator torque [Nm] 
    I_DNPC_OUT_PITCH1                   ,   //!<    Demanded pitch angle/rate blade 1 [rad - rad/s]
    I_DNPC_OUT_PITCH2                   ,   //!<    Demanded pitch angle/rate blade 2 [rad - rad/s]
#if NR_BLADES == 3                          
    I_DNPC_OUT_PITCH3                   ,   //!<    Demanded pitch angle/rate blade 3 [rad - rad/s]
#endif
    
    DNPC_NR_OUTPUTS                         //!<    Total number of outputs

} DNPC_SIGNALS_OUT ;

//!@}
/* ------------------------------------------------------------------------------ */


#define DOUBLEPRECISION                     //!<    Controller compiled in double precision
typedef double REAL;                        //!<    Definition of REAL as a double

/* [EAN 25/MAY/2013] Not yet operational */
//#define FLOATPRECISION                    //!<    Controller compiled in float precision
//typedef float REAL;                       //!<    Definition of REAL as a float

#define R_(x)           ((REAL)(x))
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#define ABS(a)          ((a) > R_(0.0)  ? (a) : -(a))
#define NINT(a)         ((a) >= R_(0.0) ? (int)((a)+R_(0.5)) : (int)((a)-R_(0.5)))
#define NINTF(a)        ((a) >= 0.0F ? (int)((a)+0.5F) : (int)((a)-0.5F))
#define deg2rad(X)      (M_PI/R_(180.0)*(X))
#define rad2deg(X)      (R_(180.0)/M_PI*(X))
#define rpm2rps(X)      (M_PI/R_(30.0)*(X))
#define rps2rpm(X)      (R_(30.0)/M_PI*(X))

// rate [unit/sec] and ts [sec] strictly positive (rate > 0 and ts > 0)
#define SRAMP(actual,desired,rate,ts)        ((desired) > (actual) ? (MIN((desired),((actual)+((ABS(ts))*(ABS(rate)))))) : (MAX((desired),((actual)-((ABS(ts))*(ABS(rate)))))))
#define D_DISABLE R_(0.0)
#define D_ENABLE R_(1.0)
#define I_DISABLE 0
#define I_ENABLE 1

#define TRUE            1                   //!<    Boolean identifier for "true"
#define FALSE           0                   //!<    Boolean identifier for "false"
#define MAX_SCHED_SIZE  30                  //!<    Maximum number of elements in ALL schedule-tables
#define FILENAMESIZE    256                 //!<    Maximum length of filenames
#define BUFSIZE         1024                //!<    Buffer size
#define EPS             1e-12               //!<    Smallest number for double comparison to zero, i.e. ABS(value) < EPS

#define SIGN(a)         ((a) / MAX(EPS,ABS((a)))  )


#define N_HPF_FILTERS 1                     //!<    Number of HPF in ALL filter sequences
#define N_NFP_FILTERS 12                    //!<    Number of NFP in ALL filter sequences
#define N_NFF_FILTERS 6                     //!<    Number of NFF in ALL filter sequences
#define N_LPF_FILTERS 2                     //!<    Number of LPF in ALL filter sequences
#define N_FILTERS  N_HPF_FILTERS+N_NFP_FILTERS+N_NFF_FILTERS+N_LPF_FILTERS  //!< Length of ALL filter sequences

#ifndef M_PI
    #define M_PI    R_(3.14159265358979323846)  //!<    Mathematical constant pi
#endif


#endif

/* ---------------------------------------------------------------------------------
 End _SIGNAL_DEFINITIONS_INTERNAL_H_
--------------------------------------------------------------------------------- */
