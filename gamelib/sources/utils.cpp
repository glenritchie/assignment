//***********************************************************************//
//
//		$Author: spikeles $Program:		MapEdit
//
//		$Description: Debug utils - output to screen etc
//
//		$Date: 2004/10/01 07:04:40 $History: $
//
//***********************************************************************//
#include "utils.h"
#include "font.h"
#include "log.h"
#include <stdarg.h>
#include <time.h>
#include <vector>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
typedef struct serr_struct { char *err_msg; err_type error_type; } serr_struct;
serr_struct error_struct[]={
	"EMSG",EMSG,
	"ERROR_INFO",ERROR_INFO,
	"ERROR_NONCRITICAL",ERROR_NONCRITICAL,
	"ERROR_SEVERE",ERROR_SEVERE,
	"ERROR_CRITICAL",ERROR_CRITICAL,
	"ERROR_2BFIXED",ERROR_2BFIXED,
	"ERROR_UNKNOWN",ERROR_UNKOWN,
	"ERROR_FATAL", ERROR_FATAL,
	0,EMSG
};

char *ErrorType2Msg(err_type e_type)
{
/*	unsigned int i=0;
	while (error_struct[i].err_msg!=0)
	{
		if (error_struct[i].error_type==e_type)
			return error_struct[i].err_msg;
		i++;
	}
*/
	if (e_type <= ERROR_FATAL)
		return error_struct[e_type].err_msg;
	// Oh no... We didn't find the right kind of error
	// Write DIRECTLY to the log saying so.. this NEEDS TO BE FIXED
	char tmsg[100];
	sprintf(tmsg,"ERROR! ERROR! \n UNABLE TO FIND ERROR MSG FOR ID: %d\n",e_type);
	Log.Output(tmsg);
	
	// Return a default
	return "UNKETYPE";
}
void DebugOutA(int error_type, char *format, ...)
{
	char msg[20000];
	char tmsg[20000];
	va_list argptr;
	va_start (argptr, format);
	vsprintf (msg, format, argptr);
	va_end (argptr);
	
	time_t timer;
	struct tm *tblock;
	timer = time(NULL);
	tblock = localtime(&timer);
	sprintf(tmsg, "%d:%d:%d ",
		tblock->tm_hour,
        tblock->tm_min,
        tblock->tm_sec);
	// Attach error msg to it ( but not if it's a EMSG type or ERROR_INFO)
	strcat(tmsg,msg);
	strcat(tmsg,"\n");
	
	Log.Output(tmsg);
	printf(tmsg);
	extern bool developer;
	if (developer)
	{
#ifdef WIN32
		OutputDebugString(tmsg); //Output debug string
#endif
//		extern void addHistoryItem(std::string item);
//		addHistoryItem(tmsg);
	}	
}

void DebugOutA(err_type error_type, char *format, ...)
{
	char msg[20000];
	char tmsg[20000];
	va_list argptr;
	va_start (argptr, format);
	vsprintf (msg, format, argptr);
	va_end (argptr);
	
	time_t timer;
	struct tm *tblock;
	timer = time(NULL);
	tblock = localtime(&timer);
	sprintf(tmsg, "%d:%d:%d ",
		tblock->tm_hour,
        tblock->tm_min,
        tblock->tm_sec);
	// Attach error msg to it ( but not if it's a EMSG type or ERROR_INFO)
	if (error_type!=EMSG && error_type!=ERROR_INFO)
	{
		char *emsg=ErrorType2Msg(error_type);
		strcat(tmsg,emsg);
		strcat(tmsg,": ");
	}
	strcat(tmsg,msg);
	strcat(tmsg,"\n");
	
	Log.Output(tmsg);
	printf(tmsg);
	extern bool developer;
	if (developer)
	{
#ifdef WIN32
		OutputDebugString(tmsg); //Output debug string
#endif
//		extern void addHistoryItem(std::string item);
//		addHistoryItem(tmsg);
	}
}
void Ortho()
{
	extern int width;
	extern int height;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void Perspective()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
/*********************************
Spikeles(26/11/02):

  1.	This needs to become more robust. A good idea would be to use a list and keep
		track of all the stuff sent. Then ONCE during the render loop, loop through and
		render all the writing at once. This way we minimize state changes.
  2.	Needs to have extra things like center text, left justify etc

********************************/
struct textitem
{
	textitem(int newx, int newy, unsigned long newc, char *newmsg)	{ x = newx; y = newy; colour = newc; strcpy(msg, newmsg); }
	int x, y;
	unsigned long colour;
	char msg[20000];
};
std::vector<CFont::fontStrings> textItems;
//std::vector<textitem> textItems;
void drawFinal2DText()
{
	
	//return;
	extern CFont *util_font;

	if (!util_font)
		return;

	util_font->setBatch(true);
	glDisable(GL_FOG);
	// Goto othographic mode	
	Ortho();
	CFont::fontStrings *f = new CFont::fontStrings[textItems.size()];
	std::copy(textItems.begin(), textItems.end(), f);

	util_font->Draw2DStrings(f, textItems.size());

	textItems.clear();
	delete [] f;

	util_font->setBatch(false);

	// Go back to perspective
	Perspective();
}

void Draw2DText(int x, int y, unsigned long colour,  char *format, ... )
{
	char msg[20000];
	std::fill(msg, &msg[1999], '\0');
	va_list argptr;
	va_start (argptr, format);
	vsprintf (msg, format, argptr);
	va_end (argptr);
	CFont::fontStrings f;// = { x, y, colour, msg };
	f.x = x;
	f.y = y;
	f.colour = colour;
	std::fill(f.msg, &f.msg[199], '\0');

	strncpy(f.msg, msg, strlen(msg));
	
	textItems.push_back(f);
}


