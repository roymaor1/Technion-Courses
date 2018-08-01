//
// Created by Roy on 25/04/2018.
//
/********************************************/
/*				commands.cc					*/
/********************************************/

/********************************************/

// Includes:

#include "commands.h"
#include "signals.h"

#include <string>

/********************************************/

// Constants:

typedef enum EXECUTION_MODE
{
	BG_EXEC_MODE,
	FG_EXEC_MODE,

} EXECUTION_MODE;

typedef enum RET_CODE
{
	SUCCESS=0,
	FAILURE=-1,
} RET_CODE;


typedef enum ERROR_CODE
{
	ERR_NONE,
	ERR_INVALID_PARAM,
	ERR_INVALID_PATH,
	ERR_INVALID_JOB,
	ERR_KILL_FAILED,
	ERR_MV_FAILED,
	ERR_WAITPID_FAILED,
} ERROR_CODE;


/********************************************/

// Static function declarations:
static ERROR_CODE handlePwd(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleCd(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleHistory(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleJobs(int num_arg);
static ERROR_CODE handleKill(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleShowPid(int num_arg);
static ERROR_CODE handleFg(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleBg(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleQuit(char *args[MAX_ARG], int num_arg);
static ERROR_CODE handleMv(char *args[MAX_ARG], int num_arg);

static void extractArgs(const char* delimiters, char* lineSize, char *args[MAX_ARG], int *num_args);
static void execute_command(char* args[MAX_ARG], EXECUTION_MODE exec_mode, bool is_complicated);
static bool isCmdComplicated(string cmd);
static bool errorHandler(ERROR_CODE err ,char* cmdString);
static bool isBgCmd(string cmd);
static bool is_number(const std::string& s);

/********************************************/

// External Variables:
extern smashProcess sp;
extern pid_t Current_Fg_Pid;

/********************************************/

// Macros:

#define DISPLAY_ERROR(cmdString) cout << "smash error: > \"" << cmdString << "\"" << endl
#define DISPLAY_PATH_NOT_FOUND_ERROR(path) cout << "smash error: > \"" << path << "\" - path not found" << endl
#define DISPLAY_KILL_INVALID_JOB(job_id) cout << "kill " << job_id << " - job does not exist" << endl
#define DISPLAY_FG_INVALID_JOB(job_id) cout << "Job not found" << endl
#define DISPLAY_KILL_FAILED(job_id) cout << "kill " << job_id << " - cannot send signal" << endl

/********************************************/
// function name: 	errorHandler
// description:   	handle the different errors which can come up during executing commands.
// 					returns true if no error was found.
static bool errorHandler(ERROR_CODE err ,char* cmdString)
{
	switch (err)
	{
		case ERR_INVALID_PARAM:
		{
			DISPLAY_ERROR(cmdString);
			return false;
		}
		case ERR_INVALID_PATH:
		case ERR_INVALID_JOB:
		case ERR_KILL_FAILED:
		case ERR_MV_FAILED:
		case ERR_WAITPID_FAILED:
		{
			return false;
		}

		case ERR_NONE:
			return true;
	}
	return false;

}

// function name: isCmdComplicated
// description:   this function gets a command string and returns true if it is considered "complicated",
// 				  or false if it isn't.
static bool isCmdComplicated(string cmd){
	if (cmd.find("|")!=string::npos ||
		cmd.find("<")!=string::npos ||
		cmd.find(">")!=string::npos ||
		cmd.find("*")!=string::npos ||
		cmd.find("?")!=string::npos ||
		cmd.find(">>")!=string::npos||
		cmd.find("|&")!=string::npos)
		return true;
	return false;
}

// function name: 	isBgCmd
// description: 	this function gets a command string and returns true if it is considered "background",
// 					or false if it isn't.
static bool isBgCmd(string cmd)
{
	if (cmd.length() == 0)
		return false;
	if (cmd.at(cmd.length()-1) == '&')
	{
		return true;
	}
	return false;
}

// function name: 	execute_command
// description: 	this function gets an args array,
// 					execution mode flag and an is_complicated flag
// 					to determine where the name of the command is in the args array.
// 					This function creates a child process and executes the external command in it.
// 					In the father process, the command is pushed to the job list.
// 					When the child process is done, the job is cleaned from the job list by SIGNALS_waitpid.
static void execute_command(char* args[MAX_ARG], EXECUTION_MODE exec_mode, bool is_complicated)
{
	pid_t pID;
	switch(pID = fork())
	{
		case -1:
		{
			perror("Error: fork");
			return;
		}
		case 0 :
		{
			// Child Process
			setpgrp();
			// Execute an external command
			if (execvp(args[0], args))
			{
				perror("Error Executing External Command");
			}
			exit(1);
		}

		default:
		{
			int name_index = 0;
			if (is_complicated)
				name_index = 3;
			// Add to jobs
			sp.jobs.push_back(Job(pID, args[name_index], false));
			list<Job>::iterator j = sp.jobs.end();
			j--;
			// Wait if running in FG
			if (exec_mode == FG_EXEC_MODE)
			{
				// Set the Global variable to current pid:
				Current_Fg_Pid = pID;
				SIGNALS_waitpid(j, WUNTRACED);
			}
		}

	}
}

// function name: 	handlePwd
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the pwd command (displays the current working directory and returns errors if there are any).
static ERROR_CODE handlePwd(char *args[MAX_ARG], int num_arg)
{
	if (num_arg != 1)
	{
		return ERR_INVALID_PARAM;
	}
	cout << sp.cwd << endl;
	return ERR_NONE;
}

// function name: 	handleCd
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the cd command (change the current working directory and return errors if there are any).
static ERROR_CODE handleCd(char *args[MAX_ARG], int num_arg)
{
	int comma = 0;
	if (num_arg != 2)
	{
		return ERR_INVALID_PARAM;
	}

	string path = args[1];
	string next_path;
	if (!strcmp(path.c_str(), "-"))
	{
		path = sp.lwd;
		comma = 1;
	}

	if(chdir(path.c_str()))
	{
		DISPLAY_PATH_NOT_FOUND_ERROR(path);
		return ERR_INVALID_PATH;
	}
	if(comma == 1){
		cout << path << endl;
	}
	char current_path[MAX_LINE_SIZE];
	getcwd(current_path, MAX_LINE_SIZE);
	sp.setLWD();
	sp.setCWD(current_path);

	return ERR_NONE;

}

// function name: 	handleHistory
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the history command (display the commands history of smash, up to 50 commands back).
static ERROR_CODE handleHistory(char *args[MAX_ARG], int num_arg)
{
	if (num_arg != 1)
	{
		return ERR_INVALID_PARAM;
	}
	if(sp.history.empty())
		return ERR_NONE;
	for(std::vector<string>::iterator it = sp.history.begin(); it != sp.history.end(); ++it)
	{
		cout << *it << endl;
	}

	return ERR_NONE;
}

// function name: 	handleJobs
// description: 	this function gets num_arg which states the number of arguments.
//					It handles the jobs command (display the current job list, which contains the running in the background and suspended processes).
static ERROR_CODE handleJobs(int num_arg)
{
	if (num_arg != 1)
	{
		return ERR_INVALID_PARAM;
	}

	for (list<Job>::iterator j = sp.jobs.begin(); j != sp.jobs.end(); ++j)
	{
		j->printJob();
	}
	return ERR_NONE;
}

// function name: 	handleKill
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the kill command (send signal to a different process).
static ERROR_CODE handleKill(char *args[MAX_ARG], int num_arg)
{
	if (num_arg != 3)
	{
		return ERR_INVALID_PARAM;
	}
	int job_id = atoi(args[2]);
	list<Job>::iterator job = sp.findJobById(job_id);
	if (job== sp.jobs.end())
	{
		DISPLAY_KILL_INVALID_JOB(job_id);
		return ERR_INVALID_JOB;
	}
	int signum = atoi(args[1]+1);
	if (!SIGNALS_kill(job->pid, signum))
	{
		DISPLAY_KILL_FAILED(job_id);
		return ERR_KILL_FAILED;
	}
	return ERR_NONE;
}

// function name: 	handleShowPid
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the showpid command (show the pid of smash).
static ERROR_CODE handleShowPid(int num_arg)
{
	if (num_arg != 1)
	{
		return ERR_INVALID_PARAM;
	}
	cout << "smash pid is " << sp.id << endl;
	return ERR_NONE;
}

// function name: 	handleFg
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the fg command (move a job running in the background or suspended to the foreground).
//					If no arguments are given, the fg command moves the newest job to the foreground
static ERROR_CODE handleFg(char *args[MAX_ARG], int num_arg)
{
	if ((num_arg != 1) && (num_arg != 2))
	{
		return ERR_INVALID_PARAM;
	}

	if (num_arg == 2 && (!is_number(args[1])))
		return ERR_INVALID_PARAM;

	if (sp.jobs.empty())
		return ERR_NONE;

	list<Job>::iterator job_to_fg;
	if (num_arg == 1)
		job_to_fg = sp.findLatestJob();
	else
	{

		int id_to_fg = atoi(args[1]);
		job_to_fg = sp.findJobById(id_to_fg);
		if (job_to_fg == sp.jobs.end())
		{
			DISPLAY_FG_INVALID_JOB(id_to_fg);
			return ERR_INVALID_JOB;
		}
	}
	cout << job_to_fg->name << endl;

	Current_Fg_Pid = job_to_fg->pid;
	if (job_to_fg->is_suspended)
	{
		job_to_fg->is_suspended = false;
		if (!SIGNALS_kill(job_to_fg->pid, SIGCONT))
		{
			return ERR_KILL_FAILED;
		}
	}
	SIGNALS_waitpid(job_to_fg, WUNTRACED);
	return ERR_NONE;

}

// function name: 	handleBg
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the bg command (move a job running in the foreground to the background).
//					If no arguments are given, the bg command moves the job which has been suspended last to run in the background
static ERROR_CODE handleBg(char *args[MAX_ARG], int num_arg)
{
	if ((num_arg != 1) && (num_arg != 2))
	{
		return ERR_INVALID_PARAM;
	}

	if (num_arg == 2 && (!is_number(args[1])))
		return ERR_INVALID_PARAM;

	list<Job>::iterator job_to_bg;
	if (num_arg == 1)
	{
		job_to_bg = sp.findLatestSuspendedJob();
		if (job_to_bg == sp.jobs.end())
			return ERR_NONE;
	}
	else
	{
		int id_to_bg = atoi(args[1]);
		job_to_bg = sp.findJobById(id_to_bg);
		if (job_to_bg == sp.jobs.end())
		{
			DISPLAY_FG_INVALID_JOB(id_to_bg);
			return ERR_INVALID_JOB;
		}
	}
	cout << job_to_bg->name << endl;
	if (!SIGNALS_kill(job_to_bg->pid, SIGCONT))
	{
		return ERR_KILL_FAILED;
	}

	return ERR_NONE;
}

// function name: 	handleMv
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the mv command (rename a file from its old name to a new name, given as arguments).
static ERROR_CODE handleMv(char *args[MAX_ARG], int num_arg){
	if (num_arg != 3){
		return ERR_INVALID_PARAM;
	}
	string oldFileName = args[1];
	string newFileName = args[2];
	if(rename(oldFileName.c_str(),newFileName.c_str()) == -1){
		perror("mv failed");
		return ERR_MV_FAILED;
	}
	cout << oldFileName << " has been renamed to " << newFileName << endl;
	return ERR_NONE;

}
// function name: 	handleQuit
// description: 	this function gets an args array and num_arg which states the number of arguments.
//					It handles the quit and quit kill commands
//					If quit is called, the command exits smash (calling the smashProcess destructor to erase the job list memory).
//					If quit kill is called, the command sends SIGTERM to all processes running in the background, waits 5 seconds, and if they don't respond it sends SIGKILL.
static ERROR_CODE handleQuit(char *args[MAX_ARG], int num_arg)
{
	if ((num_arg != 1) && (num_arg != 2))
	{
		return ERR_INVALID_PARAM;
	}


	if (num_arg == 1)
	{
		// Note: deleting the list is done in smashProcess destructor
		exit(0);
	}

	// At this point, we know num_arg = 2
	if (strcmp(args[1], "kill")) // illegal command
	{
		return ERR_INVALID_PARAM;
	}
	signal(SIGCHLD, SIG_DFL);

	list<Job>::iterator j;
	for(j = sp.jobs.begin(); j != sp.jobs.end(); ++j)
	{
		cout << "Sending SIGTERM... ";
		if (kill(j->pid, SIGTERM))
		{
			perror("kill failed");
			return ERR_KILL_FAILED;
		}

		sleep(5);
		int stat;
		pid_t returned_pid = waitpid(j->pid, &stat, WNOHANG | WUNTRACED);
		if (returned_pid == -1)
		{
			perror("waitpid failed!");
			return ERR_WAITPID_FAILED;
		}
		else if (returned_pid == 0)
		{
			cout << "(5 seconds passed) ";

			cout << "Sending SIGKILL... ";
			if (kill(j->pid, SIGKILL))
			{
				perror("kill failed");
				return ERR_KILL_FAILED;
			}

		}
		cout << "Done." << endl;
		j = sp.jobs.erase(j);
		j--;
	}
	exit(0);

}


//********************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: command string
// Returns: 0 - success,-1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{

	char* args[MAX_ARG];
	string dels = " \t\n";
	const char* delimiters = dels.c_str();
	int num_arg;
	bool is_history = false;
	ERROR_CODE err = ERR_NONE;
	char* cmd_str = strtok(lineSize, delimiters);
		if (cmd_str == NULL)
			return FAILURE;
	args[0] = cmd_str;
	num_arg = 1;
	for (int i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
		{
			num_arg++;
		}

	}

	/*************************************************/
	/*						pwd						 */
	/*************************************************/
	if (!strcmp(cmd_str, "pwd"))
	{
		err = handlePwd(args, num_arg);
	}

	/*************************************************/
	/*						cd						 */
	/*************************************************/
	else if (!strcmp(cmd_str, "cd") )
	{
		err = handleCd(args, num_arg);
	}
	/*************************************************/
	/*						history					 */
	/*************************************************/
	else if (!strcmp(cmd_str, "history")) //history commands will not be listed
	{
		err = handleHistory(args, num_arg);
		is_history = true;
	}
	/*************************************************/
	/*						jobs					 */
	/*************************************************/
	else if (!strcmp(cmd_str, "jobs"))
	{
		err = handleJobs(num_arg);
	}
	/*************************************************/
	/*						kill					 */
	/*************************************************/
	else if (!strcmp(cmd_str, "kill"))
	{
		err = handleKill(args, num_arg);
	}

	/*************************************************/
	/*						showpid					 */
	/*************************************************/
	else if (!strcmp(cmd_str, "showpid"))
	{
		err = handleShowPid(num_arg);
	}
	/*************************************************/
	/*						fg						 */
	/*************************************************/
	else if (!strcmp(cmd_str, "fg"))
	{
		err = handleFg(args, num_arg);

	}
	/*************************************************/
	/*						bg						 */
	/*************************************************/
	else if (!strcmp(cmd_str, "bg"))
	{
		err = handleBg(args, num_arg);
	}
	/*************************************************/
	/*						quit					 */
	/*************************************************/
	else if (!strcmp(cmd_str, "quit"))
	{
		err = handleQuit(args, num_arg);
	}
	/*************************************************/
	/*						mv						 */
	/*************************************************/
	else if (!strcmp(cmd_str, "mv"))
	{
		err = handleMv(args, num_arg);
	}
	/*************************************************/
	else // external command
	{
		ExeExternal(args, cmdString);
		err = ERR_NONE;
	}
	if (!is_history) // do not add history command to history vector
		sp.addToHistory(cmdString);

	if (!errorHandler(err, cmdString))
		return FAILURE;

	return SUCCESS;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], string cmdString)
{
	execute_command(args, FG_EXEC_MODE, false);
}

//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	string ExtCmd(lineSize);
	if (isCmdComplicated(ExtCmd))
	{
		const char *args[MAX_ARG] = { "csh", "-f", "-c", lineSize, NULL};
		if (isBgCmd(ExtCmd))
		{
			execute_command((char**)args, BG_EXEC_MODE, true);
		}
		else
		{
			execute_command((char**)args, FG_EXEC_MODE, true);
		}
		sp.addToHistory(ExtCmd);
		return SUCCESS;

	}
	return FAILURE;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, char* cmdString)
{
	string cmd = lineSize;

	if (isBgCmd(cmd))
	{
		lineSize[cmd.length()-1] = '\0';
		string dels = " \t\n";
		const char* delimiters = dels.c_str();
		char *args[MAX_ARG];
		int num_arg;
		extractArgs(delimiters, lineSize, args, &num_arg);
		execute_command(args, BG_EXEC_MODE, false);
		sp.addToHistory(cmdString);
		return SUCCESS;

	}
	return FAILURE;
}

// function name: 	extractArgs
// description: 	this function extracts the arguments and number of arguments from the command string given delimiters to separate
static void extractArgs(const char* delimiters, char* lineSize, char *args[MAX_ARG], int *num_args)
{
	char *cmd = strtok(lineSize, delimiters);
	args[0] = cmd;
	int i;
	for (i = 1; i < MAX_ARG; ++i)
	{
		args[i] = strtok(NULL, delimiters);

		if (args[i] == NULL)
			break;
	}
	*num_args = i;
}

// function name: 	is_number
// description: 	this function determines if a string is a number or not
static bool is_number(const std::string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

