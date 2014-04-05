#ifndef __globals_h__
#define __globals_h__
#define GLUT_API_VERSION 4
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <math.h>
#include <string>
#include <time.h>

#include "common.h"
#include "frames.h"
#include "object.h"

#include "model.h"
#include "menu.h"
#include "utils.h"
#include "3dmath.h"
#include "camera.h"
#include "font.h"
#include "ctexture.h"
#include "lightobject.h"
#include "lightmanager.h"
#include "waypointobject.h"
#include "resource_manager.h"

// A pointer to the main character model
extern Object *mainModel;
// A pointer to the chair model
extern Object *chairModel;
// The ROOT node of the entire world
extern Object *world;
// A pointer to the currently selected object
extern Object *selectedObject;

// Pointers to hard-coded lights
extern Light *light[];
// The bot's current yaw
extern float botYaw;
// The bot's current origin
extern float botOrigin[3];
// The current frame
extern int frame;

// Last recorded time 
extern clock_t clocks;
// The recorded time at the end of the last frame
extern float clockAtLastFrame;
// The time we are exepcting it to be at the next frame
extern float clocks2NextFrame;
// The current frames per second we want to animate at
extern float fps;
// How much time one frame takes
extern float testTime; 
// Are we playing animations
extern bool playing;
// A camera instance.
extern CCamera camera;
// Is multi-texture availiable on this card?
extern bool multitexture_availiable;
// Do we want to ue multitexturing
extern bool use_multitexture;
// The number of streams availiable to use in multi-texturing
extern int maxTexelUnits;
// The width of the window
extern int width;
// The height of the window
extern int height;
// The current time ( should be same as clocks )
extern veryLong currentTime;
// A scale value that *ALL* loaded textures are scaled to. (eg, 64, 128, 256)
// Good for slow vid-cards that can't handle large (>256x256) textures
extern int scaleval;
// Should be scale the textures
extern bool doscale;
// Show the resources on screen?
extern bool show_resources;
// Do we want to display textures or just blank materials?
extern bool useTextures;
// The far clip of the camera
extern float farClip;
// Are we in wireframe mode
extern bool wireframe;
// Has the program been terminated correctly?
extern bool end_right;
// Should the application quit?
extern bool quit;
// The yaw of the head
extern float headYaw;
// The pitch of the head
extern float headPitch;
// Is the left mouse button down?
extern bool mouseDown;
// is the middle mouse button down?
extern bool middleMouseDown;
// The current yaw of the camera
extern float camX;
// The current pitch of the camera
extern float camY;
// The difference between the last recorded mouse pos and the current pos
extern float diffX, diffY;
// The amount of distance the camera should be orbititing from
extern float orbitDistance;
// Is the system paused?
extern bool pauseSystem;
// An array holding the state of all key's on the keyboard ( true = down )
extern bool keys[];
// Use interpolation to interpolate angles of the character
extern bool useInterp;
// The current yaw(y rot) of each hard-coded light
extern float lightRot[];

// Are we in the middle of reloading data from files
extern bool reloading;
// Should be do a shadow trace? (slow!!)
extern bool dotrace;
// Are we ready to start display? or are we still loading?
extern bool ready;
// Are we in developer mode and should be print out MORE debug stuff
extern bool developer;

// The name of the currently selected object
extern std::string selectedName;
// Stack holding the waypoints needed to go to a path
extern std::stack<WaypointObject *> pathStack;

// Function definitions
extern void idle();
extern void readSettings();
extern bool init();
extern void display();
void keyUp(int key, int x, int y);
void specialKeyboard(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
extern bool checkReload();
void orbit(float target[3], float pitch, float yaw, float distance, float p[3]);
extern bool reload();
void displayHelp();
extern void cleanup();

#endif // __globals_h__
