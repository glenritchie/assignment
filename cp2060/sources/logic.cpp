#include "globals.h"
#include "waypointobject.h"
#include "sphere.h"
#include "frames.h"

float ChangeYaw(float &current_angle, const float &ideal_yaw, float speed);

// Different bot states
enum e_botState
{
	WALKING,
	IDLE,
	ACTION
};

// Current bot state
e_botState botState = IDLE;
// What the previous bot state was
e_botState oldBotState = IDLE;
// Is the bot following a path
bool follow = false;
// The waypoint stack
std::stack<WaypointObject *> pathStack;
// Should be running an action at this waypoint?
bool doWayAction = false;
// Are we actually doing the action at this waypoint or are we waiting ( eg. turning so we can sit?)
bool doingAction = false;
// The last waypoint that we reached
WaypointObject *lastWaypoint = 0;

void setBotState(e_botState newState)
{
	oldBotState = botState;
	botState = newState;
}

// Stop the bot and reset it's frames to idle
void stop()
{
	//stopped = true;
	setBotState(IDLE);
	readFrames(idleFrameFile, mainModel);
}

// move the bot forward a bit
void moveUp()
{
	float cy = cosf(deg2rad(botYaw));
	float sy = sinf(deg2rad(botYaw));
	botOrigin[0] += (sy / 2) ;
	botOrigin[2] -= -(cy / 2) ;
	setBotState(WALKING);
}

void processTime()
{
	// Get the amount of time per frame	
	testTime = (float)1/float(fps);
	
	// Get the current wall-time
	currentTime = clock();
	clock_t diff = currentTime - clocks;
	float secs = ((float)diff/(float)CLOCKS_PER_SEC);
	// If the amount of time that has passed is larger than a single frame then we should go to the next frame.
	if (secs > testTime)
	{
		// The current time
		clocks = currentTime;
		// Get the amount of clocks to the next frame
		clocks2NextFrame = clocks + testTime * CLOCKS_PER_SEC;
		// If we are playing then increase the frame count
		if (playing)	
			frame++;
		// If we have gone passed the number of frames for this anim and we are in a different anim then change to the idle action.
		if (frame >= FRAMES && currentFrameFile != defaultFrames )
		{
			// This frame has ended so read the default back in
			readFrames(defaultFrames, mainModel);
		}
		// Make sure we wrap around the frames 
		frame %= FRAMES;
	}
}

