/******************************************************************************
 *
 * Computer Graphics Programming 2020 Project Template v1.0 (11/04/2021)
 *
 * Based on: Animation Controller v1.0 (11/04/2021)
 *
 * This template provides a basic FPS-limited render loop for an animated scene,
 * plus keyboard handling for smooth game-like control of an object such as a
 * character or vehicle.
 *
 * A simple static lighting setup is provided via initLights(), which is not
 * included in the animationalcontrol.c template. There are no other changes.
 *
 ******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

 /******************************************************************************
  * Animation & Timing Setup
  ******************************************************************************/

  // Target frame rate (number of Frames Per Second).
#define TARGET_FPS 30				
#define PI 3.1416

#define NUM_RAIN_DROPS 1000

// Ideal time each frame should be displayed for (in milliseconds).
const unsigned int FRAME_TIME = 1000 / TARGET_FPS;

// Frame time in fractional seconds.
// Note: This is calculated to accurately reflect the truncated integer value of
// FRAME_TIME, which is used for timing, rather than the more accurate fractional
// value we'd get if we simply calculated "FRAME_TIME_SEC = 1.0f / TARGET_FPS".
const float FRAME_TIME_SEC = (1000 / TARGET_FPS) / 1000.0f;

// Time we started preparing the current frame (in milliseconds since GLUT was initialized).
unsigned int frameStartTime = 0;

/******************************************************************************
 * Some Simple Definitions of Motion
 ******************************************************************************/

#define MOTION_NONE 0				// No motion.
#define MOTION_CLOCKWISE -1			// Clockwise rotation.
#define MOTION_ANTICLOCKWISE 1		// Anticlockwise rotation.
#define MOTION_BACKWARD -1			// Backward motion.
#define MOTION_FORWARD 1			// Forward motion.
#define MOTION_LEFT -1				// Leftward motion.
#define MOTION_RIGHT 1				// Rightward motion.
#define MOTION_DOWN -1				// Downward motion.
#define MOTION_UP 1					// Upward motion.

 // Represents the motion of an object on four axes (Yaw, Surge, Sway, and Heave).
 // 
 // You can use any numeric values, as specified in the comments for each axis. However,
 // the MOTION_ definitions offer an easy way to define a "unit" movement without using
 // magic numbers (e.g. instead of setting Surge = 1, you can set Surge = MOTION_FORWARD).
 //
typedef struct {
	int Yaw;		// Turn about the Z axis	[<0 = Clockwise, 0 = Stop, >0 = Anticlockwise]
	int Surge;		// Move forward or back		[<0 = Backward,	0 = Stop, >0 = Forward]
	int Sway;		// Move sideways (strafe)	[<0 = Left, 0 = Stop, >0 = Right]
	int Heave;		// Move vertically			[<0 = Down, 0 = Stop, >0 = Up]
} motionstate4_t;

/******************************************************************************
 * Keyboard Input Handling Setup
 ******************************************************************************/

 // Represents the state of a single keyboard key.Represents the state of a single keyboard key.
typedef enum {
	KEYSTATE_UP = 0,	// Key is not pressed.
	KEYSTATE_DOWN		// Key is pressed down.
} keystate_t;

typedef enum {
	VIEW_BEHIND,
	VIEW_LEFT,
	VIEW_FRONT,
	VIEW_RIGHT,
	VIEW_TOP,
	NUM_VIEWS
} CameraView;

CameraView currentView = VIEW_BEHIND;

// Represents the states of a set of keys used to control an object's motion.
typedef struct {
	keystate_t MoveForward;
	keystate_t MoveBackward;
	keystate_t MoveLeft;
	keystate_t MoveRight;
	keystate_t MoveUp;
	keystate_t MoveDown;
	keystate_t TurnLeft;
	keystate_t TurnRight;
} motionkeys_t;

//rain effect
typedef struct {
	float x, y, z;  // Position of the raindrop
	float speed;    // Falling speed
} Raindrop;

// Current state of all keys used to control our "player-controlled" object's motion.
motionkeys_t motionKeyStates = {
	KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP,
	KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP };

// How our "player-controlled" object should currently be moving, solely based on keyboard input.
//
// Note: this may not represent the actual motion of our object, which could be subject to
// other controls (e.g. mouse input) or other simulated forces (e.g. gravity).
motionstate4_t keyboardMotion = { MOTION_NONE, MOTION_NONE, MOTION_NONE, MOTION_NONE };

// Define all character keys used for input (add any new key definitions here).
// Note: USE ONLY LOWERCASE CHARACTERS HERE. The keyboard handler provided converts all
// characters typed by the user to lowercase, so the SHIFT key is ignored.

#define KEY_MOVE_FORWARD	'w'
#define KEY_MOVE_BACKWARD	's'
#define KEY_MOVE_LEFT		'a'
#define KEY_MOVE_RIGHT		'd'
#define KEY_RENDER_FILL		'l'
#define KEY_EXIT			27 // Escape key.

#define KEY_CHANGE_VIEW 'v'

// Define all GLUT special keys used for input (add any new key definitions here).

#define SP_KEY_MOVE_UP		GLUT_KEY_UP
#define SP_KEY_MOVE_DOWN	GLUT_KEY_DOWN
#define SP_KEY_TURN_LEFT	GLUT_KEY_LEFT
#define SP_KEY_TURN_RIGHT	GLUT_KEY_RIGHT

/******************************************************************************
 * GLUT Callback Prototypes
 ******************************************************************************/

void display(void);
void reshape(int width, int h);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
void keyReleased(unsigned char key, int x, int y);
void specialKeyReleased(int key, int x, int y);
void idle(void);

void drawOriginMarker(void);
void drawXZGrid(float size, int divisions);

void drawSkybox(float size);

void drawHouse(float width, float height, float depth);
void applyMetallicMaterial();
void drawAircraft();

void drawTree(float trunkHeight, float trunkRadius, float foliageHeight, float foliageRadius);

void setupFog();

void drawAirplaneParkingHall(float radius, float height);

void drawAirstrip(float width, float length, float thickness);

void drawTank(float topWidth, float bottomWidth, float height, float depth);
void drawFrontBackPart(float topWidth, float bottomWidth, float height, float depth, float cylinderRadius, float cylinderLength);

void createTreeDisplayList();
void drawMultipleTrees();

void createHouseDisplayList();
void drawMultipleHouses();

void createTankDisplayList();
void drawMultipleTanks();
void setupNightMode();

/******************************************************************************
 * Animation-Specific Function Prototypes (add your own here)
 ******************************************************************************/

void main(int argc, char** argv);
void init(void);
void think(void);
void initLights(void);

void loadTextures(void);

int isGrounded();

void initializeRain();
void updateRain();
void drawRain();

/******************************************************************************
 * Animation-Specific Setup (Add your own definitions, constants, and globals here)
 ******************************************************************************/

int texID[4]; // Texture ID's for the four textures
char* textureFileNames[4] = {   // file names for the files from which texture images are loaded
			"assets//WoodRoofl1.ppm",
			"assets//grass_debug.ppm",
			"assets//HouseWall1.ppm",
			"assets//TankBody1.ppm"
			
};

// Render objects as filled polygons (1) or wireframes (0). Default filled.
int renderFillEnabled = 1;

GLint windowWidth = 800;
GLint windowHeight = 600;

GLUquadricObj* sphereQuadric;
GLUquadricObj* quadricPtr;

float wingAngle = 0.0f;  // Angle of wing rotation
int wingDirection = 1;   // Direction of wing movement: 1 for up, -1 for down

GLfloat cameraLookAt[] = { 0.0f, 1.0f, 10.0f };

int cameraLookAtState = 0;

#define BODY_RADIUS 1.0
#define BEAK_LENGTH 1.0

float objectLocation[3] = { 0.0f, 0.8f, 0.0f };
float objectRotation[3] = { 0.0f, 0.0f, 0.0f };

const float moveSpeed = 20.0f;
const float rotationSpeed = 35.0f;
const GLfloat PALE_GREEN[3] = { 0.596f, 0.984f, 0.596f };
const GLfloat PALE_ORANGE[3] = { 1.0f, 0.714f, 0.221f };
const GLfloat BLACK[3] = { 1.0f, 1.0f, 1.0f };
const GLfloat WHITE[3] = { 0.0f, 0.0f, 0.0f };

float cameraDistance = 5.0f; // Distance of camera from bird
float cameraHeight = 2.0f; // Height of camera above bird
float cameraAngle = 0.0f; // Angle of camera around bird (in radians)

// Global variable for propeller rotation
float propellerRotationAngle = 0.0f;
int rotorsActive = 0;  // Rotors start off (0 = off, 1 = on)
float cameraAngleOffset = PI; // Default to behind the bird

float rotorSpeed = 0.0f;  // Speed of the rotors (starts at 0)
int heaveEnabled = 0;     // Flag to check if Heave (upward movement) is allowed
float timeHeldW = 0.0f;   // Time the "w" key has been held to speed up rotors

// Add this global variable to track if the helicopter has taken off
int hasTakenOff = 0;  // 0 = not taken off yet, 1 = has taken off at least once
int rotorSpeedLocked = 0;

float timeGrounded = 0.0f;
int engineShutdownInitiated = 0;

GLuint treeDisplayList, HouseDisplayList, TankDisplayList;


Raindrop rain[NUM_RAIN_DROPS];

float tankSpeed = 2.0f;        // Tank forward speed
float tankRotationSpeed = 30.0f; // Tank rotation speed in degrees per second

float tankPosition[3] = { 0.0f, 0.0f, 0.0f }; // X, Y, Z position of the tank
float tankRotation = 0.0f; // Rotation angle of the tank around the Y-axis

int rainActive = 0;  // 0 = Rain off, 1 = Rain on

const float GROUND_WIDTH = 100.0f;  // Width of the ground
const float GROUND_LENGTH = 100.0f; // Length of the ground

/******************************************************************************
 * Entry Point (don't put anything except the main function here)
 ******************************************************************************/





void main(int argc, char** argv)
{
	// Initialize the OpenGL window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Animation");

	// Set up the scene.
	init();

	// Disable key repeat (keyPressed or specialKeyPressed will only be called once when a key is first pressed).
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Register GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutSpecialUpFunc(specialKeyReleased);
	glutIdleFunc(idle);

	// Record when we started rendering the very first frame (which should happen after we call glutMainLoop).
	frameStartTime = (unsigned int)glutGet(GLUT_ELAPSED_TIME);

	// Enter the main drawing loop (this will never return).
	glutMainLoop();
}

/******************************************************************************
 * GLUT Callbacks (don't add any other functions here)
 ******************************************************************************/

 /*
	 Called when GLUT wants us to (re)draw the current animation frame.

	 Note: This function must not do anything to update the state of our simulated
	 world. Animation (moving or rotating things, responding to keyboard input,
	 etc.) should only be performed within the think() function provided below.
 */
