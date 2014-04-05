#ifndef WIN32
#define GL_GLEXT_PROTOTYPES
#endif
#include "ctexture.h"
#include "utils.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>


// #include <GL/glext.h>
#include "config.h"
#include <math.h>
#include <string.h>
std::string CTexture::globalTextureDir("./");
using namespace std;
extern bool doscale;
extern int scaleval;
//
// TexMapScalePow2 - Scale image to power of 2 in height and width.
//
double ScalePow2(int scale, int colourtype, unsigned int &m_iWidth, unsigned int &m_iHeight, unsigned char *(&data) )
{
	 GLint glMaxTexDim;
	 double xPow2, yPow2;
	 int ixPow2, iyPow2;
	 int xSize2, ySize2;
//		int m_iWidth=Getwidth();
//	int m_iHeight=Getheight();
//	BYTE *data=GetimageData();

	 glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
//	 DebugText(EMSG, "Scaling image: maxsize: %d, size:x:%d, y:%d", glMaxTexDim, m_iWidth, m_iHeight);
	 if (scale!=0)
		 glMaxTexDim = min(scale, glMaxTexDim);
	 else glMaxTexDim = min(128, glMaxTexDim);

	 if (m_iWidth <= glMaxTexDim)
			xPow2 = log((double)m_iWidth) / log(2.0);
	 else
			xPow2 = log((double)glMaxTexDim) / log(2.0);

	 if (m_iHeight <= glMaxTexDim)
			yPow2 = log((double)m_iHeight) / log(2.0);
	 else
			yPow2 = log((double)glMaxTexDim) / log(2.0);

	 ixPow2 = (int)xPow2;
	 iyPow2 = (int)yPow2;

	 if (xPow2 != (double)ixPow2)
			ixPow2++;
	 if (yPow2 != (double)iyPow2)
			iyPow2++;

	 xSize2 = 1 << ixPow2;
	 ySize2 = 1 << iyPow2;

	 int alpha;
	 if (colourtype==GL_RGBA)
			alpha=4;
	 else alpha=3;
	 //BYTE *pData = (BYTE*) malloc(xSize2 * ySize2 * alpha * sizeof(BYTE));
	 unsigned char *pData = new unsigned char[xSize2*ySize2*alpha];
	 if (!pData)
		{
//			DebugText(ERROR_CRITICAL, "Unable to allocate %d bytes of memory",xSize2*ySize2*alpha);
			return 0;
		}

	 gluScaleImage(colourtype, m_iWidth, m_iHeight,
								 GL_UNSIGNED_BYTE, data,
								 xSize2, ySize2, GL_UNSIGNED_BYTE,
								 pData);
//	 DebugText(EMSG, "scale image done..new size: %d, %d", xSize2, ySize2);

	delete data;
	data = pData;
	m_iWidth = xSize2;
	m_iHeight = ySize2;
//SetimageData(pData);
//	Setwidth(xSize2);
//	Setheight(ySize2);

	return (xSize2*ySize2*alpha*sizeof(unsigned char))/1024;	//Return kbytes used during scale
}
void CTexture::buildTexture( bool scale, float mipmode, bool clamp)
{
		glEnable(GL_TEXTURE_2D);
		// Generate an id	
		glGenTextures(1, &id);					// Generate OpenGL texture IDs

	if (bpp == 24)													// Was The TGA 24 Bits
		colourtype = GL_RGB;										// If So Set The 'type' To GL_RGB
	else
		colourtype = GL_RGBA;										// If So Set The 'type' To GL_RGBA

	 int alpha;
	 if (colourtype == GL_RGBA)
			alpha = 4;
	 else alpha = 3;
	if (doscale)
		ScalePow2(scaleval, colourtype, width, height, imageData);
	if (imageData == 0)
	{
	  invalidate();		
		return;
	}
	ActivateTexture(0);	// Bind texture
	if (clamp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	float mipmode1;
	float mipmode2;
	if (mipmode==GL_LINEAR_MIPMAP_LINEAR || mipmode==GL_LINEAR)
	{
		mipmode1=GL_LINEAR_MIPMAP_NEAREST;
		mipmode2=GL_LINEAR;
	}
	else
	{
		mipmode1=GL_NEAREST_MIPMAP_NEAREST;
		mipmode2=GL_NEAREST;
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmode1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmode2);
	gluBuild2DMipmaps(GL_TEXTURE_2D, alpha, width, height, colourtype, GL_UNSIGNED_BYTE, imageData);
}
std::string CTexture::getGlobalDirectory()
{
	return globalTextureDir;
}
void CTexture::setGlobalDirectory(std::string newGlobalDir)
{
	globalTextureDir = newGlobalDir;
}

CTexture::CTexture() : texture_init(false), id(0) 
{ 
	filterType = GL_LINEAR; 
	imageData = 0;
}
    

std::string CTexture::toString()
{
	std::string final;
	final += "CTexture::";
	final += CResource::toString();

	char tmp[1000];
	sprintf(tmp, ", width: %d, height: %d, textureid: %d, texture_init: %d", 
			width, height, id, texture_init);
	final.append(tmp);
	return final;
}

unsigned int CTexture::GetWidth()
{
	if (!texture_init)
		LoadResource();
	return width;
}
unsigned int CTexture::GetHeight()
{
	if (!texture_init)
		LoadResource();
	return height;
}

void CTexture::ActivateTexture(const unsigned short int &stream)
{
	// If not in memory and no memory allocated then load the texture
	if (!in_memory && id == 0)
		LoadResource();
	// If in memory and has a texture associated then use it
#ifdef WIN32
#ifdef PFNGLACTIVETEXTUREARBPROC 

	extern void APIENTRY glActiveTextureARB (GLenum p);
#endif

#endif
	extern bool multitexture_availiable;
	extern bool use_multitexture;
	if (multitexture_availiable && use_multitexture)
	{
		extern GLint maxTexelUnits;
		if (stream > maxTexelUnits)
			DebugText(ERROR_NONCRITICAL, 
					  "CTexture::ActivateTexture() Warning: stream %d is larger than maxTexelUnits: %d", 
					  stream, maxTexelUnits);
		switch (stream)
		{
#ifdef PFNGLACTIVETEXTUREARBPROC 

			case 0: 	glActiveTextureARB(GL_TEXTURE0_ARB); break;
			case 1: 	glActiveTextureARB(GL_TEXTURE1_ARB); break;
#endif

		}
		glBindTexture(GL_TEXTURE_2D, id);
		glEnable(GL_TEXTURE_2D); 
	}

	glBindTexture(GL_TEXTURE_2D, id);
//	if (in_memory && texture)
//		SetTexture(stream, texture);
	UpdateLastAccessTime();		// We used the texture, update access time
}
void CTexture::invalidate()
{
	this->UnloadResource();
	this->LoadResource();
}
void CTexture::setFilterType(unsigned int newType)
{
	filterType = newType;
	// Force a reload of the texture
	invalidate();
}
bool CTexture::LoadResource()
{
	char texturename[255];

	// If the tetxure name has a "." in it then it's a filename
	// otherwise it's an alias and we get the real name from textureAliases.txt
	const char *sub = strstr(this->GetResourceName().c_str(), ".");
	char filter[200] = "BOX";
	AppConfig c;
	c.SetConfigFile("textureAliases.txt");
	
	
	if (sub != NULL)
		sprintf(texturename,"%s%s\0", CTexture::globalTextureDir.c_str(), this->GetResourceName().c_str());
	else
	{
		char tmptexturename[255];
		c.GetSetting((char *)this->GetResourceName().c_str(), "image", tmptexturename, 255);
		sprintf(texturename,"%s%s\0", CTexture::globalTextureDir.c_str(), tmptexturename);
	}
	bool clamp = false;
	c.GetSetting((char *)this->GetResourceName().c_str(), "filter", filter, 200);
	c.GetSetting((char *)this->GetResourceName().c_str(), "clamp", clamp);
#ifdef WIN32
	if (strncmp(filter, "BOX", 3) == 0)
		filterType = GL_LINEAR;
	else if (strncmp(filter, "NONE", 4) == 0)
		filterType = GL_NEAREST;
#else
	if (strcasecmp(filter, "BOX") == 0)
		filterType = GL_LINEAR;
	else if (strcasecmp(filter, "NONE") == 0)
		filterType = GL_NEAREST;

#endif

//	DebugText(EMSG, "Reading texture: %s", texturename);
	bool result = false;

	// LOAD TEXTURE HERE
	in_memory = false;
	readFile(texturename, 0, filterType, clamp);

	if (!in_memory)
	{
		DebugText(EMSG, "Unable to load texture file: %s when loading: %s", this->GetResourceName().c_str(), texturename);
		id = 0;
		return false;
	}
//	DebugText(EMSG, "Loaded: %s", texturename);

	texture_init = true;
	UpdateLastAccessTime();	/// Update last access time
	in_memory = true;
	//DebugText(EMSG, (char *)toString().c_str());

	return true;
}

void CTexture::UnloadResource()
{
	if (!in_memory)
		return;
	in_memory = false;			// Not in memory anymore
	if (imageData)
		delete [] imageData;
	glDeleteTextures(1, &id);
	id = 0;
	
}

void CTexture::readFile(const char *texturefile, bool scale, float mipmode, bool clamp)
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable

	if (!texturefile)
	{
		DebugText(ERROR_CRITICAL, "file was null!");
		return;
	}

	char filename[255];
	strcpy(filename, "textures/");
	strcat(filename, texturefile);
	// Ensure the filename is in lower case
	GLuint i =0;

	do
	{
	 filename[i] = tolower(filename[i]);	
	 i++;
	} while(i < strlen(filename));
	
	FILE *file;
	file = fopen(filename, "rb");																								// Open The TGA File
	if (!file)                                           												// Does File Even Exist?
	{
	 	DebugText(ERROR_CRITICAL, "File not found -> %s", filename);
		return;
	}

	// File opened. Start texture read and stuff
	if(fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		 memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0				        ||	// Does The Header Match What We Want?
		 fread(header, 1, sizeof(header), file) != sizeof(header))				        // If So Read Next 6 Header Bytes
	{
		if (file)
		fclose(file);										                                          // If Anything Failed, Close The File
		DebugText(ERROR_CRITICAL, "Texture load Failed!:header");
		return;																																		// Return False
	}

	width = (header[1] * 256 + header[0]);																			// Determine The TGA Width	(highbyte*256+lowbyte)
	height = (header[3] * 256 + header[2]);																			// Determine The TGA Height	(highbyte*256+lowbyte)

	if (width	<=0	||																														// Is The Width Less Than Or Equal To Zero
  		height	<=0)//	||																											// Is The Height Less Than Or Equal To Zero
//		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);																															// If Anything Failed, Close The File
		DebugText(ERROR_CRITICAL, "Texture load Failed!:width was %d height was %d", width, height);
		return;																																		// Return False
	}

	bpp = (header[4]);							// Grab The TGA's Bits Per Pixel (24 or 32)
	if(bpp != 24 && bpp != 32)				// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		DebugText(ERROR_CRITICAL, "Texture load Failed!:bpp was %d", bpp);
		return;										// Return False
	}

	bytesPerPixel	= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	imageData = new GLubyte[imageSize];	// Reserve Memory To Hold The TGA Data
	if (!imageData)
	{
		fclose(file);										// If Anything Failed, Close The File
		DebugText(ERROR_CRITICAL, "Texture load Failed!: Memory Allocation error");
		return;										// Return False
	}

	long int size=fread(imageData, 1, imageSize, file);
	if (size != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(imageData!=NULL)						// Was Image Data Loaded
			delete imageData;						// If So, Release The Image Data
		fclose(file);										// Close The File
		DebugText(ERROR_CRITICAL, "Texture load Failed!:Reading data");
		return;										// Return False
	}

	for(i=0; i<(int)imageSize; i+=bytesPerPixel)		// Loop Through The Image Data
	{																		// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=imageData[i];								// Temporarily Store The Value At Image Data 'i'
		imageData[i] = imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);																			// Close The File

	buildTexture(scale,mipmode, clamp);               // Build A Texture From The Data
//	DebugText(EMSG, "%s loaded", texturefile);
	in_memory = true;
	return;
}
