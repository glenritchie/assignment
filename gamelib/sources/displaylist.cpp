#include "displaylist.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

DisplayList::DisplayList()
{
	// Generate 1 contiguous display list
	listID = glGenLists(1);
}

DisplayList::~DisplayList()
{
	// Delete our generated list
	glDeleteLists(listID, 1);
}

void DisplayList::record(bool execute)
{
	// Begin recording commands
	if (execute)
		glNewList(listID, GL_COMPILE_AND_EXECUTE);
	else
		glNewList(listID, GL_COMPILE);
		in_memory = true;
}

void DisplayList::stop()
{
	// Stop recording commands
	glEndList();
}

void DisplayList::display()
{
	UpdateLastAccessTime();	
	// Display the recorded commands
	glCallList(listID);
}
