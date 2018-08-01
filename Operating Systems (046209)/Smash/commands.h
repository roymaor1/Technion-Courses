#ifndef _COMMANDS_H
#define _COMMANDS_H

/************************C Includes:********************************/
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

/************************SYS Includes:*****************************/
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

/************************CPP Includes:********************************/
#include <iostream>
#include <string>
#include <list>
#include <vector> //use vector instead of stack so iterating is easy

/************************Namespaces (CPP):****************************/
using namespace std;

/************************Defines:************************************/
#define MAX_LINE_SIZE 80
#define MAX_HISTORY_SIZE 50
#define MAX_ARG 20

/************************Global_Variables:***************************/
extern int Next_Available_Job_Id;

/************************Classes:************************************/
// Job class
// This class contains job parameters and methods
class Job
{
	public:
		int id;
		pid_t pid;
		string name;
		struct timeval time;
		struct timeval suspension_time;
		bool is_suspended;


		//constructor
		Job(int my_pid, string my_name, bool suspended)
		{
			id = Next_Available_Job_Id;
			Next_Available_Job_Id ++;
			pid = my_pid;
			name = my_name;
			gettimeofday(&time, NULL);
			is_suspended = suspended;
		}

		// no need for dtor. default is enough

		//methods:
		void printJob()
		{
			struct timeval current_time;
			gettimeofday(&current_time, NULL);

			cout << "[" << this->id << "] " << this->name << " : " << this->pid << " " << (current_time.tv_sec - this->time.tv_sec) << " secs";
			if (is_suspended)
			{
				cout << " (Stopped)";
			}
			cout << endl;
		}

};

// smashProcess class
// This class contains the job list, history of commands and their methods.
class smashProcess{
public:
	int id;
	list<Job> jobs;
	vector<string> history;
	string lwd; //last working directory
	string cwd; //current working directory

	//constructor
	smashProcess(){
		id = getpid();
		jobs.clear();
		history.clear();
		char workDir[MAX_LINE_SIZE];
		getcwd(workDir,MAX_LINE_SIZE);
		cwd = workDir;
		lwd = cwd;
	}

	//destructor
	~smashProcess()
	{
		if(!jobs.empty())
		{
			for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j)
			{
				j = jobs.erase(j);
				j--;
			}
		}
	}

	//methods
	void setLWD(){ //to be called before setCWD
		lwd = cwd;
	}
	void setCWD(string path){
		cwd = path;
	}
	void addToHistory(string cmd){ //newest in index 0, oldest in last
		if(history.size() == MAX_HISTORY_SIZE)
			history.erase(history.begin());
		history.push_back(cmd);
	}

	list<Job>::iterator findJobByPID(pid_t pid)
	{
		for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j)
		{
			if (j->pid == pid)
			{
				return j;
			}
		}
		return jobs.end();
	}

	list<Job>::iterator findJobById(int id)
	{
		for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j)
		{
			if (j->id == id)
			{
				return j;
			}
		}
		return jobs.end();
	}

	list<Job>::iterator findLatestJob(void)
	{
		time_t min_time = jobs.begin()->time.tv_sec;
		list<Job>::iterator job = jobs.begin();
		for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j)
		{
			if (j->time.tv_sec < min_time)
			{
				min_time = j->time.tv_sec;
				job = j;
			}
		}
		return job;
	}
	list<Job>::iterator findLatestSuspendedJob(void)
	{
		time_t largest_suspension_time; // meaning newest job to be suspended
		list<Job>::iterator job = jobs.begin();

		for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j)
		{
			if(!j->is_suspended)
				continue;

			if (j->suspension_time.tv_sec > largest_suspension_time)
			{
				largest_suspension_time = j->time.tv_sec;
				job = j;
			}
		}
		return job;
	}

};

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, char* cmdString);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], string cmdString);
#endif

