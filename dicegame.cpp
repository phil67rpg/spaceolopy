#include <iostream>
#include <SOIL.h>
#include <math.h>
#include <freeglut.h>
#include <time.h>
#include <vector>

using namespace std;
float move_x = 0.0f;
float bullet = 0.0f;
int coll = -1;
int coll_count = 0;
int flag = 0;
int AnimationState = 0;
int AnimationCount = 0;

GLuint texture[16];
int dice[2];

class Collision
{
public:
	float coll_x;
	float coll_y;
	float collWidth;
	float collHeight;
};

vector <Collision> colls;

bool checkCollide(float x, float y, float oWidth, float oHeight, float xTwo, float yTwo, float oTwoWidth, float oTwoHeight)
{
	// AABB 1
	float x1Min = x;
	float x1Max = x + oWidth;
	float y1Max = y + oHeight;
	float y1Min = y;

	// AABB 2
	float x2Min = xTwo;
	float x2Max = xTwo + oTwoWidth;
	float y2Max = yTwo + oTwoHeight;
	float y2Min = yTwo;

	// Collision tests
	if (x1Max < x2Min || x1Min > x2Max) return false;
	if (y1Max < y2Min || y1Min > y2Max) return false;

	return true;
}

void drawDie(float xPOS, float yPOS, int dieValue)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[--dieValue]);

	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(xPOS, yPOS, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(xPOS + 10.0f, yPOS, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(xPOS + 10.0f, yPOS + 10.0f, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(xPOS, yPOS + 10.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

int getRandomDieValue()
{
	return rand() % 6 + 1;
}

void rollDice()
{
	for (int cnt = 0; cnt < 2; ++cnt)
	{
		dice[cnt] = getRandomDieValue();
	}
}

void drawDice()
{
	drawDie(95.0f, -5.0f, dice[0]);
	drawDie(115.0f, -5.0f, dice[1]);
}

void drawBug(float POSx, float POSy, int bugType, int bugFrame)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[bugType]);
	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(POSx, POSy, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(POSx + 10.0f, POSy, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(POSx + 10.0f, POSy + 10.0f, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(POSx, POSy + 10.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void ship()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-10.0f + move_x, -100.0f, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(10.0f + move_x, -100.0f, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(10.0f + move_x, -90.0f, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-10.0f + move_x, -90.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawBullet()
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(-2.5f + move_x, -90.0f + bullet, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(2.5f + move_x, -90.0f + bullet, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(2.5f + move_x, -85.0f + bullet, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-2.5f + move_x, -85.0f + bullet, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void go_space()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[10]);

	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(80.0f, 80.0f, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(130.0f, 80.0f, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(130.0f, 30.0f, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(80.0f, 30.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void electric_space()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[11]);

	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(80.0f, 80.0f, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(130.0f, 80.0f, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(130.0f, 30.0f, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(80.0f, 30.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawCollision(float POS_x, float POS_y, int collType, int collFrame)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[collType]);
	glBegin(GL_POLYGON);
	glTexCoord3f(0.0f, 0.0f, 0.0f);

	glVertex3f(POS_x, POS_y, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);

	glVertex3f(POS_x + 10.0f, POS_y, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);

	glVertex3f(POS_x + 10.0f, POS_y + 10.0f, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);

	glVertex3f(POS_x, POS_y + 10.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void timer(int val)
{
	int TransitionLimit = 5000;
	AnimationCount++;
	if (AnimationCount >= TransitionLimit * 2)
		AnimationCount = 0;
	if (AnimationCount < TransitionLimit)
	for (float i = -90.0f; i <= 60.0f; i += 20.0f)
	{
	drawBug(i, 90.0f, 12, 0);
	drawBug(i, 70.0f, 12, 0);
	drawBug(i, 50.0f, 12, 0);
	drawBug(i, 30.0f, 12, 0);
	drawBug(i, 10.0f, 12, 0);
	}
	else
	for (float i = -90.0f; i <= 60.0f; i += 20.0f)
	{
	drawBug(i, 90.0f, 8, 0);
	drawBug(i, 70.0f, 8, 0);
	drawBug(i, 50.0f, 8, 0);
	drawBug(i, 30.0f, 8, 0);
	drawBug(i, 10.0f, 8, 0);
	}
	glutPostRedisplay();
	glutTimerFunc(10000, timer, 0);
}

void coll_ship_one()
{
	//draw bullet
	float x = -10.0f + move_x;
	float y = -90.0f + bullet;
	float oWidth = 5.0f;
	float oHeight = 5.0f;
	//draw bug
	float xTwo = -10.0f;
	float yTwo = 90.0f;
	float oTwoWidth = 10.0f;
	float oTwoHeight = 10.0f;

	if (checkCollide(x, y, oWidth, oHeight, xTwo, yTwo, oTwoWidth, oTwoHeight) == 1)
	{
		coll = 1;
		coll_count++;
		if (coll_count >= 1)
		{
			coll_count = 1;
		}
		cout << coll << " " << coll_count << endl;
		glutPostRedisplay();
	}
}

void renderScene()
{
	srand(time(NULL));
	glClear(GL_COLOR_BUFFER_BIT);
	drawBullet();
	coll_ship_one();
	go_space();
	if (coll == -1)
	{
		timer(0);
	}
	if (coll == 1 && coll_count == 1)
	{
		drawCollision(-10.0f, 90.0f, 9, 0);
		coll = 0;
	}
	if (coll == 0 && coll_count >= 1)
	{
		// no bug
	}
	ship();
	drawDice();
	glFinish();
}

GLuint loadTex(const char *texname)
{
	/* load an image file directly as a new OpenGL texture */
	GLuint texture = SOIL_load_OGL_texture
	(
		texname,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
	);
	return texture;
}

void init()
{
	texture[0] = loadTex("C:\\Users\\Owner\\Desktop\\dieRed1.png");
	texture[1] = loadTex("C:\\Users\\Owner\\Desktop\\dieRed2.png");
	texture[2] = loadTex("C:\\Users\\Owner\\Desktop\\dieRed3.png");
	texture[3] = loadTex("C:\\Users\\Owner\\Desktop\\dieRed4.png");
	texture[4] = loadTex("C:\\Users\\Owner\\Desktop\\dieRed5.png");
	texture[5] = loadTex("C:\\Users\\Owner\\Desktop\\dieRed6.png");

	texture[6] = loadTex("C:\\Users\\Owner\\Desktop\\ship.png");
	texture[7] = loadTex("C:\\Users\\Owner\\Desktop\\bullet.png");
	texture[8] = loadTex("C:\\Users\\Owner\\Desktop\\bug.png");
	texture[9] = loadTex("C:\\Users\\Owner\\Desktop\\collision.png");
	texture[10] = loadTex("C:\\Users\\Owner\\Desktop\\go.png");
	texture[11] = loadTex("C:\\Users\\Owner\\Desktop\\electric.png");
	texture[12] = loadTex("C:\\Users\\Owner\\Desktop\\bugone.png");
}

void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-100.0, 105.0, -100.0 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0 * aspectRatio, 105.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void shoot()
{
	bullet += 0.07f;
	if (bullet >= 200.0f)
	{
		bullet = 0.0f;
		glutIdleFunc(NULL);
	}
	glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x, int y)
{
	int animate = 0;

	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 32:
		rollDice();
		break;
	case 's':
		animate = !animate;
		if (animate)
			glutIdleFunc(shoot);
		else
			glutIdleFunc(NULL);
		break;
	}
	glutPostRedisplay();
}

void handleSpecialKeypress(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		move_x--;
		if (move_x <= -123)
		{
			move_x = -123;
		}
		break;
	case GLUT_KEY_RIGHT:
		move_x++;
		if (move_x >= 70)
		{
			move_x = 70;
		}
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(400, 300);
	glutInitWindowSize(800, 600);
	glutCreateWindow("DiceGame");
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10000, timer, 0);
	init();
	rollDice();
	glutMainLoop();
	return 0;
}
