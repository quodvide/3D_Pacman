#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>		// Header File For Windows
#include <gl/gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\GLAUX.H>		// Header File For The Glaux Library
#include <gl/glut.h>
#include "AI.h"
#include "board.h"

//destructor
AI::~AI(void){};

//constructor
AI::AI(GLfloat tempX, GLfloat tempY) {
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 0.0;
	color[3] = 1.0;


	x = tempX;
	y = tempY;

	//move part
	dir = 0;
	aiSpeed = 0.05;
	//random
	randomTemp = 0;

	isAlive = 1;
	isEdible = 0;
	deadTimer = 0;
	edibleTimer = 0;
}

void AI::draw(void) {
	GLfloat ambient[3];
	ambient[0] = 128;
	ambient[1] = 0;
	ambient[2] = 0;

	//if ai edible , coloring black , draw
	if (isEdible) {
		ambient[0] = 0;
		ambient[1] = 0;
		ambient[2] = 0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient); // 검은색
	}
	else{
		//glBindTexture(GL_TEXTURE_2D, texture[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient); // 빨간색
	}

	glTranslatef(x, 0, y);
	//gluSphere(pacImg, 0.95, 100, 100);
	glutSolidSphere(0.95, 20, 20);
	glTranslatef(-x, 0, -y);

	// at the corner
	if (isCorner()) {
		//select direction of ai
		update();
	}
	//than move tht direction 
	move();
}

int AI::isCorner(void) {

	if (roundf(x * 10) / 10 == (float)(int)(roundf(x * 10) / 10) &&
		roundf(y * 10) / 10 == (float)(int)(roundf(y * 10) / 10))
		return 1;
	else return 0;

}


void AI::move(void) {

	switch (dir)
	{
	case 0:
		y -= aiSpeed;
		break;
	case 1:
		y += aiSpeed;
		break;
	case 2:
		x -= aiSpeed;
		break;
	case 3:
		x += aiSpeed;
		break;
	default:
		break;
	}
	// 미로를 벗어났을 때 반대쪽으로 감.
	if (x < -14)
		x = 14;
	if (x > 15)
		x = -13;

}

//dir change;
void AI::update(void) {
	switch (dir) {
	case 0:
		collisionDetection();
		srand(time(NULL));
		if (leftFlag && !upFlag && !downFlag)
			dir = dir;
		else if (!leftFlag && upFlag && !downFlag)
			dir = 3;
		else if (!leftFlag && !upFlag && downFlag)
			dir = 2;

		else if (leftFlag && upFlag && !downFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 3;
		}
		else if (leftFlag && !upFlag && downFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 2;
		}
		else if (!leftFlag && upFlag && downFlag) {
			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = 3;
			else dir = 2;
		}
		//all way
		else {
			randomTemp = rand() % 3;
			switch (randomTemp)
			{
			case 0:
				dir = dir;
				break;
			case 1:
				dir = 3;
				break;
			case 2:
				dir = 2;
				break;
			default:
				break;
			}
		}
		//}
		break;
	case 1:

		collisionDetection();
		srand(time(NULL) + 23511);
		if (rightFlag && !upFlag && !downFlag)
			dir = dir;
		else if (!rightFlag && upFlag && !downFlag)
			dir = 3;
		else if (!rightFlag && !upFlag && downFlag)
			dir = 2;

		else if (rightFlag && upFlag && !downFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 3;
		}
		else if (rightFlag && !upFlag && downFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 2;
		}
		else if (!rightFlag && upFlag && downFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = 3;
			else dir = 2;
		}
		//all way
		else {
			randomTemp = rand() % 3;
			switch (randomTemp)
			{
			case 0:
				dir = dir;
				break;
			case 1:
				dir = 3;
				break;
			case 2:
				dir = 2;
				break;
			default:
				break;
			}
		}
		//}
		break;
	case 2:

		collisionDetection();
		srand(time(NULL) + 1093);

		if (downFlag && !leftFlag && !rightFlag)
			dir = dir;
		else if (!downFlag && leftFlag && !rightFlag)
			dir = 0;
		else if (!downFlag && !leftFlag && rightFlag)
			dir = 1;

		else if (downFlag && leftFlag && !rightFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 0;
		}
		else if (downFlag && !leftFlag && rightFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 1;
		}
		else if (!downFlag && leftFlag && rightFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = 0;
			else dir = 1;
		}
		//all way
		else {

			randomTemp = rand() % 3;
			switch (randomTemp)
			{
			case 0:
				dir = dir;
				break;
			case 1:
				dir = 0;
				break;
			case 2:
				dir = 1;
				break;
			default:
				break;
			}
		}
		//}
		break;
	case 3:

		collisionDetection();
		srand(time(NULL) + 37494);

		if (upFlag && !leftFlag && !rightFlag)
			dir = dir;
		else if (!upFlag && leftFlag && !rightFlag)
			dir = 0;
		else if (!upFlag && !leftFlag && rightFlag)
			dir = 1;

		else if (upFlag && leftFlag && !rightFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 0;
		}
		else if (upFlag && !leftFlag && rightFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = dir;
			else dir = 1;
		}
		else if (!downFlag && leftFlag && rightFlag) {

			randomTemp = rand() % 2;
			if (randomTemp == 0) dir = 0;
			else dir = 1;
		}
		//all way
		else {

			randomTemp = rand() % 3;
			switch (randomTemp)
			{
			case 0:
				dir = dir;
				break;
			case 1:
				dir = 0;
				break;
			case 2:
				dir = 1;
				break;
			default:
				break;
			}
		}
		//}
		break;
	}
}

void AI::collisionDetection(void) {
	GLfloat tempX = (roundf(x * 10) / 10);
	GLfloat tempZ = (roundf(y * 10) / 10);
	GLint intX = (GLint)tempX;
	GLint intZ = (GLint)tempZ;

	if (tempX < 0) {
		if (tempX - (int)tempX != 0)
			intX -= 1;
	}
	if (tempZ < 0){
		if (tempZ - (int)tempZ != 0)
			intZ -= 1;
	}

	//side open Flag
	rightFlag = 0;
	leftFlag = 0;
	upFlag = 0;
	downFlag = 0;


	// LEFT SIDE open?
	if (dir != 1 && map_array[14 - intX][intZ + 15] != 1 && map_array[15 - intX][intZ + 15] != 1)
		leftFlag = 1;

	// RIGHT SIDE open?
	if (dir != 0 && map_array[14 - intX][intZ + 18] != 1 && map_array[15 - intX][intZ + 18] != 1)
		rightFlag = 1;

	// DOWN SIDE open?
	if (dir != 3 && map_array[16 - intX][intZ + 16] != 1 && map_array[16 - intX][intZ + 17] != 1)
		downFlag = 1;

	// UP SIDE open?
	if (dir != 2 && map_array[13 - intX][intZ + 16] != 1 && map_array[13 - intX][intZ + 17] != 1)
		upFlag = 1;

	// printf("left %d right %d up %d down %d", leftFlag, rightFlag, upFlag, downFlag);

}

int AI::catchPacman(GLfloat tx, GLfloat ty) {
	if (tx - x < 1.0 && tx - x > -1.0 && ty - y < 1.0 && ty - y > -1.0)
	{
		return 1;
	}
	return 0;
}


void AI::timer(void) {


	if (!isAlive) {
		if (deadTimer > 0) deadTimer--;
		else  isAlive = 1;
	}
	if (isEdible) {
		if (edibleTimer > 0) edibleTimer--;
		else isEdible = 0;
	}
}

void AI::setLocation(GLfloat tx, GLfloat ty) {
	x = tx;
	y = ty;
}