bool processWaypoints()
{
	// If not and we have paths to follow and we have been told to follow them.. then...
	if (follow && !pathStack.empty())
	{
		// We want to get the top of the pathStack
		WaypointObject *o = pathStack.top();
		// Find the location of the waypoint in world co-ords
		float h[3];
		o->findObjectWorldCoords(h[0], h[1], h[2]);
		// Now find 
		// get vector from bot to cam
		VECTOR3 bo(botOrigin[0], botOrigin[1], botOrigin[2]);
		VECTOR3 lv = VECTOR3(h[0], h[1], h[2]) - bo;
		lv = Normalize(lv);
		// Ok. we got some angles
		VECTOR3 va(lv.x, 0, lv.z);

		// Convert lv into angles
		float yaw = atan2(lv.x, lv.z) * (180/M_PI);
		//DebugText(EMSG, "yaw: %.2f", yaw);
		if (yaw < 0) 	yaw += 360;
		if (yaw > 360) yaw -= 360;
		//DebugText(EMSG, "yaw: %.2f", yaw);
		if (botYaw < 0) 	botYaw += 360;
		if (botYaw > 360) botYaw -= 360;
			
		// If we arn't within 10 degrees of facing the point then keep turning
		float step = 10.0f;
		if (fabs(botYaw - yaw) < step )
			botYaw = yaw;
		else
			ChangeYaw(botYaw, yaw, step);
			
		// We should be walking!	
		setBotState(WALKING);
			
		bo = VECTOR3(botOrigin[0], botOrigin[1], botOrigin[2]);
		// if the distance is less than 2 then we have reached the waypoint
		VECTOR3 vh(h[0], h[1], h[2]);
		if (Distance(bo, vh) < 1)
		{
			lastWaypoint = pathStack.top();
			
			// Pop the top of the stack OFF
			pathStack.pop();
			if (pathStack.empty())
			{	
				cout << "Reached end of stack!" << endl;
				cout << "Lastwaypoint = " << lastWaypoint << endl;
				// Reached the end of our path
				// check if the waypoint we last popped had an action.
				// If so we need to perform that action
				if (lastWaypoint != NULL)
				{
					cout << "action: " <<  lastWaypoint->getAction() << endl;
					if (lastWaypoint->getAction().size() != 0)
					{
						doWayAction = true;
						setBotState(ACTION);
						readFrames(idleFrameFile, mainModel);
						//stop();
						cout << "Telling can doWayAction to true!" << endl;
					}
				}
				follow = false;
			}
		}
		return true;
	} 
	// If we have been told to do the action and we are at the last waypoint 
	else if (doWayAction && lastWaypoint != NULL)
	{
		// Turn until we reach lastWaypoint->getActionAngle()
		float step = 10.0f;
		if (doingAction)
		{
			// Wait until current file ends. then do idle
			if (frame == FRAMES-1)
			{
				cout << "doingAction done!" << endl;
				idleFrameFile = lastWaypoint->getPostAction();
				setBotState(IDLE);
				doingAction = false;
				doWayAction = false;
				lastWaypoint = NULL;
			}
		}
		// We are done. Let's execute the action
		else if (fabs(botYaw - lastWaypoint->getActionAngle()) <= step )
		{
			cout << "Animating action!" << endl;
			botYaw = lastWaypoint->getActionAngle();
			// Reached the yaw. Now change our animation to the action!
			std::string action = lastWaypoint->getAction();
			readFrames(action, mainModel);
			setBotState(ACTION);
			doingAction = true;
			//doWayAction = false;
			frame = 0;
		}
		// Do nothing, let's keep changing the yaw
		else
			ChangeYaw(botYaw, lastWaypoint->getActionAngle(), step);
		return true;	
	}
	return false;
}
void collideBall()
{
	SphereObject *ball = CastTo<SphereObject>(world->findObjectByName("object ball"));

	
		if (ball != NULL)
		{
			float ballWorldOrigin[3];
			ball->findObjectWorldCoords(ballWorldOrigin[0], ballWorldOrigin[1], ballWorldOrigin[2]);
			VECTOR3 ballOut;
			VECTOR3 ballNormal;
			float smallest = 9999999;
			Object *ballCollide = world->sphereCollide(VECTOR3(ballWorldOrigin[0], ballWorldOrigin[1], ballWorldOrigin[2]), 
								ball->getRadius(), ballOut, smallest, ballNormal);
			static bool bonce = false;
			if (!bonce)
			{
				ball->vel[0] = -1;
				ball->vel[1] = 1;
				ball->vel[2] = -0.2;
				bonce = true;
			}
			// Let's do some tricky maths!
			ball->accel[1] -= 0.0001f;
			if (ball->accel[1] < -0.02)
				ball->accel[1] = -0.02;
			ball->vel[1] += ball->accel[1];

			ball->getOrigin(ballWorldOrigin);

			if (ballCollide != NULL)
			{
				ballNormal = Normalize(ballNormal);
				VECTOR3 velocity(ball->vel[0], ball->vel[1], ball->vel[2]);
				VECTOR3 velocityTemp = velocity * -1;
		        float projection;
		        //calculate the projection.
		        projection = Dot(velocityTemp, ballNormal);
		        //Take the length of the projection against the normal.
		        VECTOR3 length = ballNormal * projection;
		        //Lets obtain the final vector.
		        VECTOR3 reflection = length * 2;
				VECTOR3 velocityFinal = reflection + velocity;
				//velocityFinal = velocityFinal / 1.2;

				//ball->accel[1] = -ball->accel[1];
				//ball->vel[1] = -ball->vel[1]/1.2;
				ball->vel[0] = velocityFinal.x;
				ball->vel[1] = velocityFinal.y;
				ball->vel[2] = velocityFinal.z;
				// Force the ball away from the collision
				//VECTOR3 bOrigin(ballOut, ballWorldOrigin[1], ballWorldOrigin[2]);
				VECTOR3 newOrigin = ballOut + (ballNormal * (ball->getRadius()/*+(ball->getRadius()*0.5)*/));
				ballWorldOrigin[0] = newOrigin.x;
				ballWorldOrigin[1] = newOrigin.y;
				ballWorldOrigin[2] = newOrigin.z;
				ball->setOrigin(ballWorldOrigin);
			}
			else
			{
				ballWorldOrigin[0] += ball->vel[0];
				ballWorldOrigin[1] += ball->vel[1];
				ballWorldOrigin[2] += ball->vel[2];
				ball->setOrigin(ballWorldOrigin);
			}
			Draw2DText(10, 82, 0xFFFFFFFF, "ballCollide: %s: ball->vel[1]: %.2f", ((ballCollide == NULL) ? "no" : "yes"), ball->vel[1] );
			Draw2DText(10, 94, 0xFFFFFFFF, "accel x: %.2f, y: %.2f, z: %.2f", ball->accel[0], ball->accel[1], ball->accel[2] );




		}

}
void collideBot()
{
	Object *vv;
	Object *v0v;
	VECTOR3 dest, point, v, faceNormal;
	float botSmallestDistance;
	float smallestDistance;
	botSmallestDistance = 99999;

	VECTOR3 v0(0,0,0);
	point = VECTOR3(botOrigin[0], botOrigin[1]+5 , botOrigin[2]);
	dest = VECTOR3(botOrigin[0], botOrigin[1]-500 , botOrigin[2]);
	botSmallestDistance = 99999;
	v0v = world->rayCollide(point, dest, v0, botSmallestDistance, faceNormal);
	if (v0v)
		botOrigin[1] = v0.y;
	float mv[3];
	mainModel->getOrigin(mv);
	point = VECTOR3(mv[0], mv[1]+5, mv[2]);
	dest = VECTOR3(botOrigin[0], botOrigin[1]+5 , botOrigin[2]);
	botSmallestDistance = 99999;
	v0v = world->rayCollide(point, dest, v0, botSmallestDistance, faceNormal);
	if (v0v)
	{
		botOrigin[0] = mv[0];
		botOrigin[1] = mv[1];
		botOrigin[2] = mv[2];
	}
}
void collideCamera()
{
	Object *vv;
	Object *v0v;
	VECTOR3 dest, point, v, faceNormal;
	float botSmallestDistance;
	float smallestDistance;
	botSmallestDistance = 99999;

	//		v;
	point = VECTOR3(botOrigin[0], botOrigin[1]+15 , botOrigin[2]);
	float dist = 50;
	VECTOR3 p = camera.origin;
	VECTOR3 ray = p - point;

	ray = Normalize(ray);
	dest = point + ray * dist;
	smallestDistance = 99999;
	vv = world->rayCollide(point, dest, v, smallestDistance, faceNormal);
	//float d = Distance(point, v); 
	if (vv /*&& d < orbitDistance*/)
		orbitDistance = smallestDistance;
}

