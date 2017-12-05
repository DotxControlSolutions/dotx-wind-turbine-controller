/* Automatically generated file */

#include <string.h>

#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"
#include "./../maincontrollerunit.h"
#include "./bladedwrapper.h"

int bladedlogsignals ( float * pBladedData, char * cOutName, const REAL * pLogdata, const int iLogIndex, const int iLogNrVariables  ) { 

    pBladedData[ iLogIndex +   0 ] = 0.0; 
    pBladedData[ iLogIndex +   1 ] = (float)pLogdata[ BASE_SPD_RTSP ]; 
    pBladedData[ iLogIndex +   2 ] = (float)pLogdata[ BASE_SPD_RTSP_P_LPF ]; 
    pBladedData[ iLogIndex +   3 ] = (float)pLogdata[ BASE_SPD_RTSP_T_LPF ]; 
    pBladedData[ iLogIndex +   4 ] = (float)pLogdata[ BASE_SPD_TORQ_SETPOINT ]; 
    pBladedData[ iLogIndex +   5 ] = (float)pLogdata[ BASE_SPD_PIT_MIN ]; 
    pBladedData[ iLogIndex +   6 ] = (float)pLogdata[ BASE_SPD_PIT_MAX ]; 
    pBladedData[ iLogIndex +   7 ] = (float)pLogdata[ BASE_SPD_PIT_MIN_SPD ]; 
    pBladedData[ iLogIndex +   8 ] = (float)pLogdata[ BASE_SPD_PIT_MAX_SPD ]; 
    pBladedData[ iLogIndex +   9 ] = (float)pLogdata[ BASE_SPD_PIT_SETPOINT ]; 
    pBladedData[ iLogIndex +  10 ] = (float)pLogdata[ BASE_SPD_DEM_TORQ ]; 
    pBladedData[ iLogIndex +  11 ] = (float)pLogdata[ BASE_SPD_DEM_PITCH ]; 
    pBladedData[ iLogIndex +  12 ] = (float)pLogdata[ BASE_DTD_OMR ]; 
    pBladedData[ iLogIndex +  13 ] = (float)pLogdata[ BASE_DTD_OMR_LPF ]; 
    pBladedData[ iLogIndex +  14 ] = (float)pLogdata[ BASE_DTD_DEM_TORQ ]; 
    pBladedData[ iLogIndex +  15 ] = (float)pLogdata[ BASE_SWD_SIDEWACC ]; 
    pBladedData[ iLogIndex +  16 ] = (float)pLogdata[ BASE_SWD_SIDEWACC_LPF ]; 
    pBladedData[ iLogIndex +  17 ] = (float)pLogdata[ BASE_SWD_DEM_TORQ ]; 
    pBladedData[ iLogIndex +  18 ] = (float)pLogdata[ BASE_FAD_FOREAFTACC ]; 
    pBladedData[ iLogIndex +  19 ] = (float)pLogdata[ BASE_FAD_FOREAFTACC_LPF ]; 
    pBladedData[ iLogIndex +  20 ] = (float)pLogdata[ BASE_FAD_DEM_PIT ]; 
    pBladedData[ iLogIndex +  21 ] = (float)pLogdata[ BASE_IPC1P_D_MIN ]; 
    pBladedData[ iLogIndex +  22 ] = (float)pLogdata[ BASE_IPC1P_D_MAX ]; 
    pBladedData[ iLogIndex +  23 ] = (float)pLogdata[ BASE_IPC1P_D_DEM_PIT ]; 
    pBladedData[ iLogIndex +  24 ] = (float)pLogdata[ BASE_IPC1P_Q_MIN ]; 
    pBladedData[ iLogIndex +  25 ] = (float)pLogdata[ BASE_IPC1P_Q_MAX ]; 
    pBladedData[ iLogIndex +  26 ] = (float)pLogdata[ BASE_IPC1P_Q_DEM_PIT ]; 
    pBladedData[ iLogIndex +  27 ] = (float)pLogdata[ BASE_IPC1P_DEM_PIT1 ]; 
    pBladedData[ iLogIndex +  28 ] = (float)pLogdata[ BASE_IPC1P_DEM_PIT2 ]; 
    pBladedData[ iLogIndex +  29 ] = (float)pLogdata[ BASE_YAW_DEMYAWMOMENT ]; 
    pBladedData[ iLogIndex +  30 ] = (float)pLogdata[ BASE_YAWIPC_ERR ]; 
    pBladedData[ iLogIndex +  31 ] = (float)pLogdata[ BASE_YAWIPC_ERR_LPF ]; 
    pBladedData[ iLogIndex +  32 ] = (float)pLogdata[ BASE_YAWIPC_MAX ]; 
    pBladedData[ iLogIndex +  33 ] = (float)pLogdata[ BASE_YAWIPC_MIN ]; 
    pBladedData[ iLogIndex +  34 ] = (float)pLogdata[ BASE_YAWIPC_RATEMAX ]; 
    pBladedData[ iLogIndex +  35 ] = (float)pLogdata[ BASE_YAWIPC_RATEMIN ]; 
    pBladedData[ iLogIndex +  36 ] = (float)pLogdata[ BASE_YAWIPC_KP ]; 
    pBladedData[ iLogIndex +  37 ] = (float)pLogdata[ BASE_YAWIPC_KI ]; 
    pBladedData[ iLogIndex +  38 ] = (float)pLogdata[ BASE_YAWIPC_KD ]; 
    pBladedData[ iLogIndex +  39 ] = (float)pLogdata[ BASE_YAW_SETPOINT ]; 
    pBladedData[ iLogIndex +  40 ] = (float)pLogdata[ BASE_YAW_MODE ]; 
    pBladedData[ iLogIndex +  41 ] = (float)pLogdata[ BASE_YAW_DEMYAWRATE ]; 
    pBladedData[ iLogIndex +  42 ] = (float)pLogdata[ BASE_YAW_DEMYAWMOMENT ]; 
    pBladedData[ iLogIndex +  43 ] = (float)pLogdata[ OPER_DEM_YAWRATE ]; 
    pBladedData[ iLogIndex +  44 ] = (float)pLogdata[ SUP_TRIGGER ]; 
    pBladedData[ iLogIndex +  45 ] = (float)pLogdata[ SUP_TRIGGER_INDEX ]; 
    pBladedData[ iLogIndex +  46 ] = (float)pLogdata[ SUP_STATE ]; 
    pBladedData[ iLogIndex +  47 ] = (float)pLogdata[ SUP_STATE_INDEX ]; 
    pBladedData[ iLogNrVariables ] = 48.000;



    strcpy( cOutName, "" );
    strcat( cOutName, "DWTC[2.0.0]:-;" );
    strcat( cOutName, "BASE_SPD_RTSP:A/T;" );
    strcat( cOutName, "BASE_SPD_RTSP_P_LPF:A;" );
    strcat( cOutName, "BASE_SPD_RTSP_T_LPF:A;" );
    strcat( cOutName, "BASE_SPD_TORQ_SETPOINT:A/T;" );
    strcat( cOutName, "BASE_SPD_PIT_MIN:A;" );
    strcat( cOutName, "BASE_SPD_PIT_MAX:A;" );
    strcat( cOutName, "BASE_SPD_PIT_MIN_SPD:A/T;" );
    strcat( cOutName, "BASE_SPD_PIT_MAX_SPD:A/T;" );
    strcat( cOutName, "BASE_SPD_PIT_SETPOINT:A/T;" );
    strcat( cOutName, "BASE_SPD_DEM_TORQ:FL;" );
    strcat( cOutName, "BASE_SPD_DEM_PITCH:A;" );
    strcat( cOutName, "BASE_DTD_OMR:A/T;" );
    strcat( cOutName, "BASE_DTD_OMR_LPF:A/T;" );
    strcat( cOutName, "BASE_DTD_DEM_TORQ:FL;" );
    strcat( cOutName, "BASE_SWD_SIDEWACC:L/T;" );
    strcat( cOutName, "BASE_SWD_SIDEWACC_LPF:L/T;" );
    strcat( cOutName, "BASE_SWD_DEM_TORQ:FL;" );
    strcat( cOutName, "BASE_FAD_FOREAFTACC:L/T;" );
    strcat( cOutName, "BASE_FAD_FOREAFTACC_LPF:L/T;" );
    strcat( cOutName, "BASE_FAD_DEM_PIT:A;" );
    strcat( cOutName, "BASE_IPC1P_D_MIN:A;" );
    strcat( cOutName, "BASE_IPC1P_D_MAX:A;" );
    strcat( cOutName, "BASE_IPC1P_D_DEM_PIT:A;" );
    strcat( cOutName, "BASE_IPC1P_Q_MIN:A;" );
    strcat( cOutName, "BASE_IPC1P_Q_MAX:A;" );
    strcat( cOutName, "BASE_IPC1P_Q_DEM_PIT:A;" );
    strcat( cOutName, "BASE_IPC1P_DEM_PIT1:A;" );
    strcat( cOutName, "BASE_IPC1P_DEM_PIT2:A;" );
    strcat( cOutName, "BASE_YAW_DEMYAWMOMENT:FL;" );
    strcat( cOutName, "BASE_YAWIPC_ERR:A;" );
    strcat( cOutName, "BASE_YAWIPC_ERR_LPF:A;" );
    strcat( cOutName, "BASE_YAWIPC_MAX:A;" );
    strcat( cOutName, "BASE_YAWIPC_MIN:A;" );
    strcat( cOutName, "BASE_YAWIPC_RATEMAX:A/T;" );
    strcat( cOutName, "BASE_YAWIPC_RATEMIN:A/T;" );
    strcat( cOutName, "BASE_YAWIPC_KP:-;" );
    strcat( cOutName, "BASE_YAWIPC_KI:-;" );
    strcat( cOutName, "BASE_YAWIPC_KD:-;" );
    strcat( cOutName, "BASE_YAW_SETPOINT:A;" );
    strcat( cOutName, "BASE_YAW_MODE:-;" );
    strcat( cOutName, "BASE_YAW_DEMYAWRATE:A/T;" );
    strcat( cOutName, "BASE_YAW_DEMYAWMOMENT:FL;" );
    strcat( cOutName, "OPER_DEM_YAWRATE:A/T;" );
    strcat( cOutName, "SUP_TRIGGER:-;" );
    strcat( cOutName, "SUP_TRIGGER_INDEX:-;" );
    strcat( cOutName, "SUP_STATE:-;" );
    strcat( cOutName, "SUP_STATE_INDEX:-;" );


    return 0;

}
