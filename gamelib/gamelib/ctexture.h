// Disable warning 4786:
// "identifier was truncated to '255' characters in the debug information"
//#pragma warning( disable : 4786)
#ifndef _texture_h_
#define _texture_h_

#include <string>
#include "cresource.h"

class CTextureManager;
class CInitManager;
/** a texture is an image either loaded of disk or created at runtime that can be applied to an object */
class CTexture : public CResource
{
public:
	/* Set as active texture of defined stream ( for multi-texturing )
	 * @param stream The stream to set this texture active on. 0 is the base. 1 is the second. 
	 */
	void ActivateTexture(const unsigned short int &stream);		

	/* Get the width of the texture in pixels
	 * @return The width of the texture in pixels
 	 */
	unsigned int	GetWidth();
	/* Get the height of the texture in pixels
	 * @return The height of the texture in pixels
 	 */	
 	unsigned int	GetHeight();

	/// see master_class.h
	virtual std::string toString();

	/**
	 * Set the filter type of the texture. Accepts all the OpenGL texture filter types. eg. GL_LINEAR_MIPMAP_LINEAR or GL_NEAREST.
	 * Note this will FORCE a reload of the texture. ( eg. you can change textures in-game!! )
	 * @param newType The new type of filter. Accepts all the OpenGL texture filter types. eg. GL_LINEAR_MIPMAP_LINEAR or GL_NEAREST.
	*/
	void setFilterType(unsigned int newType);
	/**
	 * Force a reload of this texture by removing it from memory and reloading it 
	 */
	void invalidate();
	/** Set the directory where textures should be loaded from 
	 * @param newGlobalDir The directory where textures are loaded from
	 */
	static void setGlobalDirectory(std::string newGlobalDir);
	/** Get the directory where textures are loaded from
	 * @return The directory where textures are loaded from
	 */
	static std::string getGlobalDirectory();
	/**
	 * Get the unique ID associated with this texture. In OpenGL this is the same ID that is used in a call to glBindTexture
	 * @return The ID that is currently associated with this texture. Will be 0 if a texture has not been assigned.
	*/
	unsigned int getID() { return id; }

//	void setByte(int , int value); 
//	{ imageData[x][y] = value; }
protected:
	/** The directory where textures are loaded from */
	static std::string globalTextureDir;
	/** Create a new instance of a texture
	 * @return The new instance
	 */
	static CTexture *Create() { return new CTexture(); }

	friend class CResourceManager;
	
	/* Private construct and destruct so that the programmer CAN NOT 
	   create or delete his own textures. He must go through the manager
	 */
	CTexture();
	/// Destructor
	virtual ~CTexture() { Release(); }							

	/// Make texture think
	virtual void Think() { ; }			

	/// Loads a texture from a file
	bool LoadResource();					
	/// Unloads the texture from memory ( does not destroy ptr)
	void UnloadResource();					

	/// Width of texture
	unsigned int width;						
	/// Height of texture
	unsigned int height;					
	/* Get the number of bits per pixel in this texture	
	 * @return The number of bits per pixel. eg. if RGBA then bpp will be 32 if RGB then bpp will be 24
	 */
	unsigned int getBPP() {return bpp; }
	/** The type of colour format used
	 * @param The colour format. eg. RGBA, RGB, BGR
	 */
	unsigned int getColourType() { return colourtype; }
private:
	/// The filter type
	unsigned int filterType;
	/// Has this texture been initialized(loaded)? 
	bool texture_init;
	/**
	 * Build a texture and it's mipmaps.
	 * @param scale Should this texture be scaled up or down to a requested size?
	 * @param mipmode The mipmode that will be used
	 * @param clamp Clamp the edges instead of repeat? ( good for skyboxes )
	 */
	void buildTexture(bool scale, float mipmode, bool clamp);
	/**
	 * Read a file (.tga) and load it into memory
	 * @param file The filename to load
	 * @param scale Should this texture be scaled up or down to a requested size?
	 * @param mipmode The mipmode that will be used
	 * @param clamp Clamp the edges instead of repeat? ( good for skyboxes )
 	*/
	void readFile(const char *file, bool scale, float mipmode, bool clamp);
	/// Unique ID for this texture
	unsigned int id;
	/// Bits-per-pixel
	unsigned int bpp;
	/// Colour type
	unsigned int colourtype;
	// The actual bytes that make up the image
	unsigned char *imageData;
};
#include "resource_manager.h"

#endif // _texture_h_
//#pragma warning( pop ) 