void display(void)
{
	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Setup fog
	setupFog();


	// load the identity matrix into the model view matrix
	glLoadIdentity();

	// Calculate camera position based on bird's position and rotation
	float radians = objectRotation[1] * PI / 180.0f;  // Convert the bird's rotation (yaw) to radians

	// Camera distance behind the bird
	float cameraDistance = 5.0f;   // Distance behind the bird (adjust as needed)
	float cameraHeight = 2.0f;     // Height above the bird

	// Camera position relative to the bird
	float cameraPosX = objectLocation[0] - sinf(radians) * cameraDistance;
	float cameraPosY = objectLocation[1] + cameraHeight;  // Keep camera slightly above the bird
	float cameraPosZ = objectLocation[2] - cosf(radians) * cameraDistance;

	// Camera look-at point (look at the bird)
	float lookAtX = objectLocation[0];
	float lookAtY = objectLocation[1];
	float lookAtZ = objectLocation[2];

	// Set the camera position
	gluLookAt(cameraLookAt[0], cameraLookAt[1], cameraLookAt[2],
		objectLocation[0], objectLocation[1], objectLocation[2],
		0, 1, 0);



	drawOriginMarker();

	// Draw the XZ grid
	drawXZGrid(100.0f, 200);
	
	

	// Draw the tree (ensure tree transformations are isolated)
	glPushMatrix();
	glTranslatef(-4.0f, 0.0f, -2.0f);  // Use fixed coordinates for the tree position
	glScalef(0.75f, 0.75f, 0.75f);  // Scale the tree down to 50% of its original size
	drawTree(10.0f, 0.3f, 2.0f, 1.0f);  // Example tree: 3 unit high trunk, 0.2 radius trunk, 2 unit high foliage
	glPopMatrix();


	// Draw the house next to the aircraft (e.g., fixed position on the ground)
	glPushMatrix();
		glTranslatef(7.0f, 0.0f, -5.0f);  // Fixed position of the house (e.g., 10 units right and 10 units back)
		glScalef(2.0f, 2.0f, 2.0f);
		drawHouse(2.0f, 2.0f, 4.0f);  // Draw the house with a base size of 2 units
	glPopMatrix();

	// draw parking hall
	glPushMatrix();
	// Translate the entire parking hall behind the aircraft
	glTranslatef(0.0f, 0.0f, 8.0f);  // Adjust as needed
	glScalef(1.0f, 1.0f, 3.0f);
	// Call the function to draw the airplane parking hall
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);  // Rotate 90 degrees around Y-axis
	drawAirplaneParkingHall(3.0f, 4.0f);  // Example dimensions: radius = 3.0, height = 4.0
	glPopMatrix();

	glPushMatrix();
	// Translate the airstrip below the aircraft
	glTranslatef(0.0f, 0.0f, -10.0f);  // Adjust as needed to place under the aircraft
	glScalef(1.0f, 1.0f, 1.5f);
	// Call the function to draw the airstrip
	drawAirstrip(6.0f, 40.0f, 0.05f);  // Example dimensions: width = 6.0, length = 20.0, thickness = 0.1
	glPopMatrix();


	// Draw the tank at its current position and orientation
	glPushMatrix();
	glTranslatef(tankPosition[0], tankPosition[1], tankPosition[2]); // Move tank to current position
	glTranslatef(-10.0f, 0.0f, -20.0f);
	glRotatef(tankRotation, 0.0f, 1.0f, 0.0f);  // Rotate the tank around the Y-axis
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
	drawTank(2.0f, 1.0f, 1.0f, 1.0f);  // Draw the tank (or call your custom tank drawing function)
	glPopMatrix();

	



	// Move the aircraft to its current position (objectLocation)
	glPushMatrix();
		glTranslatef(objectLocation[0], objectLocation[1], objectLocation[2]);
		// Add rotation for the aircraft
		glRotatef(objectRotation[1], 0.0f, 1.0f, 0.0f);
		// Draw the aircraft
		drawAircraft();  // Call the new function to draw the aircraft
	glPopMatrix();
	

	// Draw the multiple trees
	drawMultipleTrees();

	// Draw the multiple Houses
	drawMultipleHouses();

	// Draw the multiple Tanks
	drawMultipleTanks();

	// Update and draw rain
	updateRain();
	drawRain();

	// swap the drawing buffers
	glutSwapBuffers();

	/*
		TEMPLATE: REPLACE THIS COMMENT WITH YOUR DRAWING CODE

		Separate reusable pieces of drawing code into functions, which you can add
		to the "Animation-Specific Functions" section below.

		Remember to add prototypes for any new functions to the "Animation-Specific
		Function Prototypes" section near the top of this template.
	*/
}

/*
	Called when the OpenGL window has been resized.
*/
void reshape(int width, int h)
{
	// update the new width
	windowWidth = width;
	// update the new height
	windowHeight = h;

	// update the viewport to still be all of the window
	glViewport(0, 0, windowWidth, windowHeight);

	// change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);

	// load the identity matrix into the projection matrix
	glLoadIdentity();

	// gluPerspective(fovy, aspect, near, far)
	gluPerspective(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);

	//glLoadIdentity();

}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is pressed.
*/
void keyPressed(unsigned char key, int x, int y)
{
	switch (tolower(key)) {

		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			Whenever one of our movement keys is pressed, we do two things:
			(1) Update motionKeyStates to record that the key is held down. We use
				this later in the keyReleased callback.
			(2) Update the relevant axis in keyboardMotion to set the new direction
				we should be moving in. The most recent key always "wins" (e.g. if
				you're holding down KEY_MOVE_LEFT then also pressed KEY_MOVE_RIGHT,
				our object will immediately start moving right).
		*/
	case KEY_MOVE_FORWARD:
		motionKeyStates.MoveForward = KEYSTATE_DOWN;
		keyboardMotion.Surge = MOTION_FORWARD;
		break;
	case KEY_MOVE_BACKWARD:
		motionKeyStates.MoveBackward = KEYSTATE_DOWN;
		keyboardMotion.Surge = MOTION_BACKWARD;
		break;
	case KEY_MOVE_LEFT:
		motionKeyStates.MoveLeft = KEYSTATE_DOWN;
		keyboardMotion.Sway = MOTION_LEFT;
		break;
	case KEY_MOVE_RIGHT:
		motionKeyStates.MoveRight = KEYSTATE_DOWN;
		keyboardMotion.Sway = MOTION_RIGHT;
		break;

		/*
			Other Keyboard Functions (add any new character key controls here)

			Rather than using literals (e.g. "t" for spotlight), create a new KEY_
			definition in the "Keyboard Input Handling Setup" section of this file.
			For example, refer to the existing keys used here (KEY_MOVE_FORWARD,
			KEY_MOVE_LEFT, KEY_EXIT, etc).
		*/
	case KEY_RENDER_FILL:
		renderFillEnabled = !renderFillEnabled;
		break;
		
	case KEY_CHANGE_VIEW:
		currentView = (currentView + 1) % NUM_VIEWS;
		switch (currentView) {
		case VIEW_BEHIND:
			cameraAngleOffset = PI;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_LEFT:
			cameraAngleOffset = PI / 2;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_FRONT:
			cameraAngleOffset = 0;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_RIGHT:
			cameraAngleOffset = -PI / 2;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_TOP:
			cameraAngleOffset = PI;
			cameraHeight = 10.0f;
			cameraDistance = 0.1f;
			break;
		}

		glutPostRedisplay();  // Request a redraw to update the view

		break;

	

	case 'e':  // Toggle rotor on/off when 'e' is pressed
		rotorsActive = !rotorsActive;  // Toggle the rotor state
		if (!rotorsActive) {
			rotorSpeed = 0.0f;  // Stop the rotor when toggled off
			rotorSpeedLocked = 0;  // Unlock the rotor speed
		}
		else {
			rotorSpeed = 2.0f;  // Start with slow rotor speed when toggled on
			rotorSpeedLocked = 0;  // Ensure it's unlocked when starting
		}
		break;

	case 'r':  // Toggle rain on/off
		rainActive = !rainActive;  // Toggle the rain state
		setupFog();  // Update the fog settings based on the rain state
		setupNightMode();  // Update the scene lighting and colors for night mode
		break;

	case KEY_EXIT:
		exit(0);
		break;
	}
}

/*
	Called each time a "special" key (e.g. an arrow key) is pressed.
*/
void specialKeyPressed(int key, int x, int y)
{
	switch (key) {

		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			This works as per the motion keys in keyPressed.
		*/
	case SP_KEY_MOVE_UP:
		motionKeyStates.MoveUp = KEYSTATE_DOWN;
		keyboardMotion.Heave = MOTION_UP;
		break;
	case SP_KEY_MOVE_DOWN:
		motionKeyStates.MoveDown = KEYSTATE_DOWN;
		keyboardMotion.Heave = MOTION_DOWN;
		break;
	case SP_KEY_TURN_LEFT:
		motionKeyStates.TurnLeft = KEYSTATE_DOWN;
		keyboardMotion.Yaw = MOTION_ANTICLOCKWISE;
		break;
	case SP_KEY_TURN_RIGHT:
		motionKeyStates.TurnRight = KEYSTATE_DOWN;
		keyboardMotion.Yaw = MOTION_CLOCKWISE;
		break;
	case KEY_CHANGE_VIEW:
		currentView = (currentView + 1) % NUM_VIEWS;
		glutPostRedisplay();
		break;
		/*
			Other Keyboard Functions (add any new special key controls here)

			Rather than directly using the GLUT constants (e.g. GLUT_KEY_F1), create
			a new SP_KEY_ definition in the "Keyboard Input Handling Setup" section of
			this file. For example, refer to the existing keys used here (SP_KEY_MOVE_UP,
			SP_KEY_TURN_LEFT, etc).
		*/
	}
}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is released.
*/
void keyReleased(unsigned char key, int x, int y)
{
	switch (tolower(key)) {

		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			Whenever one of our movement keys is released, we do two things:
			(1) Update motionKeyStates to record that the key is no longer held down;
				we need to know when we get to step (2) below.
			(2) Update the relevant axis in keyboardMotion to set the new direction
				we should be moving in. This gets a little complicated to ensure
				the controls work smoothly. When the user releases a key that moves
				in one direction (e.g. KEY_MOVE_RIGHT), we check if its "opposite"
				key (e.g. KEY_MOVE_LEFT) is pressed down. If it is, we begin moving
				in that direction instead. Otherwise, we just stop moving.
		*/
	case KEY_MOVE_FORWARD:
		motionKeyStates.MoveForward = KEYSTATE_UP;
		keyboardMotion.Surge = (motionKeyStates.MoveBackward == KEYSTATE_DOWN) ? MOTION_BACKWARD : MOTION_NONE;
		break;
	case KEY_MOVE_BACKWARD:
		motionKeyStates.MoveBackward = KEYSTATE_UP;
		keyboardMotion.Surge = (motionKeyStates.MoveForward == KEYSTATE_DOWN) ? MOTION_FORWARD : MOTION_NONE;
		break;
	case KEY_MOVE_LEFT:
		motionKeyStates.MoveLeft = KEYSTATE_UP;
		keyboardMotion.Sway = (motionKeyStates.MoveRight == KEYSTATE_DOWN) ? MOTION_RIGHT : MOTION_NONE;
		break;
	case KEY_MOVE_RIGHT:
		motionKeyStates.MoveRight = KEYSTATE_UP;
		keyboardMotion.Sway = (motionKeyStates.MoveLeft == KEYSTATE_DOWN) ? MOTION_LEFT : MOTION_NONE;
		break;

		/*
			Other Keyboard Functions (add any new character key controls here)

			Note: If you only care when your key is first pressed down, you don't have to
			add anything here. You only need to put something in keyReleased if you care
			what happens when the user lets go, like we do with our movement keys above.
			For example: if you wanted a spotlight to come on while you held down "t", you
			would need to set a flag to turn the spotlight on in keyPressed, and update the
			flag to turn it off in keyReleased.
		*/
	}
}

