/*
 * GL2PS, an OpenGL to PostScript Printing Library
 * Copyright (C) 1999-2003 Christophe Geuzaine 
 *
 * $Id$
 *
 * E-mail: geuz@geuz.org
 * URL: http://www.geuz.org/gl2ps/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Contributor(s):
 *   Rouben Rostamian <rostamian@umbc.edu>
 *   Guy Barrand <barrand@lal.in2p3.fr>
 */

/*
  To compile on a Linux system, type:

  gcc -O3 gl2psTest.c gl2ps.c -lglut -lGL -L/usr/X11R6/lib -lX11 -lm
*/


#pragma comment( linker, "/entry:\"mainCRTStartup\"" )  // set the entry point to be main()


#include <GL/glut.h>
#include <string.h>
#include "gl2ps.h"

static float rotation = -60.;
static GLsizei window_w = 0; 
static GLsizei window_h = 0;
static GLboolean display_multi = 1;
static GLboolean teapot = 0;
static char *pixmap[] = {
  "****************************************************************",
  "*..............................................................*",
  "*..................++++++++++++++++++++++++++..................*",
  "*.............+++++++++++++++++++++++++++++++++++++............*",
  "*.......++++++++++++@@@@@@@@@@@@@@@@@@@@@@@@++++++++++++.......*",
  "*.......++++++@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@++++++.......*",
  "*.......++@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@++.......*",
  "*.......++@@@@++++@@+@@@@@@@@++++@@@++++++@@@+++++@@@@++.......*",
  "*.......++@@@+@@@@+@+@@@@@@@+@@@@+@@+@@@@@+@+@@@@@+@@@++.......*",
  "*.......++@@+@@@@@@@+@@@@@@@@@@@+@@@+@@@@@+@+@@@@@@@@@++.......*",
  "*.......++@@+@@@+++@+@@@@@@@@@@+@@@@++++++@@@+++++@@@@++.......*",
  "*.......++@@+@@@@@+@+@@@@@@@@@+@@@@@+@@@@@@@@@@@@@+@@@++.......*",
  "*.......++@@@+@@@@+@+@@@@@@@@+@@@@@@+@@@@@@@+@@@@@+@@@++.......*",
  "*.......++@@@@+++++@++++++@@++++++@@+@@@@@@@@+++++@@@@++.......*",
  "*.......++@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@++.......*",
  "*.......++@@@@@@@@@++++++++++++++++++++++++++@@@@@@@@@++.......*",
  "*.......++@@@+++++++@@@@@@@@@@@+++++++++++++++++++@@@@++.......*",
  "*.......++++++@@+@@@+@@@++@@@@@+++++++++++++++++++++++++.......*",
  "*.......++@@@@@+@@@+@@@+@@@@+@@+++++++++++++++++++++++++.......*",
  "*.......++@@@@+@@@+@@+++++++@@@+++++++++++++++++++++++++.......*",
  "*.......++@@@@+++++++@@@@@@++@@++#####################++.......*",
  "*.......++@+++++@@@@@@@@@@@@@@@++#####################++.......*",
  "*.......++@+++@@@@@@@@@@@@@@@@@++#####################++.......*",
  "*.......++@++@@@@@@@+@@+@@@@@@@++#####################++.......*",
  "*.......++@@@@@+@@@+@@+@@@@@@@@++#####################++.......*",
  "*.......++@@@@+@@@+@@+@@++++++++++++++++++++++++++++++++.......*",
  "*.......++@@@@+@+++++++++@++++@+@+@+++++++++++++++++++++.......*",
  "*.......++@@@@+++@@@@@@@+@++@+++@+@+@+@+++++++++++++++++.......*",
  "*.......++@++++@@@@@@@@@@+@+++@+@+@+++++++++++++++++++++.......*",
  "*.......++@+++@@@@@@@@@@+@@@@@@@@@@@@@@+++++++++++++++++.......*",
  "*.......++@++@@@@@@+@@+@+@@@+++@@@@@@@@+++++++++++++++++.......*",
  "*.......++@@@@@+@@+@@+@@+@@+++++@@@@@@@+##############++.......*",
  "*.......++@@@@+@@+@@+@@++@@+@+++@@@@@+@+##############++.......*",
  "*.......++@@@@+@+++++++@+@@@@+++@@@@++@+##############++.......*",
  "*.......++@++++++@@@@@@++@@@@+++@@@@++@+##############++.......*",
  "*.......++@+++@@@@@@@@@@+@@@+++++@@@+@@+##############++.......*",
  "*.......++@++@@@@@@@@@@@+@+++++++++++@@+##############++.......*",
  "*.......+++++++++++++++++@+@+++++++++@@+++++++++++++++++.......*",
  "*.......+++++++++++++++++@+@+@@@@++++@@+@@@@@@@@@@@@@@++.......*",
  "*.......+++++++++++++++++@@@+@@@@@+@+@@+@@@+@@+@@@@@@@++.......*",
  "*........++++++++++++++++@@@@+@@@++@++@+@@+@@+@@@+@@@++........*",
  "*........++++++++++++++++@@@@@@@@+@@+@@+@+@@+@@@+@@@@++........*",
  "*........+++++++++++++++++@@@@@@+@@+@@+@++++++++++@@@++........*",
  "*.........++##############+@@@@@@@@@@++++@@@@@@@@++@++.........*",
  "*.........++###############+@@@@@@@@++++@@@@@@@@@@@@++.........*",
  "*..........++###############++++++++@@@@@@@+@@+@@@@++..........*",
  "*..........++##################++@@@@@+@@@+@@+@@+@@++..........*",
  "*...........++#################++@@@@+@@@+@@+@@+@@++...........*",
  "*...........++#################++@@@+@@++++++++@@@++...........*",
  "*............++++++++++++++++++++@@@++++@@@@@@++@++............*",
  "*.............+++++++++++++++++++@@+++@@@@@@@@@@@+.............*",
  "*.............+++++++++++++++++++@+++@@@@@@@@@@@++.............*",
  "*..............++++++++++++++++++@++@@@@+@@@@@@++..............*",
  "*...............+++++++++++++++++@@@+@@+@@+@@@++...............*",
  "*................++++++++++++++++@@+@@+@@+@@@++................*",
  "*.................++###########++@@+++++++@@++.................*",
  "*..................++##########++@+++@@@@+@++..................*",
  "*...................++#########++@++@@@@@@++...................*",
  "*....................+++#######++@++@@@@+++....................*",
  "*.....................++++#####++@@@@@++++.....................*",
  "*.......................++++###++@@@++++.......................*",
  "*.........................++++#++@++++.........................*",
  "*...........................++++++++...........................*",
  "*.............................++++.............................*",
  "*..............................................................*",
  "****************************************************************"};

