#include "globals.h"
#include "config.h"
#ifndef WIN32
// Linux stuff
	#include <fnmatch.h>
	#include <dirent.h>
	#include <dlfcn.h>											// Include dlopen and friends... (runtime library loading functions)
#else
// Windows stuff
	#include <windows.h>
#endif

CFont *util_font = 0;

#ifndef WIN32
	#define APIENTRY 
	#define __stdcall 
#endif
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARBProxy = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARBProxy = NULL;
PFNGLMULTITEXCOORD2FPROC		glMultiTexCoord2fARBProxy = NULL;
PFNGLMULTITEXCOORD2DPROC		glMultiTexCoord2dARBProxy = NULL;
void APIENTRY glActiveTextureARB (GLenum p)
{
	glActiveTextureARBProxy(p);
}
void __stdcall glMultiTexCoord2fARB(int stream, float u, float v)
{
	glMultiTexCoord2fARBProxy(stream, u ,v);
}
void __stdcall glMultiTexCoord2dARB(int stream, double u, double v)
{
	glMultiTexCoord2dARBProxy(stream, u ,v);
}
void APIENTRY glClientActiveTextureARB (GLenum p)
{
	glClientActiveTextureARBProxy(p);
}

void readSettings()
{
	AppConfig settings;
	settings.SetConfigFile("settings");
	settings.GetSetting("Window", "width", width);
	settings.GetSetting("Window", "height", height);

	settings.GetSetting("Textures", "scale", doscale);
	settings.GetSetting("Textures", "scaleval", scaleval);
	
	extern int defaultLightMapSize;
	extern int maxLightMapSize;
	extern bool showtrace;
	
	settings.GetSetting("Textures", "showtrace", showtrace);
	settings.GetSetting("Textures", "dotrace", dotrace);
	settings.GetSetting("Textures", "lightmapsize", defaultLightMapSize);
	settings.GetSetting("Textures", "maxlightmapsize", maxLightMapSize);
}

bool loadResources()
{
	CResourceManager::getInstance().createResource(util_font, "magicfont1");
//	CResourceManager::getInstance().createResource(groundTexture, "grass2.tga");
	if (util_font == 0)
		return false;
	return true;	
}
/**
 * This function will return a pointer to the function that is in the shared library
 * referenced by the handle.
 * @param handle The handle to the shared library
 * @param func The function name
 * @return NULL if no function otherwise a pointer to the function 
 */
void *getFunc(void *handle, char *func)
{
	void *f;
	char *error = NULL;

	#ifndef WIN32
	// We are on a unix like system use dlsym to load the functionn
	dlerror(); 																	// Clear any old errors
	f = dlsym(handle, func);										// Get the entry point to the functionn
	error = dlerror();													// Get any error 
	#else
	// Win32 stuff goes here
	f = (void*)GetProcAddress((HINSTANCE)handle, func);		// Get the entry point using Win32 funcs
	// TODO: Change to use GetLastError()	instead of returning 'Unknown error'
	if (f == NULL)														
		error = "Unknown error";
	#endif
	// If there was an error than exit with an error
	if (error != NULL)
	{		
		DebugText(ERROR_FATAL, "Fatal Error while resolving function %s: %s", func, error);
		exit(3);
	}
	// All was good return the ptr
	return f;
}

bool init()
{
//#ifdef PFNGLACTIVETEXTUREARBPROC	
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
#ifdef WIN32
		glMultiTexCoord2fARBProxy		= (PFNGLMULTITEXCOORD2FPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord2dARBProxy		= (PFNGLMULTITEXCOORD2DPROC) wglGetProcAddress("glMultiTexCoord2dARB");
		glActiveTextureARBProxy   		= (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARBProxy	= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");	
#else
	// Linux funcs
	/* 
	Use the POSIX standard way of loading a shared library(unix, linux, Sun OS, etc)
	RTLD_LAZY simply means no symbols are resolved from the library
	until they are specifically loaded.. Decreases load time.
	*/
	void *handle = dlopen("libGL.so", RTLD_LAZY);
	if (!handle)
	{
		DebugText(EMSG, "Unable to load libSDL.so: %s", dlerror());
		return false;
	}
	dlerror();
	
	// Load the symbols
//	(void*) 						= getFunc(handle, "SDL_Init"); 
		glMultiTexCoord2fARBProxy		= (PFNGLMULTITEXCOORD2FPROC) getFunc(handle, "glMultiTexCoord2fARB");
		glMultiTexCoord2dARBProxy		= (PFNGLMULTITEXCOORD2DPROC) getFunc(handle, "glMultiTexCoord2dARB");
		glActiveTextureARBProxy   		= (PFNGLACTIVETEXTUREARBPROC) getFunc(handle, "glActiveTextureARB");
		glClientActiveTextureARBProxy	= (PFNGLCLIENTACTIVETEXTUREARBPROC) getFunc(handle, "glClientActiveTextureARB");	

		
#endif
		if (glMultiTexCoord2fARBProxy != 0 && glActiveTextureARBProxy != 0)
		{
			multitexture_availiable = true;
			DebugText(EMSG, "Multi texture is availiable!");
		}
		else
		{
			multitexture_availiable = false;
			DebugText(EMSG, "Multi texture is NOT availiable! 0 DISABLING SHADOW MAPS!");
			dotrace = false;
			
		}
	
//#endif

	clocks =  clock();
	clocks2NextFrame = clocks + testTime*CLOCKS_PER_SEC;
	
	glClearColor(0,0,0,0);				// Black clear colour
	glShadeModel(GL_SMOOTH);			// Smooth shading
	glCullFace(GL_BACK);					// Cull back faces
	glEnable(GL_CULL_FACE);				// Turn on culling
	glEnable(GL_DEPTH_TEST);			// Turn on depth testing

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!loadResources())
		return false;

	//light[0] = new Light(255, 255, 255);
	//lightManager.addLight(light[0]);

	light[1] = new Light(128, 128, 128);
	light[1]->setStatic(false);	
	lightManager.addLight(light[1]);
	//light[2] = new Light(255, 0, 0);
	//lightManager.addLight(light[2]);
	
	//light[3] = new Light(255, 0, 0);
	//lightManager.addLight(light[3]);

	//world = new Object("WORLD");
//	loadModelFile(worldFile, world);
	if (checkReload() == false)
		return false;
		
	//reload(modelFile);
	//readFrames(defaultFrames, mainModel);
	//world->addChild(mainModel);

//	world->addChild(chairModel);
	
//	setupMenu();
	//initReloadThread();
	
	//world->generateMatrix(0);

	if (dotrace)
	{
		extern bool beginLight;
		beginLight = true;
		// Make sure the matrices have been generated
		world->generateMatrix(0);
		world->draw(0);
		// Get the positions of all the lights
		int l = 0;
		VECTOR3 lights[80];
		for (int i = 0; i < lightManager.getNumberOfLights(); i++ )
		{
			if (lightManager.getLight(i)->getStatic())
			{
				Light *li = lightManager.getLight(i);
				lights[l] = li->origin;
				l++;
			}
		}
		// Generate light maps for the world and it's children
		world->doLightMaps(lights, l);
		beginLight = false;
	}





	return true;	
}

