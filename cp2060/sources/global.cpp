#include "globals.h"

std::string selectedName("None");
Object *mainModel = NULL;
Object *chairModel = NULL;
Object *world = NULL;

bool docalc = false;
float botYaw = 0;
float botOrigin[3] = { 0, 0, 0 };

int frame = 0;

clock_t clocks = 0;
float clockAtLastFrame = 0;
float clocks2NextFrame = 0;
float fps = 10;
float testTime = (float)1/float(fps);

bool playing = true;

bool multitexture_availiable = false;
bool use_multitexture = false;
int maxTexelUnits = 1;
int width = 600;
int height = 600;
veryLong currentTime = 0;
int scaleval = 128;
bool doscale = false;
bool show_resources = false;
bool useTextures = true;
float farClip = 200;
bool wireframe = false;
bool end_right = false;
bool quit = true;
float headYaw = 0;
float headPitch = 0;

float lightRot[3] = { 0, 0 ,0 };
bool useInterp = true;
bool keys[256];
bool pauseSystem = false;
float orbitDistance = 10;
bool mouseDown = false;
bool middleMouseDown = false;
float camX = 0;
float camY = -190;
float diffX, diffY;
Object *selectedObject = NULL;

bool reloading = false;
bool dotrace = false;
bool ready = false;
bool developer = true;
Light *light[4];
CCamera camera;