void init(void){
  float pos[3] = {0.,0.,1000.};

  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_LIGHT0);
  glEnable(GL_SCISSOR_TEST);
}

void triangles(void){
  /* two intersecting triangles */
  glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);
  
  glColor3f(1., 0., 0.);
  glVertex3f(-1., 1., 0.);
  glColor3f(1., 1., 0.);
  glVertex3f(-1., 0., 0.);
  glColor3f(1., 0., 1.);
  glVertex3f(1., 0., 0.2);
  
  glColor3f(0., 1., 0.);
  glVertex3f(1., 0., 0.);
  glColor3f(0., 1., 1.);
  glVertex3f(1., 0.5, 0.);
  glColor3f(0., 1., 1.);
  glVertex3f(-1., 0.5, 0.1);

  glEnd();
}

void extras(void){
  glColor3f(1., 0., 0.);

  glPointSize(1.);
  gl2psPointSize(1.);
  glBegin(GL_POINTS);
  glVertex3f(-1., 1.1, 0.);
  glEnd();

  glPointSize(3.);
  gl2psPointSize(3.);
  glBegin(GL_POINTS);
  glVertex3f(-0.8, 1.1, 0.);
  glEnd();

  glPointSize(5.);
  gl2psPointSize(5.);
  glBegin(GL_POINTS);
  glVertex3f(-0.6, 1.1, 0.);
  glEnd();

  glPointSize(7.);
  gl2psPointSize(7.);
  glBegin(GL_POINTS);
  glVertex3f(-0.4, 1.1, 0.);
  glEnd();

  glLineWidth(1.);
  gl2psLineWidth(1.);
  glBegin(GL_LINES);
  glVertex3f(-0.2, 1.05, 0.);
  glVertex3f(0.2, 1.05, 0.);
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  gl2psEnable(GL2PS_LINE_STIPPLE);
  glLineStipple(1, 0x0F0F);
  glBegin(GL_LINES);
  glVertex3f(-0.2, 1.15, 0.);
  glVertex3f(0.2, 1.15, 0.);
  glEnd();
  glDisable(GL_LINE_STIPPLE);
  gl2psDisable(GL2PS_LINE_STIPPLE);

  glLineWidth(3.);
  gl2psLineWidth(3.);
  glBegin(GL_LINES);
  glVertex3f(0.4, 1.05, 0.);
  glVertex3f(0.8, 1.05, 0.);
  glEnd();

  glEnable(GL_LINE_STIPPLE);
  gl2psEnable(GL2PS_LINE_STIPPLE);
  glLineStipple(1, 0x0F0F);
  glBegin(GL_LINES);
  glVertex3f(0.4, 1.15, 0.);
  glVertex3f(0.8, 1.15, 0.);
  glEnd();
  glDisable(GL_LINE_STIPPLE);
  gl2psDisable(GL2PS_LINE_STIPPLE);

  glPointSize(1);
  gl2psPointSize(1);
  glLineWidth(1);
  gl2psLineWidth(1);
}

