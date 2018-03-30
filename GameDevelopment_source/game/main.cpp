#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <gl\glut.h>
#include <math.h>
#include "AI.h"
#include <string.h>
#include <stdlib.h>

#define aiNum 4 // AI ����
#define Distance 6 // ī�޶�� �Ѹ� �Ÿ� 
#define PI 3.1415926535 // ī�޶� ����� ���� pi��
#define CameraDegree 5 // ī�޶� ȸ���� �ε巯�� ���� �������� �ε巴���� ���� ���� 

#define ToRad(degree) ((degree) * (PI/180.0f)) // ī�޶� ȸ�� ����� ����

int window;		// ���� â
int window1;	// �� ����â(1��Ī)
int window2;	// �̴ϸ� â
int scoreWindow;// ���ھ� â


int Height = 4; // ī�޶� ���� 
int keybuffer = 0; // �Ѹǿ������� ���� ����
int life = 3; // ���
int camera_rotation = 0; // ī�޶� ��ȯ�� ���� ���� 
int count = 0; // ī�޶� ��ȯ�� ���� ���� 


GLfloat pacmanColor[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat pacX = -9.0; // �Ѹ��� ��ġ
GLfloat pacZ = 0.0; // �Ѹ��� ��ġ
GLfloat pacSpeed = 0.2; // �Ѹ� ������ �ӵ� 
GLfloat cameraX = 0.0, cameraZ = 0.0; // ī�޶� ��ȯ�� ���� ����
GLfloat camera_headX = 0.0, camera_headZ = 0.0; // ī�޶� ��ȯ�� ���� ����
GLfloat camera_angle = 0.0; // ī�޶� ��ȯ�� ���� ����. ���� ������ ���� 

//ai part
AI *ai[aiNum];

int font = (int)GLUT_BITMAP_9_BY_15;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);

	// ���� View(����������) - �׳� ���� ȭ��
	window = glutCreateWindow("Pacman Game");
	glutDisplayFunc(&renderScene);
	Init();

	glutReshapeFunc(&Resize);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutTimerFunc(50, DoKeyboard, 1);

	// ���� View(����������)
	window1 = glutCreateSubWindow(window, 128, 96, 896, 672);
	glutDisplayFunc(&RenderSub1);
	Init();

	// �̴ϸ� View(����������)
	window2 = glutCreateSubWindow(window, 0, 0, 128, 96);
	glutDisplayFunc(&RenderSub2);
	Init();

	// ���� View(����������)
	scoreWindow = glutCreateSubWindow(window, 128, 0, 256, 96);
	glutDisplayFunc(&RenderScore);
	Init();

	glutIdleFunc(&RenderAll);

	glutMainLoop();
	return (0);
}

void DoKeyboard(int x) { // �ε巯�� �������� ���� �����Լ� 
	glutTimerFunc(50, DoKeyboard, 1); // 50ms���� �ҷ��� 
	if (keybuffer == GLUT_KEY_UP)
		specialKeyPressed(keybuffer, 1, 1);
}

