#include "anim_texture.h"
#include <time.h>

extern veryLong currentTime;

CAnimatedTexture::CAnimatedTexture()
: curr_frame(0), last_time(currentTime), animating(false), fps(15), num_frames(0)
{
/*	curr_frame = 0;
	last_time = currentTime;
	animating = false;			// Default as not animating
	fps = 15;					// Default to 15
	num_frames = 0;				// Default to 0 frames
	*/
}

void CAnimatedTexture::SetFrame(unsigned int newframe)
{
	if (num_frames == 0)
		return;
	curr_frame = newframe % num_frames;	// Make sure frames wrap around
	UpdateCoords();
}

bool CAnimatedTexture::GetTexcoords(float &rsu, float &reu, float &rsv, float &rev)
{
	rsu = su; reu = eu; rsv = sv; rev = ev;
	return changed;
}

bool CAnimatedTexture::LoadResource()
{
	bool res = CTexture::LoadResource();	// Execute normal CTexture load
	// Do other stuff if needed
	if (res == false)
		return false;
//	UpdateCoords();
	return true;
}
void CAnimatedTexture::UpdateCoords()
{
	float f_w = float(frame_width);
	float f_h = float(frame_height);
	float w = float (GetWidth());
	float h = float (GetHeight());

	// Update texture co-ords
	// DirectX coord start from the top left ( i think.. or it could just be my really wierd way of loading textures??
	// Anyway. opengl co-ord should be 0,0 in the bottom left. So we need to change it.
	su = (f_w / w) * float(curr_frame % int(w / f_w));	// U start
	eu = su + (f_w / w);								// U end
	
	sv = (f_h / h) * float(curr_frame / int(w / f_w));	// V start
	ev = sv + (f_h / h);								// V end

	// Ok. These are right.. BUT.. remember co-ords start from bottom left. must fix it here
	sv = 1.0f - sv;
	ev = 1.0f - ev;
	float tmp = sv;
	sv = ev;
	ev = tmp;

	changed = true;
}
void CAnimatedTexture::Think()
{
	static bool first = true;
	//	float last_time ;
	if (!animating)				// If we arn't animating don't do anything
		return;
/*	if (num_frames == 0)		// If we havn't calculated the number of frames yet
								// Calculate number of frames in animation 
								// Assumes frame_width and frame_height have been set
	{
		int frames_in_row		=  GetWidth() / frame_width;
		int frames_in_column	=  GetHeight() / frame_height;
		num_frames = frames_in_row * frames_in_column;
	}
*/
	// Calulcate time difference
	float tdiff = (float)(currentTime - last_time) / (float)CLOCKS_PER_SEC;
	float test_time = 1/float(fps);
	
	changed = false;
	// Only update frame according to sprite FPS setting
	if (!first)
		if (tdiff < test_time)
			return;
	first = false;

  last_time = currentTime;	// Record current time

	curr_frame++;				// Next frame
	curr_frame %= num_frames;	// Make sure frames wrap around
	
	UpdateCoords();
}