void objects(void){
  glEnable(GL_LIGHTING);
  glPushMatrix();
  glRotatef(rotation, 2., 0., 1.);
  if(teapot){
    glutSolidTeapot(0.7);
  }
  else{
    glutSolidTorus(0.3, 0.6, 20, 20);
  }
  glPopMatrix();
}

void printstring(char *string){
  int len, i;

  /* call gl2psText before the glut function since glutBitmapCharacter
     changes the raster position... */
  gl2psText(string, "Helvetica", 12);

  len = (int)strlen(string);
  for (i = 0; i < len; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
}

void text(){
  double x = -1.25, y = -0.58, dy = 0.13;

  glDisable(GL_LIGHTING);
  glColor3f(1., 1., 0.);

  glRasterPos2d(x, y); y -= dy;
  printstring("Press:");
  glRasterPos2d(x, y); y -= dy;
  printstring("  s: to save the images");
  glRasterPos2d(x, y); y -= dy;
  printstring("  t: to alternate between teapot and torus");
  glRasterPos2d(x, y); y -= dy;
  printstring("  v: to alternate between single and multiple viewport modes");
  glRasterPos2d(x, y); y -= dy;
  printstring("  q: to quit");
  glRasterPos2d(x, y); y -= dy;
  printstring("Click and move the mouse to rotate the objects");
}

void cube(void) {
  glDisable(GL_LIGHTING);
  glLineWidth(4.0);
  glColor3d (1.0,0.0,0.);   
  glBegin(GL_POLYGON);
  glVertex3d( 0.5,-0.5,0.5);
  glVertex3d( 0.5, 0.5,0.5);
  glVertex3d(-0.5, 0.5,0.5);
  glVertex3d(-0.5,-0.5,0.5);
  glEnd();

  glColor3d (0.0,1.0,0.);   
  glBegin(GL_POLYGON);
  glVertex3d( 0.5,-0.5,-0.5);
  glVertex3d( 0.5, 0.5,-0.5);
  glVertex3d(-0.5, 0.5,-0.5);
  glVertex3d(-0.5,-0.5,-0.5);
  glEnd();

  glColor3d (1.0,1.0,0.);   
  glBegin(GL_LINES);
  glVertex3d( 0.5,-0.5, 0.5);
  glVertex3d( 0.5,-0.5,-0.5);
  glVertex3d( 0.5, 0.5, 0.5);
  glVertex3d( 0.5, 0.5,-0.5);
  glVertex3d(-0.5, 0.5, 0.5);
  glVertex3d(-0.5, 0.5,-0.5);
  glVertex3d(-0.5,-0.5, 0.5);
  glVertex3d(-0.5,-0.5,-0.5);
  glEnd();
}

void image(float x, float y){
  int w = 64, h = 66, row, col, pos = 0;
  float *pixels, r = 0., g = 0., b = 0.;

  /* Fill a pixmap (each pixel contains three floats defining an RGB color) */
  pixels = (float*)malloc(3*w*h*sizeof(float));

  for(row = h-1; row >= 0; row--){
    for(col = 0; col < w; col++){
      switch(pixmap[row][col]){
      case '.' : r = 255.; g = 255.; b = 255.; break;
      case '+' : r = 0.  ; g = 0.  ; b = 0.  ; break;
      case '@' : r = 255.; g = 209.; b = 0.  ; break;
      case '#' : r = 255.; g = 0.  ; b = 0.  ; break;
      case '*' : r = 0.;   g = 0.  ; b = 20. ; break;
      }
      r /= 255.; g /= 255.; b /= 255.; 
      pixels[pos] = r; pos++;
      pixels[pos] = g; pos++;
      pixels[pos] = b; pos++;
    }
  }

  glRasterPos2f(x, y);
  glDrawPixels((GLsizei)w, (GLsizei)h, GL_RGB, GL_FLOAT, pixels);
  gl2psDrawPixels((GLsizei)w, (GLsizei)h, 0, 0, GL_RGB, GL_FLOAT, pixels);

  free(pixels);
}

/* A simple drawing function, using the default viewport */
void draw_single(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  triangles();
  extras();
  objects();
  text();
  glFlush();
}

/* A more complex drawing function, using 2 separate viewports */
void draw_multi(void){
  GLint viewport[4];

  glScissor(0., 0., window_w, window_h);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* First viewport with triangles, teapot or torus, etc. */

  glViewport(window_w * 0.05, window_h * 0.525 , 
	     (GLsizei) (window_w * 0.9), (GLsizei) (window_h * 0.45));
  glScissor(window_w * 0.05, window_h * 0.525 , 
	    (GLsizei) (window_w * 0.9), (GLsizei) (window_h * 0.45));
  glClearColor(0.3, 0.3, 0.3, 0.);
  glGetIntegerv(GL_VIEWPORT, viewport);

  gl2psBeginViewport(viewport);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.3,1.3, -1.3,1.3, -1.3,1.3);
  glMatrixMode(GL_MODELVIEW);
  
  triangles();
  extras();
  objects();
  text();

  gl2psEndViewport();


  /* Second viewport with cube, image, etc. */
  glViewport(window_w * 0.05, window_h * 0.025 , 
	     (GLsizei) (window_w * 0.9), (GLsizei) (window_h * 0.45));
  glScissor(window_w * 0.05, window_h * 0.025 , 
	     (GLsizei) (window_w * 0.9), (GLsizei) (window_h * 0.45));
  glClearColor(0.5, 0.5, 0.5, 0.);

  glGetIntegerv(GL_VIEWPORT, viewport);

  gl2psBeginViewport(viewport);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.3,1.3, -1.3,1.3, -1.3,1.3);
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glRotatef(rotation, 1., 1., 1.);
  cube();
  image(-0.8, -0.8);
  image(0.8, 0.2);
   extras();
  glPopMatrix();

  gl2psEndViewport();

  glClearColor(0.7, 0.7, 0.7, 0.);
  glFlush();
}

