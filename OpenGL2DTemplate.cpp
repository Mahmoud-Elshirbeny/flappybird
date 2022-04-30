#include <Windows.h>
#include <thread>
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include <random>
#include <time.h>
#include <iostream>
#include <glut.h>


//game is over or not?
bool gameover = false;

//power-up-hit values for each set
bool safe1 = false;
bool safe2 = false;
bool safe3 = false;
//rng
int rng[3];
//create 3 randomly generated numbers to represent order of the sets
void generaterng(int a[]) {
	srand(time(0));
	int rng1 = rand() % 3;

	int rng2 = rand() % 3;
	while (rng2 == rng1) {
		rng2 = rand() % 3;
	}
	int rng3 = rand() % 3;
	while (rng3 == rng2 || rng3 == rng1) {
		rng3 = rand() % 3;
	}

	a[0] = rng1;
	a[1] = rng2;
	a[2] = rng3;
}


//reference
double absolutex = 0;


//player y - coordinate
double playery = 300;


//begining x coordinates for the 3 sets
double setx[3];

//generate the 3 sets x - coordinate 
void generateSetx(double a[]) {
	a[0] = absolutex + (1000 * rng[0]);
	a[1] = absolutex + (1000 * rng[1]);
	a[2] = absolutex + (1000 * rng[2]);
}

//draw the sets
void drawSet(int i) {
	glBegin(GL_QUADS);
	glVertex3d(setx[i], 0.0, 0.0);
	glVertex3d(setx[i], 750, 0.0);
	glVertex3d(setx[i] + 1000, 750, 0.0);
	glVertex3d(setx[i] + 1000, 0, 0);
	glEnd();
}


//begining x,y coordinates for the 12 barriers
double barriersx[12];
int barriersy[12] = { 200,350,150,400,
					400,250,350,200,
					300,450,200,300 };

//generate x coordinates for barriers according to rng
void generateBarrierx(double a[]) {
	a[0] = absolutex + 150 + (1000 * rng[0]);
	a[1] = absolutex + 400 + (1000 * rng[0]);
	a[2] = absolutex + 650 + (1000 * rng[0]);
	a[3] = absolutex + 900 + (1000 * rng[0]);

	a[4] = absolutex + 150 + (1000 * rng[1]);
	a[5] = absolutex + 400 + (1000 * rng[1]);
	a[6] = absolutex + 650 + (1000 * rng[1]);
	a[7] = absolutex + 900 + (1000 * rng[1]);

	a[8] = absolutex + 150 + (1000 * rng[2]);
	a[9] = absolutex + 400 + (1000 * rng[2]);
	a[10] = absolutex + 650 + (1000 * rng[2]);
	a[11] = absolutex + 900 + (1000 * rng[2]);
}

//draw barriers, if it's a safe set change the color of that barrier.
void drawBarrier(int i, bool safe) {
	
	//lower portion
	if (safe)
	{
		glColor3d(0.1, 0.1, 0.5);
	}
	else
	{
		glColor3d(1, 0.1, 0.1);
	}
	glBegin(GL_QUADS);
	glVertex2d(barriersx[i], 0);
	glVertex2d(barriersx[i], barriersy[i]);
	glVertex2d(barriersx[i] + 100, barriersy[i]);
	glVertex2d(barriersx[i] + 100, 0);
	glEnd();

	//upper portion
	if (safe)
	{
		glColor3d(0.5, 0.5, 0.9);
	}
	else
	{
		glColor3d(0.4, 0.4, 0.4);
	}
	glBegin(GL_POLYGON);
	glVertex2d(barriersx[i], 750);
	glVertex2d(barriersx[i], barriersy[i] + 200);
	glVertex2d(barriersx[i] + 100, barriersy[i] + 200);
	glVertex2d(barriersx[i] + 100, 750);
	glEnd();
}




//begining x,y coordinates for the 3 power-ups
double powerupx[3];
int powerupy[3] = { 250,500,350 };

//function to decide where each power up was randomly put
void generatepowerupx(double a[]) {
	a[0] = absolutex + 150 + 35 + (1000 * rng[0]);
	a[1] = absolutex + 150 + 35 + (1000 * rng[1]);
	a[2] = absolutex + 150 + 35 + (1000 * rng[2]);
}

//function to draw each power up. to be called in a loop
void drawPowerup(int i) {

	glColor3d(0.7 , 0.9 , 1);
	glBegin(GL_QUADS);
	glVertex2d(powerupx[i], powerupy[i]);
	glVertex2d(powerupx[i], powerupy[i] + 30);
	glVertex2d(powerupx[i] + 30, powerupy[i] + 30);
	glVertex2d(powerupx[i] + 30, powerupy[i]);

	glColor3d(1, 1, 0.7);
	glBegin(GL_POLYGON);
	glVertex2d(powerupx[i], powerupy[i]);
	glVertex2d(powerupx[i] + 8, powerupy[i] + 30);
	glVertex2d(powerupx[i] + 22, powerupy[i]+30);
	glVertex2d(powerupx[i] + 30, powerupy[i]);
	glEnd();


	glEnd();
}



//lives value and text
int lives = 3;
char* livesnum[1];

//damage player because they hit a barrier!
void damage(int i) {
	/*std::cout << "i" << std::endl;
	std::cout << i << std::endl;
	std::cout << "lives" << std::endl;
	std::cout << lives << std::endl;*/
	PlaySound(TEXT("fire2.wav"), NULL, SND_ASYNC);
	lives = lives - 1;
	playery = barriersy[i] + 100;
	if (lives == 0) {
		gameover = true;
	}
}


//score value and text
int score = 1;
char* scorenum[6];


//printer for text
void printM(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}