void idle()
{
	
	// Make robot face camera
/*	botFaceCam();	
	Object *o = world->findObjectByName("object head");
	if (o)
	{
		Draw2DText(10, 74, 0xFFFFFFFF, "found o");
		float p[3] = { -headPitch, headYaw-90, 0 };
	//	o->setAngles(frame, p); 
		o->setAngles(frame+1, p); 
	}
	else Draw2DText(10, 74, 0xFFFFFFFF, "NOT found o");
*/

	// If the mouse button was down then move the camera based on the difference the mouse moved
	if (mouseDown)
	{
		camY += diffX;
		camX += diffY;
	}
	else if (middleMouseDown)	// if the middle mouse is down then change how close the camera should orbit
	{
		orbitDistance += -diffY;
	}

	// These go before pauseSystem because we might still want to rotate around the scene although
	// The animation is paused
	float eye[3];
	float hx, hy ,hz;
	float t[3] = { botOrigin[0], botOrigin[1] +15, botOrigin[2] };
	orbit(t, camX, camY, orbitDistance, eye);
	// Set the camera object positions
	camera.lookAt(eye[0], eye[1], eye[2], t[0], t[1], t[2]);
//	DebugText(EMSG, 

	
	// If we haved paused the system then do no interpolation or anything
	if (pauseSystem)
	{
		glutPostRedisplay();
		return;
	}

	processTime();
	
	// Is the UP Arrow key being held DOWN
	if (keys[GLUT_KEY_UP])
		setBotState(WALKING);
	// If we didn't process any waypoint stuff then we are in IDLE state
	else if (!processWaypoints())
		setBotState(IDLE);	
	
	if (keys[GLUT_KEY_DOWN])
	{
		float cy = cosf(deg2rad(botYaw));
		float sy = sinf(deg2rad(botYaw));
		botOrigin[0] -= (sy / 2);
		botOrigin[2] += -(cy / 2);
	}
	if (keys[GLUT_KEY_LEFT])
		botYaw += 10;
	if (keys[GLUT_KEY_RIGHT])
		botYaw -= 10;
	
	lightRot[0] -= 0.1f;
	lightRot[1] += -0.2f;
	lightRot[2] += 0.4f;

	// Check the current state and make sure we are performing the right animation
	std::string walkFrameFile = "models/walk.frm";
	if (botState == WALKING)
	{
		if (oldBotState != WALKING)
		{
				readFrames(walkFrameFile, mainModel);
				idleFrameFile = "models/standing.frm";
		}
		moveUp();
	}
	else if (botState == IDLE && oldBotState != IDLE)
			readFrames(idleFrameFile, mainModel);
	
	// Check if any files need to be reloaded	
	checkReload();

	// Set the model's position based off the recorded origin
	mainModel->setOrigin(botOrigin);
	float br[] = { 0, botYaw, 0 };
	mainModel->setAngles(frame,  br);
	mainModel->setAngles((frame+1)%FRAMES,  br);

	// Do some collision checking
	collideBot();
	collideCamera();

	collideBall();
	
	glutPostRedisplay();
}


