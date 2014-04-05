#ifndef _font_h_
#define _font_h_

#include "cresource.h"
#include "anim_texture.h"

#include <string>

class CResourceManager;
/** A font class that allows drawing of FIXED-WIDTH fonts on the screen 
*/
class CFont : public CResource
{
public:
	typedef struct fontStrings { int x; int y; char msg[200]; unsigned long colour; } fontStrings;
	typedef struct charUV	{ float su, sv, eu, ev; } charUV;

	/** Draw some text at te specified x, y co-ord with the given colour.
	 * @param x The x co-ord where 0 is the left of the screen
	 * @param y The y co-ord where 0 is the top of the screen
	 * @param color The color of the text. 0xFFFFFFFF = white. 0x0000FFFF = blue. Use COLOR_RGBA helper macro to make things easier.
	 * @param format A string that takes the same format as a printf call followed by the same functions 
	 */
	void	Draw2DText(int x, int y, unsigned long color,  char *format, ... );

	/** Draw an array of strings. Each string item has an x,y coord, color and text
	 * @param stringArray An array holding fontStrings structs.
	 * @param count The number of strings in the array
	 */
	void	Draw2DStrings(fontStrings stringArray[], unsigned int count);
	/** Get the width of a character in the font 
	 * @return The width of a character. Because this is a fixed-width font, all characters will be the same width.
	 */
	int		GetWidth()	{ return font_width; }
	/** Get the height of a character in the font. 
	 * @return The height of a character. Because this is a fixed-width font, all characters will be the same height.
	 */
	int		GetHeight()	{ return font_height; }
	
	/** Set the height of the font. Good for alternate font maps
	 * @param newHeight The new height of the font 
	 */ 
	void	setHeight(unsigned int newHeight);
	/** Tell the font rendering code that alot of strings are about to be rendered at once and so allow certain
	 * optimizations ( don't forget to call setBatch(false) after the initial setBatch(true) call.
	 * @param newStatus The new batch status. Set to true, call Draw2D* functions and then call setBatch(false).
	*/
	void setBatch(bool newstatus);
private:
	/** Allow resource manager to access these variables*/
	friend class CResourceManager;
	/** Overridden function from CResource. see cresource.h for details */
	bool	LoadResource();							// Loads a font
	/** Overridden function from CResource. see cresource.h for details */
	void	UnloadResource();						// Loads a font
	
	/** Create a new instance of a font class 
	 * @return A new instance of a font class
	*/
	static	CFont *Create() { return new CFont(); }
	/// Private constructor
	CFont();
	/// Destructor
	~CFont() { ; }				
	
	/** Overridden function from CResource. see cresource.h for details */
	void Think();

	/// Obsolete function. Does nothing!
	void createRenderTarget();

	// Members
	/// The texture that this font uses
	CAnimatedTexture *fontTexture;
	/// Width of a character in the font
	unsigned int			font_width;
	/// Height of a character in the font
	unsigned int			font_height;
	/// Batch status
	bool		batch;
	/// Stored UV co-ords for each character in the ASCII table to save some CPU cycles
	charUV		uvList[256];
};

#endif // _font_h_
