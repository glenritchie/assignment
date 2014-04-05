#include <stdlib.h>

/*#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
*/
#include "utils.h"
#include "textutils.h"
//#include "lightmanager.h"
//#include "light.h"
//#include "bezier.h"
#include "model.h"

#ifndef WIN32
#include <dlfcn.h>				// Include dlopen and friends.. 
													// (dynamic runtime loading of shared libs)
#include <fnmatch.h>
#include <dirent.h>
#else
#include <windows.h>
#endif

//#define cout1 /
//#define cout /cout1
using namespace std;
//unsigned int level = 0;
//unsigned int lineNumber = 0;


bool readObjectType(ifstream &file, Object *(&target), string type, unsigned int &lineNumber, unsigned int &level)
{
//	DebugText(EMSG, "readobjecttype: %s", type.c_str());
	Object *o = createObject(type, target);
	if (o == NULL)
		return false;
	// Ask the object to read it's own params in the way it knows how	
	o->readParams(file, lineNumber, level);
	// Destroy any object we had. It's values have been copied to the new object
	delete target;
	// Change the pointer that used to point to target to now point to our new
	// object that the library created! NEAT HUH?
	target = o;
	return true;
}

/*********************************
END OF DLL STUFF
**********************************/

bool readObject(ifstream &file, Object *parent, string name, unsigned int &lineNumber, unsigned int &level)
{
	string line;
	Object *object = new Object(name);
	level++;
	readline(file, line, lineNumber);
	//cout << "Reading object " << name << " adding to " << parent->getName() << endl;
	object->setParent(parent);
	if (line.find("{") != 0)
	{
		cout << "*****Invalid start of object at line: " << lineNumber << " : expected { found " << line << endl;
		return false;
	}
	bool gotType = false;
	
	// If the parent of this object is not static and this object was static then
	// MAKE this object non static because an object attached to a non-static should be non-static as well.
//	DebugText(EMSG, "parent(\"%s\")->isStatic(): %d, object(\"%s\")->isStatic(): %d", 
//			parent->getName().c_str(), parent->isStatic(), object->getName().c_str(), object->isStatic()); 
	if (!parent->isStatic() && object->isStatic())
		object->setStatic(false);
	
	do
	{
		file.clear();
		string text;
	//	text.clear();
		// Read data for this object
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
//		cout << "got line " << lineNumber << " line: " << line << endl;
		float v[3] = { 0, 0, 0 };
		if (line.find("origin") == 0)
		{
			lineStream >> text >> v[0] >> v[1] >> v[2];
			object->setOrigin(v);
		}
		else if (line.find("angles") == 0)
		{
			// Read angles 
			lineStream >> text >> v[0] >> v[1] >> v[2];
			object->setFixedAngles(v);
		}
		else if (line.find("pivot") == 0)
		{
			// Read pivot point 
			lineStream >> text >> v[0] >> v[1] >> v[2];
			object->setPivot(v);
		}
		else if (line.find("twosided") == 0)
		{
			bool twosided = false;
			lineStream >> text >> twosided;
			object->set2sided(twosided);
		}
		else if (line.find("texture") == 0)
		{
			std::string textureFile;
			lineStream >> text >> textureFile;
			object->setTextureFile(textureFile);
		}
		else if (line.find("transparent") == 0)
		{
			float trans = 0.0;
			lineStream >> text >> trans;
			object->setTransparent(trans);
		}
		else if (line.find("lighted") == 0)
		{
			bool lighted = false;
			lineStream >> text >> lighted;
			object->setLighted(lighted);
		}
		else if (line.find("scale") == 0)
		{
			// Read pivot point 
			lineStream >> text >> v[0] >> v[1] >> v[2];
			object->setScale(v);
		}
		else if (line.find("limits") == 0)
		{
			// Read limit angles 
			lineStream >> text >> v[0] >> v[1] >> v[2];
			object->setLimits(v);
		}
		else if (line.find("static") == 0)
		{
			bool staticFlag;
			// Read limit angles 
			lineStream >> text >> staticFlag;
			object->setStatic(staticFlag);
		}
/*		else if (line.find("param") == 0)
		{
			lineStream >> text;
			float value;
			for (unsigned int i = 0; i < MAX_PARAMS; i++)
			{
				lineStream >> value;
//				object->setParam(i, value);
			}
		}
*/	else if (line.find("type") == 0)
		{
			if (gotType == true)
			{
				DebugText(EMSG, "Warning: object type already set. Error at line %s", lineNumber);
				continue;
			}
			gotType = true;
			lineStream >> text;
			string type;
			lineStream >> type;
			//cout << "type: text: '" << text << "' type: '" << type << "'" << endl;
			eatWhiteSpace(type);
			// Text contains object type
			if (readObjectType(file, object, type, lineNumber, level) == false)
			//cout << "setting object type '" << text << "' at line " << lineNumber << endl;
			//if (object->setType(type) == false)
				cout << "Warning: unkown object type '" << type << "' at line " << lineNumber << endl;
		}
		else if (line.find("object") == 0)
		{
			readObject(file, object, line, lineNumber, level);
		}
		else if (line.find("}") == 0)
		{
			// Leaving this object
		}
		if (line.find("}") != -1)
			break;
	}
	while (1);
	

		
	parent->addChild(object);
	level--;
	//cout << "End reading object: level: " << level << endl;
	//cout << object->toString() << endl;
	file.clear();
	return true;
}

