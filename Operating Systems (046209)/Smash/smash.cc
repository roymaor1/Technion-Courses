/**************************smash.c**********************************/
	/* main file. This file contains the main function of smash */
/*******************************************************************/

// Includes:

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "commands.h"
#include "signals.h"

/*******************************************************************/

// Constants:

#define MAX_LINE_SIZE 80
#define MAXARGS 20

/*******************************************************************/

// Globals:

smashProcess sp;
char lineSize[MAX_LINE_SIZE];
pid_t Current_Fg_Pid;
int Next_Available_Job_Id;

/*******************************************************************/

// Functions:


// function name: main
// Description: main function of smash. get command from user and calls command functions
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];

	//signal declarations:
	if (SIGNALS_setSignalHandlers() == -1)
	{
		cout << "Error setting signal handlers" << endl;
		exit(1);
	}

	// Init globals 
	Current_Fg_Pid = -1;
	Next_Available_Job_Id = 1;

    while (1)
    {
	 	cout << "smash > ";
	 	cin.getline(lineSize, MAX_LINE_SIZE, '\n');
	 	strcpy(cmdString, lineSize);
		// perform a complicated Command
		if(!ExeComp(cmdString)) continue;
		// background command
	 	if(!BgCmd(lineSize, cmdString)) continue;
		// built in commands
		ExeCmd(lineSize, cmdString);
	}
    return 0;
}

