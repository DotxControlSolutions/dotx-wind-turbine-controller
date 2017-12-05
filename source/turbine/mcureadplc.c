/* ---------------------------------------------------------------------------------
 *          file : mcu_readfile.c                                                 *
 *   description : C-source file, functions for reading the inputfiles            *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./mcudata.h"


int read_plc_var( text_struct * iText  ,
		             char * VarName
)
{
	int iError = MCU_OK;
	char     TempVal[MAXPARCHAR];
	// read text content from SVI variable and store in TempVal
	// TODO
	// copy variable contents and reset position
	strcpy( iText->alltext, TempVal);
	strcat( iText->alltext, " \n" );
	iText->position = 0;

	return iError;
}

int read_plc_array( text_struct * iText  ,
		               char * ArrayName
)
{
	int i, iError = MCU_OK;

	char TempVal[MAXPARCHAR];
	char TempName[MAXPARCHAR];
	char TempNum[5];

	// put all filter variables together and reset position
	for (i = 0; i < 21; i++) // from 0 to 20
	{
		sprintf(TempNum, "_%d", i);  // Construct ending number e.g. "_12"
		strcpy(TempName, ArrayName); // Copy original name without ending number e.g. "RotSp_Pit_"
		strcat(TempName, TempNum);   // Append ending number e.g. "RotSp_Pit__12"

		// read text content from SVI variable and store in TempVal
		// TODO

		if (i == 0)
		{
			strcpy( iText->alltext, TempVal); // copy
			strcat( iText->alltext, " \n" );
		}
		else
		{
			strcat( iText->alltext, TempVal); // concatenate
			strcat( iText->alltext, " \n" );
		}
	}

	iText->position = 0;


	return iError;
}



int  mcureadplc(

		char * varName,
		char * newValue

)
{

	int iError = MCU_OK;

	if (g_MCUS == NULL || g_MCUD == NULL)
	{
		return -1; // DOTX not initialized
	}


	text_struct * text_input;
	// allocate max necessary memory (for filters 21 * max size of line)
	text_input = (text_struct*) calloc( 1, sizeof(text_struct) );
	text_input->alltext  = (char*) calloc( 21*MAXPARCHAR, sizeof(char) );

	if ( strcmp( varName, "GearboxRatio" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "GearboxRatio" );
		iError += par_readline_txt_d ( text_input, &g_MCUS->iGB      , "GearboxRatio" );
	}

	if ( strcmp( varName, "ToptCoef" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "ToptCoef" );
		iError += par_readline_txt_d ( text_input, &g_MCUS->ToptCoef , "ToptCoef"     );
	}

	if ( strcmp( varName, "OmegaMax" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "OmegaMax" );
		iError += par_readline_txt_d ( text_input, &g_MCUS->Wmax     , "OmegaMax"     );
	}

	if ( strcmp( varName, "OmegaMin" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "OmegaMin" );
		iError += par_readline_txt_d ( text_input, &g_MCUS->Wmin     , "OmegaMin"     );
	}

	if ( strcmp( varName, "ToptCurveN" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "ToptCurveN" );
		iError += par_readline_txt_i ( text_input, &g_MCUS->ToptCurveN           , "ToptCurveN"          );
	}

	if ( strcmp( varName, "ToptCurveOmg" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "ToptCurveOmg" );
		iError += par_readline_txt_d ( text_input,  g_MCUS->ToptCurveOmg         , "ToptCurveOmg"        );
	}

	if ( strcmp( varName, "ToptCurveTor" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "ToptCurveTor" );
		iError += par_readline_txt_d ( text_input,  g_MCUS->ToptCurveTor         , "ToptCurveTor"        );
	}


	if ( strstr( varName, "RotSp_Pit_" ) != NULL ) //  ARRAY
	{
		iError += read_mita_dbarray    ( text_input, "RotSp_Pit_" );
		iError += par_readfilt_txt_series(
				text_input,
				g_MCUD->RotSpd_Pit, g_MCUS->RotSpd_Pit_damp, g_MCUS->Ts, "RotSp_Pit_",
				N_HPF_FILTERS, N_NFP_FILTERS, N_NFF_FILTERS, N_LPF_FILTERS
		);
	}

	if ( strstr( varName, "RotSp_Tor_" ) != NULL )
	{
		iError += read_plc_array    ( text_input, "RotSp_Tor_" );
		iError += par_readfilt_txt_series(
				text_input,
				g_MCUD->RotSpd_Tor, g_MCUS->RotSpd_Tor_damp, g_MCUS->Ts, "RotSp_Tor_",
				N_HPF_FILTERS, N_NFP_FILTERS, N_NFF_FILTERS, N_LPF_FILTERS
		);
	}

	if ( strcmp( varName, "RotSp_LPSCHED" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "RotSp_LPSCHED" );
		iError += par_readfilt_txt_fxd ( text_input,  g_MCUD->RotSpd_SCHED , "RotSp_LPSCHED" , T_LOWPASS , g_MCUS->Ts );
	}

	if ( strcmp( varName, "RotSp_LPFDBCK" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "RotSp_LPFDBCK" );
		iError += par_readfilt_txt_fxd ( text_input,  g_MCUD->RotSpd_FDBCK , "RotSp_LPFDBCK" , T_LOWPASS , g_MCUS->Ts );
	}

	if ( strcmp( varName, "Power_LPF" ) == 0 )
	{
		iError += read_plc_var    ( text_input, "Power_LPF" );
		iError += par_readfilt_txt_fxd ( text_input,  g_MCUD->Power_LPF    , "Power_LPF"     , T_LOWPASS , g_MCUS->Ts );
	}

	if ( strcmp( varName, "Pitch_LPF" ) == 0 )
	{
		iError += read_mita_dbvar    ( text_input, "Pitch_LPF" );
		iError += par_readfilt_txt_fxd ( text_input,  g_MCUD->Pitch_LPF    , "Pitch_LPF"     , T_LOWPASS , g_MCUS->Ts );
	}


	mita_free( text_input->alltext );
	mita_free( text_input );
	return iError;

}
