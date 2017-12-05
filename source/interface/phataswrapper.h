/* ---------------------------------------------------------------------------------
 *          file : phataswrapper.h                                                *
 *   description : C-header file                                                  *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
 *     reference : E.C.N. - D.A.J. Wouters, 23-04-2007, "Linking a controller to  *
 *                   Phatas" and "IO_list.pdf"                                    *
--------------------------------------------------------------------------------- */

#ifndef _PHATASWRAPPER_H_
#define _PHATASWRAPPER_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup entrypoints 
 *  @{*/

/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FILES

/*! \file phataswrapper.h
    \brief This file holds the entry point for Focus/Phatas.
*/
#endif


/* ------------------------------------------------------------------------------ */
#ifndef DXG_SKIP_FUNCTIONS

//! Phatas entry point of the dll. 
/*!
    For phatas it is required that dll file is called controller.dll and 
    the access function _fullxs2control.
*/
int __declspec(dllexport) _fullxs2control(
        
              double        * const pdThdtset1,      
              double        * const pdThdtset2,      
              double        * const pdThdtset3,      
              double        * const pTeset,          
              long          * const pBrakeCmd,       
              long          * const pYawAct,         
              double        * const pYawRate,        
              long          * const pCtrlId,         
              unsigned long * const pOutputFlags,    
        const double        * const pOmega,          
        const double        * const pTh1,            
        const double        * const pTh2,            
        const double        * const pTh3,            
        const double        * const pPe,             
        const long          * const pEnable,         
        const double        * const pTowerTopAccelX, 
        const double        * const pTowerTopAccelY, 
        const double        * const pYawAngle,       
        const double        * const pWindDir,        
        const double        * const pWindSpeed,      
        const double        * const pBlade1FlapMom,  
        const double        * const pBlade2FlapMom,  
        const double        * const pBlade3FlapMom,  
        const double        * const pBlade1LeadMom,  
        const double        * const pBlade2LeadMom,  
        const double        * const pBlade3LeadMom,  
        const double        * const pDriveShaftBenddMomY,
        const double        * const pDriveShaftBenddMomZ,
        const double        * const pYawBearMomX,    
        const double        * const pYawBearMomY,    
        const double        * const pYawBearMomZ,    
        const double        * const pAzimuth,        
        const unsigned long * const pInputFlags,     
        const double        * const pt,              
        const double        * const pH,              
              long          * const pCallStatus,     
              char          * const Message,         
              double        * const dummy1,          
              double        * const dummy2,          
              double        * const dummy3,          
              double        * const dummy4           

);

#endif

/** @}*/
/* ------------------------------------------------------------------------------ */

#endif  

/* ---------------------------------------------------------------------------------
 End _PHATASWRAPPER_H_
--------------------------------------------------------------------------------- */
