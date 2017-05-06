#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <cstdio>
using namespace std;
#define A/2 (A>>1)
#define NUM 96
#define SCALE 5

float matrix[NUM][NUM]={0};
float r=0,thetaDeg=0,theta=0;
float maximum;

float temp=0;
int window_w,window_h;

int mode=0;	//0:points,1:lines

const float gradient[3][3]={{0,0.05,0.1},{0,0.5,1},{0.675,0.925,1}};

//Get color from gradient
inline void color(float p){
	static int i;
	static float col[3];

	if(p<=0.5){
		p*=2;
		for(i=0;i<3;++i)
			col[i]=p*gradient[1][i]+(1-p)*gradient[0][i];
	}
	else{
		p=(p-0.5)*2;
		for(i=0;i<3;++i)
			col[i]=p*gradient[2][i]+(1-p)*gradient[1][i];
	}

	glColor3fv(col);
}

inline void drawPoints(){
	static int i,j;
	for(i=0;i<NUM;++i){
		glBegin(GL_POINTS);
			for(j=0;j<NUM;++j){
				color(matrix[i][j]/maximum);
				glVertex3f((j-(NUM/2))<<2,matrix[i][j]*(NUM>>4),(i-(NUM/2))<<2);
			}
		glEnd();
	}
}

inline void drawlines(){
	static int i,j;
	for(i=0;i<NUM;++i){
		glBegin(GL_LINE_STRIP);
			for(j=0;j<NUM;++j){
				color(matrix[i][j]/maximum);
				glVertex3f((j-(NUM/2))<<2,matrix[i][j]*(NUM>>4),(i-(NUM/2))<<2);
			}
		glEnd();
	}

	for(j=0;j<NUM;++j){
		glBegin(GL_LINE_STRIP);
			for(i=0;i<NUM;++i){
				color(matrix[i][j]/maximum);
				glVertex3f((j-(NUM/2))<<2,matrix[i][j]*(NUM>>4),(i-(NUM/2))<<2);
			}
		glEnd();
	}
}

char *s="Hello World!!!",*p;
static void display(){
	static int i,j;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	for(i=0;i<NUM;++i)
		for(j=0;j<NUM;++j)
		  matrix[i][j]=(sin(temp+(j-(NUM/2))*M_PI/(NUM/2))+1)*(sin(temp/2+(i-(NUM/2))*M_PI/(NUM/2))+1)*SCALE;

	maximum=0;
	for(i=0;i<NUM;++i)
		for(j=0;j<NUM;++j)
			if(maximum<matrix[i][j])
				maximum=matrix[i][j];

	if(mode==0)
		drawPoints();
	else
		drawlines();

	theta+=M_PI_2/1440;

	glLoadIdentity();
	gluLookAt(	r*cos(theta),NUM*3.75,r*sin(theta),	//camera position
				0,0,0,								//center is at (0,0,0)
				0,1,0	);							//up is in positive Y direction

	temp+=0.01;
	glutSwapBuffers();
}

static void idle(){
	glutPostRedisplay();
}

static void reshape(int w,int h){
	window_w=w;
	window_h=h;
	glViewport(0,0,w,h);

	//Setup the view
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45,(GLdouble)w/h,0.01,6400);
	glMatrixMode(GL_MODELVIEW);
}

#define ZOOMSTEP NUM>>4
static void keys(GLubyte key,int x,int y){
	switch(key){
		case 'w':
		case 'W':		r-=ZOOMSTEP;
						break;
		case 's':
		case 'S':		r+=ZOOMSTEP;
						break;
		case '\033':	exit(0);	//Esc to quit
						break;
/*
		case 'a':
		case 'A':	thetaDeg=fmod(thetaDeg+1,360);
					break;
		case 'd':
		case 'D':	thetaDeg=(thetaDeg-1)>=0?thetaDeg-1:thetaDeg+359;
					break;
*/
	}

	glutPostRedisplay();
}

inline void init(){
	//Initialize window
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE|GLUT_MULTISAMPLE);	//RGB+3D+Animated+Antialiased
	glutInitWindowSize(640,480);
	glutCreateWindow("Graph");
	glutFullScreen();

	//Assign callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);
	glutIdleFunc(idle);

	//Use depth buffering for hidden surface elimination.
	glEnable(GL_DEPTH_TEST);

	//Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	r=7.071068*NUM;									//5 root 2
	thetaDeg=45;
	theta=thetaDeg*M_PI/180;

	glLoadIdentity();
	gluLookAt(	r*cos(theta),NUM*3.75,r*sin(theta),	//camera position
				0,0,0,								//center is at (0,0,0)
				0,1,0	);							//up is in positive Y direction

	glLineWidth(0.75);
}

int main(int argc,char **argv){
	glutInit(&argc,argv);
	init();
	glutMainLoop();
	return 0;
}
