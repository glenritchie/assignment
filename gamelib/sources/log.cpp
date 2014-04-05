/*
	Filename:		log.cpp
	Description:	Log file for CLog class implementation
	Author:			Spikeles
	Last Modified:	20/11/2003 12:33am

	History:				None
*/

#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>							// Include for time functions
#include <errno.h>
#include <memory.h>
#include "utils.h"
// One global Log Instance
// TODO: Do proper singleton?
//CRITICAL_SECTION CriticalSection;
//using namespace std;
bool onceC = false;
bool onceD = false;
CLog Log("log/gamelog.txt");
// Constructor
// Input:	Ptr to filename to load. Must be NON-NULL
CLog::CLog(const char *filename)
{
//	OutputDebugString("CLog::Clog()");
//	InitializeCriticalSection(&CriticalSection);
						  
	logfile = 0;			// Initialize ptrs to zero
	setFile(filename);	
}
void CLog::setFile(const char *file)
{
	// Use memcpy instead of strcpy to avoid buffer overflow problems
	// If we use strcpy there is the potential that filename could be more than MAX_LOGFILENAME_SIZE long
	// So we insist on only copying the max chars we want.
	if (logfile != 0)
		fclose(logfile);
	memcpy(logfilename, file, MAX_LOGFILENAME_SIZE);		
	
	Init();					// Init the log file
}
CLog::~CLog()
{
//	OutputDebugString("CLog::~Clog()");
	char msg[200];
	
	// Output time/date of end of log
	time_t t;
	time(&t);
	sprintf(msg,"\nLog Ended: %s\n", ctime(&t));
	Output(msg);
	
	// Close the file stream
	if (logfile)
		fclose(logfile);
//	DeleteCriticalSection(&CriticalSection);
}

// Init
// Input: 	None
// Output:	Returns a boolean value of true if successfully initialized else returns false
bool CLog::Init()
{
	char msg[2000];
	
	// Open the file stream
	logfile = fopen(logfilename ,"w");
	if (!logfile)								// If an error occurred
	{
		char *reasonMsg = strerror(errno);
		printf("Log file '%s' could not be opened: Reason: %s\n", this->logfilename, reasonMsg);
		return false;							// Exit
	}
	
	// Turn OFF buffering so output is written instantly
	// This way if the program crashes the last msg will be there
	setvbuf( logfile, NULL, _IONBF, 0 );

	// Output time/date of start of log
	// Figure out the time
	time_t t;
	time(&t);
	sprintf(msg, "Log started: %s\n", ctime(&t));
	Output(msg);
	return true;
}

// Output
// Input:	String to output. Must be NON-NULL
void CLog::Output(const char *msg)
{
	if (!logfile)			// If the log file faile
		if (!Init())		// Try to initialize it again
			return;			// If that failed then exit
	char m = msg[0];

	fprintf(logfile,msg);	// Print out the msg to the file
	return;								
}
