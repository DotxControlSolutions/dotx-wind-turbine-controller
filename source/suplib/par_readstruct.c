/* ---------------------------------------------------------------------------------
 *          file : par_readstruct.c                                               *
 *   description : C-source file, functions for reading a struct from inputfile   *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#include <sys/stat.h> 
#include <unistd.h> 
#include <time.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./../signals/signal_definitions_internal.h"

#include "./matrix.h"
#include "./system.h"
#include "./filter.h"
#include "./pid.h"
#include "./par.h"


/* ---------------------------------------------------------------------------------
 Read a series of filters
--------------------------------------------------------------------------------- */
int par_readfilt_series (
		FILE   *    fidInFile     ,
		FILE   *    fidOutFile    ,
		Filter **   filtSeries    ,
		REAL   *    dampSeries    ,
		const REAL        timestep      ,
		const char   *    basetag       ,
		const int         nr_highpass   ,
		const int         nr_notchvar   ,
		const int         nr_notchfxd   ,
		const int         nr_lowpass
)
{

	int iError = 0;
	char cTag[50];
	int k;
	int offset = 0;
	for ( k = 0; k < nr_highpass; ++k )
	{
		sprintf( cTag, "%sHP%dF", basetag, k+1 );
		iError += par_readfilt_fxd ( fidInFile, fidOutFile, filtSeries[k+offset], cTag, T_HIGHPASS , timestep );
	}
	offset += nr_highpass;
	for ( k = 0; k < nr_notchvar; ++k )
	{
		sprintf( cTag, "%sNF%dP", basetag, k+1 );
		iError += par_readfilt_var ( fidInFile, fidOutFile, filtSeries[k+offset], cTag, T_NOTCH, dampSeries+k );
	}
	offset += nr_notchvar;
	for ( k = 0; k < nr_notchfxd; ++k )
	{
		sprintf( cTag, "%sNF%dF", basetag, k+1 );
		iError += par_readfilt_fxd ( fidInFile, fidOutFile, filtSeries[k+offset], cTag, T_NOTCH, timestep );
	}
	offset += nr_notchfxd;
	for ( k = 0; k < nr_lowpass; ++k )
	{
		sprintf( cTag, "%sLP%dF", basetag, k+1 );
		iError += par_readfilt_fxd ( fidInFile, fidOutFile, filtSeries[k+offset], cTag, T_LOWPASS, timestep );
	}

	return iError;
}

/* ---------------------------------------------------------------------------------
 Read a filter with variable frequency
--------------------------------------------------------------------------------- */
int par_readfilt_var( 
		FILE   * fidInFile  ,
		FILE   * fidOutFile ,
		Filter * filter     ,
		char   * tag        ,
		int      type       ,
		REAL   * value
)
{

	int iError = MCU_OK;
	*value = -1.0;

	iError += par_readline_d ( fidInFile, fidOutFile, value, tag );

	if ( iError < 0 )
	{

		filter->active = 0 ;
		return 0;
	}

	return iError;

}; /* end par_readfilt_var() */

/* ---------------------------------------------------------------------------------
 Read a filter with fixed frequency
--------------------------------------------------------------------------------- */
int par_readfilt_fxd( 

		FILE   * fidInFile  ,
		FILE   * fidOutFile ,
		Filter * filter     ,
		char   * tag        ,
		int      type       ,
		REAL     timestep

)
{

	REAL R[ 20 ];
	int iError = MCU_OK;

	iError += par_readline_d ( fidInFile, fidOutFile, R, tag );

	if ( iError < 0 )
	{

		filter->active = 0 ;
		return 0;

	}

	switch (type)
	{

	case T_NOTCH    : filter_setNotch_sca( filter, R[0], R[1], timestep ); break;
	case T_NOTCH2   : filter_setNotch2_sca( filter, 0.8, R[0], R[1], timestep ); break;
	case T_LOWPASS  : filter_setLowPass_sca( filter, R[0], R[1], timestep ); break;
	case T_HIGHPASS : filter_setHighPass_sca( filter, R[0], R[1], timestep ); break;

	}

	return iError;

}/* end par_readfilt_fxd() */

