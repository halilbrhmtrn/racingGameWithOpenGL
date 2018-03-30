/*********
CTIS164 - Template Source Program
----------
STUDENT :Halil İbrahim Turan
ID: 21503113
SECTION :03
HOMEWORK:01

----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define TIMER_PERIOD  80 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define D2R 0.0174532


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool timerActive = true;
int r = 20;
int xVelocity[5] = { 0,0,0,0,0 };
int line[5] = { 0,0,0,0,0 };
int winnerFinal;


//time Counter 
typedef struct {
	int timeMilisec;
	int timeSec;
	int timeMin;
}timer;
timer timeCounter = { 0,0,0 };

//basic Functions to find winner
int biggestIndex(int *arr,int size) {
	int biggesT = arr[0];
	int maxIndex = 0;
	for (int i = 1; i < size; i++) {

		if (arr[i] > biggesT) {
			biggesT = arr[i];
			maxIndex = i;
		}
			

	}
	return maxIndex+1;
}
int lowestIndex(int *arr, int size) {
	int lowest = arr[0];
	int lowIndex = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i] < lowest) {
			lowest = arr[i];
			lowIndex = i;
		}
	}
	return lowIndex + 1;
}
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}
void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}
void print(int x, int y, char *string, void *font)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}
// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}
// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);
	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void displayObject(int x,int y,int objectNumber) {
	glColor3f(1, 0, 0);
	//head part of stick man
	circle(-270 + x, 270 + y, r);
	glColor3f(0, 0, 0);
	glRectf(-280+x, 275+y, -275+x, 270+y);
	glRectf(-260 + x, 275 + y, -265 + x, 270 + y);
	//body
	glBegin(GL_LINES);
	glVertex2f(-270 + x, 250 + y);
	glVertex2f(-270 + x, 225 + y);
	glEnd();
	//Arms
	glBegin(GL_LINES);
	glVertex2f(-270 + x, 245 + y);
	glVertex2f(-285 + x, 245 + y);
	glVertex2f(-270 + x, 245 + y);
	glVertex2f(-255 + x, 245 + y);
	glEnd();
	//footer
	glBegin(GL_TRIANGLES);
	glVertex2f(-270 + x, 226 + y);
	glVertex2f(-260 + x, 210 + y);
	glVertex2f(-280 + x, 210 + y);
	glEnd();
	//objectNumber
	vprint((-274 + x), 258 + y, GLUT_BITMAP_8_BY_13, "%d", objectNumber);

}
void displayGameScreen() {
	
	glColor3f(0.3, 0.2, 0.5);
	for (int i = -3; i < 3; i++) {

		glBegin(GL_LINES);
		glVertex2f(-300, -i * 100);
		glVertex2f(300, -100 * i);
		glEnd();
	}
	//header part of GameScreen
	glColor3f(0, 1, 1);
	glRectf(-300, 300, 300, 200);
	glColor3f(0, 0, 0.5);
	vprint(-250, 260, GLUT_BITMAP_9_BY_15, "TIME");
	glColor3f(1, 0.1, 0.4);
	vprint(-260, 230, GLUT_BITMAP_9_BY_15, "%02d:%02d:%02d", timeCounter.timeMin, timeCounter.timeSec, timeCounter.timeMilisec);
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-160, 280);
	glVertex2f(-160, 240);
	glVertex2f(160, 240);
	glVertex2f(160, 280);
	glEnd();
	glColor3ub(255, 123, 44);
	glRectf(-155, 275, 155, 245);
	//-150, 270
	glColor3f(1, 1, 1);
	vprint(-150, 260, GLUT_BITMAP_HELVETICA_10, "RACING THINGS GAME BY HALIL IBRAHIM TURAN");
	//
	glColor3f(0, 0, 0.5);
	vprint(230, 260, GLUT_BITMAP_9_BY_15, "WINNER");
	glColor3f(1, 0.1, 0.4);
	vprint(240, 230, GLUT_BITMAP_9_BY_15, "%02d", winnerFinal);
	vprint(-170, 220, GLUT_BITMAP_8_BY_13, "Press <SpaceBar> to stop/restart animation");


}
//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	displayGameScreen();
	//drawin' objects
	for (int i = 0; i < 5; i++) {
		displayObject(xVelocity[i], -100 * (i+1), i + 1);
	}


	glutSwapBuffers();
}
//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		timerActive = !timerActive;
	// to refresh the window it calls display() function
	glutPostRedisplay();
}
void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	case GLUT_KEY_F1://F1 to restart;
		timeCounter = { 0,0,0 };
		winnerFinal = 0;
		for (int j = 0; j < 5; j++) {
			line[j] = 0;
			xVelocity[j] = 0;
		}
		break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}
void onMoveDown(int x, int y) {
	// Write your codes here.

	// to refresh the window it calls display() function   
	glutPostRedisplay();
}
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
#if TIMER_ON == 1
void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (timerActive) {
		//time calc
		timeCounter.timeMilisec+=5;
		if (timeCounter.timeMilisec % 100 == 0) {
			timeCounter.timeSec++;
			timeCounter.timeMilisec = 0;
			if (timeCounter.timeSec % 60 == 0) {
				timeCounter.timeMin++;
				timeCounter.timeSec = 0;
			}
		}
		//Moving objects
		srand(time(NULL));
		for (int j = 0; j < 5; j++) {
			if (line[j] == 0) {
				xVelocity[j] += (rand() % 7) + 4;

				if (xVelocity[j] > (WINDOW_WIDTH - 45)) {
					line[j] = 1;
				}
				winnerFinal = biggestIndex(xVelocity, 5);
			}

			else {
				xVelocity[j] -= (rand() % 7) + 3;

				if (xVelocity[j] < 0) {
					timerActive = !timerActive;
				}
				winnerFinal = lowestIndex(xVelocity, 5);
			}

		}
		
	}


	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif
void Init() {
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CTIS HW-01");
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);
	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);
#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
	Init();
	glutMainLoop();
}