/*
	Called each time a "special" key (e.g. an arrow key) is released.
*/
void specialKeyReleased(int key, int x, int y)
{
	switch (key) {
		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			This works as per the motion keys in keyReleased.
		*/
	case SP_KEY_MOVE_UP:
		motionKeyStates.MoveUp = KEYSTATE_UP;
		keyboardMotion.Heave = (motionKeyStates.MoveDown == KEYSTATE_DOWN) ? MOTION_DOWN : MOTION_NONE;
		break;
	case SP_KEY_MOVE_DOWN:
		motionKeyStates.MoveDown = KEYSTATE_UP;
		keyboardMotion.Heave = (motionKeyStates.MoveUp == KEYSTATE_DOWN) ? MOTION_UP : MOTION_NONE;
		break;
	case SP_KEY_TURN_LEFT:
		motionKeyStates.TurnLeft = KEYSTATE_UP;
		keyboardMotion.Yaw = (motionKeyStates.TurnRight == KEYSTATE_DOWN) ? MOTION_CLOCKWISE : MOTION_NONE;
		break;
	case SP_KEY_TURN_RIGHT:
		motionKeyStates.TurnRight = KEYSTATE_UP;
		keyboardMotion.Yaw = (motionKeyStates.TurnLeft == KEYSTATE_DOWN) ? MOTION_ANTICLOCKWISE : MOTION_NONE;
		break;
	case KEY_CHANGE_VIEW:
		currentView = (currentView + 1) % NUM_VIEWS;
		switch (currentView) {
		case VIEW_BEHIND:
			cameraAngleOffset = PI;
			cameraHeight = 5.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_LEFT:
			cameraAngleOffset = PI / 2;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_FRONT:
			cameraAngleOffset = 0;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_RIGHT:
			cameraAngleOffset = -PI / 2;
			cameraHeight = 2.0f;
			cameraDistance = 5.0f;
			break;
		case VIEW_TOP:
			cameraAngleOffset = PI;
			cameraHeight = 10.0f;
			cameraDistance = 0.1f;
			break;
		}
		glutPostRedisplay();
		break;

		/*
			Other Keyboard Functions (add any new special key controls here)

			As per keyReleased, you only need to handle the key here if you want something
			to happen when the user lets go. If you just want something to happen when the
			key is first pressed, add you code to specialKeyPressed instead.
		*/
	}
}

/*
	Called by GLUT when it's not rendering a frame.

	Note: We use this to handle animation and timing. You shouldn't need to modify
	this callback at all. Instead, place your animation logic (e.g. moving or rotating
	things) within the think() method provided with this template.
*/
void idle(void)
{
	// Wait until it's time to render the next frame.

	unsigned int frameTimeElapsed = (unsigned int)glutGet(GLUT_ELAPSED_TIME) - frameStartTime;
	if (frameTimeElapsed < FRAME_TIME)
	{
		// This frame took less time to render than the ideal FRAME_TIME: we'll suspend this thread for the remaining time,
		// so we're not taking up the CPU until we need to render another frame.
		unsigned int timeLeft = FRAME_TIME - frameTimeElapsed;
		Sleep(timeLeft);
	}

	// Begin processing the next frame.

	frameStartTime = glutGet(GLUT_ELAPSED_TIME); // Record when we started work on the new frame.

	think(); // Update our simulated world before the next call to display().

	// Only rotate the rotors if they are active (rotorsActive == 1)
	if (rotorsActive == 1) {
		propellerRotationAngle += rotorSpeed;  // Rotate the propellers based on rotor speed
		if (propellerRotationAngle >= 360.0f) {
			propellerRotationAngle -= 360.0f;
		}
	}

	glutPostRedisplay(); // Tell OpenGL there's a new frame ready to be drawn.


	

}

/******************************************************************************
 * Animation-Specific Functions (Add your own functions at the end of this section)
 ******************************************************************************/

 /*
	 Initialise OpenGL and set up our scene before we begin the render loop.
 */
void init(void)
{
	// Set the background color to sky blue
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);  // Sky blue color (RGBA)

	// enable depth testing
	glEnable(GL_DEPTH_TEST);


	// Enable texturing globally
    glEnable(GL_TEXTURE_2D);
    
    // Load textures
    loadTextures();
    
    // Configure texture parameters globally
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // Enable color material (allows textures to show their true colors)
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	// set background color to be black
	//glClearColor(0, 0, 0, 1.0);

	// NOTE: Don't need to set up projection here anymore
	//       since the reshape callback function will be called when
	//		 the window is first created

	//need some lighting or our cup will look flat!
	// Configure global ambient lighting.
	// Simple lighting setup
	GLfloat globalAmbient[] = { 0.8f, 0.8f, 0.8f, 1 };\
	GLfloat diffuseLight[] = { 1, 1, 1, 1 };
	GLfloat specularLight[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Make GL normalize the normal vectors we supply.
	glEnable(GL_NORMALIZE);

	// Enable use of simple GL colours as materials.
	glEnable(GL_COLOR_MATERIAL);



	initLights();

	initializeRain();

	// Initialize position of helicopter (already done with objectLocation)
	objectLocation[1] = 0.8f;  // Helicopter starts 1.0 unit above the ground

	sphereQuadric = gluNewQuadric();
	quadricPtr = gluNewQuadric();

	// Anything that relies on lighting or specifies normals must be initialised after initLights.
}

/*
	Advance our animation by FRAME_TIME milliseconds.

	Note: Our template's GLUT idle() callback calls this once before each new
	frame is drawn, EXCEPT the very first frame drawn after our application
	starts. Any setup required before the first frame is drawn should be placed
	in init().
*/
void think(void)
{
	// Update wing angle for animation
	wingAngle += wingDirection * 0.5f;  // Adjust speed by changing 0.5f

	// Reverse direction when reaching limits (e.g., 30 degrees up, 0 degrees down)
	if (wingAngle > 15.0f || wingAngle < -15.0f) {
		wingDirection = -wingDirection;
	}

	
	

	/*
		TEMPLATE: REPLACE THIS COMMENT WITH YOUR ANIMATION/SIMULATION CODE

		In this function, we update all the variables that control the animated
		parts of our simulated world. For example: if you have a moving box, this is
		where you update its coordinates to make it move. If you have something that
		spins around, here's where you update its angle.

		NOTHING CAN BE DRAWN IN HERE: you can only update the variables that control
		how everything will be drawn later in display().

		How much do we move or rotate things? Because we use a fixed frame rate, we
		assume there's always FRAME_TIME milliseconds between drawing each frame. So,
		every time think() is called, we need to work out how far things should have
		moved, rotated, or otherwise changed in that period of time.

		Movement example:
		* Let's assume a distance of 1.0 GL units is 1 metre.
		* Let's assume we want something to move 2 metres per second on the x axis
		* Each frame, we'd need to update its position like this:
			x += 2 * (FRAME_TIME / 1000.0f)
		* Note that we have to convert FRAME_TIME to seconds. We can skip this by
		  using a constant defined earlier in this template:
			x += 2 * FRAME_TIME_SEC;

		Rotation example:
		* Let's assume we want something to do one complete 360-degree rotation every
		  second (i.e. 60 Revolutions Per Minute, or RPM).
		* Each frame, we'd need to update our object's angle like this (we'll use the
		  FRAME_TIME_SEC constant as per the example above):
			a += 360 * FRAME_TIME_SEC;

		This works for any type of "per second" change: just multiply the amount you'd
		want to move in a full second by FRAME_TIME_SEC, and add or subtract that
		from whatever variable you're updating.

		You can use this same approach to animate other things like color, opacity,
		brightness of lights, etc.
	*/

	/*
		Keyboard motion handler: complete this section to make your "player-controlled"
		object respond to keyboard input.
	*/
	if (keyboardMotion.Yaw != MOTION_NONE) {
		objectRotation[1] += keyboardMotion.Yaw * rotationSpeed * FRAME_TIME_SEC;
		if (objectRotation[1] <= -360 || objectRotation[1] >= 360)
			objectRotation[1] = 0.0f;
		printf("X: %f, Z: %f, R: %f\n", objectLocation[0], objectLocation[2], objectLocation[1]);

		/* TEMPLATE: Turn your object right (clockwise) if .Yaw < 0, or left (anticlockwise) if .Yaw > 0 */
	}
	if (keyboardMotion.Surge != MOTION_NONE) {
		float radians = objectRotation[1] * PI / 180.0f;
		float dx = -sinf(radians) * keyboardMotion.Surge * moveSpeed * FRAME_TIME_SEC;
		dx * dx == -0.0f ? 0.0f : dx;
		float dz = -cosf(radians) * keyboardMotion.Surge * moveSpeed * FRAME_TIME_SEC;
		dz* dz == -0.0f ? 0.0f : dz;

		objectLocation[0] += dx;
		objectLocation[2] += dz;
		printf("X: %f, Z: %f, R: %f\n", dx, dz, objectLocation[1]);

		/* TEMPLATE: Move your object backward if .Surge < 0, or forward if .Surge > 0 */
	}
	if (keyboardMotion.Sway != MOTION_NONE) {
		float radians = objectRotation[1] * PI / 180.0f;  // Convert rotation to radians
		float dx = cosf(radians) * keyboardMotion.Sway * moveSpeed * FRAME_TIME_SEC;  // Move along the perpendicular direction
		dx* dx == -0.0f ? 0.0f : dx;
		float dz = -sinf(radians) * keyboardMotion.Sway * moveSpeed * FRAME_TIME_SEC;  // Perpendicular movement in z-axis
		dz* dz == -0.0f ? 0.0f : dz;

		// Apply the sway movement to the object's location
		objectLocation[0] += dx;
		objectLocation[2] += dz;

		printf("X: %f, Z: %f, R: %f\n", dx, dz, objectRotation[1]);

		/* TEMPLATE: Move (strafe) your object left if .Sway < 0, or right if .Sway > 0 */
	}

	if (rotorsActive) {
		propellerRotationAngle += rotorSpeed;
		if (propellerRotationAngle >= 360.0f) {
			propellerRotationAngle -= 360.0f;
		}

		if (isGrounded()) {
			if (!hasTakenOff) {
				// Initial takeoff logic
				if (motionKeyStates.MoveForward == KEYSTATE_DOWN) {
					timeHeldW += FRAME_TIME_SEC;
					if (timeHeldW >= 2.0f) {
						rotorSpeed = 15.0f;
						heaveEnabled = 1;
					}
				}
				else {
					timeHeldW = 0.0f;
				}
			}
			else {
				// After landing logic
				if (motionKeyStates.MoveForward == KEYSTATE_DOWN) {
					timeGrounded += FRAME_TIME_SEC;
					if (timeGrounded >= 2.0f && !engineShutdownInitiated) {
						engineShutdownInitiated = 1;
					}
				}
				else {
					timeGrounded = 0.0f;
					engineShutdownInitiated = 0;
				}

				if (engineShutdownInitiated) {
					rotorSpeed = 0.0f;
					rotorsActive = 0;
					heaveEnabled = 0;
					hasTakenOff = 0;
				}
				else {
					rotorSpeed = 2.0f;  // Idle speed when on ground
				}
			}
		}
		else {
			// In-air logic
			rotorSpeed = 15.0f;
			heaveEnabled = 1;
			hasTakenOff = 1;
			timeGrounded = 0.0f;
			engineShutdownInitiated = 0;
		}
	}
	else {
		// Rotors are inactive
		rotorSpeed = 0.0f;
		heaveEnabled = 0;
	}

	// Prevent the helicopter from going below ground level
	if (objectLocation[1] < 0.8f) {
		objectLocation[1] = 0.8f;
	}

	// In the think function, update the vertical movement handling:
	if (keyboardMotion.Heave != MOTION_NONE && heaveEnabled) {
		float verticalSpeed = keyboardMotion.Heave * moveSpeed * FRAME_TIME_SEC;
		float newAltitude = objectLocation[1] + verticalSpeed;
		if (newAltitude >= 0.8f) {
			objectLocation[1] = newAltitude;
		}
		else {
			objectLocation[1] = 0.8f;
		}
	}
	

	// Update camera position to follow the bird
	float birdAngle = objectRotation[1] * PI / 180.0f; // Convert bird rotation to radians
	float cameraAngle = birdAngle + cameraAngleOffset;

	// Calculate camera position
	cameraLookAt[0] = objectLocation[0] - cameraDistance * sinf(cameraAngle);
	cameraLookAt[1] = objectLocation[1] + cameraHeight;
	cameraLookAt[2] = objectLocation[2] - cameraDistance * cosf(cameraAngle);


	// Update spotlight to follow the helicopter's position
	GLfloat spotlightPosition[] = { objectLocation[0], objectLocation[1] + 2.0f, objectLocation[2], 1.0f };  // Spotlight follows the helicopter

	// Calculate the spotlight direction based on the helicopter's rotation (yaw)
	float radians = objectRotation[1] * PI / 180.0f;  // Convert the yaw angle (objectRotation[1]) to radians
	GLfloat spotlightDirection[] = { -sinf(radians), -0.5f, -cosf(radians) };  // Spotlight points forward and downward


	float moveSpeed = tankSpeed;            // Speed of the tank's forward movement
	float rotationSpeed = tankRotationSpeed; // Speed of the tank's rotation

	// Update tank's position based on its current rotation
	float tankRadians = tankRotation * PI / 180.0f; // For tank rotation
	tankPosition[0] -= sinf(tankRadians) * moveSpeed * FRAME_TIME_SEC; // Update X position
	tankPosition[2] -= cosf(tankRadians) * moveSpeed * FRAME_TIME_SEC; // Update Z position

	// Rotate the tank gradually (e.g., make the tank move in a circular path)
	tankRotation += rotationSpeed * FRAME_TIME_SEC;
	if (tankRotation >= 360.0f) tankRotation -= 360.0f; // Keep rotation within 0-360 degrees


	// Update spotlight properties
	glLightfv(GL_LIGHT2, GL_POSITION, spotlightPosition);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotlightDirection);

}

