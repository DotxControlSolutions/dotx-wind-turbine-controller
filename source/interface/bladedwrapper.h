/* ---------------------------------------------------------------------------------
 *          file : bladedwrapper.h                                                *
 *   description : C-header file, contains bladed wrapper definitions             *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _BLADEDWRAPPER_H_
#define _BLADEDWRAPPER_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup entrypoints 
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES

/*! \file bladedwrapper.h
    \brief This file holds the entry point for GH Bladed.
*/
#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_TYPES

#define BUFSIZE              1024   //!< Size of the Bladed message string.

//! \name Indices for the bladed in and output data array.
//@{
#define BLD_StatusFlag          0   //!< Index of Bladed Status Flag
#define BLD_CurrentTime         1   //!< Index of Current Simulation Time
#define BLD_TimeStep            2   //!< Index of Communication Interval
#define BLD_TimeOutput        117   //!< Index of Time to Start Writing Output
                                         
#define BLD_MinPitchAngle       5   //!< Index of Minimum Allowable Pitch Angle
#define BLD_MaxPitchAngle       6   //!< Index of Maximum Allowable Pitch Angle
#define BLD_MinPitchRate        7   //!< Index of Minimum Allowable Pitch Rate
#define BLD_MaxPitchRate        8   //!< Index of Maximum Allowable Pitch Rate
                                         
#define BLD_OptModeGain        15   //!< Index of Optimal Mode Gain
#define BLD_MinGenSpd          16   //!< Index of Minimum Generator Speed
#define BLD_OptModeMaxSpeed    17   //!< Index of Optimal Mode Maximum Speed
                                         
#define BLD_ActuatorType        9   //!< Index of Pitch Postion or Pitch Rate Actuator
#define BLD_ControllerType     27   //!< Index of Individual or Collective Pitch Control
                                         
#define BLD_ShaftBrakeStatus   35   //!< Index of Shaft Brake Status
                                         
#define BLD_StTorqSpeedTable   24   //!< Index of Start Torque-Speed Look-up Table
#define BLD_NrTorqSpeedTable   25   //!< Index of Nr Points on Torque-Speed Look-up Table
#define BLD_NrBlades           60   //!< Index of Number of Blades
                                         
#define BLD_DemGenSpdAbRat     18   //!< Index of Demanded Genarator Speed Above Rated
#define BLD_DemGenTorqAbRat    21   //!< Index of Demanded Genarator Torque Above Rated
#define BLD_DemGridContactor   34   //!< Index of Demanded Grid Contractor  //SDG
                                         
#define BLD_MeaPitchAngle1      3   //!< Index of Measured Pitch Angle Blade 1
#define BLD_MeaPitchAngle2     32   //!< Index of Measured Pitch Angle Blade 2
#define BLD_MeaPitchAngle3     33   //!< Index of Measured Pitch Angle Blade 3
#define BLD_MeaElecPowerOut    14   //!< Index of Measured Electrical Power Output
#define BLD_MeaGenSpd          19   //!< Index of Measured Generator Speed
#define BLD_MeaRotorSpeed      20   //!< Index of Measured Rotor Speed
#define BLD_MeaGenTorque       22   //!< Index of Measured Generator Torque
#define BLD_MeaYawError        23   //!< Index of Measured Yaw Error
                                         
#define BLD_MeaRootOutBendM1   29   //!< Index of Root Out of Plane Bend Moment Blade 1
#define BLD_MeaRootOutBendM2   30   //!< Index of Root Out of Plane Bend Moment Blade 2
#define BLD_MeaRootOutBendM3   31   //!< Index of Root Out of Plane Bend Moment Blade 3

#define BLD_MeaRootInBendM1   68   //!< Index of In Plane Bend Moment Blade 1
#define BLD_MeaRootInBendM2   69   //!< Index of In Plane Bend Moment Blade 2
#define BLD_MeaRootInBendM3   70   //!< Index of In Plane Bend Moment Blade 3



#define BLD_MeaTowForeAftAcc   52   //!< Index of Tower Top Fore-Aft Acceleration
#define BLD_MeaTowSideAcc      53   //!< Index of Tower Top Sidewards Acceleration
#define BLD_MeaRotorAziAngle   59   //!< Index of Rotor Azimuth Angle
                                                      
#define BLD_HubWindSpeed       26   //!< Index of Hub Wind Speed
                                         
#define BLD_DemPitch1          41   //!< Index of Demanded Pitch Angle or Rate Blade 1
#define BLD_DemPitch2          42   //!< Index of Demanded Pitch Angle or Rate Blade 2
#define BLD_DemPitch3          43   //!< Index of Demanded Pitch Angle or Rate Blade 3
#define BLD_DemPitchAngleCol   44   //!< Index of Demanded Pitch Angle Collective
#define BLD_DemPitchRateCol    45   //!< Index of Demanded Pitch Rate Collective
#define BLD_DemGenTorque       46   //!< Index of Demanded Generator Torque
                                         
#define BLD_DemYawActTorq      40   //!< Index of Demanded yaw actuator torque
#define BLD_DemNacelleYawRate  47   //!< Index of Demanded Nacelle Yaw Rate
                                         
#define BLD_YawContrType       28   //!< Index of Yaw Control 
#define BLD_YawContrFlag      101   //!< Index of Yaw Control Flag
                                         
#define BLD_MaxCharMessage     48   //!< Index of Max Characters in "Message" argument
#define BLD_NrCharInFile       49   //!< Index of Nr of Characters in "InFile" argument
#define BLD_NrCharOutName      50   //!< Index of Nr of Characters in "OutName" argument
#define BLD_MaxCharOutName     63   //!< Index of Max Characters in "OutName" argument
                                         
#define BLD_LogMaxVariables    61   //!< Index of Max Number of Variables for Logging
#define BLD_LogNrVariables     64   //!< Index of Nr Variables Returned for Logging
#define BLD_LogFirstRecNr      62   //!< Index of Record Number for Logging First Var
                                         
//@}

#endif

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! The entry point off controller. 
/*! For GH Bladed it is required that the access function is DISCON().
*/
void __declspec(dllexport) __cdecl DISCON( float*, int*, char*, char*, char* );

/* Automatically generated function (from MATLAB script genconfig.m). Sets signals definitions for GH Bladed. */
int bladedlogsignals ( float * pBladedData, char * cOutname, const REAL * pLogdata, const int iLogIndex, const int iLogNrVariables  );

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif  

/* ---------------------------------------------------------------------------------
 End _BLADEDWRAPPER_H_
--------------------------------------------------------------------------------- */
