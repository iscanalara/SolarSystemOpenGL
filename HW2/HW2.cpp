#include <GL/glut.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h> 
#pragma warning(disable:4996)



float angle[13];
float anglex = 35.0;
float angley = -35.0;
float anglez = 0.0;
float locZ = 0.0;
float locY = 0.0;
float locX = 0.0;
float scale = 1;



GLUquadric* sun;
GLUquadric* mercury;
GLUquadric* venus;
GLUquadric* earth;
GLUquadric* moon;
GLUquadric* mars;
GLUquadric* jupiter;
GLUquadric* saturn;
GLUquadric* saturnRing;
GLUquadric* uranus;
GLUquadric* neptune;
GLUquadric* pluto;


GLuint sunTexture;
GLuint mercuryTexture;
GLuint venusTexture;
GLuint earthTexture;
GLuint moonTexture;
GLuint marsTexture;
GLuint jupiterTexture;
GLuint saturnTexture;
GLuint saturnRingTexture;
GLuint uranusTexture;
GLuint neptuneTexture;
GLuint plutoTexture;

int loadBitmap(const char* filename)
{
	FILE* file;
	char temp;
	long i;

	// own version of BITMAPINFOHEADER from windows.h for Linux compile
	struct {
		int biWidth;
		int biHeight;
		short int biPlanes;
		unsigned short int biBitCount;
		unsigned char* data;
	} infoheader;

	GLuint num_texture;

	if ((file = fopen(filename, "rb")) == NULL) return (-1); // Open the file for reading

	fseek(file, 18, SEEK_CUR);  /* start reading width & height */
	fread(&infoheader.biWidth, sizeof(int), 1, file);

	fread(&infoheader.biHeight, sizeof(int), 1, file);

	fread(&infoheader.biPlanes, sizeof(short int), 1, file);
	if (infoheader.biPlanes != 1) {
		printf("Planes from %s is not 1: %u", filename, infoheader.biPlanes);
		return 0;
	}

	// read the bpp
	fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
	if (infoheader.biBitCount != 24) {
		printf("Bpp from %s is not 24: %d", filename, infoheader.biBitCount);
		return 0;
	}

	fseek(file, 24, SEEK_CUR);

	// read the data
	if (infoheader.biWidth < 0) {
		infoheader.biWidth = -infoheader.biWidth;
	}
	if (infoheader.biHeight < 0) {
		infoheader.biHeight = -infoheader.biHeight;
	}
	infoheader.data = (unsigned char*)malloc(infoheader.biWidth * infoheader.biHeight * 3);
	if (infoheader.data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}

	if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
		printf("Error reading image data from %s.", filename);
		return 0;
	}

	for (i = 0; i < (infoheader.biWidth * infoheader.biHeight * 3); i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = infoheader.data[i];
		infoheader.data[i] = infoheader.data[i + 2];
		infoheader.data[i + 2] = temp;
	}


	fclose(file); // Closes the file stream

	glGenTextures(1, &num_texture);
	glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

	// The next commands sets the texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Finally we define the 2d texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

	// And create 2d mipmaps for the minifying function
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

	free(infoheader.data); // Free the memory we used to load the texture

	return (num_texture); // Returns the current texture OpenGL ID
}


void keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 27: exit(0); break;

	case 'a': anglex += 5; break;
	case 'A': anglex -= 5; break;
	case 's': angley -= 5; break;
	case 'S': angley += 5; break;
	case 'd': anglez += 5; break;
	case 'D': anglez -= 5; break;
	case '+': scale += 0.1; break;//zoom in
	case '-': scale -= 0.1; break;	//zoom out
	case 'f':
		//turn off the lights 
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		break;
	case 'o':
		//sun light open
		glEnable(GL_LIGHT0);
		break;
	case 'p':
		//pink light open 
		glEnable(GL_LIGHT1);
		break;

	case 'k':
		//close textures
		sunTexture = loadBitmap(" ");
		mercuryTexture = loadBitmap(" ");
		venusTexture = loadBitmap(" ");
		earthTexture = loadBitmap(" ");
		moonTexture = loadBitmap(" ");
		marsTexture = loadBitmap(" ");
		jupiterTexture = loadBitmap(" ");
		saturnTexture = loadBitmap(" ");
		saturnRingTexture = loadBitmap(" ");
		uranusTexture = loadBitmap(" ");
		neptuneTexture = loadBitmap(" ");
		plutoTexture = loadBitmap(" ");

		break;

	case 'n':
		//open textures
		sunTexture = loadBitmap("sunmap.bmp");
		mercuryTexture = loadBitmap("mercurymap.bmp");
		venusTexture = loadBitmap("venusmap.bmp");
		earthTexture = loadBitmap("earthmap.bmp");
		moonTexture = loadBitmap("moonmap.bmp");
		marsTexture = loadBitmap("marsmap.bmp");
		jupiterTexture = loadBitmap("jupitermap.bmp");
		saturnTexture = loadBitmap("saturnmap.bmp");
		saturnRingTexture = loadBitmap("saturnringmap.bmp");
		uranusTexture = loadBitmap("uranusmap.bmp");
		neptuneTexture = loadBitmap("neptunemap.bmp");
		plutoTexture = loadBitmap("plutomap.bmp");

	default: break;
	}
	glutPostRedisplay();
}




void rotate()
{
	angle[0] += 0.0;//sun

	angle[1] += 0.159;//mercury

	angle[2] += 0.118;//venus

	angle[3] += 0.1;//earth

	angle[4] += 0.0808;//mars

	angle[5] += 0.0439;//jupiter

	angle[6] += 0.0325;//saturn

	angle[7] += 0.0228;//uranus

	angle[8] += 0.0182;//neptune

	angle[9] += 0.0157;//pluto

	angle[10] += 0.3;//moon 0.0343 actually but ýt doesnt work so ý wrote 0.3 

	angle[11] += 0.1;

	glutPostRedisplay();

}

void drawSun() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glRotatef(angle[0], 0.0, 1.0, 0.0);
	gluSphere(sun, 10.9, 50, 50);

	glPopMatrix();
}

void drawMercury() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mercuryTexture);
	glTranslatef(14.9, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(mercury, 0.38, 50, 50);
	glPopMatrix();
}

void drawVenus() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, venusTexture);
	glTranslatef(20.08, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(venus, 0.95, 50, 50);
	glPopMatrix();
}

void drawEarth() {

	glPushMatrix();

	glTranslatef(25.08, 0.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(angle[11], 0.0, -10.0, 0.0);
	glPushMatrix();

	// rotation for moon
	glRotatef(angle[10], 0.0, 0.0, 1.0);
	// translation for moon.
	glTranslatef(3, 0.0, 1.0);
	// around earth rotation.
	glRotatef(-angle[10], 0.0, 0.0, 1.0);
	// moon rotation about it self.
	glRotatef(50, 0.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	// draw the moon
	gluSphere(moon, 0.5, 8, 8);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, earthTexture);
	gluSphere(earth, 1, 50, 50);
	glPopMatrix();

}



void drawMars() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, marsTexture);
	glTranslatef(28.88, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(mars, 0.53, 50, 50);
	glPopMatrix();
}

void drawJupiter() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, jupiterTexture);
	glTranslatef(40.31, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(jupiter, 11.2, 50, 50);
	glPopMatrix();
}



void drawSaturn() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, saturnTexture);
	glTranslatef(63.63, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(saturn, 9.45, 36, 72);
	glPopMatrix();
}


void drawSaturnRing() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, saturnRingTexture);
	glTranslatef(63.63, 0.0, 0.0);
	glRotatef(90, -1.0, 0.5, 0.0);
	gluDisk(saturnRing, 12, 15, 38, 3);
	glPopMatrix();
}

void drawUranus() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, uranusTexture);
	glTranslatef(78.85, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(uranus, 4.0, 50, 50);
	glPopMatrix();
}

void drawNeptune() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, neptuneTexture);
	glTranslatef(89.87, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(neptune, 3.88, 50, 50);
	glPopMatrix();
}