/*
	Initialise OpenGL lighting before we begin the render loop.

	Note (advanced): If you're using dynamic lighting (e.g. lights that move around, turn on or
	off, or change colour) you may want to replace this with a drawLights function that gets called
	at the beginning of display() instead of init().
*/
void initLights(void)
{
	
	// Global ambient lighting
	GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Directional Light (Sunlight)
	GLfloat directionalLightDirection[] = { -1.0f, -1.0f, -1.0f, 0.0f };  // Directional light from top-right
	GLfloat directionalLightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat directionalLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat directionalLightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_LIGHT1);  // Light 1 for the directional light
	glLightfv(GL_LIGHT1, GL_POSITION, directionalLightDirection);
	glLightfv(GL_LIGHT1, GL_AMBIENT, directionalLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, directionalLightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, directionalLightSpecular);

	// Spotlight (attached to helicopter)
	GLfloat spotlightPosition[] = { objectLocation[0], objectLocation[1], objectLocation[2], 1.0f };  // Dynamic position
	GLfloat spotlightDirection[] = { 0.0f, -1.0f, 0.0f };  // Pointing downward
	GLfloat spotlightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spotlightDiffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };  // Soft yellow spotlight
	GLfloat spotlightSpecular[] = { 1.0f, 1.0f, 0.8f, 1.0f };

	glEnable(GL_LIGHT2);  // Light 2 for the spotlight
	glLightfv(GL_LIGHT2, GL_POSITION, spotlightPosition);
	glLightfv(GL_LIGHT2, GL_AMBIENT, spotlightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, spotlightDiffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spotlightSpecular);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);  // Spotlight cutoff angle (30 degrees)
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotlightDirection);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0f);  // Focus the beam

	// Enable general lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);  // Ensure normals are normalized
	
}



void drawOriginMarker(void){

	glColor3f(0.0, 1.0, 1.0);
	glutSolidSphere(0.1, 10, 10);

	glBegin(GL_LINES);
	//draw red x axes line from -2.0 to 2.0
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3d(-2.0, 0.0, 0.0);
	glVertex3d(2.0, 0.0, 0.0);

	//draw blue y axes line from -2.0 to 2.0
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3d(0.0, 2.0, 0.0);
	glVertex3d(0.0, -2.0, 0.0);

	//draw green z axes line from -2.0 to 2.0
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3d(0.0, 0.0, 2.0);
	glVertex3d(0.0, 0.0, -2.0);

	glEnd();
}

void drawXZGrid(float size, int divisions) {
	
	// Bind the grass texture for the grid
	glBindTexture(GL_TEXTURE_2D, texID[1]);  // Assuming texID[1] is the grass texture

	glEnable(GL_TEXTURE_2D);  // Enable 2D texture mapping

	// Set the color to a darker green (multiply the texture color)
	glColor3f(0.6f, 0.6f, 0.6f);  // Darken the texture by setting a darker color

	// Set polygon mode based on renderFillEnabled (1 = filled, 0 = wireframe)
	if (renderFillEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Filled mode
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe mode
	}

	float halfSize = size / 2.0f;  // Half the size for centering the grid
	float step = size / (float)divisions;  // Distance between each line/grid square

	// Scale the texture repetition. Higher values make the texture repeat more.
	float textureScale = 100.0f;  // Adjust this value to control how often the texture repeats

	glBegin(GL_QUADS);
	for (float i = -halfSize; i < halfSize; i += step) {
		for (float j = -halfSize; j < halfSize; j += step) {
			// Map the texture to each grid cell with scaled texture coordinates
			glTexCoord2f(i / size * textureScale, j / size * textureScale); glVertex3f(i, 0.0f, j);            // Bottom-left corner
			glTexCoord2f((i + step) / size * textureScale, j / size * textureScale); glVertex3f(i + step, 0.0f, j);     // Bottom-right corner
			glTexCoord2f((i + step) / size * textureScale, (j + step) / size * textureScale); glVertex3f(i + step, 0.0f, j + step);  // Top-right corner
			glTexCoord2f(i / size * textureScale, (j + step) / size * textureScale); glVertex3f(i, 0.0f, j + step);     // Top-left corner
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);  // Disable texture mapping after drawing
}



void loadTextures(void)
{
	glGenTextures(4, texID);  // Get the texture object IDs.
	for (int j = 0; j < 4; j++) {
		GLubyte* imageData;
		// image size
		int imageWidth, imageHeight;
		// the ID of the image file
		FILE* fileID;
		// maxValue
		int  maxValue;
		// total number of pixels in the image
		int  totalPixels;
		// temporary character
		char tempChar;
		// counter variable for the current pixel in the image
		int i;
		// array for reading in header information
		char headerLine[100];
		// if the original values are larger than 255
		float RGBScaling;
		// temporary variables for reading in the red, green and blue data of each pixel
		int red, green, blue;
		// open the image file for reading - note this is hardcoded would be better to provide a parameter which
		//is the file name. There are 3 PPM files you can try out mount03, sky08 and sea02.
		fileID = fopen(textureFileNames[j], "r");
		// read in the first header line
		//    - "%[^\n]"  matches a string of all characters not equal to the new line character ('\n')
		//    - so we are just reading everything up to the first line break
		if (fscanf(fileID, "%[^\n] ", headerLine));
		// make sure that the image begins with 'P3', which signifies a PPM file
		if ((headerLine[0] != 'P') || (headerLine[1] != '3'))
		{
			printf("This is not a PPM file!\n");
			exit(0);
		}
		// we have a PPM file
		printf("This is a PPM file\n");
		// read in the first character of the next line
		if (fscanf(fileID, "%c", &tempChar));
		// while we still have comment lines (which begin with #)
		while (tempChar == '#')
		{
			// read in the comment
			if (fscanf(fileID, "%[^\n] ", headerLine));
			// print the comment
			printf("%s\n", headerLine);
			// read in the first character of the next line
			if (fscanf(fileID, "%c", &tempChar));
		}
		// the last one was not a comment character '#', so we need to put it back into the file stream (undo)
		ungetc(tempChar, fileID);
		// read in the image hieght, width and the maximum value
		if (fscanf(fileID, "%d %d %d", &imageWidth, &imageHeight, &maxValue));
		// print out the information about the image file
		printf("%d rows  %d columns  max value= %d\n", imageHeight, imageWidth, maxValue);
		// compute the total number of pixels in the image
		totalPixels = imageWidth * imageHeight;
		// allocate enough memory for the image  (3*) because of the RGB data
		imageData = malloc(3 * sizeof(GLuint) * totalPixels);
		// determine the scaling for RGB values
		RGBScaling = 255.0f / maxValue;
		// if the maxValue is 255 then we do not need to scale the 
		//    image data values to be in the range or 0 to 255
		if (maxValue == 255)
		{
			for (i = 0; i < totalPixels; i++)
			{
				// read in the current pixel from the file
				if (fscanf(fileID, "%d %d %d", &red, &green, &blue));
				// store the red, green and blue data of the current pixel in the data array
				imageData[3 * totalPixels - 3 * i - 3] = red;
				imageData[3 * totalPixels - 3 * i - 2] = green;
				imageData[3 * totalPixels - 3 * i - 1] = blue;
			}
		}
		else  // need to scale up the data values
		{
			for (i = 0; i < totalPixels; i++)
			{
				// read in the current pixel from the file
				if (fscanf(fileID, "%d %d %d", &red, &green, &blue));
				// store the red, green and blue data of the current pixel in the data array
				imageData[3 * totalPixels - 3 * i - 3] = red * (GLubyte)RGBScaling;
				imageData[3 * totalPixels - 3 * i - 2] = green * (GLubyte)RGBScaling;
				imageData[3 * totalPixels - 3 * i - 1] = blue * (GLubyte)RGBScaling;
			}
		}
		// close the image file
		fclose(fileID);
		if (imageData)
		{
			glBindTexture(GL_TEXTURE_2D, texID[j]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGB,
				GL_UNSIGNED_BYTE, imageData);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Required since there are no mipmaps.
		}
		else {
			printf("Failed to get texture data from %s\n", textureFileNames[j]);
		}
	}

}



// Update the isGrounded function
int isGrounded() {
	const float groundLevel = 0.8f;
	const float threshold = 0.01f;
	int grounded = objectLocation[1] <= (groundLevel + threshold);
	
	return grounded;
}




void drawSkybox(float size) {

	glDisable(GL_DEPTH_TEST);  // Disable depth testing so skybox appears behind everything

	// Bind the same sky texture for all faces
	glBindTexture(GL_TEXTURE_2D, texID[3]);

	// Add this line to ensure full texture color
	glColor3f(1.0f, 1.0f, 1.0f);
	// Enable texturing for the skybox
	glEnable(GL_TEXTURE_2D);


	glBegin(GL_QUADS);

	// Front face
	glTexCoord2f(0.0f, 0.33f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25f, 0.33f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.25f, 0.66f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.66f); glVertex3f(-size, size, -size);

	// Back face
	glTexCoord2f(0.0f, 0.33f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25f, 0.33f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.25f, 0.66f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.66f); glVertex3f(-size, size, -size);

	// Left face
	glTexCoord2f(0.0f, 0.33f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25f, 0.33f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.25f, 0.66f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.66f); glVertex3f(-size, size, -size);

	// Right face
	glTexCoord2f(0.0f, 0.33f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25f, 0.33f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.25f, 0.66f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.66f); glVertex3f(-size, size, -size);

	// Top face
	glTexCoord2f(0.0f, 0.33f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25f, 0.33f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.25f, 0.66f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.66f); glVertex3f(-size, size, -size);

	// Bottom face
	glTexCoord2f(0.0f, 0.33f); glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25f, 0.33f); glVertex3f(size, -size, -size);
	glTexCoord2f(0.25f, 0.66f); glVertex3f(size, size, -size);
	glTexCoord2f(0.0f, 0.66f); glVertex3f(-size, size, -size);

	glEnd();


	glDisable(GL_TEXTURE_2D);


	glEnable(GL_DEPTH_TEST);  // Re-enable depth testing after drawing the skybox
}

