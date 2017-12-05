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
int par_readfilt_txt_series (

		text_struct * iText       ,		   // Just passed on
		Filter **   filtSeries    ,        // Pointer to array of filter structures (pointer of a pointer)
		REAL   *    dampSeries    ,        // Array of reals containing damping factors (???)
		const REAL        timestep      ,  // Sampling period Ts
		const char   *    basetag       ,  // Name that filter series starts with in input file (e.g. "RotSp_Pit_")
		const int         nr_highpass   ,  // N_HPF_FILTERS 1  : Number of HPF in ALL filter sequences
		const int         nr_notchvar   ,  // N_NFP_FILTERS 12 : Number of NFP in ALL filter sequences (Variable Notch)
		const int         nr_notchfxd   ,  // N_NFF_FILTERS 6  : Number of NFF in ALL filter sequences
		const int         nr_lowpass       // N_LPF_FILTERS 2  : Number of LPF in ALL filter sequences

)
{

	int iError = 0;
	char cTag[50];
	int k;
	int offset = 0;
	for ( k = 0; k < nr_highpass; ++k )
	{
		sprintf( cTag, "%sHP%dF", basetag, k+1 ); // Create tag for specific HPF filter to find (e.g. "RotSp_Pit_HP1F")
		iError += par_readfilt_txt_fxd ( iText, filtSeries[k+offset], cTag, T_HIGHPASS , timestep );
	}
	offset += nr_highpass;
	for ( k = 0; k < nr_notchvar; ++k ) // VARIABLE NOTCH: Receives dampSeries
	{
		sprintf( cTag, "%sNF%dP", basetag, k+1 ); // Create tag for specific NFP filter to find (e.g. "RotSp_Pit_NF1P")
		iError += par_readfilt_txt_var ( iText, filtSeries[k+offset], cTag, T_NOTCH, dampSeries+k );
	}
	offset += nr_notchvar;
	for ( k = 0; k < nr_notchfxd; ++k )
	{
		sprintf( cTag, "%sNF%dF", basetag, k+1 ); // Create tag for specific NFF filter to find (e.g. "RotSp_Pit_HF1F")
		iError += par_readfilt_txt_fxd ( iText, filtSeries[k+offset], cTag, T_NOTCH, timestep );
	}
	offset += nr_notchfxd;
	for ( k = 0; k < nr_lowpass; ++k )
	{
		sprintf( cTag, "%sLP%dF", basetag, k+1 ); // Create tag for specific LPF filter to find (e.g. "RotSp_Pit_LP1F")
		iError += par_readfilt_txt_fxd ( iText, filtSeries[k+offset], cTag, T_LOWPASS, timestep );
	}

	return iError;
}

/* ---------------------------------------------------------------------------------
 Read a filter with variable frequency
--------------------------------------------------------------------------------- */
int par_readfilt_txt_var(         // Special variable notch (note the 'var')

		text_struct * iText ,		   // Just passed on
		Filter * filter     , // Pointer to just one filter structure
		char   * tag        , // Tag of the specific filter to find (e.g. "RotSp_Pit_NF1P")
		int      type       , // Type of filter ENUM (not used in case of variable filter)
		REAL   * value        // Variable notch reads only one value per filter (damping coeff.)

)
{

	int iError = MCU_OK;
	*value = -1.0;

	// Read N = 1 value which is the damping coeff. N is defined in input file (must be 1)
	iError += par_readline_txt_d ( iText, value, tag );
	// Change for: int par_readline_txt_d(  text_struct * iText, double * dData, const char * cTagRef  )

	// If it did not find the line then disable filter
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
int par_readfilt_txt_fxd(

		text_struct * iText , // Just passed on
		Filter * filter     , // Pointer to just one filter structure
		char   * tag        , // Tag of the specific filter to find (e.g. "RotSp_Pit_HP1F")
		int      type       , // Type of filter ENUM
		REAL     timestep     // Time step Ts

)
{

	REAL R[ 20 ];
	int iError = MCU_OK;

	// Read N values which are 2 for fixed filters. N is defined in input file (must be 2)
	iError += par_readline_txt_d ( iText, R, tag );
	// Change for: int par_readline_txt_d(  text_struct * iText, double * dData, const char * cTagRef  )

	if ( iError < 0 )
	{

		filter->active = 0 ;
		return 0;

	}

	// Allocate parameters read where necessary wrt filter type (filter.c)
	switch (type)
	{
	// Can use original functions. No need to create new ones.
	case T_NOTCH    : filter_setNotch_sca( filter, R[0], R[1], timestep ); break;
	case T_NOTCH2   : filter_setNotch2_sca( filter, 0.8, R[0], R[1], timestep ); break;
	case T_LOWPASS  : filter_setLowPass_sca( filter, R[0], R[1], timestep ); break;
	case T_HIGHPASS : filter_setHighPass_sca( filter, R[0], R[1], timestep ); break;

	}

	return iError;

}/* end par_readfilt_fxd() */

