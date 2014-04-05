/*
	Filename:		log.h
	Description:	Log Header file for CLog class declaration
	Author:			Spikeles
	Last Modified:	20/11/2003 12:26AM


	History:		
			11/04/2004 12:54PM	- Glen - Added javadoc style comments
*/

#ifndef _LOG_H_									// Only include this file once
#define _LOG_H_

#include <stdio.h>								// Include standard header files
#define MAX_LOGFILENAME_SIZE  255
/** 
 * A simple class to handle a file like a log.
 * @author Glen Ritchie
*/
class CLog
{
public:
	/** Constructor
	 * @param filename Ptr to filename to load. Must be NON-NULL
	 */
	CLog(const char *filename);
  
	/** Destructor */
	~CLog();													

	/** Set the log to use a specific file
	 * @param file Ptr to filename to load. Must be NON-NULL
	 */
	void setFile(const char *file);

	/** Output a string
	 * @param string String to output. Must be NON-NULL
	 */
	void Output(const char *string);	

	/** Get the name of the log file
	 * @return A constant ptr to the filename
	 */
	const char *getFilename() 
	{
		return logfilename;
	}
private:

 	/** Initialize the file
 	 * @return Returns a boolean value of true if successfully initialized else returns false
	 */
 	bool Init();

	/** Ptr to FILE instance */
	FILE *logfile;
	/** Storage for logfilename */
	char logfilename[MAX_LOGFILENAME_SIZE];		
};
extern CLog Log;

#endif //_LOG_H_