//button press to fly
void Key(unsigned char key, int x, int y) {
	if (playery < 725) {
		if (key == 'w') {
			playery += 25;
		}
	}
	else {
		gameover = true;
	}
}


//display function
void Display() {
	//duh
	glClear(GL_COLOR_BUFFER_BIT);

	/*std::cout << powerupx[0] << std::endl;
	std::cout << powerupx[1] << std::endl;
	std::cout << powerupx[2] << std::endl;
	std::cout << "end power" << std::endl;
	std::cout << setx[0] << std::endl;
	std::cout << setx[1] << std::endl;
	std::cout << setx[2] << std::endl;
	std::cout << "end set" << std::endl;*/

	/*std::cout << lives << std::endl;
	std::cout << "end lives" << std::endl;

	std::cout << barriersx[8] << std::endl;
	std::cout << barriersx[9] << std::endl;
	std::cout << barriersx[10] << std::endl;
	std::cout << "end barriers" << std::endl;*/

	//Sets
	generateSetx(setx);

	glColor3d(1, 0.9, 0.9);
	drawSet(0);

	glColor3d(0.9, 1, 0.9);
	drawSet(1);

	glColor3d(0.9, 0.9,1);
	drawSet(2);



	//Barriers
	generateBarrierx(barriersx);

	//draw barriers of set 1
	for (int i = 0; i < 4; i++) {
		drawBarrier(i, safe1);
	}

	//draw barriers of set 2
	for (int i = 4; i < 8; i++) {
		drawBarrier(i, safe2);
	}

	//draw barriers of set 3
	for (int i = 8; i < 12; i++) {
		drawBarrier(i, safe3);
	}

	//powerup
	generatepowerupx(powerupx);
	for (int i = 0; i < 3; i++) {
		drawPowerup(i);
	}

	//player
	glBegin(GL_POLYGON);
	glColor3d(0.4, 0.6 , 0.9);
	glVertex3f(20, playery, 0.0);
	glVertex3f(20, playery + 20, 0.0);
	glVertex3f(40, playery + 20, 0.0);
	glVertex3f(40, playery, 0);
	glEnd();

	glColor3d(1, 1, 0.4);
	glBegin(GL_TRIANGLES);
	glVertex2f(28, playery);
	glVertex2f(40, playery );
	glVertex2f(40, playery + 15);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(36, playery + 15);
	glVertex2f(38, playery + 15);
	glVertex2f(38, playery + 17);
	glVertex2f(36, playery + 17);
	glEnd();

	//lives
	glColor3d(0.6, 0, 0.2);
	sprintf((char*)livesnum, "Lives: %d", lives);
	printM(15, 725, (char*)livesnum);


	//score
	glColor3d(0.1 , 0.3 , 0);
	sprintf((char*)scorenum, "Score: %d", score);
	printM(480, 725, (char*)scorenum);


	glFlush();
}

//timer function
void Timer(int value) {

	//move everything to the left, if the reference is still ahead of the player
	if (absolutex > -2960) {
		absolutex -= 1;
		//pull player down if they are not yet at the ground
		if (playery > 1) {
			playery -= 1.1;
		}
		else {
			gameover = true;
		}
	}
	else {
		gameover = true;
	}

	//score
	score += 1;


	//collisions
	//barriers
	//loop on all barriers
	if (score > 50){
		for (int i = 0; i < 12; i++) {
			//check if the left side or the right side of the player are in between this barrier's x coordinates.
			if (((barriersx[i] <= 20) && ((barriersx[i] + 100) >= 20)) || ((barriersx[i] <= 40) && ((barriersx[i] + 100) >= 40))) {
				//check if the player's bottom is lower than the lower barrier's top, or if their top is higher than the upper barrier's bottom.
				if ((barriersy[i] > playery) || (barriersy[i] + 200 < playery + 20)) {
					switch (i)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						if (!safe1) {
							
							damage(i);
						}
						break;


					case 4:
					case 5:
					case 6:
					case 7:
						if (!safe2) {
							damage(i);
						}
						break;

					case 8:
					case 9:
					case 10:
					case 11:
						if (!safe3) {
							damage(i);
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	//powerup
	//loop on all 3 power ups
	if (score > 50) {
		for (int i = 0; i < 3; i++) {
			//check if player's left side or right side are in between the power-up's x-coordinates
			if (((powerupx[i] <= 20) && ((powerupx[i] + 30) >= 20)) || ((powerupx[i] <= 40) && ((powerupx[i] + 30) >= 40))) {
				//since player can't hit power up from below, we only check if it's upper bound is higher than his lower bound.
				if (((powerupy[i] <= playery) && ((powerupy[i] + 30) >= playery)) || ((powerupy[i] <= playery+20) && ((powerupy[i] + 30) >= playery+20))) {
					//send the power up away now
					powerupx[i] = -1000;
					powerupy[i] = 50000;
					switch (i)
					{
					case 0:
						PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);
						safe1 = true;
						break;
					case 1: 
						safe2 = true;
						PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);
						break;
					default:
						safe3 = true;
						PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);
						break;
					}
				}
			}
		}
	}
	glutPostRedisplay();

	//if game isn't over call itself again.
	if (!gameover) {
		glutTimerFunc(10, Timer, 0);
	}
	else {
		PlaySound(TEXT("tuturu.wav"), NULL, SND_ASYNC);
	}
}

void main(int argc, char** argr) {
	generaterng(rng);
	glutInit(&argc, argr);

	glutInitWindowSize(600, 750);
	glutInitWindowPosition(250, 0);

	glutCreateWindow("flappy birb");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);
	glutTimerFunc(0, Timer, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 600, 0.0, 750);

	glutMainLoop();
}
