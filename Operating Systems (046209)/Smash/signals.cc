// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "signal.h"
#include <sys/wait.h>
#include "commands.h"


extern smashProcess sp;
extern pid_t Current_Fg_Pid;
static bool handleStatus(int stat_val, pid_t pid);
static bool isReadyToRemove(list<Job>::iterator j, int options);
static void signal_sent_display(pid_t pid, int signum);
static string get_signal_string(int signum);

// function name: 	SIGNALS_setSignalHandlers
// description:   	Set the signal handlers using the signal function for SIGCHLD, SIGINT and SIGTSTP
int SIGNALS_setSignalHandlers(void)
{
	//SIGCHLD
	if (signal(SIGCHLD, SIGNALS_sigChldHandler) == SIG_ERR)
	{
		perror("Error on signal");
		return -1;
	}

	//SIGINT - CTRL+C
	if (signal(SIGINT,&catch_int) == SIG_ERR)
	{
		perror("Error on signal");
		return -1;
	}

	//SIGTSTP - CTRL+Z
	if (signal(SIGTSTP,&catch_tstp) == SIG_ERR)
	{
		perror("Error on signal");
		return -1;
	}
	return 0;
}

// function name: 	catch_tstp
// description:   	Handle the SIGTSTP signal (Ctrl+Z)
void catch_tstp(int sig_num){ //for CTRL+Z
	if(Current_Fg_Pid == -1){
		return;
	}
	//send SIGTSTP to current process on fg
	list<Job>::iterator fg_job = sp.findJobByPID(Current_Fg_Pid);
	if(!SIGNALS_kill(Current_Fg_Pid,SIGTSTP)){
		return;
	}
	fg_job->is_suspended = true;
	return;
}

// function name: 	catch_int
// description:   	Handle the SIGINT signal (Ctrl+C)
void catch_int(int sig_num){ //for CTRL+C
	if(Current_Fg_Pid == -1){
		return;
	}
	//send SIGINT to current process on fg
	list<Job>::iterator fg_job = sp.findJobByPID(Current_Fg_Pid);
	SIGNALS_kill(Current_Fg_Pid,SIGINT);
}

// function name: 	handleStatus
// description:   	After waitpid is returned, see the stat_val variable to handle the different options
static bool handleStatus(int stat_val, pid_t pid)
{
	list<Job>::iterator j;
	if (WIFEXITED(stat_val) || WIFSIGNALED(stat_val))
		return true;
	else if (WIFSTOPPED(stat_val))
	{
		list<Job>::iterator j;
		j = sp.findJobByPID(pid);
		j->is_suspended = true;
		if (time(&j->suspension_time.tv_sec) == -1)
		{
			perror("time function failed");
		}
	}
	else if (WIFCONTINUED(stat_val))
	{
		list<Job>::iterator j;
		j = sp.findJobByPID(pid);
		j->is_suspended = false;
	}
	return false;

}

// function name: 	SIGNALS_kill
// description:   	A wrapper function to kill system call, which displays the signal sent message and prints error on kill system call
bool SIGNALS_kill(pid_t pid, int signum)
{
	// We chose not to support pid 0 or -1 cases
	// (special cases in which signal is to be sent to all processes)
	// This function is only called when trying to kill a job,
	// which has a pid which is not 0 or -1.
	if ((pid == -1) || (pid == 0))
	{
		return false;
	}
	signal_sent_display(pid, signum);

	if (kill(pid, signum) == -1)
	{
		perror("kill failed");
		return false;
	}
	return true;
}

// function name: 	SIGNALS_sigChldHandler
// description:   	A handler to the SIGCHLD signal, which cleans the background running jobs from job list when done
//					The current foreground process will be handled in SIGNALS_waitpid
void SIGNALS_sigChldHandler(int sig_num)
{
	for (list<Job>::iterator j = sp.jobs.begin(); j != sp.jobs.end() && !sp.jobs.empty(); ++j)
	{
		if (j->pid != Current_Fg_Pid)
		{
			if (isReadyToRemove(j, WUNTRACED | WNOHANG | WCONTINUED))
			{
				j = sp.jobs.erase(j);
				j--;
			}
		}
	}
}

// function name: 	isReadyToRemove
// description:   	A function which helps determine if a job is done running, and needs to be removed from job list.
static bool isReadyToRemove(list<Job>::iterator j, int options)
{
	int stat_val;
	bool ret = false;
	pid_t returned_pid = waitpid(j->pid, &stat_val, options);
	if (returned_pid == -1)
	{
		perror("Error on waitpid");
	}
	else if (returned_pid == j->pid)
	{
		ret = handleStatus(stat_val, j->pid);
	}
	return ret;
}

// function name: 	SIGNALS_waitpid
// description:   	A waitpid wrapping function, which also restarts the Current_Fg_Pid global variable and removes the foreground job from job list
void SIGNALS_waitpid(list<Job>::iterator j, int options)
{
	if (isReadyToRemove(j, options))
	{
		j = sp.jobs.erase(j);
	}
	Current_Fg_Pid = -1;
}

// function name: 	signal_sent_display
// description:		Display the signal sent message
static void signal_sent_display(pid_t pid, int signum)
{
	cout << "signal " << (get_signal_string(signum)) << " was sent to pid " << pid << endl;
}
// function name: 	get_signal_string
// description:		given a signum, return its string. return "" for unknown signals.
static string get_signal_string(int signum)
{
	switch (signum)
	{
		case SIGHUP:
			return "SIGHUP";
		case SIGINT:
			return "SIGINT";
		case SIGQUIT:
			return "SIGQUIT";
		case SIGILL:
			return "SIGILL";
		case SIGTRAP:
			return "SIGTRAP";
		case SIGABRT:
			return "SIGABRT";
		case SIGFPE:
			return "SIGFPE";
		case SIGKILL:
			return "SIGKILL";
		case SIGUSR1:
			return "SIGUSR1";
		case SIGSEGV:
			return "SIGSEGV";
		case SIGUSR2:
			return "SIGUSR2";
		case SIGPIPE:
			return "SIGPIPE";
		case SIGALRM:
			return "SIGALRM";
		case SIGTERM:
			return "SIGTERM";
		case SIGSTKFLT:
			return "SIGSTKFLT";
		case SIGCONT:
			return "SIGCONT";
		case SIGSTOP:
			return "SIGSTOP";
		case SIGTSTP:
			return "SIGTSTP";
		case SIGTTIN:
			return "SIGTTIN";
		case SIGTTOU:
			return "SIGTTOU";
		case SIGURG:
			return "SIGURG";
		case SIGXCPU:
			return "SIGXCPU";
		case SIGXFSZ:
			return "SIGXFSZ";
		case SIGVTALRM:
			return "SIGVTALRM";
		case SIGPROF:
			return "SIGPROF";
		case SIGWINCH:
			return "SIGWINCH";
		case SIGIO:
			return "SIGIO";
		case SIGPWR:
			return "SIGPWR";
		case SIGSYS:
			return "SIGSYS";
	}
	return "";
}