void drawHouse(float width, float height, float depth) {
	// Bind the marble texture for the walls
	glBindTexture(GL_TEXTURE_2D, texID[2]);  // Assuming texID[2] is the marble texture

	// Enable 2D texture mapping
	glEnable(GL_TEXTURE_2D);

	// Set the house color to white to avoid altering the texture's color
	glColor3f(1.0f, 1.0f, 1.0f);

	// Half dimensions for positioning
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	// Draw the walls of the house (a cuboid with texture)
	glPushMatrix();

	// Front wall with texture
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfDepth);   // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfDepth);    // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, halfDepth);     // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfDepth);    // Top-left
	glEnd();

	// Back wall with texture
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfDepth);   // Bottom-right
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfDepth);  // Bottom-left
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfDepth);   // Top-left
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfDepth);    // Top-right
	glEnd();

	// Left wall with texture
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -halfDepth);  // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, halfDepth);   // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, halfDepth);    // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -halfDepth);   // Top-left
	glEnd();

	// Right wall with texture
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, halfDepth);    // Bottom-right
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -halfDepth);   // Bottom-left
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -halfDepth);    // Top-left
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, halfDepth);     // Top-right
	glEnd();

	glPopMatrix();

	// Disable 2D texture mapping after drawing the walls
	glDisable(GL_TEXTURE_2D);


	// **Drawing the roof with texture coordinates**
	glBindTexture(GL_TEXTURE_2D, texID[0]);  // Assuming texID[0] is the roof texture

	// Enable 2D texture mapping
	glEnable(GL_TEXTURE_2D);

	// Draw the roof of the house (5-sided roof)
	glColor3f(1.0f, 1.0f, 1.0f);  // White color to avoid altering the roof texture

	float roofHeight = height / 2.0f;  // Half the height for the roof
	float overhang = 0.2f * width;  // Amount to extend the roof beyond the house edges

	// Front triangular gable (with texture coordinates)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth - overhang, halfHeight, halfDepth);  // Left bottom corner (extended)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth + overhang, halfHeight, halfDepth);   // Right bottom corner (extended)
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, halfHeight + roofHeight, halfDepth);  // Roof peak (center top)
	glEnd();

	// Back triangular gable (with texture coordinates)
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth - overhang, halfHeight, -halfDepth);  // Left bottom corner (extended)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth + overhang, halfHeight, -halfDepth);   // Right bottom corner (extended)
	glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, halfHeight + roofHeight, -halfDepth);  // Roof peak (center top)
	glEnd();

	// Left sloped roof side (with texture coordinates)
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth - overhang, halfHeight, halfDepth);   // Front left bottom (extended)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth - overhang, halfHeight, -halfDepth);  // Back left bottom (extended)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, halfHeight + roofHeight, -halfDepth);  // Back roof peak
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, halfHeight + roofHeight, halfDepth);   // Front roof peak
	glEnd();

	// Right sloped roof side (with texture coordinates)
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth + overhang, halfHeight, halfDepth);    // Front right bottom (extended)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth + overhang, halfHeight, -halfDepth);   // Back right bottom (extended)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, halfHeight + roofHeight, -halfDepth);  // Back roof peak
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, halfHeight + roofHeight, halfDepth);   // Front roof peak
	glEnd();

	// Disable 2D texture mapping after drawing the roof
	glDisable(GL_TEXTURE_2D);

	// **Draw windows on the walls**
	glColor3f(0.53f, 0.81f, 0.98f);  // Light blue for the windows

	// Window on the front wall (left side)
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-halfWidth / 1.5, halfHeight / 3, halfDepth + 0.01f);  // Bottom-left (raised)
	glVertex3f(-halfWidth / 4, halfHeight / 3, halfDepth + 0.01f);    // Bottom-right (raised)
	glVertex3f(-halfWidth / 4, halfHeight / 1.5, halfDepth + 0.01f);  // Top-right (raised)
	glVertex3f(-halfWidth / 1.5, halfHeight / 1.5, halfDepth + 0.01f); // Top-left (raised)
	glEnd();
	glPopMatrix();

	// Window on the back wall (centered)
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-halfWidth / 1.5, halfHeight / 3, -halfDepth - 0.01f);  // Bottom-left (raised)
	glVertex3f(-halfWidth / 4, halfHeight / 3, -halfDepth - 0.01f);    // Bottom-right (raised)
	glVertex3f(-halfWidth / 4, halfHeight / 1.5, -halfDepth - 0.01f);  // Top-right (raised)
	glVertex3f(-halfWidth / 1.5, halfHeight / 1.5, -halfDepth - 0.01f); // Top-left (raised)
	glEnd();
	glPopMatrix();

	// Left wall - first window (left side)
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-halfWidth - 0.01f, halfHeight / 3, halfDepth / 2);  // Bottom-left (raised)
	glVertex3f(-halfWidth - 0.01f, halfHeight / 3, halfDepth / 1.5); // Bottom-right (raised)
	glVertex3f(-halfWidth - 0.01f, halfHeight / 1.5, halfDepth / 1.5); // Top-right (raised)
	glVertex3f(-halfWidth - 0.01f, halfHeight / 1.5, halfDepth / 2); // Top-left (raised)
	glEnd();
	glPopMatrix();

	// Left wall - second window (right side)
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-halfWidth - 0.01f, halfHeight / 3, -halfDepth / 1.5);  // Bottom-left (raised)
	glVertex3f(-halfWidth - 0.01f, halfHeight / 3, -halfDepth / 2); // Bottom-right (raised)
	glVertex3f(-halfWidth - 0.01f, halfHeight / 1.5, -halfDepth / 2); // Top-right (raised)
	glVertex3f(-halfWidth - 0.01f, halfHeight / 1.5, -halfDepth / 1.5); // Top-left (raised)
	glEnd();
	glPopMatrix();

	// Right wall - first window (left side)
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(halfWidth + 0.01f, halfHeight / 3, halfDepth / 2);  // Bottom-left (raised)
	glVertex3f(halfWidth + 0.01f, halfHeight / 3, halfDepth / 1.5); // Bottom-right (raised)
	glVertex3f(halfWidth + 0.01f, halfHeight / 1.5, halfDepth / 1.5); // Top-right (raised)
	glVertex3f(halfWidth + 0.01f, halfHeight / 1.5, halfDepth / 2); // Top-left (raised)
	glEnd();
	glPopMatrix();

	// Right wall - second window (right side)
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(halfWidth + 0.01f, halfHeight / 3, -halfDepth / 1.5);  // Bottom-left (raised)
	glVertex3f(halfWidth + 0.01f, halfHeight / 3, -halfDepth / 2); // Bottom-right (raised)
	glVertex3f(halfWidth + 0.01f, halfHeight / 1.5, -halfDepth / 2); // Top-right (raised)
	glVertex3f(halfWidth + 0.01f, halfHeight / 1.5, -halfDepth / 1.5); // Top-left (raised)
	glEnd();
	glPopMatrix();

	// Front wall - Wooden door (centered on the front wall)
	glPushMatrix();
	glColor3f(0.55f, 0.27f, 0.07f);  // Brown color for the wooden door
	glBegin(GL_QUADS);
	glVertex3f(-halfWidth / 8, 0.0f, halfDepth + 0.01f);  // Bottom-left (moved right)
	glVertex3f(halfWidth / 8 + halfWidth / 8, 0.0f, halfDepth + 0.01f);   // Bottom-right (moved right)
	glVertex3f(halfWidth / 8 + halfWidth / 8, halfHeight / 2, halfDepth + 0.01f);  // Top-right (moved right)
	glVertex3f(-halfWidth / 8, halfHeight / 2, halfDepth + 0.01f);  // Top-left (moved right)
	glEnd();
	glPopMatrix();

	
}

