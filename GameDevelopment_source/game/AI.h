#ifndef AI_H
#define AI_H_

#include <stdio.h>
#include <math.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <gl\glut.h>


class AI {
public:
	~AI();
	AI(GLfloat, GLfloat);

	GLfloat x, y;
	GLint intX, intY;
	// GLfloat tempX, tempY;
	// GLint intX, intY;
	GLfloat aiSpeed;
	GLfloat color[4];

	//timer
	int edibleTimer;
	int deadTimer;

	int isEdible;
	int isAlive;
	int randomTemp;
	int dir;
	int rightFlag;
	int leftFlag;
	int upFlag;
	int downFlag;


	void draw();
	void update();
	void move(void);
	void setLocation(GLfloat, GLfloat);

	void collisionDetection(void);
	int catchPacman(GLfloat, GLfloat);
	int isCorner(void);
	void timer(void);
};

#endif