/* ---------------------------------------------------------------------------------
 *          file : signal_gui_commands.h                                          *
 *   description : C-header file, contains command definitions for the gui        *
 *       toolbox : DotX Wind Turbine Control Software (User Interface)            *
 *        author : DotX Control Solutions, www.dotxcontrol.com                    *
--------------------------------------------------------------------------------- */

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

// Enum of all possible commands ----------------------------------------------
enum
{
    GUI_DO_NOTHING=0,

// Commands from the GUI to the controller
    GUI_SERV_ECHO,      
    GUI_SERV_DATA,                  // The data indeces given as parameters to this command return data from the input array.
    GUI_SERV_DATA_CONT,             // Same as SERV_DATA but the data will be send back every interation withoud further notice. Sending -1 as parameter will switch it off. Sending different indexecs will switch off the previously given indexes.
    GUI_SERV_PAUSE,                 // Pause the simulation. Send a positive number to as a parameter to pause, send zero or negative to start again.
    
    GUI_CONTR_UPDATEPARAM = 100,    // Reload the controlers parameter file.	
    GUI_CONTR_SIMID,                // Request the simulation id.		
    GUI_CONTR_PARAMID,              // Request the parameter file id.	

// Commands from the controller to the GUI.
    GUI_CLIENT_CONNECT = 200,       // The server lets the client know it has connected.  
    GUI_CLIENT_SIMID,               // The simulation id is send to the GUI.		
    GUI_CLIENT_PARAMID,             // The id of the parameter file is send to the GUI.
    GUI_CLIENT_MESSAGE,             // The controller sends a text message to the GUI.
    
};

#endif