void applyMetallicMaterial() {
	// Set up material properties for a metallic look
	GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // Low ambient reflectance
	GLfloat mat_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f }; // High diffuse reflectance for metal
	GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // High specular reflectance for shininess
	GLfloat mat_shininess[] = { 10.0f }; // High shininess for sharp highlights

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void drawAircraft() {

	// Apply metallic material
	//applyMetallicMaterial();

	// Define material properties for the matte body
	GLfloat body_ambient[] = { 0.2f, 0.3f, 0.7f, 1.0f };
	GLfloat body_diffuse[] = { 0.2f, 0.3f, 0.7f, 1.0f };
	GLfloat body_specular[] = { 0.1f, 0.1f, 0.7f, 1.0f }; // Low specular for matte effect
	GLfloat body_shininess[] = { 10.0f };                 // Low shininess for matte

	// Define material properties for the shiny glass cockpit
	GLfloat cockpit_ambient[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat cockpit_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat cockpit_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // High specular for glass shine
	GLfloat cockpit_shininess[] = { 100.0f };                // High shininess for glossy effect

	// Material properties for the landing gear and other metallic parts
	GLfloat landing_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat landing_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat landing_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat langding_shininess[] = { 50.0f };

	// Material properties for the propeller blades
	GLfloat propeller_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat propeller_diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat propeller_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat propeller_shininess[] = { 10.0f };

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Apply metallic material for the body
	glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, body_shininess);

	//Double Propeller Fighter
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);

	// Draw body
	glPushMatrix();
	glScalef(0.25f, 0.25f, 1.0f);  // Scale along the x, y, and z axes to create an oval
	gluSphere(sphereQuadric, BODY_RADIUS, 50.0, 50.0);
	glPopMatrix();


	// draw Tail
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(0.0f, 0.0f, BODY_RADIUS * 0.5f);
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	glScalef(0.9f, 0.9f, 1.5f);
	glutSolidCone(1.0, 1.0, 50.0, 50.0);
	glPopMatrix();


	// Apply glass-like material for the cockpit
	glMaterialfv(GL_FRONT, GL_AMBIENT, cockpit_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cockpit_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cockpit_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, cockpit_shininess);

	// Draw cockpit
	glPushMatrix();
	//glColor3f(0.9f, 0.9f, 0.9f);
	glScalef(0.125f, 0.25f, 0.5f);  // Scale along the x, y, and z axes to create an oval
	glTranslatef(0.0f, 0.5f, -0.5f);  // Adjust to position the left body beside the main body
	gluSphere(sphereQuadric, BODY_RADIUS, 50.0, 50.0);
	glPopMatrix();


	// Apply matte material again for the rest of the body
	glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, body_shininess);

	// Draw Left engine and Tail
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);  // Same color as the main body
	glScalef(0.25f, 0.25f, 1.0f);
	// Translate the left side body to the left of the main body
	glTranslatef(-3.5f, -0.5f, 0.0f);  // Adjust to position the left body beside the main body
	// Scale along the x, y, and z axes to create an oval (same as main body)
	glScalef(0.75f, 0.75f, 0.5f);
	// Draw the left engine
	gluSphere(sphereQuadric, BODY_RADIUS, 50.0, 50.0);
	// Draw the left enginetail
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);  // Color for the tail
	glTranslatef(0.0f, 0.0f, BODY_RADIUS * 0.5f);  // Position the tail at the back of the body
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);  // Rotate if needed
	glScalef(0.9f, 0.9f, 1.5f);  // Scale the tail to desired size
	glutSolidCone(1.0, 1.0, 50.0, 50.0);  // Draw the cone-shaped tail
	glPopMatrix();
	// Draw the left enginehead
	glPushMatrix();
	//glColor3fv(PALE_ORANGE);  // Color for the tail
	glTranslatef(0.0f, 0.0f, -BODY_RADIUS * 0.9f);  // Position the tail at the back of the body
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotate if needed
	glScalef(0.25f, 0.25f, 0.25f);  // Scale the tail to desired size
	glutSolidCone(1.0, 1.0, 50.0, 50.0);  // Draw the cone-shaped tail
	glPopMatrix();
	glPopMatrix();


	// Draw Right Side engine and Tail
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f); // Same color as the main body
	glScalef(0.25f, 0.25f, 1.0f);
	// Translate the right side body to the right of the main body
	glTranslatef(3.5f, -0.5f, 0.0f);  // Adjust to position the right body beside the main body
	// Scale along the x, y, and z axes to create an oval (same as main body)
	glScalef(0.75f, 0.75f, 0.5f);
	// Draw the right engine
	gluSphere(sphereQuadric, BODY_RADIUS, 50.0, 50.0);
	// Draw the right enginetail
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f); // Color for the tail
	glTranslatef(0.0f, 0.0f, BODY_RADIUS * 0.5f);  // Position the tail at the back of the body
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);  // Rotate if needed
	glScalef(0.9f, 0.9f, 1.5f);  // Scale the tail to desired size
	glutSolidCone(1.0, 1.0, 50.0, 50.0);  // Draw the cone-shaped tail
	glPopMatrix();
	// Draw the right enginehead
	glPushMatrix();
	//glColor3fv(PALE_ORANGE);  // Color for the tail
	glTranslatef(0.0f, 0.0f, -BODY_RADIUS * 0.9f);  // Position the tail at the back of the body
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotate if needed
	glScalef(0.25f, 0.25f, 0.25f);  // Scale the tail to desired size
	glutSolidCone(1.0, 1.0, 50.0, 50.0);  // Draw the cone-shaped tail
	glPopMatrix();
	glPopMatrix();



	// Draw center tailfan
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);  // Color of the cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	// Translate the cylinder to be on top of the tail (adjust the height and position based on your model)
	glTranslatef(0.0f, BODY_RADIUS * 1.2f, BODY_RADIUS * 1.6f);  // Adjust to place on top of the tail
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make it vertical (standing on the tail)
	// Flatten the cylinder by scaling it along the z-axis
	glScalef(0.5f, 1.0f, 1.0f);  // Scale along z-axis to make it flatter
	// Draw the base disk of the cylinder
	GLUquadric* quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.2f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder (base radius, top radius, height, slices, stacks)
	gluCylinder(quadric, 0.2f, 0.3f, 1.25f, 32, 32);  // Small radius cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();

	// Draw Left tailfan
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);// Same color for the left cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(-1.3f, BODY_RADIUS * 0.0f, BODY_RADIUS * 1.6f);  // Translate to the left of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.2f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.2f, 0.3f, 1.25f, 32, 32);  // Left cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Draw Right tailfan
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);  // Same color for the right cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(1.3f, BODY_RADIUS * 0.0f, BODY_RADIUS * 1.6f);  // Translate to the right of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.2f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.2f, 0.3f, 1.25f, 32, 32);  // Right cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();



	// Draw Leftbody tailfan
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);  // Same color for the left cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(-6.3f, BODY_RADIUS * 0.0f, -BODY_RADIUS * 0.0f);  // Translate to the left of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.2f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.2f, 0.3f, 5.5f, 32, 32);  // Left cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Draw Rightbody tailfan
	glPushMatrix();
	//glColor3f(0.1f, 0.1f, 0.3f);  // Same color for the right cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(6.3f, BODY_RADIUS * 0.0f, -BODY_RADIUS * 0.0f);  // Translate to the right of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.2f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.2f, 0.3f, 5.5f, 32, 32);  // Right cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Apply metallic material for the body
	glMaterialfv(GL_FRONT, GL_AMBIENT, landing_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, landing_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, landing_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, langding_shininess);

	// Draw center landing gear
	glPushMatrix();
	//glColor3f(0.5f, 0.5f, 0.5f);  // Same color for the right cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(0.0f, -2.0f, -0.5f);  // Translate to the right of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(270.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.05f, 0.075f, 1.5f, 32, 32);  // Right cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();

	// Draw wheel under center landing gear
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 0.0f);   // Color for the wheel
	// Translate the wheel to be under the Rightbody cylinder
	glTranslatef(-0.025f, -0.5f, -0.5f);  // Lower the wheel slightly under the Rightbody cylinder
	// Rotate the wheel by 90 degrees along the Z-axis
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // Rotate the wheel by 90 degrees around the Z-axis
	// Rotate to make the wheel lie flat on the ground
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make the wheel lie flat (along the ground plane)
	// No scaling to ensure the wheel remains round
	// Draw the cylinder (body of the wheel)
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.05f, 0.05f, 0.05f, 32, 32);  // Cylinder for the wheel (same radius for top and bottom)
	// Draw the front disk of the wheel
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);  // Place the disk at the front
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Front disk with radius 0.2
	glPopMatrix();
	// Draw the back disk of the wheel
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.05f);  // Move slightly along the z-axis to place the disk at the back
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Back disk with radius 0.2
	glPopMatrix();
	// Clean up
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Draw Leftbody landing gear
	glPushMatrix();
	//glColor3f(0.5f, 0.5f, 0.5f);  // Same color for the right cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(-3.3f, -2.0f, 0.0f);  // Translate to the right of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(270.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.05f, 0.075f, 1.5f, 32, 32);  // Right cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Draw Rightbody landing gear
	glPushMatrix();
	//glColor3f(0.5f, 0.5f, 0.5f);  // Same color for the right cylinder
	glScalef(0.25f, 0.25f, 1.0f);
	glTranslatef(3.3f, -2.0f, 0.0f);  // Translate to the right of the first cylinder
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);  // No vertical rotation
	glRotatef(270.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make it lie down (along the z-axis)
	// Flatten the cylinder by scaling it along the y-axis
	glScalef(1.0f, 0.5f, 1.0f);  // Scale it along the y-axis to make it flatter
	// Draw the base disk of the cylinder
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Disk at the base with radius 0.2
	// Draw the cylinder
	gluCylinder(quadric, 0.05f, 0.075f, 1.5f, 32, 32);  // Right cylinder
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Draw wheel under Rightbody landing gear
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 0.0f);  // Color for the wheel
	// Translate the wheel to be under the Rightbody cylinder
	glTranslatef(0.8f, -0.5f, 0.0f);  // Lower the wheel slightly under the Rightbody cylinder
	// Rotate the wheel by 90 degrees along the Z-axis
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // Rotate the wheel by 90 degrees around the Z-axis
	// Rotate to make the wheel lie flat on the ground
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make the wheel lie flat (along the ground plane)
	// No scaling to ensure the wheel remains round
	// Draw the cylinder (body of the wheel)
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.05f, 0.05f, 0.05f, 32, 32);  // Cylinder for the wheel (same radius for top and bottom)
	// Draw the front disk of the wheel
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);  // Place the disk at the front
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Front disk with radius 0.2
	glPopMatrix();
	// Draw the back disk of the wheel
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.05f);  // Move slightly along the z-axis to place the disk at the back
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Back disk with radius 0.2
	glPopMatrix();
	// Clean up
	gluDeleteQuadric(quadric);
	glPopMatrix();



	// Draw wheel under Leftbody cylinder
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 0.0f);   // Color for the wheel
	// Translate the wheel to be under the Rightbody cylinder
	glTranslatef(-0.85f, -0.5f, 0.0f);  // Lower the wheel slightly under the Rightbody cylinder
	// Rotate the wheel by 90 degrees along the Z-axis
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // Rotate the wheel by 90 degrees around the Z-axis
	// Rotate to make the wheel lie flat on the ground
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate to make the wheel lie flat (along the ground plane)
	// No scaling to ensure the wheel remains round
	// Draw the cylinder (body of the wheel)
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.05f, 0.05f, 0.05f, 32, 32);  // Cylinder for the wheel (same radius for top and bottom)
	// Draw the front disk of the wheel
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);  // Place the disk at the front
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Front disk with radius 0.2
	glPopMatrix();
	// Draw the back disk of the wheel
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.05f);  // Move slightly along the z-axis to place the disk at the back
	gluDisk(quadric, 0.0f, 0.05f, 32, 1);  // Back disk with radius 0.2
	glPopMatrix();
	// Clean up
	gluDeleteQuadric(quadric);
	glPopMatrix();


	// Apply metallic material for the body
	glMaterialfv(GL_FRONT, GL_AMBIENT, propeller_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, propeller_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, propeller_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, propeller_shininess);

	// Draw propeller blades on the  left engine head
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 0.0f);  // Blade color (light gray)
	// Translate to the tip of the engine head cone
	glTranslatef(-0.87f, -0.12f, -0.55f * BODY_RADIUS);  // Adjust the translation based on the size of the cone
	// Rotate propeller blades (add animation here if needed)
	glRotatef(propellerRotationAngle, 0.0f, 0.0f, 1.0f);  // Rotate around z-axis for spinning effect
	// Draw four blades (thin rectangles)
	for (int i = 0; i < 4; ++i) {
		glPushMatrix();
		// Rotate each blade by 90 degrees relative to the previous one
		glRotatef(i * 90.0f, 0.0f, 0.0f, 1.0f);

		// Draw a single blade (a thin rectangle)
		glScalef(0.25f, 0.25f, 0.5f);  // Scale to make the blade long and thin
		glBegin(GL_QUADS);
		glVertex3f(-1.0f, 0.0f, -0.05f);  // Quad vertices to form a rectangle
		glVertex3f(1.0f, 0.0f, -0.05f);
		glVertex3f(1.0f, 0.0f, 0.05f);
		glVertex3f(-1.0f, 0.0f, 0.05f);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();


	// Draw propeller blades on the  right engine head
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 0.0f);  // Blade color (light gray)

	// Translate to the tip of the engine head cone
	glTranslatef(0.87f, -0.12f, -0.55f * BODY_RADIUS);  // Adjust the translation based on the size of the cone

	// Rotate propeller blades (add animation here if needed)
	glRotatef(propellerRotationAngle, 0.0f, 0.0f, 1.0f);  // Rotate around z-axis for spinning effect

	// Draw four blades (thin rectangles)
	for (int i = 0; i < 4; ++i) {
		glPushMatrix();
		// Rotate each blade by 90 degrees relative to the previous one
		glRotatef(i * 90.0f, 0.0f, 0.0f, 1.0f);

		// Draw a single blade (a thin rectangle)
		glScalef(0.25f, 0.25f, 0.5f);  // Scale to make the blade long and thin
		glBegin(GL_QUADS);
		glVertex3f(-1.0f, 0.0f, -0.05f);  // Quad vertices to form a rectangle
		glVertex3f(1.0f, 0.0f, -0.05f);
		glVertex3f(1.0f, 0.0f, 0.05f);
		glVertex3f(-1.0f, 0.0f, 0.05f);
		glEnd();
		glPopMatrix();
	}

	glPopMatrix();


	glPopMatrix();
}

