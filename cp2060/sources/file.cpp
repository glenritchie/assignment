#include "file.h"
#include "model.h"
FileObject::FileObject(string newName, string newFilename) : Object(newName)
{
		fileName = newFilename;
}
FileObject::FileObject(Object *o) : Object(o)
{
	// Do nothing
}
void FileObject::readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level)
{
	string line;
	level++;
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of file object params at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	
	if (!getParent()->isStatic() && this->isStatic())
		setStatic(false);
	
	do
	{
		file.clear();
		string text;
//		text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("filename") == 0)
		{
			lineStream >> text >> this->fileName;
		}
		else if (line.find("idleframes") == 0)
		{
			lineStream >> text >> this->idleFrames;
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
	}
	while (line.find("}") == -1);
	level--;
	file.clear();
	// Ok we have read in the params
	// Now we need to read in the file that we wanted straight into here!
	// We have one child and it's this file!
	if (idleFrames.size() == 0)
		DebugText(ERROR_NONCRITICAL, "Warning.. '%s' has no idle frames!", getName().c_str());
	Object *o = NULL;
	
	loadModelFile(fileName, o);
	//o->setName(getName());
	if (idleFrames.size() != 0)
		readFrames(idleFrames, o);

	this->addChild(o);
	
	if (!getParent()->isStatic())
		setStatic(false);
}


