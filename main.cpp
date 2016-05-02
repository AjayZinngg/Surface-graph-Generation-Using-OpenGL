#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdio>
using namespace std;
#define NUM 64
#define SCALE 7
#define A/2 A>>1

GLfloat matrix[NUM][NUM]{{rand()%100},{1,2,3,4},{2,3,4,5},{3,4,5,6}};
GLfloat r=0,thetaDeg=0,theta=0;

void display(){
	GLint i,j;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	for(i=0;i<NUM;++i)
        for(j=0;j<NUM;++j)
          matrix[i][j]=(sin((j-(NUM/2))*M_PI/(NUM/2))+1)*(sin((i-(NUM/2))*M_PI/(NUM/2))+1)*SCALE;

	for(i=0;i<NUM;++i){
		glBegin(GL_LINE_STRIP);
			for(j=0;j<NUM;++j)
				glVertex3f((j-(NUM/2))<<2,matrix[i][j]*(NUM>>4),(i-(NUM/2))<<2);
		glEnd();
	}

	for(j=0;j<NUM;++j){
		glBegin(GL_LINE_STRIP);
			for(i=0;i<NUM;++i)
				glVertex3f((j-(NUM/2))<<2,matrix[i][j]*(NUM>>4),(i-(NUM/2))<<2);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

static void reshape(int w,int h){
	glViewport(0,0,w,h);

    //Setup the view
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45,(GLdouble)w/h,0.01,6400);
	glMatrixMode(GL_MODELVIEW);
}

void keys(GLubyte key,GLint x,GLint y){
    switch(key){
        case 'w':
        case 'W':   --r;
                    break;
        case 's':
        case 'S':   ++r;
                    break;
        case 'a':
        case 'A':   thetaDeg=(thetaDeg-1)>=0?thetaDeg-1:thetaDeg+359;
                    break;
        case 'd':
        case 'D':   thetaDeg=fmod(thetaDeg+1,360);
                    break;
    }
    theta=thetaDeg*M_PI/180;

    glLoadIdentity();
    gluLookAt(	r*cos(theta),NUM*3.75,r*sin(theta),   //camera position
                0,0,0,	    //center is at (0,0,0)
                0,1,0	);	//up is in positive Y direction
    glutPostRedisplay();
}

inline void init(){
	glEnable(GL_DEPTH_TEST);	//use depth buffering for hidden surface elimination.
/*
	gluLookAt(	NUM*5,NUM*3.75,NUM*5,   //camera position
				0,0,0,	    //center is at (0,0,0)
				0,1,0	);	//up is in positive Y direction
*/
    r=7.071068*NUM; //5 root 2
    thetaDeg=45;
    theta=thetaDeg*M_PI/180;

    glLoadIdentity();
    gluLookAt(	r*cos(theta),NUM*3.75,r*sin(theta),   //camera position
                0,0,0,	    //center is at (0,0,0)
                0,1,0	);	//up is in positive Y direction

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

	glColor4f(1,1,1,1);
}

int main(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutCreateWindow("Window 1");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keys);

	init();
	glutMainLoop();
	return 0;
}