void setupFog() {
	glEnable(GL_FOG);  // Enable fog

	// Set fog color (RGBA)
	GLfloat fogColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };  // Light gray fog
	glFogfv(GL_FOG_COLOR, fogColor);

	// Set fog mode (GL_LINEAR, GL_EXP, GL_EXP2)
	glFogi(GL_FOG_MODE, GL_LINEAR);  // Linear fog for gradual transition

	if (rainActive) {
		// Heavy fog when rain is active
		glFogf(GL_FOG_START, 10.0f);  // Fog starts closer when heavy
		glFogf(GL_FOG_END, 30.0f);   // Fog ends sooner when heavy
		glFogf(GL_FOG_DENSITY, 1.05f);  // Denser fog for heavy fog effect
	}
	else {
		// Light fog when rain is not active
		glFogf(GL_FOG_START, 30.0f);  // Fog starts farther away
		glFogf(GL_FOG_END, 80.0f);   // Fog ends farther away for light fog
		glFogf(GL_FOG_DENSITY, 0.35f);  // Lighter density for subtle fog effect
	}

	// Enable smooth shading for better fog effects
	glHint(GL_FOG_HINT, GL_NICEST);
}

void drawTree(float trunkHeight, float trunkRadius, float foliageHeight, float foliageRadius) {
	GLUquadric* quadric = gluNewQuadric();

	// Disable GL_COLOR_MATERIAL to use glMaterialfv for setting materials
	glDisable(GL_COLOR_MATERIAL);

	// Set material properties for the trunk (wood-like, matte finish)
	GLfloat trunkAmbient[] = { 0.35f, 0.16f, 0.14f, 1.0f }; // Ambient color (darker)
	GLfloat trunkDiffuse[] = { 0.24f, 0.07f, 0.07f, 1.0f }; // Diffuse color (main color of the trunk)
	GLfloat trunkSpecular[] = { 0.05f, 0.05f, 0.05f, 1.0f }; // Low specular for matte appearance
	GLfloat trunkShininess = 2.0f;  // Low shininess for a broad, soft reflection (matte)

	// Enable lighting
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, trunkAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, trunkDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, trunkSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, trunkShininess);

	// Draw the trunk (cylinder)
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // Rotate the cylinder to stand vertically (point along the y-axis)
	glTranslatef(0.0f, 0.0f, -trunkHeight / 2.0f);  // Center the trunk vertically
	gluCylinder(quadric, trunkRadius, trunkRadius, trunkHeight, 32, 32);  // Draw the trunk
	glPopMatrix();

	// Set material properties for the foliage (leaf-like)
	GLfloat foliageAmbient[] = { 0.1f, 0.3f, 0.1f, 1.0f };
	GLfloat foliageDiffuse[] = { 0.2f, 0.5f, 0.2f, 1.0f };
	GLfloat foliageSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat foliageShininess = 5.0f;  // Less shiny

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, foliageAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, foliageDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, foliageSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, foliageShininess);

	// Draw the 1st foliage (cone) on top of the trunk
	glPushMatrix();
	glTranslatef(0.0f, trunkHeight / 2, 0.0f);  // Position the cone on top of the trunk (at trunk height)
	glScalef(1.0f, 0.5f, 1.0f);  // Scale: wider in X and Z, flatter in Y
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);   // Rotate the cone to point upwards
	glutSolidCone(foliageRadius, foliageHeight, 32, 32);  // Draw the cone as foliage
	glPopMatrix();

	// Draw the 2nd foliage (cone) on top of the trunk
	glPushMatrix();
	glTranslatef(0.0f, trunkHeight / 2.5, 0.0f);  // Position the cone on top of the trunk (at trunk height)
	glScalef(2.0f, 0.6f, 2.0f);  // Scale: wider in X and Z, flatter in Y
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);   // Rotate the cone to point upwards
	glutSolidCone(foliageRadius, foliageHeight, 32, 32);  // Draw the cone as foliage
	glPopMatrix();

	// Draw the 3rd foliage (cone) on top of the trunk
	glPushMatrix();
	glTranslatef(0.0f, trunkHeight / 3.5, 0.0f);  // Position the cone on top of the trunk (at trunk height)
	glScalef(3.0f, 0.8f, 3.0f);  // Scale: wider in X and Z, flatter in Y
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);   // Rotate the cone to point upwards
	glutSolidCone(foliageRadius, foliageHeight, 32, 32);  // Draw the cone as foliage
	glPopMatrix();

	gluDeleteQuadric(quadric);

	// Disable GL_COLOR_MATERIAL to use glMaterialfv for setting materials
	glEnable(GL_COLOR_MATERIAL);
}








void drawAirplaneParkingHall(float radius, float height) {
	
	GLUquadric* quadric = gluNewQuadric();

	// Disable GL_COLOR_MATERIAL to use glMaterialfv for setting materials
	glDisable(GL_COLOR_MATERIAL);

	// Define material properties for a matte surface
	GLfloat mat_ambient[] = { 0.65f, 0.37f, 0.17f, 1.0f };  // Ambient color (similar to diffuse color)
	GLfloat mat_diffuse[] = { 0.55f, 0.27f, 0.07f, 1.0f };  // Diffuse color (the main color seen under light)
	GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };    // Very low specular reflection for a matte effect
	GLfloat mat_shininess[] = { 1.0f };                     // Low shininess for a matte look

	// Enable lighting and material
	glEnable(GL_LIGHTING);

	// Set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	// Set the color for the cylinder (Red wood color)
	//glColor3f(0.55f, 0.27f, 0.07f);
	// Rotate the cylinder to lay horizontally (along the X-axis)
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotate 90 degrees around Y-axis to make it horizontal
	// Translate the cylinder so half of it is underground
	glTranslatef(0.0f, -radius / 10.0f, 0.0f);  // Move downwards to sink half into the ground
	// Draw the half-cut cylinder
	gluCylinder(quadric, radius, radius, height, 32, 1);  // Draw the cylinder
	// Move to the back of the cylinder and draw the closing disk
	glTranslatef(0.0f, 0.0f, height);  // Move to the back of the cylinder (height of the cylinder)
	gluDisk(quadric, 0.0f, radius, 32, 1);  // Draw the disk with the same radius

	glPopMatrix();

	// Clean up the quadric object
	gluDeleteQuadric(quadric);

	// Disable GL_COLOR_MATERIAL to use glMaterialfv for setting materials
	glEnable(GL_COLOR_MATERIAL);
}

void drawAirstrip(float width, float length, float thickness) {
	// Set the color for the airstrip (dark gray)
	glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray for the airstrip surface

	glPushMatrix();
	// Translate to position the airstrip below the aircraft
	glTranslatef(0.0f, 0.0f, 0.0f);  // Move down slightly to put it on the ground
	// Scale and draw the airstrip
	glScalef(width, thickness, length);  // Width (X), thickness (Y), and length (Z)
	glutSolidCube(1.0f);  // A solid cube scaled to form the airstrip
	glPopMatrix();

	// Draw the center white line
	glPushMatrix();
	// Set the color for the white line
	glColor3f(1.0f, 1.0f, 1.0f);  // White color for the line
	// Translate the line to the center of the airstrip
	glTranslatef(0.0f, 0.01f, 0.0f);  // Slightly above the airstrip surface
	// Scale and draw the thin white line at the center
	glScalef(0.1f, thickness, length);  // Narrow width, same thickness, full length
	glutSolidCube(1.0f);  // A solid cube scaled to form the line
	glPopMatrix();
}

void drawTank(float topWidth, float bottomWidth, float height, float depth) {
	glPushMatrix();

	// Enable texture mapping and bind the texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID[3]);  // Assuming texID[3] is your tank texture
	glColor3f(0.2f, 0.3f, 0.2f);
	glBegin(GL_QUADS);

	//main body
	// Front face (connects top and bottom widths)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomWidth / 2, 0.0f, depth / 2);  // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomWidth / 2, 0.0f, depth / 2);   // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topWidth / 2, height, depth / 2);    // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topWidth / 2, height, depth / 2);   // Top-left

	// Back face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomWidth / 2, 0.0f, -depth / 2);  // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topWidth / 2, height, -depth / 2);    // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topWidth / 2, height, -depth / 2);   // Top-left

	// Left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomWidth / 2, 0.0f, depth / 2);    // Bottom-front
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-back
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topWidth / 2, height, -depth / 2);    // Top-back
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topWidth / 2, height, depth / 2);     // Top-front

	// Right face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomWidth / 2, 0.0f, depth / 2);    // Bottom-front
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-back
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topWidth / 2, height, -depth / 2);    // Top-back
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topWidth / 2, height, depth / 2);     // Top-front

	// Top face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topWidth / 2, height, depth / 2);    // Top-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topWidth / 2, height, depth / 2);     // Top-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topWidth / 2, height, -depth / 2);    // Bottom-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topWidth / 2, height, -depth / 2);   // Bottom-left

	// Bottom face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomWidth / 2, 0.0f, depth / 2);    // Bottom-left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomWidth / 2, 0.0f, depth / 2);     // Bottom-right
	glTexCoord2f(1.0f, 1.0f); glVertex3f(bottomWidth / 2, 0.0f, -depth / 2);    // Top-right
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-bottomWidth / 2, 0.0f, -depth / 2);   // Top-left

	glEnd();

	glPopMatrix();

	// Disable texture mapping after drawing the tank body
	glDisable(GL_TEXTURE_2D);

	// Draw the top cylinder on the main body
	GLUquadric* quadric = gluNewQuadric();
	glPushMatrix();
	glColor3f(0.2f, 0.3f, 0.2f);
	glTranslatef(0.0f, 0.7f, 0.0f);
	glTranslatef(0.0f, height + 0.1f, 0.0f);  // Translate cylinder above the main body

	// Enable texture for the cylinder
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID[3]);
	gluQuadricTexture(quadric, GL_TRUE);  // Enable texture for the quadric

	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Rotate the cylinder to be vertical
	gluCylinder(quadric, 0.3f, 0.5f, 0.8f, 32, 32);  // Draw the cylinder (radius = 0.5, height = 1.5)
	// Draw the disk to close the top of the cylinder
	gluDisk(quadric, 0.0f, 0.3f, 32, 1);  // Draw the disk (radius = 0.3)

	// Disable texture mapping for the cylinder
	glDisable(GL_TEXTURE_2D);

	// Draw the cannon (horizontal cylinder)
	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -0.4f);  // Position the cannon on top of the vertical cylinder
	glRotatef(60.0f, 0.0f, 1.0f, 0.0f);  // Rotate the cannon cylinder to be horizontal

	// Enable texture for the cannon
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID[3]);
	gluQuadricTexture(quadric, GL_TRUE);  // Enable texture for the quadric

	gluCylinder(quadric, 0.1f, 0.1f, 1.5f, 32, 32);  // Draw the cannon (radius = 0.15, length = 2.0)
	glPopMatrix();

	glPopMatrix();
	gluDeleteQuadric(quadric);

	// Draw the front and back smaller parts
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID[3]);  // Bind the texture for smaller parts

	// Front smaller part
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, depth / 1.5 + 0.01f);  // Position it in front of the main body
	glScalef(1.5f, 1.0f, 1.0f);
	drawFrontBackPart(topWidth / 1.5f, bottomWidth / 1.5f, height / 2, depth / 2, 0.3f, depth / 4);  // Smaller trapezoid with cylinders
	glPopMatrix();

	// Back smaller part
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -depth / 1.35 - 0.01f);  // Position it behind the main body
	glScalef(1.5f, 1.0f, 1.0f);
	drawFrontBackPart(topWidth / 1.5f, bottomWidth / 1.5f, height / 2, depth / 2, 0.3f, depth / 4);  // Smaller trapezoid with cylinders
	glPopMatrix();

	// Disable texture mapping after drawing
	glDisable(GL_TEXTURE_2D);


}