/**
 * Interpolate between angle and tangle. It uses the some global information to do so
 */
float interp(float angle, float tangle)
{
	// if we arn't playing and arn't interpolating then don't do anything
	if (!useInterp || !playing)
		return angle;
	clock_t t = clocks;
	clock_t diff = (clock_t)clocks2NextFrame - t;
	if (clocks2NextFrame < 0)
		return tangle;
	float diff2 = clocks - currentTime;	
	float ratio = diff / diff2;

	if (tangle > angle)
	{
		float adiff = fabs(angle - tangle) / ratio;
		return angle - adiff;
	}

	float adiff = fabs(tangle - angle) / ratio;
	return angle + adiff;
}

// Make the head of the bot follow the camera
void botFaceCam()
{
	// get vector from bot to cam
	VECTOR3 bo(botOrigin[0], botOrigin[1] + 13, botOrigin[2]);
	VECTOR3 lv = camera.origin - bo;
	lv = Normalize(lv);
	// Ok. we got some angles
	VECTOR3 va(lv.x, 0, lv.z);

	headPitch = acos(Dot(lv, va)) * (180/M_PI);		
	if (lv.y < 0)
		headPitch = -headPitch;
	// Convert lv into angles
	headYaw = atan2(lv.x, lv.z) * (180/M_PI);
//	headPitch = atan2(lv.z, lv.y) * (180/M_PI);
}

