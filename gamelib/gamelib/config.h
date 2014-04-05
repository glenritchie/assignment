#ifndef __config_h__
#define __config_h__
#include <iostream>
#include <fstream>
#include <sstream>
/** 
	Has functions to load and manipulate a file of settings.<br>
	The format of the file is like so:<br>
	<br>
	[Section]<br>
	Key = value;<br>
	<br>	
*/
class AppConfig
{
public:
	/** Constructor */
	AppConfig();
	/** Destructor */
	~AppConfig();

	/** Check if the file is opened 
	 * @return True if it is
   	 */
	bool IsOpen() { return config_file.is_open(); }
	/** Set the file to open
	* @param configfile The string to open */
	void SetConfigFile(const char *configfile);
	
/*
	template <typename datatype>
	bool SetSetting(const char *section, const char *key, datatype value)
	{
//		config_file.seekg(0);
//		config_file << value << endl;	// Write value
		if (!SeekTo(section, key))		// To to seek to section and key
		{
			Create(section, key);		// If not found.. create it
			if (!SeekTo(section, key))	// Seek again to new section and key
				return false;			// If we still can't get there then exit
		}
		int fpos = config_file.tellg();
		EatLine();						// Delete the rest of the line
		int rpos = config_file.tellg();
		char *tmp = new char[filesize - rpos];
		config_file.read(tmp, filesize - rpos);
		int count = config_file.gcount();
		config_file.clear();
		config_file.seekg(fpos);
		config_file << value << endl;	// Write value
		if (config_file.fail())
			return false;
		config_file.write(tmp, count);
		if (config_file.fail())
			return false;
		//config.flush();
		delete [] tmp;

		return true;
	}
*/	
	/** 
	 * Default function to input most things like ints, floats etc
	 * @param section The section to search in: eg [Game]
	 * @param key The key to search for eg: mykey ( searches for mykey=something )
	 * @param valu The reference parameter to fill with the gotten value
	 * @return True if we successfully got the key
	 */
	template <typename datatype>
	bool GetSetting(const char *section, const char *key, datatype &value)
	{
		if (!SeekTo(section, key))
			return false;
		//getline(config_file, value);
		//config_file >> value;			// Get value
		
		dataStream >> value;
		return true;
	}
	
	/** 
	 * Specialized function to input a string ( can handle spaces! )
	 * @param section The section to search in: eg [Game]
	 * @param key The key to search for eg: mykey ( searches for mykey=something )
	 * @param value The reference parameter to fill with the gotten value
	 * @param size The maximumum size of the string
	 * @return True if we successfully got the key
	 */
	bool GetSetting(const char *section, const char *key, char *value, const unsigned int &size);

	/** 
	 * Get a simple string enclosed in quotes
	 * @param section The section to search in: eg [Game]
	 * @param key The key to search for. It <b>MUST</b> be enclosed in qoutes eg: mykey ( searches for mykey="something" )
	 * @param value The reference parameter to fill with the gotten value
	 * @param size The maximumum size of the string
	 * @return True if we successfully got the key
	 */
	bool GetSimpleString(const char *section, const char *key, char *value, const unsigned int &size);

	static void setGlobalConfigDirectory(std::string newDir);
	static std::string getGlobalConfigDirectory();

	void setLocalConfigDirectory(std::string newLocal);
private:
	static std::string globalConfigDirectory;

	std::string localConfigDirectory;

	void EatLine();
	bool SeekToSection(const char *section);
	bool SeekToKey(const char *key);

	bool SeekTo(const char *section, const char *key);
	bool Create(const char *section, const char *key);

	std::istringstream dataStream;
	std::ifstream config_file;
	int filesize;
	// char config_file[100];

};
#endif
