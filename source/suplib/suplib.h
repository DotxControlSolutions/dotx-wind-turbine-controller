/* ---------------------------------------------------------------------------------
 *          file : suplib.h                                                       *
 *   description : C-header file, defines the filter struct                       *
 *       toolbox : DotX Wind Turbine Control Software (support library)           *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _SUPLIB_H_
#define _SUPLIB_H_

/* ------------------------------------------------------------------------------ */
/** \addtogroup suplib Support Library

This is the documentation for the Support Library

 *  @{*/

#include <sys/stat.h> 
#include <unistd.h> 
#include <time.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"
#include "./filter.h"
#include "./pid.h"
#include "./hp_pid.h"
#include "./par.h"
#include "./bicubic.h"

/** @}*/

#endif


