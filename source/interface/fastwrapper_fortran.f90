!          file : fastwrapper_fortran.f90                                                
!   description : F90-source file, entry point of DLL for FAST          
!       toolbox : DotX Wind Turbine Control Software                             
!        author : DotX Control Solutions, www.dotxcontrol.com                    

SUBROUTINE DISCON ( avrSWAP, avIFAIL, accINFILE, avcOUTNAME, avcMSG )

IMPLICIT   NONE

!GCC$ ATTRIBUTES CDECL, DLLEXPORT :: DISCON

! !DEC$ ATTRIBUTES DLLEXPORT :: DISCON
! !DEC$ ATTRIBUTES ALIAS:'DISCON' :: DISCON

! Passed Variables:
REAL(4),    INTENT(INOUT)    :: avrSWAP   (*)                                    ! The swap array, used to pass data to, and receive data from, the DLL controller.
INTEGER(4), INTENT(  OUT)    :: avIFAIL                                          ! A flag used to indicate the success of this DLL call set as follows: 0 IF the DLL call was successful, >0 IF the DLL call was successful but cMessage should be issued as a warning messsage, <0 IF the DLL call was unsuccessful or for any other reason the simulation is to be stopped at this point with cMessage as the error message.
INTEGER(1), INTENT(IN   )    :: accINFILE (*)                                    ! The address of the first record of an array of 1-byte CHARACTERs giving the name of the parameter input file, 'DISCON.IN'.
INTEGER(1), INTENT(  OUT)    :: avcMSG    (*)                                    ! The address of the first record of an array of 1-byte CHARACTERS giving the message contained in cMessage, which will be displayed by the calling program IF avIFAIL <> 0.
INTEGER(1), INTENT(IN   )    :: avcOUTNAME(*)                                    ! The address of the first record of an array of 1-byte CHARACTERS giving the simulation run name without extension.

REAL(4), PARAMETER           :: R2D               = 57.295780                    ! Factor to convert radians to degrees.

call discon_ansi_c( avrSWAP, avIFAIL, accINFILE, avcOUTNAME, avcMSG )

! Torque open loop
! avrSWAP(35) = 1.0          
! avrSWAP(56) = 0.0          
! avrSWAP(47) = 0.0
! 
! ! Pitch open loop
! avrSWAP(42) = 90.0 / R2D
! avrSWAP(43) = 90.0 / R2D
! avrSWAP(44) = 90.0 / R2D
! avrSWAP(45) = 90.0 / R2D
 
RETURN

    END SUBROUTINE DISCON
