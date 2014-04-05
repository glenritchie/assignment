#include "globals.h"

void keyUp(int key, int x, int y)
{
//	printf("keyup: %d\n", key);
	keys[key] = false;
	//switch (key)
}



void specialKeyboard(int key, int x, int y)
{
	if (pauseSystem)
		pauseSystem = false;
	//mod = glutGetModifier()
	//	if (mod == GLUT_ACTIVE_SHIFT)

//	printf("speckey: %d\n", key);
	keys[key] = true;
	switch (key)
	{
		// Turn light 0 off
		case GLUT_KEY_F9:
			//light[0]->flipEnabled();
			handleOption(LIGHT0);
			break;
			// Turn light 1 off
		case GLUT_KEY_F10:
			handleOption(LIGHT1);
			//light[1]->flipEnabled();
			break;
			// Turn light 2 off
		case GLUT_KEY_F11:
			handleOption(LIGHT2);
			//light[2]->flipEnabled();
			break;
		case GLUT_KEY_F1:
			handleOption(HELP);
//			pauseSystem = true;
			break;
		// Turn textures on/off
		case GLUT_KEY_F3:
			handleOption(TEXTURES);
	//		useTextures = !useTextures;
			break;

		// Turn wireframe mode on/off
		case GLUT_KEY_F2:
			handleOption(WIREFRAME);
			break;
/*			{	
				wireframe = !wireframe;
				if (wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
			*/
	}
}

void keyboard(unsigned char key, int x, int y)
{
//	printf("keyboard: %d\n", key);
//	keys[key] = true;
//	Draw2DText(10, 62, 0xFFFFFFFF, "bPoint: 16%%%d", bPoint%4, bPoint);
//	Draw2DText(10, 74, 0xFFFFFFFF, "bPoint: 16/%d", bPoint/4, bPoint);

if (pauseSystem)
		pauseSystem = false;
	switch (key)
	{
/*		case 'w':
			ctrlPts[i][j][0] -= 0.1;
			break;
		case 's':
			ctrlPts[i][j][0] += 0.1;
			break;
		case 'a':
			ctrlPts[i][j][2] -= 0.1;
			break;
		case 'd':
			ctrlPts[i][j][2] += 0.1;
			break;
		case 'e':
			ctrlPts[i][j][1] += 0.1;
			break;
		case 'c':
			ctrlPts[i][j][1] -= 0.1;
			break;
	
		
		case 'q':
			bPoint--;
			break;
		case 'z':
			bPoint++;
			break;
*/
		case 'f':
			handleOption(FOLLOW); break;	
		case 'c':
			handleOption(DOCALC); break;
		case '=':
			handleOption(ANIMATION_FPS_UP); break;
		case '-':
			handleOption(ANIMATION_FPS_DOWN); break;
//		case 'F':
//		case 'f':
//			handleOption(SWITCH_FONTS); break;
		case ']':
			handleOption(FORWARD_FRAME); break;
		case '[':
			handleOption(BACKWARD_FRAME); break;
		case 'P':
		case 'p':
			handleOption(PAUSE); break;
		case 'W':
			handleOption(WIREFRAME); break;
		case 'R':
		case 'r':
			handleOption(RIGHT_KICK); break;
		case 'L':
		case 'l':
		//	handleOption(LEFT_KICK); break;
			handleOption(LEFT_PUNCH); break;
		case 'H':
		case 'h':
			handleOption(HEAD_KICK); break;
		case 'a':
		case 'A':
			handleOption(LOOK_AT_WATCH); break;
		// Quit the program
		case 27:		// ESC key
		case 'q':
		case 'Q':
			handleOption(QUIT); break;
		//case 'i':
		//	handleOption(INTERP); break;
	}
}