void Init(void)
{
	state = 0; // ���� ���� �� ȭ��
	Map(); // �� : �̷� �� ��ǥ ����

	GLfloat ambientLight[] = { 4.0, 1.0, 0.0, 1.0 };
	GLfloat diffuseLight[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LightPosition[] = { 0.0, 3.0, 2.0, 0.0 };

	//AI initialization
	ai[0] = new AI(13.0, -15.0);
	ai[1] = new AI(13.0, 13.0);
	ai[2] = new AI(-12.0, -15.0);
	ai[3] = new AI(-12.0, 13.0);


	//camera positon 
	cameraX = -Distance;
	cameraZ = 0;
	camera_headX = 1.0;
	camera_headZ = 0.0;
	camera_angle = 270.0;

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ��ü ����� ����������
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	glEnable(GL_LIGHTING); // ���� Ȱ��ȭ
	LoadGLTextures();
}
void Display(void)
{
	if (state == 0)
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		// ī�޷� ��ġ, ���� ����, �Ӹ� ����
		gluLookAt(0, 40, 0, 0, 0, 0, 1.0, 0.0, 0.0);
		glDisable(GL_LIGHTING);


		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-14, 1.0f, -17);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 1.0f, -17);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 1.0f, 15);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-14, 1.0f, 15);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glutSwapBuffers();
	}
	else if (state == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		// �̷θ� ����� �� �ݴ������� ��. ��Ż
		if (pacX < -14)
			pacX = 14;
		if (pacX > 15)
			pacX = -13;

		//// ī�޷� ��ġ, ���� ����, �Ӹ� ����
		//if (pacDirState == 1) // ���� ���� ����
		//	gluLookAt(pacX-Distance, Height, pacZ, pacX, 0, pacZ, 1.0, 0.0, 0.0);
		//else if (pacDirState == 2) // �Ʒ��� ���� ����
		//	gluLookAt(pacX + Distance, Height, pacZ, pacX, 0, pacZ, -1.0, 0.0, 0.0);
		//else if (pacDirState == 3) // �������� ���� ����
		//	gluLookAt(pacX, Height, pacZ + Distance, pacX, 0, pacZ, 0.0, 0.0, -1.0);
		//else// ���������� ���� ����
		//	gluLookAt(pacX, Height, pacZ - Distance, pacX, 0, pacZ, 0.0, 0.0, 1.0);
	
		//gluLookAt(pacX + Distance*sin(camera_angle), Height, pacZ + Distance*cos(camera_angle), pacX, 0, pacZ, camera_headX, 0.0, camera_headZ);

		//gluLookAt(cameraX, Height, cameraZ, pacX, 0, pacZ, camera_headX, 0.0, camera_headZ);

		switch (camera_rotation) // ī�޶�ȸ�� 
		{
		case 1: // �������� ȸ�� 
			if (count != 0) {
				cameraX = Distance*sin(ToRad(camera_angle + (CameraDegree + 1 - count) * 90.0/CameraDegree));
				cameraZ = Distance*cos(ToRad(camera_angle + (CameraDegree + 1 - count) * 90.0 / CameraDegree));
				camera_headX = cos(ToRad(camera_angle - 270 + (CameraDegree + 1 - count)*90.0 / CameraDegree));
				camera_headZ = -sin(ToRad(camera_angle - 270 + (CameraDegree + 1 - count)*90.0 / CameraDegree));
				// count�� ȸ���ε巯�� �����̰�, �ﰢ�Լ��� ī�޶� ȸ�� �߰��ܰ� ��� 
				count--;
			}
			else {
				camera_angle += 90.0; // ���� ����
				camera_rotation = 0; // ȸ���� �������Ƿ� 0���� �ٲپ��ش�
			}
			break;
		case 2: // ���������� ȸ��
			if (count != 0) {
				cameraX = Distance*sin(ToRad(camera_angle - (CameraDegree + 1 - count) *90.0 / CameraDegree));
				cameraZ = Distance*cos(ToRad(camera_angle - (CameraDegree + 1 - count) *90.0 / CameraDegree));
				camera_headX = cos(ToRad(camera_angle - 270 - (CameraDegree + 1 - count)*90.0 / CameraDegree));
				camera_headZ = -sin(ToRad(camera_angle - 270 - (CameraDegree + 1 - count)*90.0 / CameraDegree));
				// count�� ȸ���ε巯�� �����̰�, �ﰢ�Լ��� ī�޶� ȸ�� �߰��ܰ� ��� 
				count--;
			}
			else {
				camera_angle -= 90.0; // ���� ����
				camera_rotation = 0; // ȸ���� �������Ƿ� 0���� �ٲپ��ش�
			}
			break;
		case 3: // �ڷ� ����!
			if (count != 0) {
				cameraX = Distance*sin(ToRad(camera_angle - (CameraDegree * 2 + 1 - count) * 90.0 / CameraDegree));
				cameraZ = Distance*cos(ToRad(camera_angle - (CameraDegree * 2 + 1 - count) * 90.0 / CameraDegree));
				camera_headX = cos(ToRad(camera_angle - 270 - (CameraDegree * 2 + 1 - count)*90.0 / CameraDegree));
				camera_headZ = -sin(ToRad(camera_angle - 270 - (CameraDegree * 2 + 1 - count)*90.0 / CameraDegree));
				// count�� ȸ���ε巯�� �����̰�, �ﰢ�Լ��� ī�޶� ȸ�� �߰��ܰ� ��� 
				count--;
			} 
			else {
				camera_angle += 180.0; // ���� ����
				camera_rotation = 0; // ȸ���� �������Ƿ� 0���� �ٲپ��ش�
			}
			break;
		default:
			break;
		}

		gluLookAt(pacX + cameraX, Height, pacZ + cameraZ, pacX, 0, pacZ, camera_headX, 0.0, camera_headZ);
		// ��� ����� ����Ͽ� ī�޶� ��ġ ��� 

		glDisable(GL_LIGHTING);
		GLfloat ambient[] = {0.0, 0.0, 128.0, 0.0 }; // ������ �׸��� ���� ���� ������ ��(�ؽ���X)
		GLfloat diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
		GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat shininess[] = { 5.0 };
		GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);

		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glEnable(GL_TEXTURE_2D);
		int repeat = 1;
		/* �̷�(��)�� �ؽ��� ���� */
		while (repeat < 50)
		{
			glBegin(GL_QUADS);
			
			glTexCoord2f(0.0f, 0.0f);  glVertex3f(mapX1[repeat], 1.0f, mapZ1[repeat]);
			glTexCoord2f(1.0f, 0.0f);  glVertex3f(mapX2[repeat], 1.0f, mapZ2[repeat]);
			glTexCoord2f(1.0f, 1.0f);  glVertex3f(mapX2[repeat], -1.0f, mapZ2[repeat]);
			glTexCoord2f(0.0f, 1.0f);  glVertex3f(mapX1[repeat], -1.0f, mapZ1[repeat]);


			glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX3[repeat], 1.0f, mapZ3[repeat]);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX4[repeat], 1.0f, mapZ4[repeat]);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX4[repeat], -1.0f, mapZ4[repeat]);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX3[repeat], -1.0f, mapZ3[repeat]);


			glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX1[repeat], 1.0f, mapZ1[repeat]);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX1[repeat], 1.0f, mapZ4[repeat]);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX1[repeat], -1.0f, mapZ4[repeat]);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX1[repeat], -1.0f, mapZ1[repeat]);


			glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX2[repeat], 1.0f, mapZ2[repeat]);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX2[repeat], 1.0f, mapZ3[repeat]);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX2[repeat], -1.0f, mapZ3[repeat]);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX2[repeat], -1.0f, mapZ2[repeat]);


			glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX1[repeat], 1.0f, mapZ1[repeat]);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX2[repeat], 1.0f, mapZ2[repeat]);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX3[repeat], 1.0f, mapZ3[repeat]);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX4[repeat], 1.0f, mapZ4[repeat]);

			glEnd();
			repeat++;
		}

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);  glVertex3f(mapX1[0], 1.0f, mapZ1[0]);
		glTexCoord2f(0.0f, 1.0f);  glVertex3f(mapX2[0], 1.0f, mapZ2[0]);
		glTexCoord2f(0.0f, 0.0f);  glVertex3f(mapX2[0], -1.0f, mapZ2[0]);
		glTexCoord2f(1.0f, 0.0f);  glVertex3f(mapX1[0], -1.0f, mapZ1[0]);


		glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX3[0], 1.0f, mapZ3[0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX4[0], 1.0f, mapZ4[0]);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX4[0], -1.0f, mapZ4[0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX3[0], -1.0f, mapZ3[0]);


		glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX1[0], 1.0f, mapZ1[0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX1[0], 1.0f, mapZ4[0]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX1[0], -1.0f, mapZ4[0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX1[0], -1.0f, mapZ1[0]);


		glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX2[0], 1.0f, mapZ2[0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX2[0], 1.0f, mapZ3[0]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX2[0], -1.0f, mapZ3[0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX2[0], -1.0f, mapZ2[0]);


		glTexCoord2f(1.0f, 0.0f); glVertex3f(mapX1[0], 1.0f, mapZ1[0]);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(mapX2[0], 1.0f, mapZ2[0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(mapX3[0], 1.0f, mapZ3[0]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(mapX4[0], 1.0f, mapZ4[0]);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		
		// ���� �����ִ� food���� ���(ó�� ������ ���� 286����
		number_food = 0;
		for (int i = 0; i < 30; i++)
		for (int j = 0; j < 33; j++)
		{
			if (food_array[i][j] == 0)
				number_food++;
		}

		// �Ѹ��� food�� ������ food_array�� �ش��ϴ� ��ġ�� 1�� �ٲ㼭 food�׸� �� ������.
		for (int i = 0; i < 30; i++)
		for (int j = 0; j < 33; j++)
		{
			if (food_array[i][j] == 0)
			{

				if (((15 - i - 0.5 < pacX) && (15 - i + 0.5 > pacX)) && ((-17 + j - 0.5 < pacZ) && (-17 + j + 0.5 > pacZ))){
					food_array[i][j] = 1;
					// �׳� Ȯ���ϴ� �ڵ� ���߿� ������!!
					//printf("���� Food ���� : %d\n", 286 - (number_food - 1));
					number_draw_food = number_food - 1;
				}
			}
		}
		number_food = 0;
		// food �׸���
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 33; j++)
			{
				if (food_array[i][j] == 0)
				{
					ambient[0] = 0;
					ambient[1] = 128;
					ambient[2] = 128;
					glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
					glTranslatef(15 - i, 0, -17 + j);
					glutSolidSphere(0.2, 20, 20);
					glTranslatef(i - 15, 0, 17 - j);
				}

				//if there is special, ai become Edible mode  
				if (food_array[i][j] == 2) {

					//draw special food
					ambient[0] = 128;
					ambient[1] = 128;
					ambient[2] = 0;
					glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
					glTranslatef(15 - i, 0, -17 + j);
					glutSolidSphere(0.4, 20, 20);
					glTranslatef(i - 15, 0, 17 - j);

					if (((15 - i - 0.5 < pacX) && (15 - i + 0.5 > pacX)) && ((-17 + j - 0.5 < pacZ) && (-17 + j + 0.5 > pacZ))) {
						food_array[i][j] = 1;
						EdibleMode();
					}
				}
			}
		}

		// �Ѹ� �׸���////////////////////////////////////////////////////////////////////
		//glDisable(GL_LIGHTING);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, texture[2]);
		//
		ambient[0] = 128;
		ambient[1] = 128;
		ambient[2] = 0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glTranslatef(pacX, 0, pacZ);
		//glRotatef(90, pacX, 0, pacZ);
		
		glutSolidSphere(0.95, 20, 20);
		//gluSphere(pacImg, 0.95, 100, 100);
		//glRotatef(-90, -pacX, 0, -pacZ);
		
		glTranslatef(-pacX, 0, -pacZ);
		//
		glDisable(GL_TEXTURE_2D);
		

		//ai 
		// draw ai
		glEnable(GL_LIGHTING);
		for (int i = 0; i < aiNum; i++){
			if (ai[i]->isAlive)
				ai[i]->draw();

			//if pacman catch black ai, than disappear 
			if (ai[i]->catchPacman(pacX, pacZ)) {
				if (ai[i]->isEdible) {
					ai[i]->isAlive = 0;
					ai[i]->deadTimer = 400;
				}
				//if pacman collision with ai than life--, goto initial position
				else {
					life--;

					//set ai to init position
					ai[0]->setLocation(13.0, -15.0);
					ai[1]->setLocation(13.0, 13.0);
					ai[2]->setLocation(-12.0, -15.0);
					ai[3]->setLocation(-12.0, 13.0);

					//set pacman to init position
					pacX = -9.0;
					pacZ = 0.0;

					

				}
			}
			ai[i]->timer();
		}
		if (life <= 0)
			state = 2;
		if (number_draw_food == 0)
			state = 2;

		glutSwapBuffers();
	}
	else if (state == 2){ // ���ӿ��� �װų� ������ ��� Ŭ���� �ϸ�
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glDisable(GL_LIGHTING);


		glPushMatrix();
		gluLookAt(0.0f, 1.75f, 5.0f, 0.0f, 1.75f, 4.0f, 0.0f, 1.0f, 0.0f);
		char buffer[100];
		if (life > 0){ // ������ Ŭ���� �ϸ�
			renderBitmapCharacher(-1, 2.5, 0, (void*)font, "Congratulation!! You Are Win!!");
			renderBitmapCharacher(-1, 2.0, 0, (void*)font, "You Clear the PacMan Game!!!");

			_itoa(life, buffer, 10);
			strcat(buffer, " is Remains Of Your Life");
			renderBitmapCharacher(-1, 1.5, 0, (void*)font, buffer);

		}
		else{ // ������ Ŭ���� ���� ���ϸ�
			renderBitmapCharacher(-1, 2.5, 0, (void*)font, "That's Too Bad... You Are Lose...");
			renderBitmapCharacher(-1, 2.0, 0, (void*)font, "You Use All The Your Life...");

			_itoa(number_draw_food, buffer, 10);
			strcat(buffer, "'s Foods Are Remains...");
			renderBitmapCharacher(-1, 1.5, 0, (void*)font, buffer);
		}
		glPopMatrix();
		
		glutSwapBuffers();

	}
	else{
		exit(0);
	}
}
void SubDraw(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (state == 1){
		glLoadIdentity();

		// �̷θ� ����� �� �ݴ������� ��.
		if (pacX < -12.4)
			pacX = 12.4;
		if (pacX > 13.4)
			pacX = -11.4;

		// ī�޷� ��ġ, ���� ����, �Ӹ� ����
		gluLookAt(-20, 35, 0, 0, 0, 0, 1.0, 0.0, 0.0);

		glDisable(GL_LIGHTING);
		GLfloat ambient[] = { 0.0, 0.0, 128.0, 0.0 }; // ������ �׸��� ���� ���� ������ ��(�ؽ���X)
		GLfloat diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
		GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat shininess[] = { 5.0 };
		GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);

		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// ���� �����ִ� food���� ���(ó�� ������ ���� 286����
		number_food = 0;
		for (int i = 0; i < 30; i++)
		for (int j = 0; j < 33; j++)
		{
			if (food_array[i][j] == 0)
				number_food++;
		}

		// �Ѹ��� food�� ������ food_array�� �ش��ϴ� ��ġ�� 1�� �ٲ㼭 food�׸� �� ������.
		for (int i = 0; i < 30; i++)
		for (int j = 0; j < 33; j++)
		{
			if (food_array[i][j] == 0)
			{

				if (((15 - i - 0.5 < pacX) && (15 - i + 0.5 > pacX)) && ((-17 + j - 0.5 < pacZ) && (-17 + j + 0.5 > pacZ))){
					food_array[i][j] = 1;
				}
			}
		}
		number_food = 0;
		// food �׸���
		for (int i = 0; i < 30; i++)
		for (int j = 0; j < 33; j++)
		{
			if (food_array[i][j] == 0)
			{
				glTranslatef(15 - i, 0, -17 + j);
				glutSolidSphere(0.2, 20, 20);
				glTranslatef(i - 15, 0, 17 - j);
			}
		}

		// �Ѹ� �׸���////////////////////////////////////////////////////////////////////
		ambient[0] = 128;
		ambient[1] = 128;
		ambient[2] = 0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glTranslatef(pacX, 0, pacZ);
		glutSolidSphere(0.95, 20, 20);
		glTranslatef(-pacX, 0, -pacZ);
	}

	glutSwapBuffers();

}
void ScoreDraw(void){
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (state == 1){
		glPushMatrix();
		gluLookAt(0.0f, 1.75f, 5.0f, 0.0f, 1.75f, 4.0f, 0.0f, 1.0f, 0.0f);
		char buffer[100];
		
		renderBitmapCharacher(-4, 2.5, 0, (void*)font, "Pac Man Score!!!!");

		_itoa(life, buffer, 10);
		strcat(buffer, " is your Life");
		renderBitmapCharacher(-4, 0.5, 0, (void*)font, buffer);

		_itoa(number_draw_food, buffer, 10);
		strcat(buffer, "'s food remains...");
		renderBitmapCharacher(-4, 1.5, 0, (void*)font, buffer);

		glPopMatrix();
	}


	glutSwapBuffers();
}
void Resize(int width, int height)
{	
	// main â resize
	glViewport(0, 0, (GLsizei)(double)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (height == 0)
		height = 1;

	// ������ ���� �� ������ �����մϴ�.
	int w = width;
	int h = height;
	
	// ���� View ũ�� ����
	glutSetWindow(window1);
	glutPositionWindow(w / 8, h / 8);
	glutReshapeWindow((w / 8) * 7, (h / 8) * 7);
	setSize((w / 8) * 7, (h / 8) * 7);
	if (state == 2)
		gluLookAt(0.0f, 1.75f, 5.0f, 0.0f, 1.75f, 4.0f, 0.0f, 1.0f, 0.0f);

	// �̴ϸ� View ũ�� ����
	glutSetWindow(window2);
	glutPositionWindow(0, 0);
	glutReshapeWindow((w / 8), (h / 8));
	setSize((w / 8), (h / 8));

	// ���� View ũ�� ����
	glutSetWindow(scoreWindow);
	glutPositionWindow(w / 8, 0);
	glutReshapeWindow((w / 8) * 2, (h / 8));
	setSize((w / 8) * 2, (h / 8));
	gluLookAt(0.0f, 1.75f, 5.0f, 0.0f, 1.75f, 4.0f, 0.0f, 1.0f, 0.0f);
	
}
// ����â ũ�� ������ ���� �ΰ����� �Լ�(Resize �Լ� �ȿ��� ����)
void setSize(int w1, int h1)
{
	// �����찡 �ʹ� �۾����� ��
	// 0���� ������ ���� ���� �ڵ��Դϴ�.
	int ratio = 1.0f * w1/h1;

	// �����ϱ� ���� ��ǥ�踦 �ʱ�ȭ�մϴ�.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ����Ʈ�� ��ü ������ ũ��� �����մϴ�.
	glViewport(0, 0, (GLsizei)(double)w1, (GLsizei)h1);

	// ���� ������ �����մϴ�.
	gluPerspective(45, ratio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: // ESC��ư ������ ���� 
		exit(0);
		break;
	}
	if (state == 0)
	{
		switch (key){
		case 13: // EnterŰ�� ������ ���� ���� state�� �����Ѵ�.
			state = 1;
			break;

		}
		glutPostRedisplay();
	}
}
void specialKeyPressed(int key, int x, int y) // �Ѹ��� �̵��� ���� �Լ�. �ſ� �����ϴ�. ..
{
	if (camera_rotation != 0){ // ���� ī�޶� ȸ���ϰ������� Ű���� �Է� �������� 
		return;
	}
	keybuffer = key; // ���ۿ� key�� �־��ش� 
	
	switch (pacDirState) { // ���� �Ѹ��� ���⿡ ���� Ű���� �Է��� �����ϰ� �־��ش�. 
	case 1:
		break;
	case 2: // ������� ���� �Ʒ��� �����ִٸ� 
		switch (key) { // �Ʒ� �ڵ�ó�� ���ʹ��� �������� ������ ������ ������ó�� �ٲپ��ش� 
		case GLUT_KEY_LEFT:
			key = GLUT_KEY_RIGHT;
			break;
		case GLUT_KEY_RIGHT:
			key = GLUT_KEY_LEFT;
			break;
		case GLUT_KEY_DOWN:
			key = GLUT_KEY_UP;
			break;
		case GLUT_KEY_UP:
			key = GLUT_KEY_DOWN;
			break;
		default:
			break;
		}
		break;
	case 3: // ���ϵ���
		switch (key) {
		case GLUT_KEY_LEFT:
			key = GLUT_KEY_DOWN;
			break;
		case GLUT_KEY_RIGHT:
			key = GLUT_KEY_UP;
			break;
		case GLUT_KEY_DOWN:
			key = GLUT_KEY_RIGHT;
			break;
		case GLUT_KEY_UP:
			key = GLUT_KEY_LEFT;
			break;
		default:
			break;
		}
		break;
	case 4:
		switch (key) {
		case GLUT_KEY_LEFT:
			key = GLUT_KEY_UP;
			break;
		case GLUT_KEY_RIGHT:
			key = GLUT_KEY_DOWN;
			break;
		case GLUT_KEY_DOWN:
			key = GLUT_KEY_LEFT;
			break;
		case GLUT_KEY_UP:
			key = GLUT_KEY_RIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	switch (key) { // ������ �ٲپ��� ���� ����Ͽ� ������ �̵��ϴ°� ���� 
	case GLUT_KEY_LEFT: // �� ������� ���� ����Ű �Է� 
		if (roundf(pacX * 10) / 10 != (float)(int)(roundf(pacX * 10) / 10)){ // pacX�� �����ڸ��� �ƴϸ� �̵����� �ʾƾ��ϳ� �ε巯�� �������� ���ؼ� ���� ���ǹ� �߰�
			if (roundf((pacX + pacSpeed + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))){ // Ȥ�� ���� ��ĭ �̵��� �������� �̵��̰����ϴٸ� �����ϼ��ְ� �Ѵ� 
				pacX += pacSpeed;
				pacX += pacSpeed;
				if (CollisionDetection(0) == FALSE){ // ���� ��ĭ�̵��� �������� ���� �ε������ʰ� �̵��Ҽ� �����Ƿ� 
					pacX -= pacSpeed;  // ���� ��ĭ �÷��ش� 
					if (pacDirState == 2){ // �Ѹ��� ������ �ٲپ��ִµ� ������ �Ʒ������� �����־��ٸ� 
						pacDirState = 3; // �Ѹ��� ���� �������� �ٲپ��ְ� 
						count = CameraDegree; // ī�޶� ȸ�������� ī��Ʈ �÷��ְ� 
						camera_rotation = 2; // �Ʒ����� �������� ī�޶� ȸ���̹Ƿ� ������ ī�޶�ȸ���� ��Ÿ���� 2�� �־��ش� 
					}
					else if (pacDirState == 1) { //����������� ���ϵ���.
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 1;
					} 
					else if (pacDirState == 4) {
						pacDirState = 3;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX -= pacSpeed;
				pacX -= pacSpeed;
			}
			else if (roundf((pacX - pacSpeed - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))) { // ���� if���� ���������̳� �Ʒ��� ��ĭ�̵��ÿ������� �̵������Ұ�� 
				pacX -= pacSpeed;
				pacX -= pacSpeed;
				if (CollisionDetection(0) == FALSE){
					pacX += pacSpeed;
					if (pacDirState == 2){
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 1) {
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 3;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX += pacSpeed;
				pacX += pacSpeed;
			}
			else if (roundf((pacX + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))){ // ���� if����� ���������̳� ��ĭ�̵��� �̵����ɿ��� 
				pacX += pacSpeed;
				if (CollisionDetection(0) == FALSE){
					pacZ -= pacSpeed;
					if (pacDirState == 2){
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 1) {
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 3;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX -= pacSpeed;
			}
			else if (roundf((pacX - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))) { // ���ϵ���
				pacX -= pacSpeed;
				if (CollisionDetection(0) == FALSE){
					pacZ -= pacSpeed;
					if (pacDirState == 2){
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 1) {
						pacDirState = 3;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 3;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX += pacSpeed;
			} // ������������� ����� ��ġ ���� �������� �̵��Ҽ� �ִ� ��찡 �����Ƿ� �����Ѵ�. break
			break;
		}
		if (roundf(pacZ * 10) / 10 != (float)(int)(roundf(pacZ * 10) / 10)){ // pacZ�� �����ڸ��� �ƴϸ�, �� �̵��� ������ �ڸ��ϰ�� 
			pacZ -= pacSpeed;
			if (pacDirState == 2){ // ���� �������� ������� ī�޶� ȸ������
				pacDirState = 3;
				count = CameraDegree;
				camera_rotation = 2;
			}
			else if (pacDirState == 1) {
				pacDirState = 3;
				count = CameraDegree;
				camera_rotation = 1;
			}
			else if (pacDirState == 4) {
				pacDirState = 3;
				count = CameraDegree * 2;
				camera_rotation = 3;
			}
			break;
		} // ������� ������� pacX, pacZ ���� �����ڸ��̰�, �浹�ϴ��� Ȯ���ؾ��Ѵ� 
		if (CollisionDetection(0) == TRUE) // ������ �ϴ°��� ���� �ִٸ�
			break;
		pacZ -= pacSpeed;
		if (pacDirState == 2){ // ���������� ī�޶� ȸ�� ���� 
			pacDirState = 3;
			count = CameraDegree;
			camera_rotation = 2;
		}
		else if (pacDirState == 1) {
			pacDirState = 3;
			count = CameraDegree;
			camera_rotation = 1;
		}
		else if (pacDirState == 4) {
			pacDirState = 3;
			count = CameraDegree * 2;
			camera_rotation = 3;
		}
		break;
	case GLUT_KEY_RIGHT: // LEFT�� ���������� UP, DOWN ��� ����������� ó�� 
		if (roundf(pacX * 10) / 10 != (float)(int)(roundf(pacX * 10) / 10)){ // �����ڸ��� �ƴϸ�
			if (roundf((pacX + pacSpeed + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))){
				pacX += pacSpeed;
				pacX += pacSpeed;
				if (CollisionDetection(1) == FALSE){
					pacX -= pacSpeed;
					if (pacDirState == 2){
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 1) {
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 3) {
						pacDirState = 4;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX -= pacSpeed;
				pacX -= pacSpeed;
			}
			else if (roundf((pacX - pacSpeed - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))) {
				pacX -= pacSpeed;
				pacX -= pacSpeed;
				if (CollisionDetection(1) == FALSE){
					pacX += pacSpeed;
					if (pacDirState == 2){
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 1) {
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 3) {
						pacDirState = 4;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX += pacSpeed;
				pacX += pacSpeed;
			}
			else if (roundf((pacX + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))){
				pacX += pacSpeed;
				if (CollisionDetection(1) == FALSE){
					pacZ += pacSpeed;
					if (pacDirState == 2){
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 1) {
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 3) {
						pacDirState = 4;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX -= pacSpeed;
			}
			else if (roundf((pacX - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacX))) {
				pacX -= pacSpeed;
				if (CollisionDetection(1) == FALSE){
					pacZ += pacSpeed;
					if (pacDirState == 2){
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 1) {
						pacDirState = 4;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 3) {
						pacDirState = 4;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					break;
				}
				pacX += pacSpeed;
			}
			break;
		}
		if (roundf(pacZ * 10) / 10 != (float)(int)(roundf(pacZ * 10) / 10)){ // �����ڸ��� �ƴϸ�
			pacZ += pacSpeed;
			if (pacDirState == 2){
				pacDirState = 4;
				count = CameraDegree;
				camera_rotation = 1;
			}
			else if (pacDirState == 1) {
				pacDirState = 4;
				count = CameraDegree;
				camera_rotation = 2;
			}
			else if (pacDirState == 3) {
				pacDirState = 4;
				count = CameraDegree * 2;
				camera_rotation = 3;
			}
			break;
		}
		if (CollisionDetection(1) == TRUE) // ������ �ϴ°��� ���� �ִٸ�
			break;
		pacZ += pacSpeed;
		if (pacDirState == 2){
			pacDirState = 4;
			count = CameraDegree;
			camera_rotation = 1;
		}
		else if (pacDirState == 1) {
			pacDirState = 4;
			count = CameraDegree;
			camera_rotation = 2;
		}
		else if (pacDirState == 3) {
			pacDirState = 4;
			count = CameraDegree * 2;
			camera_rotation = 3;
		}
		break;
	case GLUT_KEY_DOWN:
		if (roundf(pacZ * 10) / 10 != (float)(int)(roundf(pacZ * 10) / 10)){ // �����ڸ��� �ƴϸ�
			if (roundf((pacZ + pacSpeed + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))){
				pacZ += pacSpeed;
				pacZ += pacSpeed;
				if (CollisionDetection(2) == FALSE){
					pacZ -= pacSpeed;
					if (pacDirState == 1) {
						pacDirState = 2;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 2;
					}
					break;
				}
				pacZ -= pacSpeed;
				pacZ -= pacSpeed;
			}
			else if (roundf((pacZ - pacSpeed - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))) {
				pacZ -= pacSpeed;
				pacZ -= pacSpeed;
				if (CollisionDetection(2) == FALSE){
					pacZ += pacSpeed;
					if (pacDirState == 1) {
						pacDirState = 2;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 2;
					}
					break;
				}
				pacZ += pacSpeed;
				pacZ += pacSpeed;
			}
			else if (roundf((pacZ + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))){
				pacZ += pacSpeed;
				if (CollisionDetection(2) == FALSE){
					pacX -= pacSpeed;
					if (pacDirState == 1) {
						pacDirState = 2;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 2;
					}
					break;
				}
				pacZ -= pacSpeed;
			}
			else if (roundf((pacZ - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))) {
				pacZ -= pacSpeed;
				if (CollisionDetection(2) == FALSE){
					pacX -= pacSpeed;
					if (pacDirState == 1) {
						pacDirState = 2;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 1;
					}
					else if (pacDirState == 4) {
						pacDirState = 2;
						count = CameraDegree;
						camera_rotation = 2;
					}
					break;
				}
				pacZ -= pacSpeed;
			}
			break;
		}
		if (roundf(pacX * 10) / 10 != (float)(int)(roundf(pacX * 10) / 10)){ // �����ڸ��� �ƴϸ�
			pacX -= pacSpeed;
			if (pacDirState == 1) {
				pacDirState = 2;
				count = CameraDegree * 2;
				camera_rotation = 3;
			}
			else if (pacDirState == 3) {
				pacDirState = 2;
				count = CameraDegree;
				camera_rotation = 1;
			}
			else if (pacDirState == 4) {
				pacDirState = 2;
				count = CameraDegree;
				camera_rotation = 2;
			}
			break;
		}
		if (CollisionDetection(2) == TRUE) // ������ �ϴ°��� ���� �ִٸ�
			break;
		pacX -= pacSpeed;
		if (pacDirState == 1) {
			pacDirState = 2;
			count = CameraDegree * 2;
			camera_rotation = 3;
		}
		else if (pacDirState == 3) {
			pacDirState = 2;
			count = CameraDegree;
			camera_rotation = 1;
		}
		else if (pacDirState == 4) {
			pacDirState = 2;
			count = CameraDegree;
			camera_rotation = 2;
		}
		break;
	case GLUT_KEY_UP:
		if (roundf(pacZ * 10) / 10 != (float)(int)(roundf(pacZ * 10) / 10)){ // �����ڸ��� �ƴϸ�
			if (roundf((pacZ + pacSpeed + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))){
				pacZ += pacSpeed;
				pacZ += pacSpeed;
				if (CollisionDetection(3) == FALSE){
					pacZ -= pacSpeed;
					if (pacDirState == 2) {
						pacDirState = 1;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 4) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 1;
					}
					break;
				}
				pacZ -= pacSpeed;
				pacZ -= pacSpeed;
			}
			else if (roundf((pacZ - pacSpeed - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))) {
				pacZ -= pacSpeed;
				pacZ -= pacSpeed;
				if (CollisionDetection(3) == FALSE){
					pacZ += pacSpeed;
					if (pacDirState == 2) {
						pacDirState = 1;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 4) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 1;
					}
					break;
				}
				pacZ += pacSpeed;
				pacZ += pacSpeed;
			}
			else if (roundf((pacZ + pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))){
				pacZ += pacSpeed;
				if (CollisionDetection(3) == FALSE){
					pacX += pacSpeed;
					if (pacDirState == 2) {
						pacDirState = 1;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 4) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 1;
					}
					break;
				}
				pacZ -= pacSpeed;
			}
			else if (roundf((pacZ - pacSpeed) * 10) / 10 == (float)(int)(roundf(pacZ))) {
				pacZ -= pacSpeed;
				if (CollisionDetection(3) == FALSE){
					pacX += pacSpeed;
					if (pacDirState == 2) {
						pacDirState = 1;
						count = CameraDegree * 2;
						camera_rotation = 3;
					}
					else if (pacDirState == 3) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 2;
					}
					else if (pacDirState == 4) {
						pacDirState = 1;
						count = CameraDegree;
						camera_rotation = 1;
					}
					break;
				}
				pacZ -= pacSpeed;
			}
			break;
		}
		if (roundf(pacX * 10) / 10 != (float)(int)(roundf(pacX * 10) / 10)){ // �����ڸ��� �ƴϸ�
			pacX += pacSpeed;
			if (pacDirState == 2) {
				pacDirState = 1;
				count = CameraDegree * 2;
				camera_rotation = 3;
			}
			else if (pacDirState == 3) {
				pacDirState = 1;
				count = CameraDegree;
				camera_rotation = 2;
			}
			else if (pacDirState == 4) {
				pacDirState = 1;
				count = CameraDegree;
				camera_rotation = 1;
			}
			break;
		}
		if (CollisionDetection(3) == TRUE) // ������ �ϴ°��� ���� �ִٸ�
			break;
		pacX += pacSpeed;
		if (pacDirState == 2) {
			pacDirState = 1;
			count = CameraDegree * 2;
			camera_rotation = 3;
		}
		else if (pacDirState == 3) {
			pacDirState = 1;
			count = CameraDegree;
			camera_rotation = 2;
		}
		else if (pacDirState == 4) {
			pacDirState = 1;
			count = CameraDegree;
			camera_rotation = 1;
		}
		break;
	}
	glutPostRedisplay(); // �ٽ� display ���ش�. 
}
void mouse(int button, int state, int x, int y)
{

}
void Map(void){
	/* �߽� */
	mapX1[0] = -3, mapZ1[0] = -2;
	mapX2[0] = 4, mapZ2[0] = -2;
	mapX3[0] = 4, mapZ3[0] = 2;
	mapX4[0] = -3, mapZ4[0] = 2;

	/* �̷� */
	mapX1[1] = -11, mapZ1[1] = -14;
	mapX2[1] = -2, mapZ2[1] = -14;
	mapX3[1] = -2, mapZ3[1] = -13;
	mapX4[1] = -11, mapZ4[1] = -13;

	mapX1[2] = 0, mapZ1[2] = -14;
	mapX2[2] = 1, mapZ2[2] = -14;
	mapX3[2] = 1, mapZ3[2] = -11;
	mapX4[2] = 0, mapZ4[2] = -11;


	mapX1[3] = 3, mapZ1[3] = -14;
	mapX2[3] = 12, mapZ2[3] = -14;
	mapX3[3] = 12, mapZ3[3] = -13;
	mapX4[3] = 3, mapZ4[3] = -13;


	mapX1[4] = -6, mapZ1[4] = -13;
	mapX2[4] = -5, mapZ2[4] = -13;
	mapX3[4] = -5, mapZ3[4] = -10;
	mapX4[4] = -6, mapZ4[4] = -10;


	mapX1[5] = 6, mapZ1[5] = -13;
	mapX2[5] = 7, mapZ2[5] = -13;
	mapX3[5] = 7, mapZ3[5] = -10;
	mapX4[5] = 6, mapZ4[5] = -10;


	mapX1[6] = -13, mapZ1[6] = -11;
	mapX2[6] = -11, mapZ2[6] = -11;
	mapX3[6] = -11, mapZ3[6] = -10;
	mapX4[6] = -13, mapZ4[6] = -10;


	mapX1[7] = -3, mapZ1[7] = -11;
	mapX2[7] = 4, mapZ2[7] = -11;
	mapX3[7] = 4, mapZ3[7] = -10;
	mapX4[7] = -3, mapZ4[7] = -10;


	mapX1[8] = 12, mapZ1[8] = -11;
	mapX2[8] = 14, mapZ2[8] = -11;
	mapX3[8] = 14, mapZ3[8] = -10;
	mapX4[8] = 12, mapZ4[8] = -10;


	mapX1[9] = -9, mapZ1[9] = -11;
	mapX2[9] = -8, mapZ2[9] = -11;
	mapX3[9] = -8, mapZ3[9] = -8;
	mapX4[9] = -9, mapZ4[9] = -8;


	mapX1[10] = 9, mapZ1[10] = -11;
	mapX2[10] = 10, mapZ2[10] = -11;
	mapX3[10] = 10, mapZ3[10] = -8;
	mapX4[10] = 9, mapZ4[10] = -8;


	mapX1[11] = -11, mapZ1[11] = -8;
	mapX2[11] = -8, mapZ2[11] = -8;
	mapX3[11] = -8, mapZ3[11] = -7;
	mapX4[11] = -11, mapZ4[11] = -7;


	mapX1[12] = -6, mapZ1[12] = -8;
	mapX2[12] = -2, mapZ2[12] = -8;
	mapX3[12] = -2, mapZ3[12] = -7;
	mapX4[12] = -6, mapZ4[12] = -7;


	mapX1[13] = 0, mapZ1[13] = -8;
	mapX2[13] = 1, mapZ2[13] = -8;
	mapX3[13] = 1, mapZ3[13] = -5;
	mapX4[13] = 0, mapZ4[13] = -5;


	mapX1[14] = 3, mapZ1[14] = -8;
	mapX2[14] = 7, mapZ2[14] = -8;
	mapX3[14] = 7, mapZ3[14] = -7;
	mapX4[14] = 3, mapZ4[14] = -7;


	mapX1[15] = 9, mapZ1[15] = -8;
	mapX2[15] = 12, mapZ2[15] = -8;
	mapX3[15] = 12, mapZ3[15] = -7;
	mapX4[15] = 9, mapZ4[15] = -7;


	mapX1[16] = -13, mapZ1[16] = -5;
	mapX2[16] = -8, mapZ2[16] = -5;
	mapX3[16] = -8, mapZ3[16] = -4;
	mapX4[16] = -13, mapZ4[16] = -4;


	mapX1[17] = -3, mapZ1[17] = -5;
	mapX2[17] = 4, mapZ2[17] = -5;
	mapX3[17] = 4, mapZ3[17] = -4;
	mapX4[17] = -3, mapZ4[17] = -4;


	mapX1[18] = 9, mapZ1[18] = -5;
	mapX2[18] = 14, mapZ2[18] = -5;
	mapX3[18] = 14, mapZ3[18] = -4;
	mapX4[18] = 9, mapZ4[18] = -4;


	mapX1[19] = -9, mapZ1[19] = -4;
	mapX2[19] = -8, mapZ2[19] = -4;
	mapX3[19] = -8, mapZ3[19] = -2;
	mapX4[19] = -9, mapZ4[19] = -2;


	mapX1[20] = -6, mapZ1[20] = -5;
	mapX2[20] = -5, mapZ2[20] = -5;
	mapX3[20] = -5, mapZ3[20] = -1;
	mapX4[20] = -6, mapZ4[20] = -1;


	mapX1[21] = 6, mapZ1[21] = -5;
	mapX2[21] = 7, mapZ2[21] = -5;
	mapX3[21] = 7, mapZ3[21] = -1;
	mapX4[21] = 6, mapZ4[21] = -1;


	mapX1[22] = 9, mapZ1[22] = -4;
	mapX2[22] = 10, mapZ2[22] = -4;
	mapX3[22] = 10, mapZ3[22] = -2;
	mapX4[22] = 9, mapZ4[22] = -2;


	mapX1[23] = -13, mapZ1[23] = -2;
	mapX2[23] = -8, mapZ2[23] = -2;
	mapX3[23] = -8, mapZ3[23] = -1;
	mapX4[23] = -13, mapZ4[23] = -1;


	mapX1[24] = 9, mapZ1[24] = -2;
	mapX2[24] = 14, mapZ2[24] = -2;
	mapX3[24] = 14, mapZ3[24] = -1;
	mapX4[24] = 9, mapZ4[24] = -1;


	mapX1[25] = -13, mapZ1[25] = 1;
	mapX2[25] = -8, mapZ2[25] = 1;
	mapX3[25] = -8, mapZ3[25] = 2;
	mapX4[25] = -13, mapZ4[25] = 2;


	mapX1[26] = 9, mapZ1[26] = 1;
	mapX2[26] = 14, mapZ2[26] = 1;
	mapX3[26] = 14, mapZ3[26] = 2;
	mapX4[26] = 9, mapZ4[26] = 2;


	mapX1[27] = -9, mapZ1[27] = 2;
	mapX2[27] = -8, mapZ2[27] = 2;
	mapX3[27] = -8, mapZ3[27] = 4;
	mapX4[27] = -9, mapZ4[27] = 4;


	mapX1[28] = -6, mapZ1[28] = 1;
	mapX2[28] = -5, mapZ2[28] = 1;
	mapX3[28] = -5, mapZ3[28] = 8;
	mapX4[28] = -6, mapZ4[28] = 8;


	mapX1[29] = 6, mapZ1[29] = 1;
	mapX2[29] = 7, mapZ2[29] = 1;
	mapX3[29] = 7, mapZ3[29] = 8;
	mapX4[29] = 6, mapZ4[29] = 8;


	mapX1[30] = 9, mapZ1[30] = 2;
	mapX2[30] = 10, mapZ2[30] = 2;
	mapX3[30] = 10, mapZ3[30] = 4;
	mapX4[30] = 9, mapZ4[30] = 4;


	mapX1[31] = -13, mapZ1[31] = 4;
	mapX2[31] = -8, mapZ2[31] = 4;
	mapX3[31] = -8, mapZ3[31] = 5;
	mapX4[31] = -13, mapZ4[31] = 5;


	mapX1[32] = -5, mapZ1[32] = 4;
	mapX2[32] = -2, mapZ2[32] = 4;
	mapX3[32] = -2, mapZ3[32] = 5;
	mapX4[32] = -5, mapZ4[32] = 5;


	mapX1[33] = 0, mapZ1[33] = 4;
	mapX2[33] = 1, mapZ2[33] = 4;
	mapX3[33] = 1, mapZ3[33] = 7;
	mapX4[33] = 0, mapZ4[33] = 7;


	mapX1[34] = 3, mapZ1[34] = 4;
	mapX2[34] = 6, mapZ2[34] = 4;
	mapX3[34] = 6, mapZ3[34] = 5;
	mapX4[34] = 3, mapZ4[34] = 5;


	mapX1[35] = 9, mapZ1[35] = 4;
	mapX2[35] = 14, mapZ2[35] = 4;
	mapX3[35] = 14, mapZ3[35] = 5;
	mapX4[35] = 9, mapZ4[35] = 5;


	mapX1[36] = -11, mapZ1[36] = 7;
	mapX2[36] = -8, mapZ2[36] = 7;
	mapX3[36] = -8, mapZ3[36] = 8;
	mapX4[36] = -11, mapZ4[36] = 8;


	mapX1[37] = -3, mapZ1[37] = 7;
	mapX2[37] = 4, mapZ2[37] = 7;
	mapX3[37] = 4, mapZ3[37] = 8;
	mapX4[37] = -3, mapZ4[37] = 8;


	mapX1[38] = 9, mapZ1[38] = 7;
	mapX2[38] = 12, mapZ2[38] = 7;
	mapX3[38] = 12, mapZ3[38] = 8;
	mapX4[38] = 9, mapZ4[38] = 8;


	mapX1[39] = -11, mapZ1[39] = 10;
	mapX2[39] = -8, mapZ2[39] = 10;
	mapX3[39] = -8, mapZ3[39] = 12;
	mapX4[39] = -11, mapZ4[39] = 12;


	mapX1[40] = -6, mapZ1[40] = 10;
	mapX2[40] = -2, mapZ2[40] = 10;
	mapX3[40] = -2, mapZ3[40] = 12;
	mapX4[40] = -6, mapZ4[40] = 12;


	mapX1[41] = 0, mapZ1[41] = 10;
	mapX2[41] = 1, mapZ2[41] = 10;
	mapX3[41] = 1, mapZ3[41] = 14;
	mapX4[41] = 0, mapZ4[41] = 14;


	mapX1[42] = 3, mapZ1[42] = 10;
	mapX2[42] = 7, mapZ2[42] = 10;
	mapX3[42] = 7, mapZ3[42] = 12;
	mapX4[42] = 3, mapZ4[42] = 12;


	mapX1[43] = 9, mapZ1[43] = 10;
	mapX2[43] = 12, mapZ2[43] = 10;
	mapX3[43] = 12, mapZ3[43] = 12;
	mapX4[43] = 9, mapZ4[43] = 12;


	/* ���� �𼭸� */
	mapX1[44] = -14, mapZ1[44] = -17;
	mapX2[44] = 15, mapZ2[44] = -17;
	mapX3[44] = 15, mapZ3[44] = -16;
	mapX4[44] = -14, mapZ4[44] = -16;

	/* ���� �𼭸�(��, ��)*/
	mapX1[45] = 14, mapZ1[45] = -16;
	mapX2[45] = 15, mapZ2[45] = -16;
	mapX3[45] = 15, mapZ3[45] = -1;
	mapX4[45] = 14, mapZ4[45] = -1;


	mapX1[46] = 14, mapZ1[46] = 1;
	mapX2[46] = 15, mapZ2[46] = 1;
	mapX3[46] = 15, mapZ3[46] = 14;
	mapX4[46] = 14, mapZ4[46] = 14;


	/* ������ �����ڸ�*/
	mapX1[47] = -14, mapZ1[47] = 14;
	mapX2[47] = 15, mapZ2[47] = 14;
	mapX3[47] = 15, mapZ3[47] = 15;
	mapX4[47] = -14, mapZ4[47] = 15;

	/* �Ʒ��� �����ڸ�(��, ��)*/
	mapX1[48] = -14, mapZ1[48] = -16;
	mapX2[48] = -13, mapZ2[48] = -16;
	mapX3[48] = -13, mapZ3[48] = -1;
	mapX4[48] = -14, mapZ4[48] = -1;


	mapX1[49] = -14, mapZ1[49] = 1;
	mapX2[49] = -13, mapZ2[49] = 1;
	mapX3[49] = -13, mapZ3[49] = 14;
	mapX4[49] = -14, mapZ4[49] = 14;
}

// �ؽ��� ���� ���� �Լ�
AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle
	if (!Filename)										// Make Sure A Filename Was Given
		return NULL;									// If Not Return NULL
	
	File = fopen(Filename, "r");						// Check To See If The File Exists
	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
	return NULL;										// If Load Failed Return NULL
}
int LoadGLTextures(){
	pacImg = gluNewQuadric();
	gluQuadricDrawStyle(pacImg, GLU_FILL);
	gluQuadricTexture(pacImg, GL_TRUE);

	AUX_RGBImageRec *TextureImage[3];					// Create Storage Space For The Texture
	int Status = FALSE;									// Status Indicator
	memset(TextureImage, 0, sizeof(void *)* 1);           	// Set The Pointer To NULL
	glGenTextures(3, &texture[0]);


	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP("wall3.bmp"))
	{
		Status = TRUE;									// Set The Status To TRUE
		//glGenTextures(1, &texture[0]);					// Create Three Textures 2���� 1�� �ٲ�.

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

	}
	if (TextureImage[1] = LoadBMP("start_512.bmp"))
	{
		Status = TRUE;
		//glGenTextures(1, &texture[1]);					// Create Three Textures

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
	}
	if (TextureImage[2] = LoadBMP("oh3.bmp"))
	{
		Status = TRUE;

		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
	}
	
	for (int i = 0; i < 2; i++){
		if (TextureImage[i])								// If Texture Exists
		{
			if (TextureImage[i]->data)						// If Texture Image Exists
			{
				free(TextureImage[i]->data);				// Free The Texture Image Memory
			}

			free(TextureImage[i]);							// Free The Image Structure
		}
	}

	return Status;										// Return The Status
}

boolean CollisionDetection(int dir) {
	GLfloat tempX = (roundf(pacX * 10) / 10);
	GLfloat tempZ = (roundf(pacZ * 10) / 10);
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
	switch (dir) {
	case 0: // LEFT SIDE
		if (collision_array[14 - intX][intZ + 15] == 1 || collision_array[15 - intX][intZ + 15] == 1)
			return TRUE;
		return FALSE;
		break;
	case 1: // RIGHT SIDE
		if (collision_array[14 - intX][intZ + 18] == 1 || collision_array[15 - intX][intZ + 18] == 1)
			return TRUE;
		return FALSE;
		break;
	case 2: // DOWN SIDE
		if (collision_array[16 - intX][intZ + 16] == 1 || collision_array[16 - intX][intZ + 17] == 1)
			return TRUE;
		return FALSE;
		break;
	case 3: // UP SIDE
		if (collision_array[13 - intX][intZ + 16] == 1 || collision_array[13 - intX][intZ + 17] == 1)
			return TRUE;
		return FALSE;
		break;
	default: return FALSE;
		break;
	}
}

void renderScene(){
	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}
void RenderSub1(void){
	glutSetWindow(window1);
	glLoadIdentity();
	Display();
}
void RenderSub2(void){
	glutSetWindow(window2);
	glLoadIdentity();
	SubDraw();
}
void RenderScore(void){
	glutSetWindow(scoreWindow);
	glLoadIdentity();
	ScoreDraw();
}

void RenderAll(void){
	RenderSub1();
	RenderSub2();
	RenderScore();
}


//if pac eat banana than become Edible Mode
void EdibleMode(void) {
	for (int i = 0; i < aiNum; i++) {
		ai[i]->isEdible = 1;
		ai[i]->edibleTimer = 400;
	}
}

void renderBitmapCharacher(float x, float y, float z, void *font, char *string)
{

	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);

	}
}