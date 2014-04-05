#ifndef _anim_texture_h_
#define _anim_texture_h_

#include "ctexture.h"
class CTextureManager;
/**
	A texture that animates using a single file that has multiple frames in it
*/
class CAnimatedTexture : public CTexture	// Inherit from CTexture
{
	friend class CTexture;
	friend class CResourceManager;
public:
	bool GetTexcoords(float &su, float &eu, float &sv, float &ev);
	void SetFPS(int newfps)					{ fps = newfps; }
	void SetFrame(unsigned int newframe);										// Set the current frame
	void SetNumFrames(unsigned int newframes)	{ num_frames = newframes; }		// Set the number of frames

	unsigned int getFrame() { return curr_frame; }
	unsigned int getNumberFrames() { return num_frames; }
		
	void SetFrameSize(unsigned int width, unsigned int height)
		{frame_width = width; frame_height = height;}							// Set the frame size

	void StartAnimation()					{ animating = true; }		// Start animating
	void StopAnimation()					{ animating = false; }		// Stop animating
private:
	virtual bool LoadResource();								// Loads a texture from a file
	static CAnimatedTexture *Create() { return new CAnimatedTexture(); }
	
	CAnimatedTexture();
	~CAnimatedTexture() { ; }
	
	void Think();					// Allow texture to animate
	void UpdateCoords();			// Instantly Update coords

	float last_time;				// Last time
	unsigned int curr_frame;		// Current frame
	unsigned int num_frames;		// Number of frames in the animation
	int fps;						// Frames per second
	unsigned int frame_width;		// Height of each frame
	unsigned int frame_height;		// Width  of each frame

	float su,eu,sv,ev;
	bool animating;					// Is the texture animating?

	bool changed;					// Have the tex-coords changed from the last GetTexcoords call?
};


#endif // _anim_texture_h_