void readObjectFrames(ifstream &file, Object *parent, string name, Object *root, unsigned int &lineNumber)
{
	string line;
	// Find the object in the tree that these frames belong to
	Object *object = root->findObjectByName(name);
	if (object == NULL)
	{
		cout << "****Invalid object name: " << name << endl;
		return;
	}
	readline(file, line, lineNumber);
	if (line.find("{") != 0)
	{
		cout << "****Invalid start of object at line: " << lineNumber << " : expected { found " << line << endl;
		return;
	}
	//cout << "Reading frames for: " << name << endl;
	int frameNo = 0;
	do
	{
		string temp;
	//	temp.clear();
		readline(file, line, lineNumber);
		basic_stringstream<char> lineStream;
		lineStream.str(line);
		if (line.find("frame") != 0)
			continue;
		//float *angles = object->angles[frame];
		float ang[3] = { 0, 0, 0 };
		float org[3] = { 0, 0, 0 };

		lineStream >> temp >> ang[0] >> ang[1] >> ang[2];
		lineStream >> org[0] >> org[1] >> org[2];
		
	//	cout << "lineStream:" << org[0] << " " << org[1] << " " << org[2] << endl;
		object->setAngles(frameNo, ang);
		object->setOriginOffsets(frameNo, org);
//		lineStream >> temp >> object->angles[frame][0] >> object->angles[frame][1] >> object->angles[frame][2];
//		lineStream >> object->originOffsets[frame][0] >> object->originOffsets[frame][1] >> object->originOffsets[frame][2];
		
		frameNo++;
		if (frameNo > FRAMES)
		{
			cout << "Too many frames specified!" << endl;
			break;
		}
	
	}
	while (line.find("}") == -1);
	//cout << "Read " << frame << " frames for object: " << object->getName() << endl;
}
extern string currentFrameFile;

void loadModelFile(string &fileName, Object *(&root))
{
	//selectedObject = NULL;
	unsigned int lineNumber = 0;
	//initModel = true;
	DebugText(EMSG, "Loading model %s", fileName.c_str());	

	if (root)
		delete root;

	// Creat the root object
	string f = fileName;
	root = new Object(f);

	// Load the file
	ifstream file(fileName.c_str());
	if (!file.is_open())
	{
		DebugText(ERROR_CRITICAL, "loadModelFile:  Unable to open file: %s", fileName.c_str());
		return;
	}
	unsigned int level = 0;
	do
	{
			file.clear();
			string line;
			readline(file, line, lineNumber);
			eatWhiteSpace(line);			
			// Skip comments and blank lines
			if (line.find("object") == 0)
				readObject(file, root, line, lineNumber, level);
	} while (!file.eof());
	DebugText(EMSG, "Done loading model");	
}

void readFrames(string &frameFile, Object *root)
{
//	currentFrameFile = frameFile;
	unsigned int lineNumber = 0;
	string line;
	root->nullChildren();
	DebugText(EMSG, "Reading frames '%s' into object: %s",
						frameFile.c_str(), root->getName().c_str());
	ifstream file(frameFile.c_str());
	if (!file.is_open())
	{
		DebugText(ERROR_CRITICAL, "readFrames: Error! could not open %s", frameFile.c_str());
	}
	do
	{
		file.clear();
		string line;
		readline(file, line, lineNumber);
		eatWhiteSpace(line);			
		// Skip comments and blank lines
		if (line.find("object") == 0)
			readObjectFrames(file, root, line, root, lineNumber);
	} while (!file.eof());
}

