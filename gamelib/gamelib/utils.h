#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>

void Draw2DText(int x, int y, unsigned long colour, char *format, ...);
void drawFinal2DText();
#define COLOR_RGB(r, g, b) ((unsigned long)((((0xff)&0xff)<<24|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))))
#define COLOR_RGBA(r, g, b, a) ((unsigned long)((((a)&0xff)<<24|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))))
#define COLOR_ARGB(a, r, g, b) ((unsigned long)((((a)&0xff)<<24|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))))

// Template for casts
template <class CastToType, typename Item> 
CastToType *CastTo(Item *i)
{
	return dynamic_cast<CastToType*>(i);
}

/** An enumerated type specifiying the types of errors that can occur */
enum err_type
{
	EMSG=0,					/// A standard msg to be sent
 	ERROR_INFO,				/// A standard msg to be sent
	ERROR_NONCRITICAL,		/// An error has occured but you can continue
	ERROR_SEVERE,
	ERROR_CRITICAL,
	ERROR_2BFIXED,
	ERROR_UNKOWN,
	ERROR_FATAL
};

void Ortho();
void Perspective();

extern void DebugOutA(err_type e_type ,char *format, ...);
#define DebugText DebugOutA
/*#ifdef _DEBUG
#define DebugText DebugOutA
#define DEBUGTRACED(flag) DebugText(EMSG,#flag "=%d", flag)
#define DEBUGTRACEF(flag) DebugText(EMSG,#flag "=%.4g", flag)
#else
#define DebugText ;
#define DEBUGTRACED(flag) ;
#define DEBUGTRACEF(flag) ;
#endif
*/

#endif // _UTILS_H
