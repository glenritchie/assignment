#include <signal.h>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <queue>


#include "globals.h"

void cleanup();
using namespace std;
void setupMenu(void);

/** Reload the world files */
bool reload()
{
	// Load main model
	loadModelFile(worldFile, world);
	
	Object *parentMainModel = world->findObjectByName("object robotA");
	if (parentMainModel == NULL)
	{
		DebugText(ERROR_FATAL, "Cannot find object 'robotA' in world file!");
		return false;
	}
	mainModel = parentMainModel->getChild(0);
	if (mainModel == NULL)
	{
		DebugText(ERROR_FATAL, "Cannot find child object of 'robotA'. MUST have at least 1 child!!");
		return false;
	}
	parentMainModel->getOrigin(botOrigin);
	float zero[3] = { 0,0,0 };
	parentMainModel->setOrigin(zero);
	readFrames(currentFrameFile, mainModel);

	setupMenu();

	return true;
}
/** Does the file in memory data need to be reloaded. Works good on linux cause it can stat the modification time of the directory.
 * But under windows it don't work so good 
 */
bool checkReload()
{
	// Record the files current time
	static struct stat lastObjectFileStat;
	
	struct stat currentObjectFileStat;
	stat("models", &currentObjectFileStat);
	
	if (lastObjectFileStat.st_mtime != currentObjectFileStat.st_mtime)
	{
		DebugText(EMSG, "Directory changed: Reloading model files");
		stat("models", &lastObjectFileStat);
		return reload();
	}
	return true;
}


float oz = 0;
void setupMenu(void);

void reshape(int w, int h)
{
	width = w;
	height = h;
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float x = w, y = h;
	if (x > y)
	{
		x = h; y = w;
	}
		
	float aspect = (float)w / (float)h;
	gluPerspective(70.0, aspect, 0.1f, farClip);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.SetFarClip(farClip);
	double p[16];
	glGetDoublev(GL_PROJECTION_MATRIX, p);
	MATRIX p_m(p);
	camera.SetProjectionMatrix(p_m);
}
float lang = 0;

void visible(int vis)
{
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(idle);
	else 
		glutIdleFunc(NULL);
}

VECTOR3 origin(0,0,0);


/**
 * This function is called on cleanup no matter what happens.
*/
void cleanup()
{
	
	if (!end_right)
		DebugText(EMSG, "Program failed!!.....");
	// Free up pointers
	CResourceManager::getInstance().Cleanup();
}

/**
 * This is called whenever a program receives a signal
*/
void signalHandler(int signum)
{
	// Break out if these occur
	quit = true;
	DebugText(EMSG, "Got signal: 0x%x", signum);
	switch (signum)
	{
		case SIGINT:	// Ctrl-C
			DebugText(EMSG, "Program asked to quit with 'SIGINT'");
			// Do not send this signal on. We only want to set a quit flag and ignore this signal
			return;
		case SIGTERM:	// Progam asked to quit ( eg. someone typed 'kill' )
			DebugText(EMSG, "Program asked to quit with 'SIGTERM'");
			// Do not send this signal on. We only want to set a quit flag and ignore this signal
			return;
		case SIGSEGV:	// Program caused a core dump!!
			// Damn! Print error messages
			// 9/07/2004 12:33AM: 
			// NOTE: I don't know how to make a platform independant stack trace
			// I half-know how to do it in win32 but i have no idea how in Linux.
			// If the library is POSIX compatible i can use sigaction instead of signal
			// and get the address of the crash but the mingw32 3.2 library dosn't have sigaction defined.
			DebugText(EMSG, "Program caused a segmentation fault!!");
			printf("Segmentation fault....");
			exit(-1);
			break;
		default:
			DebugText(EMSG, "Unknown signal caught!!??? 0x%x", signum);
	}
	// By default send this signal on to any other handlers
//	raise(signum);
}

void setupSignalHandlers()
{
	// Set up some signal handlers
	signal(SIGINT, signalHandler);			// Catch Ctrl-C
	signal(SIGTERM, signalHandler);			// Catch "kill"
	
	// Catch segmentation faults ( CORE DUMPS ). So we can do some type of handling
	signal(SIGSEGV, signalHandler);

	// This function will be called on exit unless it is forced with a 'kill -9' or something similar
	atexit(cleanup);

	DebugText(EMSG, "Crash handlers initialized...");
}

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		DebugText(EMSG, "Overriding default world with '%s'", argv[1]);
		worldFile = string(argv[1]);
	}
	
	//setupSignalHandlers();
	// Read settings from file
	readSettings();
	
	// Let glut handle it's own commands. This includes things
	// like indirect contexts, X syncing, etc
	// Defaults are usually fine
	glutInit(&argc, argv);
	
	// Mode settings
	// DOUBLE 		- Double buffer
	// RGBA  			- Red, Green, Blue AND Alpha bits
	// DEPTH			- Want depth buffer
	unsigned int modeSettings = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH; 
	
	// Init the display
	glutInitDisplayMode(modeSettings);
	DebugText(EMSG, "Creating window of size: width:%d, height:%d", width, height);

	// Create a window
	glutInitWindowSize(width ,height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	// Init functions that only need to be done once
	if (!init())
	{
		DebugText(ERROR_FATAL, "There was a fatal error while initializing the system");
		// Print out the tree if the world is not null
		if (world != NULL)
		{
			DebugText(EMSG, "Printing tree:");
			world->printTree(0);
		}
		return -1;
	}
	
	glutDisplayFunc(display);
	glutSpecialUpFunc(keyUp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutVisibilityFunc(visible);

	ready = true;
	// Start main loop
	glutMainLoop();
	return 0;
}


