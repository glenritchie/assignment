#include "globals.h"
#ifndef WIN32
// Linux stuff
	#include <fnmatch.h>
	#include <dirent.h>
	#include <dlfcn.h>											// Include dlopen and friends... (runtime library loading functions)
#else
// Windows stuff
	#include <windows.h>
#endif

string models[90];
string frames[90];
string objectNames[100];

void handleOption(int itemID);
void fillMenuWithFiles(const char *directory, const char *wildcard, string store[])
{
	// Find all files that fullfill the criteria
	int id = 0;
#ifdef WIN32
	// Directory search for MS-Windows machines
	WIN32_FIND_DATA findData;
	HANDLE file;
	char previousDir[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, previousDir);
	SetCurrentDirectory(directory);

	file = FindFirstFile(wildcard, &findData);
	if (file == INVALID_HANDLE_VALUE)
	{
		SetCurrentDirectory(previousDir);
		FindClose(file);
		return;
	}
	do
	{
		store[id] = string(findData.cFileName);
		glutAddMenuEntry(store[id].c_str(), id);
		id++;
	}while (FindNextFile(file, &findData) != 0);
	FindClose(file);
	SetCurrentDirectory(previousDir);

#else
	// Directory search routine for POSIX conforming machines ( ie. NOT windows )
	struct dirent *dirent;
	DIR *dir;
	int n;

	// Open the directory we wish to search
	dir = opendir(directory);
	if (dir == NULL)
	{
		DebugText(EMSG, "error getting directory contents");
		return;
	}

 	// Read the directory entries
	while((dirent = readdir(dir)) != NULL	)
	{
		// If the name of the entry matches our wildcard ( == 0)
		if (fnmatch(wildcard, dirent->d_name, FNM_PATHNAME) == 0)
		{
			// Add it to the menu
			store[id] = string(dirent->d_name);
			glutAddMenuEntry(dirent->d_name, id);
			id++;
		}
	}
#endif
}

void objectMenuFunction(int itemID)
{
	string objectName = objectNames[itemID];
	selectedName = objectName;
//	selectedObject = root->findObjectByName(objectName);
}
// Fill a menu recursively with menuID with the children of Object and create
// submenus for any children that have children
int fillObjectMenu(Object *object, unsigned int &i, int menuID)
{
	int objectMenuID = glutCreateMenu(objectMenuFunction);
	
	unsigned int children = object->getNumberOfChildren();
	unsigned int j = 0;
	for (j = 0; j < children; j++)
	{
		string name = object->getChild(j)->getName();
//		DebugText(EMSG, "Got '%s' :child of %s", object->getName().c_str(), name.c_str());
		glutAddMenuEntry(name.c_str(), i);
		objectNames[i] = name;
		i++;
	}
	for (j = 0; j < children; j++)
	{
		if (object->getNumberOfChildren() > 0)
			fillObjectMenu(object->getChild(j), i, objectMenuID);
	}

	if (menuID != -1 && object->getNumberOfChildren() > 0)
	{
		glutSetMenu(menuID);	
		glutAddSubMenu(object->getName().c_str(), objectMenuID);
	}
	return objectMenuID;
}

void modelMenuFunction(int itemID)
{
	string mfile = string("models/") + models[itemID];
	reload();	
//	frame = 0;
}
void frameMenuFunction(int itemID)
{
		string ffile = string("models/") + frames[itemID];
		readFrames(ffile, mainModel);
		frame  = 0;
}
void mainMenuFunction(int itemID)
{
	handleOption(itemID);
}
int mainMenuID = -1;
void setupMenu(void)
{
	if (mainMenuID != -1)
	{
		glutDetachMenu(GLUT_RIGHT_BUTTON);	
		glutDestroyMenu(mainMenuID);
	}
	int modelMenuID = glutCreateMenu(modelMenuFunction);
	fillMenuWithFiles("models/","*.pov", models);
	int frameMenuID = glutCreateMenu(frameMenuFunction);
	fillMenuWithFiles("models/","*.frm", frames);
	
	unsigned int i = 0;
	int objectMenu = fillObjectMenu(world, i, -1);

	int frameMenu =  glutCreateMenu(mainMenuFunction);
			glutAddSubMenu("From File", frameMenuID);
			glutAddMenuEntry("Right Kick", RIGHT_KICK);
			glutAddMenuEntry("Left Kick", LEFT_KICK);
			glutAddMenuEntry("Head Kick", HEAD_KICK);
			glutAddMenuEntry("Look at watch", LOOK_AT_WATCH);

	int animations = glutCreateMenu(mainMenuFunction);
			glutAddSubMenu("Load Model from file", modelMenuID);
			glutAddSubMenu("Frames", frameMenu);
			glutAddMenuEntry("Forward Frame", FORWARD_FRAME);
			glutAddMenuEntry("Backward Frame", BACKWARD_FRAME);
			glutAddMenuEntry("Interpolate", INTERP);
			glutAddMenuEntry("Pause animationn", PAUSE);
			glutAddMenuEntry("Reload animation/model", RELOAD);
	
	mainMenuID = glutCreateMenu(mainMenuFunction);
		glutAddSubMenu("Objects", objectMenu);
		glutAddSubMenu("Animation", animations);
		glutAddMenuEntry("Toggle textures", TEXTURES);
		glutAddMenuEntry("Toggle wireframe", WIREFRAME);
		glutAddMenuEntry("Invalidate Resources", RELOADRESOURCES);
		glutAddMenuEntry("Help", HELP);
		glutAddMenuEntry("Print Tree", PRINTTREE);
		glutAddMenuEntry("Quit", QUIT);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);	

}


