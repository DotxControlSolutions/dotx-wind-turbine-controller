/* ---------------------------------------------------------------------------------
 *          file : logdata.c                                                      *
 *   description : C-source file, log functions for the MCU signals               *
 *       toolbox : DotX Wind Turbine Control Software                             *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */


#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_external.h"

#include "./../suplib/suplib.h"

#include "./logdata.h"

#include "./sqlite3.h"
#include "logstrings.h"

/* ---------------------------------------------------------------------------------
    Main call of the log functionaility.
--------------------------------------------------------------------------------- */
int logdata(



        const REAL   * pData      ,
        const int      iStatus    ,
        const int      iDevice    ,
              char   * cMessage   ,
        const char   * cSimID     ,
        const char   * cOutputDir ,
        const int      iActive

) {

    int k;
    int iError = MCU_OK;
    
    static char cLog[200];
//    static FILE * fidLog;
    static sqlite3 *db;
    static sqlite3_stmt* stmt;
    static int flag = 0;
	int sqlerror;
	char stmt_text[100];
	char stmt_large[100000];
	char num_buffer[40];
	int size_buffer;

	// Create Table Chars
	char table_text_1 [] = "CREATE TABLE IF NOT EXISTS ";
	char table_text_2 [] = " ( ";
	char table_text_3 [] = " FLOAT );";
	char table_text_4 [] = " FLOAT, ";
	char table_text_5 [] = "ALTER TABLE ";
	char table_text_6 [] = " ADD COLUMN ";
	char table_text_7 [] = " FLOAT;";

	// Insert Values Chars
	char insert_text_1 [] = "INSERT INTO ";
	char insert_text_2 [] = " ( ";
	char insert_text_3 [] = " ) VALUES ( ";
	char insert_text_4 [] = " );";
    
    if ( !iActive ) return iError;
    
    switch (iStatus) {
    
        case MCU_STATUS_INIT: {
        
            strcpy( cLog, cOutputDir );
            if (iDevice != BLADED) strcat( cLog, cSimID );
            strcat( cLog, ".LOG" );

            // Initialize SQLite
            sqlite3_initialize();
			// Open Database
			int error = sqlite3_open_v2( cLog, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL );
			if (error)
			{
				sqlite3_close(db);
				strcat( cMessage, "[mcu]  <err> cannot open logfile <");
				strcat( cMessage, cLog );
				strcat( cMessage, ">\t\n" );
				return MCU_ERR;
			}

			strcat( cMessage, "[mcu]  Logging simulation data to <" );
			strcat( cMessage, cLog );
			strcat( cMessage, ">\t\n" );

			// Create First Statement
			strcpy(stmt_text, table_text_1); // Copy CreateTable
			strcat(stmt_text, "DATA");         // Cat Tablename
			strcat(stmt_text, table_text_2); // Cat Parenthesis
			strcat(stmt_text, tableNames[0]);// Cat First ColumnName
			strcat(stmt_text, table_text_3); // Cat type and End of Stmt
			// Prepare First Statement
			sqlerror = sqlite3_prepare_v2(db,stmt_text,-1,&stmt,NULL);
			if (sqlerror != SQLITE_OK)
			{
				sqlite3_close(db);
				strcat( cMessage, "[mcu]  <err> cannot prepare stmt for table creation <");
				strcat( cMessage, stmt_text );
				strcat( cMessage, ">\t\n" );
				return MCU_ERR;
			}
			// Execute First Statement
			sqlerror = sqlite3_step(stmt);
			if (sqlerror != SQLITE_DONE)
			{
				sqlite3_close(db);
				strcat( cMessage, "[mcu]  <err> cannot execute stmt for table creation <");
				strcat( cMessage, stmt_text );
				strcat( cMessage, ">\t\n" );
				return MCU_ERR;
			}
			// Finalize First Statement
			sqlite3_finalize(stmt);

			for ( k = 1; k < MAXLOG; ++k ) // Starts from 1 because we already added the first one
			{
				// Clear Char Arrays
				memset( stmt_text, 0, sizeof(stmt_text) );
				// Create Loop Statement
				strcpy(stmt_text, table_text_5); // Copy AlterTable
				strcat(stmt_text, "DATA");       // Cat Tablename
				strcat(stmt_text, table_text_6); // Cat AddColumn
				strcat(stmt_text, tableNames[k]);// Cat k-th ColumnName
				strcat(stmt_text, table_text_7); // Cat type and End of Stmt
				// Prepare Loop Statement
				sqlerror = sqlite3_prepare_v2(db,stmt_text,-1,&stmt,NULL);
				if (sqlerror != SQLITE_OK)
				{
					sqlite3_close(db);
					strcat( cMessage, "[mcu]  <err> cannot prepare stmt for table loop creation <");
					strcat( cMessage, stmt_text );
					strcat( cMessage, ">\t\n" );
					return MCU_ERR;
				}
				// Execute Loop Statement
				sqlerror = sqlite3_step(stmt);
				if (sqlerror != SQLITE_DONE)
				{
					sqlite3_close(db);
					strcat( cMessage, "[mcu]  <err> cannot execute stmt for table loop creation <");
					strcat( cMessage, stmt_text );
					strcat( cMessage, ">\t\n" );
					return MCU_ERR;
				}
				// Finalize Loop Statement
				sqlite3_finalize(stmt);

			}

			// Create large prepared statement to reset
			strcpy(stmt_large, insert_text_1); // Copy InsertInto
			strcat(stmt_large, "DATA");        // Concatenate Tablename
			strcat(stmt_large, insert_text_2); // Concatenate Parenth

			for ( k = 0; k < MAXLOG; ++k ) // Starts from 1 because we already added the first one
			{
				strcat(stmt_large, tableNames[k]); // Concatenate k-th ColumnName
				if ( k < MAXLOG -1 )
				{
					strcat(stmt_large, ", ");
				}
			}
			strcat(stmt_large, insert_text_3); // Concatenate Values text
			for ( k = 0; k < MAXLOG; ++k ) // Starts from 1 because we already added the first one
			{
				strcat(stmt_large, "?");    // Cat Token Value
				if ( k < MAXLOG -1 )
				{
					strcat(stmt_large, ", ");
				}
			}
			strcat(stmt_large, insert_text_4); // Cat End of Statement

			// Prepare Large Statement
			sqlerror = sqlite3_prepare_v2(db,stmt_large,-1,&stmt,NULL);
			if (sqlerror != SQLITE_OK)
			{
				sqlite3_close(db);
				strcat( cMessage, "[mcu]  <err> cannot prepare stmt for inserting values <");
				strcat( cMessage, stmt_large );
				strcat( cMessage, ">\t\n" );
				return MCU_ERR;
			}

            break;
        }
        
		// -----------------------------------------------------------------------------------
        case MCU_STATUS_RUN: {

        	if (flag == 0)
        	{
				// Bind parameters to Reseted Large Statement
				for ( k = 0; k < MAXLOG; ++k )
				{
					sqlerror = sqlite3_bind_double(stmt,k+1,(double)pData[k]);
					if (sqlerror != SQLITE_OK)
					{
						strcat( cMessage, "[mcu]  <err> cannot BIND stmt for inserting values <");
						strcat( cMessage, ">\t\n" );
						return MCU_ERR;
					}
				}
				// Execute Large Statement
				sqlerror = sqlite3_step(stmt);
				if (sqlerror != SQLITE_DONE)
				{
					sqlite3_close(db);
					strcat( cMessage, "[mcu]  <err> cannot EXECUTE stmt for inserting values  <");
					strcat( cMessage, ">\t\n" );
					return MCU_ERR;
				}
				// Reset Statement
				sqlite3_reset(stmt);
        	}

			// Increase flag
			flag += 1;

			// Reset flag <--------------------
        	if (flag > 0)
        	{
        		flag = 0;
        	}


        
            break;
        }
        
        case MCU_STATUS_EXIT: {
        
            //fclose( fidLog );
        	sqlite3_finalize(stmt);
        	sqlite3_close(db);
        
            break;
        }
    }

    return iError;
    
} /* end logdata() */
