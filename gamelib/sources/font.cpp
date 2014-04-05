#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>

#include "font.h"
#include "utils.h"
#include <map>
#include <string>
#include "gl_varray.h"
#include <stdarg.h>


bool useGlutFonts = true;



CFont::CFont()
{
	fontTexture = 0;
	//	font_height = 18;
	//	font_width = 16;
	float ratio = 16.0f/18.0f;
	font_height = 10;
	font_width = (unsigned int)((float)font_height * ratio);
}

void CFont::createRenderTarget()
{
	
}
void CFont::setHeight(unsigned int newHeight)
{ 
	font_height = newHeight; 
	font_width = newHeight;
}


/*
0-3
| |
1-2
*/
/*		FVERTEX box[4] = {
{ x,				y + font_height,	0, 1.0f, colour, 0.0f, 1.0f }, 
{ x,				y,					0, 1.0f, colour, 0.0f, 0.0f },
{ x + font_width,	y + font_height,	0, 1.0f, colour, 1.0f, 1.0f },
{ x + font_width,	y,					0, 1.0f, colour, 1.0f, 0.0f },
};															 
*/

void CFont::Draw2DStrings(fontStrings stringArray[], unsigned int count)
{
	if (!in_memory || !fontTexture)	// If we ain't in mem or we ain't loaded
		if(!LoadResource())		// Load it!
			return;				// But don't do anything if load failed
		if (count == 0)
			return;
		UpdateLastAccessTime();
		
		// Figure out how many verts we are going to need
		// Each character needs 4 vertices so...
		//	unsigned int vertsNeeded = 0;
		//	for (unsigned int i = 0; i < count; i++)
		//		vertsNeeded += strlen(stringArray[i].msg);
		//	vertsNeeded *= 4;
		
		
		// Fill the vertex buffer with a list of verts and coords for all the strings in the array
		unsigned int characterCount = 0;
		//	float su,eu,sv,ev;
		fontTexture->ActivateTexture(0);
		unsigned int tid = fontTexture->getID();
		this->setBatch(true);
		
		for (unsigned int i = 0; i < count; i++)
		{
			// Create a vertex buffer of size 
			//		VertexSet *v = new gl_varray(VertexSet::STRIP);
			//		v->setUseNormal(false);
			
			int x = stringArray[i].x;
			int y = stringArray[i].y;
			unsigned long colour = stringArray[i].colour;
			
			//		glLoadIdentity();
			//		glTranslatef(stringArray[i].x, stringArray[i].y, 0)
			
			//FVERTEX &letterVerts = pVertices[characterCount];
			//			0-2
			//			|\|
			//			1-3
			// Colour comes in as a DWORD int each colour being 0-255. We need to break it up into the component parts
			// Each colour is 8 bits or one byte ( 0-255 )
			// Colour is RGBA
			// AARRGGBB
			unsigned int alpha_mask = 0xFF000000;
			unsigned int red_mask   = 0x00FF0000;
			unsigned int green_mask = 0x0000FF00;
			unsigned int blue_mask  = 0x000000FF;
			
			unsigned int a = (colour & alpha_mask) 	>> 24;
			unsigned int r = (colour & red_mask) 		>> 16; 
			unsigned int g = (colour & green_mask) 	>> 8;
			unsigned int b = (colour & blue_mask);
			glColor4f(
				(((float)r)/255), 
				(((float)g)/255), 
				(((float)b)/255), 
				(((float)a)/255));
				/*	
				Vertex letterVerts[4] = {
				// X                Y                   Z     red       green     blue     alpha   tex1u tex1v  tex2u   tex2v   normal(3 floats)
				{ x,							y,										0, (float)r, (float)g, (float)b, (float)a, 0.0f, 0.0f,  0.0f,	0.0f, 0, 0, 0 },	// BL
				{ x,							y + font_height,			0, (float)r, (float)g, (float)b, (float)a, 0.0f, 1.0f,  0.0f,	0.0f, 0, 0, 0 },  // TL
				{ x + font_width,	y,										0, (float)r, (float)g, (float)b, (float)a, 1.0f, 0.0f,  0.0f,	0.0f, 0, 0, 0 },	// BR
				{ x + font_width,	y + font_height,			0, (float)r, (float)g, (float)b, (float)a, 1.0f, 1.0f,  0.0f,	0.0f, 0, 0, 0 },	// TR
				};															 
			*/
			if (useGlutFonts)

			{

				glRasterPos2i(x, y);

				for (unsigned int letter = 0; letter < strlen(stringArray[i].msg); letter++)

				{

					//glutStrokeCharacter(GLUT_STROKE_ROMAN, stringArray[i].msg[letter]);

					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, stringArray[i].msg[letter]);

				}

			}

			else

			{

				glBegin(GL_TRIANGLE_STRIP);
				for (unsigned int letter = 0; letter < strlen(stringArray[i].msg); letter++)
				{
					char printValue = stringArray[i].msg[letter];
					glTexCoord2f(uvList[printValue].su, uvList[printValue].ev); glVertex2f(x, y);
					glTexCoord2f(uvList[printValue].su, uvList[printValue].sv); glVertex2f(x, y + font_height);
					glTexCoord2f(uvList[printValue].eu, uvList[printValue].ev); glVertex2f(x + font_width, y);
					glTexCoord2f(uvList[printValue].eu, uvList[printValue].sv); glVertex2f(x + font_width, y + font_height);
					x += font_width;
				}
				glEnd();
			}

		}
		this->setBatch(false);

}