void drawFrontBackPart(float topWidth, float bottomWidth, float height, float depth, float cylinderRadius, float cylinderLength) {
	
	GLUquadric* quadric = gluNewQuadric();  // Create a new quadric object for drawing disks

	glPushMatrix();
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);

	/*
	// Front face (connects top and bottom widths)
	glVertex3f(-bottomWidth / 2, 0.0f, depth / 2);  // Bottom-left
	glVertex3f(bottomWidth / 2, 0.0f, depth / 2);   // Bottom-right
	glVertex3f(topWidth / 2, height, depth / 2);    // Top-right
	glVertex3f(-topWidth / 2, height, depth / 2);   // Top-left

	// Back face
	glVertex3f(-bottomWidth / 2, 0.0f, -depth / 2);  // Bottom-left
	glVertex3f(bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-right
	glVertex3f(topWidth / 2, height, -depth / 2);    // Top-right
	glVertex3f(-topWidth / 2, height, -depth / 2);   // Top-left
	*/

	// Left face
	glVertex3f(-bottomWidth / 2, 0.0f, depth / 2);    // Bottom-front
	glVertex3f(-bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-back
	glVertex3f(-topWidth / 2, height, -depth / 2);    // Top-back
	glVertex3f(-topWidth / 2, height, depth / 2);     // Top-front

	// Right face
	glVertex3f(bottomWidth / 2, 0.0f, depth / 2);    // Bottom-front
	glVertex3f(bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-back
	glVertex3f(topWidth / 2, height, -depth / 2);    // Top-back
	glVertex3f(topWidth / 2, height, depth / 2);     // Top-front

	// Top face
	glVertex3f(-topWidth / 2, height, depth / 2);    // Top-left
	glVertex3f(topWidth / 2, height, depth / 2);     // Top-right
	glVertex3f(topWidth / 2, height, -depth / 2);    // Bottom-right
	glVertex3f(-topWidth / 2, height, -depth / 2);   // Bottom-left

	// Bottom face
	glVertex3f(-bottomWidth / 2, 0.0f, depth / 2);    // Top-left
	glVertex3f(bottomWidth / 2, 0.0f, depth / 2);     // Top-right
	glVertex3f(bottomWidth / 2, 0.0f, -depth / 2);    // Bottom-right
	glVertex3f(-bottomWidth / 2, 0.0f, -depth / 2);   // Bottom-left

	glEnd();

	// Draw three cylinders horizontally aligned in this part
	for (int i = -1; i <= 1; i++) {
		glPushMatrix();
		glTranslatef(0.0f, 0.11f, -0.18f);
		glTranslatef(i * (topWidth / 5), height / 4, 0.0f);  // Adjust the cylinder positions based on the part width
		glScalef(0.4f, 0.8f, 1.7f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // Rotate cylinder to be horizontal along the X-axis
		gluCylinder(quadric, cylinderRadius, cylinderRadius, cylinderLength, 32, 32);

		// Draw the disk for the front side of the cylinder
		glPushMatrix();
		glColor3f(0.1f, 0.1f, 0.1f);
		glTranslatef(0.0f, 0.0f, 0.0f);  // Position at the front end of the cylinder
		gluDisk(quadric, 0.0f, cylinderRadius, 32, 1);  // Disk covering the front
		glPopMatrix();

		// Draw the disk for the back side of the cylinder
		glPushMatrix();
		glColor3f(0.1f, 0.1f, 0.1f);
		glTranslatef(0.0f, 0.0f, cylinderLength);  // Position at the back end of the cylinder
		gluDisk(quadric, 0.0f, cylinderRadius, 32, 1);  // Disk covering the back
		glPopMatrix();

		glPopMatrix();
	}

	

	glPopMatrix();

	gluDeleteQuadric(quadric);  // Clean up the quadric object

}


void createTreeDisplayList() {
	treeDisplayList = glGenLists(1);  // Generate a unique display list identifier
	glNewList(treeDisplayList, GL_COMPILE);  // Start defining the display list

	// Draw your tree
	glScalef(0.75f, 0.75f, 0.75f);
	drawTree(10.0f, 0.3f, 2.0f, 1.0f); // Example dimensions of the tree

	glEndList();  // End the display list definition
}

void drawMultipleTrees() {
	// Create the display list (only need to call this once, at initialization)
	createTreeDisplayList();

	// Draw the trees at different positions using the display list
	//north-west
	glPushMatrix();
	glTranslatef(-5.0f, 0.0f, -10.0f);  // Position of the first tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-14.0f, 0.0f, -15.0f);   // Position of the second tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-27.0f, 0.0f, -20.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-34.0f, 0.0f, -25.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25.0f, 0.0f, -30.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();
	// You can add more trees by changing the positions and calling the display list

	//east-north
	glPushMatrix();
	glTranslatef(6.0f, 0.0f, -20.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9.0f, 0.0f, -11.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0f, 0.0f, -17.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(23.0f, 0.0f, -27.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35.0f, 0.0f, -22.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 0.0f, -15.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	//west-south
	glPushMatrix();
	glTranslatef(-5.0f, 0.0f, 10.0f);  // Position of the first tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-14.0f, 0.0f, 15.0f);   // Position of the second tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-27.0f, 0.0f, 20.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-34.0f, 0.0f,25.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25.0f, 0.0f, 30.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	//east-south
	glPushMatrix();
	glTranslatef(6.0f, 0.0f, 10.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9.0f, 0.0f, 13.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0f, 0.0f, 21.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(23.0f, 0.0f, 24.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.0f, 0.0f, 35.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.0f, 0.0f, 31.0f);    // Position of the third tree
	glCallList(treeDisplayList);       // Call the display list
	glPopMatrix();

}

void createHouseDisplayList() {
	HouseDisplayList = glGenLists(1);  // Generate a unique display list identifier
	glNewList(HouseDisplayList, GL_COMPILE);  // Start defining the display list

	// Draw your tree
	glScalef(2.0f, 2.0f, 2.0f);
	drawHouse(2.0f, 2.0f, 4.0f); // Example dimensions of the tree

	glEndList();  // End the display list definition
}

void drawMultipleHouses() {
	// Create the display list (only need to call this once, at initialization)
	createHouseDisplayList();

	// Draw the trees at different positions using the display list
	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, -5.0f);  // Position of the first tree
	glCallList(HouseDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0f, 0.0f, 0.0f);   // Position of the second tree
	glCallList(HouseDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, 5.0f);    // Position of the third tree
	glCallList(HouseDisplayList);       // Call the display list
	glPopMatrix();

	// You can add more trees by changing the positions and calling the display list
}

void initializeRain() {
	for (int i = 0; i < NUM_RAIN_DROPS; i++) {
		// Spread rain across a much larger area
		rain[i].x = (float)(rand() % 200 - 100);  // -100 to +100 range
		rain[i].y = (float)(rand() % 50 + 10);    // Higher starting point
		rain[i].z = (float)(rand() % 200 - 100);  // -100 to +100 range
		// Much slower speed for gentle rain
		rain[i].speed = (rand() % 5 + 5) / 40.0f;  // Reduced speed significantly
	}
}

void updateRain() {
	if (!rainActive) return;  // Don't update if rain is off

	for (int i = 0; i < NUM_RAIN_DROPS; i++) {
		rain[i].y -= rain[i].speed;  // Move raindrop downwards

		if (rain[i].y < 0.0f) {  // If it reaches the ground, reset the position
			rain[i].y = (float)(rand() % 50 + 10);  // Reset to a random height
			// Maintain the wide distribution when resetting positions
			rain[i].x = (float)(rand() % 200 - 100);
			rain[i].z = (float)(rand() % 200 - 100);
			// Assign new random speed when resetting
			rain[i].speed = (rand() % 5 + 5) / 40.0f;  // Keep consistent with initialization
		}
	}
}

void drawRain() {

	if (!rainActive) return;  // Don't draw if rain is off

	glColor3f(0.7f, 0.7f, 1.0f);  // Lighter blue color for gentler appearance
	glBegin(GL_LINES);
	for (int i = 0; i < NUM_RAIN_DROPS; i++) {
		glVertex3f(rain[i].x, rain[i].y, rain[i].z);
		// Shorter rain drops for gentler appearance
		glVertex3f(rain[i].x, rain[i].y + 0.5f, rain[i].z);  // Reduced drop length
	}
	glEnd();
}

void createTankDisplayList() {
	TankDisplayList = glGenLists(1);  // Generate a unique display list identifier
	glNewList(TankDisplayList, GL_COMPILE);  // Start defining the display list

	glPushMatrix();
	glTranslatef(tankPosition[0], tankPosition[1], tankPosition[2]); // Move tank to current position
	glTranslatef(-10.0f, 0.0f, -20.0f);
	glRotatef(tankRotation, 0.0f, 1.0f, 0.0f);  // Rotate the tank around the Y-axis
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
	drawTank(2.0f, 1.0f, 1.0f, 1.0f);  // Draw the tank (or call your custom tank drawing function)
	glPopMatrix();

	glEndList();  // End the display list definition
}

void drawMultipleTanks() {
	// Create the display list (only need to call this once, at initialization)
	createTankDisplayList();

	// Draw the trees at different positions using the display list
	glPushMatrix();
	glTranslatef(30.0f, 0.0f, -5.0f);  // Position of the first tree
	glCallList(TankDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25.0f, 0.0f, 40.0f);   // Position of the second tree
	glCallList(TankDisplayList);       // Call the display list
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, 0.0f, 20.0f);    // Position of the third tree
	glCallList(TankDisplayList);       // Call the display list
	glPopMatrix();

	// You can add more trees by changing the positions and calling the display list
}

void setupNightMode() {
	if (rainActive) {
		// Darker ambient light for night time effect
		GLfloat ambientLight[] = { 0.1f, 0.1f, 0.2f, 1.0f };  // Dark blue tint for night
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

		// Darker sky color for night time
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);  // Almost black sky with slight blue tint

		// Optionally dim the brightness of scene objects (like the ground)
		GLfloat materialColor[] = { 0.2f, 0.2f, 0.3f, 1.0f };  // Darker materials for night effect
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	}
	else {
		// Brighter ambient light for day time effect
		GLfloat ambientLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };  // Bright white light for daytime
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

		// Bright sky color for day time
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f);  // Light blue sky

		// Restore normal brightness for scene objects
		GLfloat materialColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Normal materials for day effect
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	}
}

/******************************************************************************/