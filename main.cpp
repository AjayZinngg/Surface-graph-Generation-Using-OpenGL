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
bool paused=false;

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

    glBegin(GL_POINTS);
	for(i=0;i<NUM;++i){
        for(j=0;j<NUM;++j){
            color(matrix[i][j]/maximum);
            glVertex3f((j-(NUM/2))<<2,matrix[i][j]*(NUM>>4),(i-(NUM/2))<<2);
        }
	}
    glEnd();
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

float p;
string s1="Surface Plot Generation";
string s11="Surface Plot";
string s12=" Generation";
string s2="Adithya Ramesh\n1PE13CS007\n\nAjay Singh\n1PE13CS013";
long long int count=0;
static void display(){
if(!paused){
		static int i,j;

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		if(count++<=600){
			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0,window_w,0,window_h,0,2);
			glMatrixMode(GL_MODELVIEW);
			if(count<=180)	p=count/180.0f;
			else			p=1-max((count-480)/120.0f,0.0f);
			color(p);

			glLoadIdentity();
			glTranslatef(	(window_w-glutStrokeLength(GLUT_STROKE_ROMAN,(const unsigned char*)s1.c_str())*0.25)/2,
							(window_h-glutStrokeHeight(GLUT_STROKE_ROMAN)*0.25)/2,0);
			glScalef(0.25,0.25,1);
			glLineWidth(1.5);
			glutStrokeString(GLUT_STROKE_ROMAN,(const unsigned char*)s11.c_str());
			glLineWidth(3);
			glutStrokeString(GLUT_STROKE_ROMAN,(const unsigned char*)s12.c_str());

			glLoadIdentity();
			glLineWidth(1.5);
			glTranslatef(0,glutStrokeHeight(GLUT_STROKE_ROMAN)*0.75,0);
			glTranslatef(20,0,0);
			glScalef(0.15,0.15,1);
			glutStrokeString(GLUT_STROKE_ROMAN,(const unsigned char*)s2.c_str());

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

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

		glBegin(GL_LINE_STRIP);
			//color(0.0625);
			color(0.111);
			glVertex3f(-NUM*2,0,-NUM*2);
			glVertex3f(NUM*2,0,-NUM*2);
			glColor3i(0,0,0);
			glVertex3f(NUM*3,0,-NUM*2);
		glEnd();

		glBegin(GL_LINE_STRIP);
			color(0.0625);
			glVertex3f(-NUM*2,0,-NUM*2);
			glVertex3f(-NUM*2,2*NUM,-NUM*2);
			glColor3i(0,0,0);
			glVertex3f(-NUM*2,2.5*NUM,-NUM*2);
		glEnd();

		glBegin(GL_LINE_STRIP);
			color(0.0625);
			glVertex3f(-NUM*2,0,-NUM*2);
			glVertex3f(-NUM*2,0,NUM*2);
			glColor3i(0,0,0);
			glVertex3f(-NUM*2,0,NUM*3);
		glEnd();

		theta+=M_PI_2/1440;

		glLoadIdentity();
		gluLookAt(	r*cos(theta),NUM*3.75,r*sin(theta),	//camera position
					0,0,0,								//center is at (0,0,0)
					0,1,0	);							//up is in positive Y direction

		temp+=0.01;
		glutSwapBuffers();
	}
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
		case ' ':		paused=!paused;
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