void CFont::Draw2DText(int x, int y, unsigned long colour,  char *format, ... )
{
	if (!in_memory || !fontTexture)	// If we ain't in mem or we ain't loaded
		if(!LoadResource())		// Load it!
			return;				// But don't do anything if load failed
		
		UpdateLastAccessTime();
		
		char msg[2000];
		va_list argptr;
		va_start (argptr, format);
		vsprintf (msg, format, argptr);
		va_end (argptr);
		
		// For each word either grab or add word
		
		/*	FVERTEX box[4] = {
		{ x,				y + font_height,	0, 1.0f, colour, 0.0f, 1.0f }, 
		{ x,				y,					0, 1.0f, colour, 0.0f, 0.0f },
		{ x + font_width,	y + font_height,	0, 1.0f, colour, 1.0f, 1.0f },
		{ x + font_width,	y,					0, 1.0f, colour, 1.0f, 0.0f },
		};															 
		*/
		
		if (!batch)
		{
		/*		SetRenderState(D3DRS_ALPHABLENDENABLE , true);
		SetRenderState( D3DRS_LIGHTING, false);
		
		 SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA  );
		 SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA  );
		 SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA );
		 SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		 SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		 SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		 SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		 SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
		 SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
		 SetTextureStageState(0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE);
			*/
		}
		
		fontTexture->ActivateTexture(0);
		
		//	SetVertexShader( D3DFVF_FVERTEX );
		//	SetStreamSource(0, NULL, sizeof(FVERTEX));
		
		for (unsigned int i = 0; i < strlen(msg); i++)
		{
			char printValue = msg[i];
			
			/*		box[0].u = uvList[printValue].su;	box[0].v = uvList[printValue].ev;
			box[1].u = uvList[printValue].su;	box[1].v = uvList[printValue].sv;
			box[2].u = uvList[printValue].eu;	box[2].v = uvList[printValue].ev;
			box[3].u = uvList[printValue].eu;	box[3].v = uvList[printValue].sv;
			
			 //	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, box, sizeof(FVERTEX));
			 
			  for (unsigned int j = 0 ;j<4;j++)
			  box[j].x += font_width;
			*/
		}
		
		if (!batch)
		{
			//		SetRenderState(D3DRS_ALPHABLENDENABLE , false);
			//		SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			//		SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		}
		
		UpdateLastAccessTime();
}


void CFont::setBatch(bool newstatus)
{ 
	//	Profile Func(__FUNCTION__);
	if (!in_memory)	// If we ain't in mem or we ain't loaded
		if(!LoadResource())		// Load it!
			return;				// But don't do anything if load failed
		UpdateLastAccessTime();
		batch = newstatus; 
		if (batch)
		{

			if (useGlutFonts)

			{

				glPushAttrib(GL_ENABLE_BIT);

				glDisable(GL_TEXTURE_2D);

			}


		/*		SetRenderState(D3DRS_ALPHABLENDENABLE , true);
		SetRenderState( D3DRS_LIGHTING, false);
		
		 SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA  );
		 SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA  );
		 SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA );
		 SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		 SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		 SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		 SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		 SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
		 SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
		 SetTextureStageState(0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE);
			*/
		}
		if (!batch)
		{
			if (useGlutFonts)

			{

				glPopAttrib();

			}

		/*		SetRenderState(D3DRS_ALPHABLENDENABLE , false);
		SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
			*/
		}
}
bool CFont::LoadResource()
{
	CResourceManager::getInstance().createResource(fontTexture, "util_font");
	
	fontTexture->SetFrameSize(8, 8);
	fontTexture->SetNumFrames(255);
	fontTexture->SetFrame(0);
	fontTexture->StopAnimation();
	
	fontTexture->setFilterType(GL_NEAREST);
	fontTexture->invalidate();
	//textureFont
	
	in_memory = true;
	
	for (unsigned int i = 0; i < 255; i++)
	{
		fontTexture->SetFrame(i);
		fontTexture->GetTexcoords(uvList[i].su, uvList[i].eu, uvList[i].sv, uvList[i].ev);
		//		DebugText(EMSG, "frame: %d, su: %.2f, eu: %.2f, sv: %.2f, ev: %.2f", 
		//				i, uvList[i].su, uvList[i].eu, uvList[i].sv, uvList[i].ev);
		
		//uvList[i].eu = 
	}
	
	UpdateLastAccessTime();
	return true;
}

void CFont::UnloadResource()
{
	
	if (fontTexture)
	{
		fontTexture->Release();
		fontTexture = 0;
	}
	in_memory=false;
}

void CFont::Think()
{
}