void display(void){
  if(display_multi){
    draw_multi();
  }
  else{
    draw_single();
  }
}

void reshape(int w, int h){
  window_w = w;
  window_h = h;

  glViewport(0, 0, (GLsizei) window_w, (GLsizei) window_h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.3,1.3, -1.3,1.3, -1.3,1.3);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.7, 0.7, 0.7, 0.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void writeps(int format, int sort, int options, int nbcol, char *file){
  FILE *fp;
  int state = GL2PS_OVERFLOW, buffsize = 0;
  GLint viewport[4];

  viewport[0] = 0;
  viewport[1] = 0;
  viewport[2] = window_w;
  viewport[3] = window_h;
 
  fp = fopen(file, "wb");

  if(!fp){
    printf("Unable to open file %s for writing\n", file);
    exit(1);
  }

  printf("Saving image to file %s... ", file);
  fflush(stdout);

  while(state == GL2PS_OVERFLOW){
    buffsize += 1024*1024;
    gl2psBeginPage(file, "test", viewport, format, sort, options,
		   GL_RGBA, 0, NULL, nbcol, nbcol, nbcol, 
		   buffsize, fp, file);
    display();
    state = gl2psEndPage();
  }

  fclose(fp);

  printf("Done!\n");
  fflush(stdout);
}

void keyboard(unsigned char key, int x, int y){
  int opt;

  switch(key){
  case 27:
  case 'q':
    exit(0);
    break;
  case 't':
    teapot = !teapot;
    display();
    break;
  case 'v':
    display_multi = !display_multi;
    if(display_multi){
      glEnable(GL_SCISSOR_TEST);
    }
    else{
      glDisable(GL_SCISSOR_TEST);
    }
    reshape(window_w, window_h);
    display();
    break;
  case 's':
//    opt = GL2PS_DRAW_BACKGROUND;
//    writeps(GL2PS_EPS, GL2PS_SIMPLE_SORT, opt, 0, "outSimple.eps");
//
//    opt = GL2PS_OCCLUSION_CULL | GL2PS_DRAW_BACKGROUND;
//    writeps(GL2PS_EPS, GL2PS_SIMPLE_SORT, opt, 0, "outSimpleCulled.eps");
//    opt = GL2PS_NO_PS3_SHADING | GL2PS_DRAW_BACKGROUND;
//    writeps(GL2PS_EPS, GL2PS_SIMPLE_SORT, opt, 2, "outSimpleShading1.eps");
//    writeps(GL2PS_EPS, GL2PS_SIMPLE_SORT, opt, 8, "outSimpleShading8.eps");
//    writeps(GL2PS_EPS, GL2PS_SIMPLE_SORT, opt, 16, "outSimpleShading16.eps");
//    opt = GL2PS_BEST_ROOT | GL2PS_DRAW_BACKGROUND;
//    writeps(GL2PS_EPS, GL2PS_BSP_SORT, opt, 0, "outBsp.eps");

    opt = GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT | GL2PS_DRAW_BACKGROUND;
    writeps(GL2PS_EPS, GL2PS_BSP_SORT, opt, 0, "outBspCulled.eps");
    writeps(GL2PS_PDF, GL2PS_BSP_SORT, opt, 0, "outBspCulled.pdf");

//    opt = GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT | GL2PS_NO_TEXT;
//    writeps(GL2PS_EPS, GL2PS_BSP_SORT, opt, 0, "outLatex.eps");
//    opt = GL2PS_NONE;
//    writeps(GL2PS_TEX, GL2PS_BSP_SORT, opt, 0, "outLatex.tex");

    printf("GL2PS %d.%d.%d done with all images\n",
	   GL2PS_MAJOR_VERSION, GL2PS_MINOR_VERSION, GL2PS_PATCH_VERSION);
    break;
  }
}

void motion(int x, int y){
  rotation += 10.;
  display();
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(400, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}