void drawPluto() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, plutoTexture);
	glTranslatef(99.18, 0.0, 0.0);
	glRotatef(angle[11], 0.0, -1.0, 0.0);
	gluSphere(pluto, 0.186, 50, 50);
	glPopMatrix();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	//sun
	GLfloat light_pos[] = { 0.0,0.0,0.0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	GLfloat ambient[] = { 2.5,2.5,2.5 };
	//GLfloat ambient[] = { 0.7,0.7,0.7 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	GLfloat specular[] = { 1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat diffuse[] = { 1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);


	//pink
	GLfloat light_pos1[] = { 0.0,0.0,0.0, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
	GLfloat ambient1[] = { 0.3,0.1,0.4 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	GLfloat specular1[] = { 3.0,1.0,2.0 };
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	GLfloat diffuse1[] = { 3.0,1.0,2.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);




	glLoadIdentity();
	//rotate/translate entire scene with a,A,s,S,D
	glRotatef(anglez, 0.0, 0.0, 1.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);

	glPushMatrix();
	drawSun();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[1], 0.0, 1.0, 0.0);
	drawMercury();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[2], 0.0, 1.0, 0.0);
	drawVenus();
	glPopMatrix();


	glPushMatrix();
	glRotatef(angle[3], 0.0, 1.0, 0.0);
	drawEarth();
	glPopMatrix();



	glPushMatrix();
	glRotatef(angle[4], 0.0, 1.0, 0.0);
	drawMars();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[5], 0.0, 1.0, 0.0);
	drawJupiter();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[6], 0.0, 1.0, 0.0);
	drawSaturn();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[6], 0.0, 1.0, 0.0);
	drawSaturnRing();
	glPopMatrix();


	glPushMatrix();
	glRotatef(angle[7], 0.0, 1.0, 0.0);
	drawUranus();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[8], 0.0, 1.0, 0.0);
	drawNeptune();
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle[9], 0.0, 1.0, 0.0);
	drawPluto();
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);



	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, -20.0, 20.0);

	sun = gluNewQuadric();
	gluQuadricTexture(sun, GL_TRUE);
	sunTexture = loadBitmap("sunmap.bmp");

	mercury = gluNewQuadric();
	gluQuadricTexture(mercury, GL_TRUE);
	mercuryTexture = loadBitmap("mercurymap.bmp");

	venus = gluNewQuadric();
	gluQuadricTexture(venus, GL_TRUE);
	venusTexture = loadBitmap("venusmap.bmp");

	earth = gluNewQuadric();
	gluQuadricTexture(earth, GL_TRUE);
	earthTexture = loadBitmap("earthmap.bmp");

	moon = gluNewQuadric();
	gluQuadricTexture(moon, GL_TRUE);
	moonTexture = loadBitmap("moonmap.bmp");

	mars = gluNewQuadric();
	gluQuadricTexture(mars, GL_TRUE);
	marsTexture = loadBitmap("marsmap.bmp");

	jupiter = gluNewQuadric();
	gluQuadricTexture(jupiter, GL_TRUE);
	jupiterTexture = loadBitmap("jupitermap.bmp");

	saturn = gluNewQuadric();
	gluQuadricTexture(saturn, GL_TRUE);
	saturnTexture = loadBitmap("saturnmap.bmp");

	saturnRing = gluNewQuadric();
	gluQuadricTexture(saturnRing, GL_TRUE);
	saturnRingTexture = loadBitmap("saturnringmap.bmp");


	uranus = gluNewQuadric();
	gluQuadricTexture(uranus, GL_TRUE);
	uranusTexture = loadBitmap("uranusmap.bmp");

	neptune = gluNewQuadric();
	gluQuadricTexture(neptune, GL_TRUE);
	neptuneTexture = loadBitmap("neptunemap.bmp");

	pluto = gluNewQuadric();
	gluQuadricTexture(pluto, GL_TRUE);
	plutoTexture = loadBitmap("plutomap.bmp");

}

int main(int argc, char** argv)
{
	glutInitWindowSize(1600, 1200);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Solar System - ALARA");
	init();
	glutDisplayFunc(display);


	glEnable(GL_LIGHTING);
	glutIdleFunc(rotate);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
