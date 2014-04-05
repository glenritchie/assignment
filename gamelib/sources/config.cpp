#include "config.h"
#include "utils.h"
#include "textutils.h"
//#include <string.h>
#include <string>
#include <stdio.h>
using namespace std;
/******************************
Spikeles(26/11/02)

  1.	A class to access a file full of [Section]:[Key]:Value stuff
  2.	Ned to make it slightly more generic
  3.	NOTE: %%%%%%%%%%########^^^^^^^^ NOT UNIX FORMAT COMPATIBLE!!!! MUST USE dos2unix command!!!*******************
******************************/
//extern char config_dir[];
std::string AppConfig::globalConfigDirectory;
void AppConfig::setGlobalConfigDirectory(std::string newGlobalDir)
{
	globalConfigDirectory = newGlobalDir;
}

std::string AppConfig::getGlobalConfigDirectory()
{
	return globalConfigDirectory;
}


AppConfig::AppConfig()
{
	//SetConfigFile("config.ini");
}
AppConfig::~AppConfig()
{
	config_file.close();
}
void AppConfig::setLocalConfigDirectory(std::string newLocal)
{
	localConfigDirectory = newLocal;
}
void AppConfig::EatLine()
{
	localConfigDirectory = AppConfig::getGlobalConfigDirectory();
	char tmp='\0';
	while (tmp!='\n')
		config_file.get(tmp);
	// config_file.get(tmp);
}
void AppConfig::SetConfigFile(const char *configfile)
{
	// If a file is open, close the old one
	if (config_file.is_open())
		config_file.close();
	// Try to open file
	config_file.open(configfile);
	// If it isn't open	
	if (!config_file.is_open())
	{
//		DebugText(EMSG, "Failed to open %s", configfile);
		localConfigDirectory = AppConfig::getGlobalConfigDirectory();
		config_file.clear();
		char cpy[100];
		strcpy(cpy, configfile);
		// Try to load in config dir
		//std::string extConfig(configfile);
		std::string dir(localConfigDirectory);
		dir.append(cpy);

		// Add config dir to the filename
		config_file.open(dir.c_str());
		if (!config_file.is_open())
		{
			DebugText(ERROR_SEVERE, "Could not open config file: %s", configfile);
			return;
		}
		// File was loaded from config dir
//		DebugText(EMSG, "File loaded correctly");
	}
//	else
	//	DebugText(EMSG, "File opened %s", configfile);

	config_file.seekg(0, ios_base::end);
	filesize = config_file.tellg();
	config_file.seekg(0);
}

bool AppConfig::SeekToSection(const char *section)
{
	if (!config_file.is_open())
		return false;
	string findsection = string("[") + string(section) + string("]");
	string line;
	config_file.clear();
	config_file.seekg(0);
//	DebugText(EMSG, "Beginning search for section: %s, good(): %d, eof: %d", section, config_file.good(), config_file.eof());
	while (!config_file.eof() && config_file.good())
	{
		getline(config_file, line);
//		DebugText(EMSG, "Read line: %s", line.c_str());
		eatWhiteSpace(line);		
		if (line != findsection)
			continue;
		return true;
	}
//	DebugText(EMSG, "Unable to seek to section %s: reached end of file: good: %d, eof: %d", section, config_file.good(), config_file.eof());
	return false;
}
bool AppConfig::SeekToKey(const char *key)
{
	string line;
	// Found the section.. now find key ( but stop when we reach next section
	config_file.clear();
//	DebugText(EMSG, "Beginning search for key: %s, good(): %d, eof: %d", key, config_file.good(), config_file.eof());
	int pos, pp, magicPositionA, difference;
	while (!config_file.eof() && config_file.good())
	{
		string keyline;
		getline(config_file, line);	// Get line
//		eatWhiteSpace(line);		
		if (line[0] == '[')			// We went through the section without finding the key!
			return false;
		int i=0;
		if (line.size() == 0)

			continue;

		do
		{
			keyline = keyline + line[i++];
		}
		while (/*isalnum(line[i])*/ line[i] != '=' && line[i] != ' ' );
		if (key == keyline)
		{
			pos = line.find("=");
			pp = config_file.tellg();
			magicPositionA = config_file.tellg();
			difference = line.length() - pos;

//			DebugText(EMSG, "tellg: %d, difference: %d, magicpositionA: %d", pp, difference, magicPositionA);
			magicPositionA = magicPositionA - difference;
//			DebugText(EMSG, "line: %s", line.c_str());
//			DebugText(EMSG, "tellg: %d, difference: %d, magicpositionA: %d", pp, difference, magicPositionA);
			
			int start = pos + 1;
			int end = line.find("//");
			int ssize = 0;
			if (end == -1)
				ssize = line.length();
			else 
				ssize = end - start;
			//DebugText(EMSG, "start: %d, end %d, ssize %d", start, end, ssize);
			std::string data = line.substr(start, ssize);
			eatWhiteSpace(data);
//			DebugText(EMSG, "Data: %s", data.c_str());		
			if (data.size() == 0)
				::exit(-2);
			
			config_file.clear();
			config_file.seekg(magicPositionA);
			dataStream.clear();
			dataStream.str(data.c_str());
//			line
	//		int valu = 0;
		//	dataStream >> valu;
//			DebugText(EMSG, "dataStream: %s", dataStream.);
			return true;
		}
	}
//	DebugText(EMSG, "Unable to seek to key %s: reached end of file: good: %d, eof: %d", key, config_file.good(), config_file.eof());
	return false;
}
bool AppConfig::SeekTo(const char *section, const char *key)
{
	if (!SeekToSection(section))
		return false;
	if (!SeekToKey(key))
		return false;
	return true;
}
bool AppConfig::GetSimpleString(const char *section, const char *key, char *value, const unsigned int &size)
{
	if (!SeekTo(section, key))
		return false;
	string s;
	char line[2000];
	config_file.getline(line, size);
	s.assign(line);
	eatWhiteSpace(s);		
	strcpy(value, s.c_str());

	return true;
}
bool AppConfig::GetSetting(const char *section, const char *key, char *value, const unsigned int &size)
{
	dataStream.clear();
	if (!SeekTo(section, key))
		return false;
	char *tmpstr = new char[size];

	//config_file.getline(tmpstr, size);
	strncpy(tmpstr, dataStream.str().c_str(), size);
//	eatWhiteSpace(line);		

	// Find first and last "
	char *first = strchr(tmpstr, '\"');
	char *last = strrchr(tmpstr, '\"');
	if (first == NULL || last == NULL)
	{
		delete [] tmpstr;
		return false;
	}
	memset(value, '\0', size);
	strncpy(value, first+1, last - first - 1);
//	DebugText(EMSG, "GetSetting: value: %s", value);
	
	delete [] tmpstr;
	return true;
}
bool AppConfig::Create(const char *section, const char *key)
{
	if (!config_file.is_open())
		return false;
	if (!SeekToSection(section))
	{
		// Create section	
	}
	// Insert key here
	// config_file << key << " = 0 " << endl;
	return true;
}

