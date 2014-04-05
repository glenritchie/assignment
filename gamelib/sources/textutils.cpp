#include "textutils.h"
#include <string>

#include <fstream>

using namespace std;


void eatWhiteSpace(std::string &line)
{
	if (line.empty())
		return;
	// Eat whitespace at the beginning of the line
	int offset = 0;
	unsigned int i = 0;
	for (i = 0; i < line.length(); i++)
	{
	  if (line[i] == '\t' || line[i] == ' ')
		  continue;
	  break;
	}
	line = line.substr(i);
	if (line.empty())
		return;
	// Now eat whitepace at the end of the line as well
	for (i = line.length()-1; i > 0; i--)
	{
	  if (line[i] == '\t' || line[i] == ' ')
		continue;
	  break;
	}
	line = line.substr(0, i+1);
}

using namespace std;
void readline(ifstream &stream, string &line, unsigned int &lineNumber)
{
	bool commentBlock = false;
	do
	{
		getline(stream, line);
		lineNumber++;	
		if (stream.bad())				 	return;
		if (stream.eof())					return;
		eatWhiteSpace(line);
		if (line.find("//") == 0)			continue;
		if (line.find("/*") == 0)	
		{
			commentBlock = true;
			continue;
		}
			
		if (line.find("*/") == 0)	
		{	
			commentBlock = false;
			continue;
		}
		if (line.size() == 0) 				continue;
		if (commentBlock) continue;
		break;
	}while(1);
}