// Turn to this yaw
// Borrowed from Half-Life bot - HPB_BOT source... by 'botman'
// Modified for my use though
// Will return a yaw or -1 if it's reached it's ideal_yaw
float ChangeYaw(float &current_angle, const float &ideal_yaw, float speed)
{
	// if we have reached the yaw return -1
	float diff;
	int dir=-1;	// start left

	// find the difference in the current and ideal angle
	diff = (float)abs(int(current_angle - ideal_yaw));
	float oppdiff = 360 - diff;

	// check if the bot is already facing the idealpitch direction...
	if (diff <= 1 || oppdiff <=1)
	{
		return -1;
	}
	
	// check if difference is less than the max degrees per turn
	if (diff < speed)
		speed = diff;  // just need to turn a little bit (less than max)
	if (oppdiff < speed)
		speed = oppdiff;  // just need to turn a little bit (less than max)
	
	if (current_angle > ideal_yaw)
	{
		if (oppdiff < diff)
			current_angle += speed;
		else 
			current_angle -= speed;
	}
	else
	{
		// current_angle < ideal_yaw eg 0 < 270
		if (oppdiff < diff)
			current_angle -= speed;
		else current_angle += speed;
	}
	
	// Keep angles between 0-360
	if (current_angle<0) 		current_angle += 360;
	if (current_angle>360) 		current_angle -= 360;
	if (current_angle==360) 	current_angle = 0;

	return 0;
}


void handleOption(int itemID)
{
	switch (itemID)
	{
		case DOCALC:
		{
			WaypointObject *toWaypoint = CastTo<WaypointObject>(world->findObjectByName("object chairwaypoint"));
			if (pathStack.empty() && toWaypoint != NULL )
			{
				std::vector<WaypointObject *> v;
				float waydist = 9999999;
				WaypointObject *wo = wo->findNearestTo(VECTOR3(botOrigin[0], botOrigin[1], botOrigin[2]), waydist);
				wo->findPathTo(toWaypoint, v, pathStack);
			}
			break;
		}
		case FOLLOW:
			follow = true;
			break;
		case ANIMATION_FPS_DOWN:
			fps--;
			break;
		case ANIMATION_FPS_UP:
			fps++;
			break;
		case PRINTTREE:
			world->printTree(0);
			break;
		case FORWARD_FRAME:
			frame = (frame+1) % FRAMES;
			break;
		case BACKWARD_FRAME:
			frame = (frame-1) % FRAMES;
			break;
		case PAUSE:
			playing = !playing;
			break;
		case RELOADRESOURCES:
			CResourceManager::getInstance().invalidateAllResources();
			break;
		case RELOAD:
			readFrames(currentFrameFile, mainModel);
			reload();
			break;
		case RIGHT_KICK:
		{
			frame = 0;
			string model("models/kick_rightleg.frm");
			readFrames(model, mainModel);
			break;
		}
		case LEFT_KICK:
		{
			frame = 0;
			string model("models/kick_leftleg.frm");
			readFrames(model, mainModel);
			break;
		}
		case LEFT_PUNCH:
		{
			frame = 0;
			string model("models/left_punch.frm");
			readFrames(model, mainModel);
			break;
		}

		case HEAD_KICK:
		{
			frame = 0;
			string model("models/headkick_leftleg.frm");
			readFrames(model, mainModel);
			break;
		}
		case LOOK_AT_WATCH:
		{
			frame = 0;
			string model("models/lookatwatch.frm");
			readFrames(model, mainModel);
			break;
		}
		case QUIT:
			cleanup();
			exit(0);
		// Use interpolation
		case INTERP:
			useInterp = !useInterp;
			break;
		case LIGHT0:
//			light[0]->flipEnabled(); 
			break;
		case LIGHT1:
			light[1]->flipEnabled(); 
			break;
		case LIGHT2:
			light[2]->flipEnabled(); 
			break;
		case TEXTURES:
			useTextures = !useTextures;
			break;
		case WIREFRAME:
			{	
				wireframe = !wireframe;
				if (wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		case HELP:
			pauseSystem = true;
			break;
		case SWITCH_FONTS:
		{
			extern bool useGlutFonts;
			useGlutFonts = !useGlutFonts;
			break;
		}
	}
}
