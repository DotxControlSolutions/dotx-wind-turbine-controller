/*!
    \file signal_definitions_external.h
    \brief Definition of signal indices used for logging data.
    
    The #LOGSIGNALS enumeration holds the indices used to adress variables in
    the pLogdata array. The signals are not individually documented as a Matlab
    scripts reads the file to auto generate the same indeces in Matlab.
*/


#ifndef _SIGNAL_DEFINITIONS_EXTERNAL_H_
#define _SIGNAL_DEFINITIONS_EXTERNAL_H_

#define MACROSTR(k) #k

//   X_NUMBERS = (Variable Name, Bladed Unit, ON/OFF DotX logging, ON/OFF Bladed logging)
//
//   Just add as many rows as desired while following the specified format, the macros take care of the rest
//   Bladed Unit can be :
//		A/T
//		FL
//		A
//		L/T
//		P

#define X_NUMBERS        						\
    X(OPER_MEAS_TIME              , - , 1 , 0 ) \
    X(OPER_MEAS_GENSPEED          , - , 1 , 0 ) \
    X(OPER_MEAS_AZIMUTH           , - , 1 , 0 ) \
    X(OPER_MEAS_FAACC             , - , 1 , 0 ) \
    X(OPER_MEAS_SWACC             , - , 1 , 0 ) \
    X(OPER_MEAS_GENTORQUE         , - , 1 , 0 ) \
    X(OPER_MEAS_ELECPOWER         , - , 1 , 0 ) \
    X(OPER_MEAS_YAWERROR          , - , 1 , 0 ) \
    X(OPER_MEAS_TIMESTEP          , - , 1 , 0 ) \
    X(OPER_MEAS_GRIDCONT          , - , 1 , 0 ) \
    X(OPER_MEAS_BENOUT1           , - , 1 , 0 ) \
    X(OPER_MEAS_BENOUT2           , - , 1 , 0 ) \
    X(OPER_MEAS_BENOUT3           , - , 1 , 0 ) \
    X(OPER_MEAS_BENIN1           , - , 1 , 0 ) \
    X(OPER_MEAS_BENIN2           , - , 1 , 0 ) \
    X(OPER_MEAS_BENIN3           , - , 1 , 0 ) \
    X(OPER_MEAS_PITCH1            , - , 1 , 0 ) \
    X(OPER_MEAS_PITCH2            , - , 1 , 0 ) \
    X(OPER_MEAS_PITCH3            , - , 1 , 0 ) \
    X(OPER_MEAS_ACTTYPE           , - , 1 , 0 ) \
    X(OPER_MEAS_HUBWIND           , - , 1 , 0 ) \
    X(OPER_DEM_GENTORQUE          , - , 1 , 0 ) \
    X(OPER_DEM_PITCH1             , - , 1 , 0 ) \
    X(OPER_DEM_PITCH2             , - , 1 , 0 ) \
    X(OPER_DEM_PITCH3             , - , 1 , 0 ) \
    X(OPER_DEM_YAWRATE            , - , 1 , 0 ) \
    X(BASE_SPD_RTSP               , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_P_HPF         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_P_NFP         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_P_NFF         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_P_LPF         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_T_HPF         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_T_NFP         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_T_NFF         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_T_LPF         , - , 1 , 0 ) \
    X(BASE_SPD_RTSP_SCHED         , - , 1 , 0 ) \
    X(BASE_SPD_POW                , - , 1 , 0 ) \
    X(BASE_SPD_POW_LPF            , - , 1 , 0 ) \
    X(BASE_SPD_PITCH              , - , 1 , 0 ) \
    X(BASE_SPD_PITCH_LPF          , - , 1 , 0 ) \
    X(BASE_SPD_OMEGA_SC           , - , 1 , 0 ) \
    X(BASE_SPD_TGEN_BOUND         , - , 1 , 0 ) \
    X(BASE_SPD_WHALF              , - , 1 , 0 ) \
    X(BASE_SPD_TORQUESELECT       , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_MIN           , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_MAX           , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_MIN_SPD       , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_MAX_SPD       , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_KP            , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_KI            , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_KD            , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_ERROR         , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_SETPOINT      , - , 1 , 0 ) \
    X(BASE_SPD_TORQ_PIDOUT        , - , 1 , 0 ) \
    X(BASE_SPD_PIT_MIN            , - , 1 , 0 ) \
    X(BASE_SPD_PIT_MAX            , - , 1 , 0 ) \
    X(BASE_SPD_PIT_MIN_SPD        , - , 1 , 0 ) \
    X(BASE_SPD_PIT_MAX_SPD        , - , 1 , 0 ) \
    X(BASE_SPD_PIT_KP             , - , 1 , 0 ) \
    X(BASE_SPD_PIT_KI             , - , 1 , 0 ) \
    X(BASE_SPD_PIT_KD             , - , 1 , 0 ) \
    X(BASE_SPD_PIT_ERROR          , - , 1 , 0 ) \
    X(BASE_SPD_PIT_SETPOINT       , - , 1 , 0 ) \
    X(BASE_SPD_PIT_PIDOUT         , - , 1 , 0 ) \
    X(BASE_SPD_DEM_TORQ           , - , 1 , 0 ) \
    X(BASE_SPD_DEM_PITCH          , - , 1 , 0 ) \
    X(BASE_TP_SELECT              , - , 1 , 0 ) \
    X(BASE_DTD_OMR                , - , 1 , 0 ) \
    X(BASE_DTD_OMR_HPF            , - , 1 , 0 ) \
    X(BASE_DTD_OMR_NFP            , - , 1 , 0 ) \
    X(BASE_DTD_OMR_NFF            , - , 1 , 0 ) \
    X(BASE_DTD_OMR_LPF            , - , 1 , 0 ) \
    X(BASE_DTD_MIN                , - , 1 , 0 ) \
    X(BASE_DTD_MAX                , - , 1 , 0 ) \
    X(BASE_DTD_MIN_SPD            , - , 1 , 0 ) \
    X(BASE_DTD_MAX_SPD            , - , 1 , 0 ) \
    X(BASE_DTD_KP                 , - , 1 , 0 ) \
    X(BASE_DTD_KI                 , - , 1 , 0 ) \
    X(BASE_DTD_KD                 , - , 1 , 0 ) \
    X(BASE_DTD_ERROR              , - , 1 , 0 ) \
    X(BASE_DTD_SETPOINT           , - , 1 , 0 ) \
    X(BASE_DTD_PIDOUT             , - , 1 , 0 ) \
    X(BASE_DTD_DEM_TORQ           , - , 1 , 0 ) \
    X(BASE_DTD_DEM_TORQ_FILT      , - , 1 , 0 ) \
    X(BASE_SWD_SIDEWACC           , - , 1 , 0 ) \
    X(BASE_SWD_SIDEWACC_HPF       , - , 1 , 0 ) \
    X(BASE_SWD_SIDEWACC_NFP       , - , 1 , 0 ) \
    X(BASE_SWD_SIDEWACC_NFF       , - , 1 , 0 ) \
    X(BASE_SWD_SIDEWACC_LPF       , - , 1 , 0 ) \
    X(BASE_SWD_MIN                , - , 1 , 0 ) \
    X(BASE_SWD_MAX                , - , 1 , 0 ) \
    X(BASE_SWD_MIN_SPD            , - , 1 , 0 ) \
    X(BASE_SWD_MAX_SPD            , - , 1 , 0 ) \
    X(BASE_SWD_KP                 , - , 1 , 0 ) \
    X(BASE_SWD_KI                 , - , 1 , 0 ) \
    X(BASE_SWD_KD                 , - , 1 , 0 ) \
    X(BASE_SWD_ERROR              , - , 1 , 0 ) \
    X(BASE_SWD_SETPOINT           , - , 1 , 0 ) \
    X(BASE_SWD_PIDOUT             , - , 1 , 0 ) \
    X(BASE_SWD_DEM_TORQ           , - , 1 , 0 ) \
    X(BASE_FAD_FOREAFTACC         , - , 1 , 0 ) \
    X(BASE_FAD_FOREAFTACC_HPF     , - , 1 , 0 ) \
    X(BASE_FAD_FOREAFTACC_NFP     , - , 1 , 0 ) \
    X(BASE_FAD_FOREAFTACC_NFF     , - , 1 , 0 ) \
    X(BASE_FAD_FOREAFTACC_LPF     , - , 1 , 0 ) \
    X(BASE_FAD_PITSPD_MIN         , - , 1 , 0 ) \
    X(BASE_FAD_PITSPD_MAX         , - , 1 , 0 ) \
    X(BASE_FAD_PITSPD_MIN_LPF     , - , 1 , 0 ) \
    X(BASE_FAD_PITSPD_MAX_LPF     , - , 1 , 0 ) \
    X(BASE_FAD_AMPLITUDE          , - , 1 , 0 ) \
    X(BASE_FAD_MIN                , - , 1 , 0 ) \
    X(BASE_FAD_MAX                , - , 1 , 0 ) \
    X(BASE_FAD_MIN_SPD            , - , 1 , 0 ) \
    X(BASE_FAD_MAX_SPD            , - , 1 , 0 ) \
    X(BASE_FAD_KP                 , - , 1 , 0 ) \
    X(BASE_FAD_KI                 , - , 1 , 0 ) \
    X(BASE_FAD_KD                 , - , 1 , 0 ) \
    X(BASE_FAD_ERROR              , - , 1 , 0 ) \
    X(BASE_FAD_SETPOINT           , - , 1 , 0 ) \
    X(BASE_FAD_PIDOUT             , - , 1 , 0 ) \
    X(BASE_FAD_DEM_PIT            , - , 1 , 0 ) \
    X(BASE_IPC_SPDLIM             , - , 1 , 0 ) \
    X(BASE_IPC_SPDLIM_LPF         , - , 1 , 0 ) \
    X(BASE_IPC_ABSLIM1            , - , 1 , 0 ) \
    X(BASE_IPC_ABSLIM1_LPF        , - , 1 , 0 ) \
    X(BASE_IPC_ABSLIM2            , - , 1 , 0 ) \
    X(BASE_IPC_ABSLIM2_LPF        , - , 1 , 0 ) \
    X(BASE_IPC_ABSLIM             , - , 1 , 0 ) \
    X(BASE_IPC_AMP                , - , 1 , 0 ) \
    X(BASE_IPC1P_BENMOM0          , - , 1 , 0 ) \
    X(BASE_IPC1P_BENMOM1          , - , 1 , 0 ) \
    X(BASE_IPC1P_BENMOM2          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERD              , - , 1 , 0 ) \
    X(BASE_IPC1P_ERD_HPF          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERD_NFP          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERD_NFF          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERD_LPF          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERQ              , - , 1 , 0 ) \
    X(BASE_IPC1P_ERQ_HPF          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERQ_NFP          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERQ_NFF          , - , 1 , 0 ) \
    X(BASE_IPC1P_ERQ_LPF          , - , 1 , 0 ) \
    X(BASE_IPC1P_D_MIN            , - , 1 , 0 ) \
    X(BASE_IPC1P_D_MAX            , - , 1 , 0 ) \
    X(BASE_IPC1P_D_MIN_SPD        , - , 1 , 0 ) \
    X(BASE_IPC1P_D_MAX_SPD        , - , 1 , 0 ) \
    X(BASE_IPC1P_D_KP             , - , 1 , 0 ) \
    X(BASE_IPC1P_D_KI             , - , 1 , 0 ) \
    X(BASE_IPC1P_D_KD             , - , 1 , 0 ) \
    X(BASE_IPC1P_D_ERROR          , - , 1 , 0 ) \
    X(BASE_IPC1P_D_SETPOINT       , - , 1 , 0 ) \
    X(BASE_IPC1P_D_PIDOUT         , - , 1 , 0 ) \
    X(BASE_IPC1P_D_DEM_PIT        , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_MIN            , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_MAX            , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_MIN_SPD        , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_MAX_SPD        , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_KP             , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_KI             , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_KD             , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_ERROR          , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_SETPOINT       , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_PIDOUT         , - , 1 , 0 ) \
    X(BASE_IPC1P_Q_DEM_PIT        , - , 1 , 0 ) \
    X(BASE_IPC1P_DEM_PIT1         , - , 1 , 0 ) \
    X(BASE_IPC1P_DEM_PIT2         , - , 1 , 0 ) \
    X(BASE_IPC1P_DEM_PIT3         , - , 1 , 0 ) \
    X(BASE_IPC2P_BENMOM0          , - , 1 , 0 ) \
    X(BASE_IPC2P_BENMOM1          , - , 1 , 0 ) \
    X(BASE_IPC2P_BENMOM2          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERD              , - , 1 , 0 ) \
    X(BASE_IPC2P_ERD_HPF          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERD_NFP          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERD_NFF          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERD_LPF          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERQ              , - , 1 , 0 ) \
    X(BASE_IPC2P_ERQ_HPF          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERQ_NFP          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERQ_NFF          , - , 1 , 0 ) \
    X(BASE_IPC2P_ERQ_LPF          , - , 1 , 0 ) \
    X(BASE_IPC2P_D_MIN            , - , 1 , 0 ) \
    X(BASE_IPC2P_D_MAX            , - , 1 , 0 ) \
    X(BASE_IPC2P_D_MIN_SPD        , - , 1 , 0 ) \
    X(BASE_IPC2P_D_MAX_SPD        , - , 1 , 0 ) \
    X(BASE_IPC2P_D_KP             , - , 1 , 0 ) \
    X(BASE_IPC2P_D_KI             , - , 1 , 0 ) \
    X(BASE_IPC2P_D_KD             , - , 1 , 0 ) \
    X(BASE_IPC2P_D_ERROR          , - , 1 , 0 ) \
    X(BASE_IPC2P_D_SETPOINT       , - , 1 , 0 ) \
    X(BASE_IPC2P_D_PIDOUT         , - , 1 , 0 ) \
    X(BASE_IPC2P_D_DEM_PIT        , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_MIN            , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_MAX            , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_MIN_SPD        , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_MAX_SPD        , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_KP             , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_KI             , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_KD             , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_ERROR          , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_SETPOINT       , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_PIDOUT         , - , 1 , 0 ) \
    X(BASE_IPC2P_Q_DEM_PIT        , - , 1 , 0 ) \
    X(BASE_IPC2P_DEM_PIT1         , - , 1 , 0 ) \
    X(BASE_IPC2P_DEM_PIT2         , - , 1 , 0 ) \
    X(BASE_IPC2P_DEM_PIT3         , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_KP           , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_KI           , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_KD           , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_MIN          , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_MAX          , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_RATEMIN      , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_RATEMAX      , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_ERR1         , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_ERR2         , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_ERR3         , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_PID_OUT1     , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_PID_OUT2     , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_PID_OUT3     , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_DEM_PIT1     , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_DEM_PIT2     , - , 1 , 0 ) \
    X(BASE_PITFOLLOW_DEM_PIT3     , - , 1 , 0 ) \
    X(BASE_YAWIPC_ERR             , - , 1 , 0 ) \
    X(BASE_YAWIPC_ERR_LPF         , - , 1 , 0 ) \
    X(BASE_YAWIPC_MAX             , - , 1 , 0 ) \
    X(BASE_YAWIPC_MIN             , - , 1 , 0 ) \
    X(BASE_YAWIPC_RATEMAX         , - , 1 , 0 ) \
    X(BASE_YAWIPC_RATEMIN         , - , 1 , 0 ) \
    X(BASE_YAWIPC_KP              , - , 1 , 0 ) \
    X(BASE_YAWIPC_KI              , - , 1 , 0 ) \
    X(BASE_YAWIPC_KD              , - , 1 , 0 ) \
    X(BASE_YAWMOT_ERR             , - , 1 , 0 ) \
    X(BASE_YAWMOT_ERR_LPF         , - , 1 , 0 ) \
    X(BASE_YAWMOT_ACTIVE          , - , 1 , 0 ) \
    X(BASE_YAW_SETPOINT           , - , 1 , 0 ) \
    X(BASE_YAW_MODE               , - , 1 , 0 ) \
    X(BASE_YAW_DEMYAWRATE         , - , 1 , 0 ) \
    X(BASE_YAW_DEMYAWMOMENT       , - , 1 , 0 ) \
    X(DNPC_TIME_MEAS              , - , 1 , 0 ) \
    X(DNPC_GENSPEED_MEAS          , - , 1 , 0 ) \
    X(DNPC_GENTORQUE_MEAS         , - , 1 , 0 ) \
    X(DNPC_HUBWIND_MEAS           , - , 1 , 0 ) \
    X(DNPC_AZIMUTH_MEAS           , - , 1 , 0 ) \
    X(DNPC_SWARDSACC_MEAS         , - , 1 , 0 ) \
    X(DNPC_FORAFTACC_MEAS         , - , 1 , 0 ) \
    X(DNPC_POWER_MEAS             , - , 1 , 0 ) \
    X(DNPC_BENMOM1_MEAS           , - , 1 , 0 ) \
    X(DNPC_BENMOM2_MEAS           , - , 1 , 0 ) \
    X(DNPC_BENMOM3_MEAS           , - , 1 , 0 ) \
    X(DNPC_PITCH1_MEAS            , - , 1 , 0 ) \
    X(DNPC_PITCH2_MEAS            , - , 1 , 0 ) \
    X(DNPC_PITCH3_MEAS            , - , 1 , 0 ) \
    X(DNPC_VAXEST_OBS             , - , 1 , 0 ) \
    X(DNPC_VAXEST1_OBS            , - , 1 , 0 ) \
    X(DNPC_VAXEST2_OBS            , - , 1 , 0 ) \
    X(DNPC_VAXEST3_OBS            , - , 1 , 0 ) \
    X(DNPC_XFAEST_OBS             , - , 1 , 0 ) \
    X(DNPC_VFAEST_OBS             , - , 1 , 0 ) \
    X(DNPC_PITCH1_OBS             , - , 1 , 0 ) \
    X(DNPC_PITCH2_OBS             , - , 1 , 0 ) \
    X(DNPC_PITCH3_OBS             , - , 1 , 0 ) \
    X(DNPC_GENSPEED_OPNL          , - , 1 , 0 ) \
    X(DNPC_GENTORQUE_OPNL         , - , 1 , 0 ) \
    X(DNPC_POWER_OPNL             , - , 1 , 0 ) \
    X(DNPC_FOREAFTACC_OPNL        , - , 1 , 0 ) \
    X(DNPC_XFAEST_OPNL            , - , 1 , 0 ) \
    X(DNPC_VFAEST_OPNL            , - , 1 , 0 ) \
    X(DNPC_BENMOM1_OPNL           , - , 1 , 0 ) \
    X(DNPC_BENMOM2_OPNL           , - , 1 , 0 ) \
    X(DNPC_BENMOM3_OPNL           , - , 1 , 0 ) \
    X(DNPC_GENSPEED_FILT          , - , 1 , 0 ) \
    X(DNPC_GENTORQUE_FILT         , - , 1 , 0 ) \
    X(DNPC_POWER_FILT             , - , 1 , 0 ) \
    X(DNPC_XFAEST_FILT            , - , 1 , 0 ) \
    X(DNPC_VFAEST_FILT            , - , 1 , 0 ) \
    X(DNPC_VAXEST_FILT            , - , 1 , 0 ) \
    X(DNPC_VAXEST1_FILT           , - , 1 , 0 ) \
    X(DNPC_VAXEST2_FILT           , - , 1 , 0 ) \
    X(DNPC_VAXEST3_FILT           , - , 1 , 0 ) \
    X(DNPC_BENMOM1_FILT           , - , 1 , 0 ) \
    X(DNPC_BENMOM2_FILT           , - , 1 , 0 ) \
    X(DNPC_BENMOM3_FILT           , - , 1 , 0 ) \
    X(DNPC_GENSPEED_SETP          , - , 1 , 0 ) \
    X(DNPC_POWER_SETP             , - , 1 , 0 ) \
    X(DNPC_GENTORQUE_SETP         , - , 1 , 0 ) \
    X(DNPC_PITCH1_SETP            , - , 1 , 0 ) \
    X(DNPC_PITCH2_SETP            , - , 1 , 0 ) \
    X(DNPC_PITCH3_SETP            , - , 1 , 0 ) \
    X(DNPC_GENTORQUE_DEM          , - , 1 , 0 ) \
    X(DNPC_PITCH1_DEM             , - , 1 , 0 ) \
    X(DNPC_PITCH2_DEM             , - , 1 , 0 ) \
    X(DNPC_PITCH3_DEM             , - , 1 , 0 ) \
    X(DNPC_COMPEN_OM              , - , 1 , 0 ) \
    X(DNPC_COMPEN_KC              , - , 1 , 0 ) \
    X(SUP_TRIGGER                 , - , 1 , 0 ) \
    X(SUP_TRIGGER_INDEX           , - , 1 , 0 ) \
    X(SUP_STATE                   , - , 1 , 0 ) \
    X(SUP_STATE_INDEX             , - , 1 , 0 ) \
    X(SUP_OVERPOWER               , - , 1 , 0 ) \
    X(SUP_OVERSPEED               , - , 1 , 0 ) \
    X(ZZZ_DEBUG0                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG1                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG2                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG3                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG4                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG5                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG6                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG7                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG8                  , - , 1 , 0 ) \
    X(ZZZ_DEBUG9                  , - , 1 , 0 ) \
    X(MAXLOG 					  , - , 1 , 0 ) // IMPORANT: MAXLOG is ALWAYS necessary at the end                       


enum {
#define X(Name,Unit,DotxLog,BldLog) Name,
    X_NUMBERS
#undef X
};


static char *tableNames[] = {
#define X(Name,Unit,DotxLog,BldLog) MACROSTR(Name),
		X_NUMBERS
#undef X
};

static char *bladedNames[] = {
#define X(Name,Unit,DotxLog,BldLog) MACROSTR(Name:Unit;),
		X_NUMBERS
#undef X
};

static int dotxLogflags[] = {
#define X(Name,Unit,DotxLog,BldLog) DotxLog,
		X_NUMBERS
#undef X
};

static int bladedLogflags[] = {
#define X(Name,Unit,DotxLog,BldLog) BldLog,
		X_NUMBERS
#undef X
};



#endif
