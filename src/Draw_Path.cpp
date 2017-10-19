#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>
#include <math.h>

#include "Draw_Path.h"

int InitWindow(int argc, char ** argv){
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("Path");
  glViewport(0, 0, 500, 500);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  gluLookAt( 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glMatrixMode(GL_MODELVIEW);
  return 0;
}

void display(double *position_p,int n){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt( 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glColor3f(0.0,0.0,0.0);
  glLineWidth(2.0);
  glBegin (GL_LINES);
  glVertex3f(-1.0, 0.0, 0.0);
  glVertex3f(+1.0, 0.0, 0.0);
  glEnd();
  glLineWidth(1.0);
  double pmax = 4;
  int i;
  for (i=0;i<n;i++){
    glLoadIdentity();
    glBegin (GL_LINE_STRIP);
    glVertex3f(((GLfloat) i)/n*2-1, position_p[i] / pmax,0.0);
    glVertex3f(((GLfloat) (i+1))/n*2-1, position_p[i+1] / pmax,0.0);    
    glEnd();
  }
  glutSwapBuffers();
}
