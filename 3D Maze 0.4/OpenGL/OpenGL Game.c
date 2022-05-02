// made by shrippie
//WADS to move player.
#include <windows.h>
#include <MMsystem.h>
#include <conio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "Textures/Sky.ppm"



typedef struct
{
 int w,a,d,s;                     //button state on off
}ButtonKeys; ButtonKeys Keys;

//-----------------------------MAP----------------------------------------------
#define mapX  17      //map width  
#define mapY  29       //map height
#define mapS 64       //map cube size

int map[]=           //the map array. Edit to change level but keep the outer walls
{
 1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,
 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
 1,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,
 1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,
 1,0,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,
 1,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,
 1,0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,
 1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,	
 1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,
 1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,
 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
 1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
 1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,
 1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,0,1,
 1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,
 1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1, 
 1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,
 1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,0,1,
 1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,	
 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
 1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,
 1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	
};

void drawMap2D()
{
 int x,y,xo,yo;
 for(y=0;y<mapY;y++)
 {
  for(x=0;x<mapX;x++)
  {
   if(map[y*mapX+x]==1){ glColor3f(1,1,1);} else{ glColor3f(0,0,0);}
   xo=x*mapS; yo=y*mapS;
   glBegin(GL_QUADS); 
   glVertex2i( 0   +xo+1, 0   +yo+1); 
   glVertex2i( 0   +xo+1, mapS+yo-1); 
   glVertex2i( mapS+xo-1, mapS+yo-1);  
   glVertex2i( mapS+xo-1, 0   +yo+1); 
   glEnd();
  } 
 } 
}//-----------------------------------------------------------------------------


//------------------------PLAYER------------------------------------------------
float degToRad(float a) { return a*M_PI/180.0;}
float FixAng(float a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}

float px,py,pdx,pdy,pa;

void drawPlayer2D()
{
 glColor3f(1,1,0);   glPointSize(8);    glLineWidth(4);
 glBegin(GL_POINTS); glVertex2i(px,py); glEnd();
 glBegin(GL_LINES);  glVertex2i(px,py); glVertex2i(px+pdx*20,py+pdy*20); glEnd();
}



 
	


//-----------------------------------------------------------------------------


//---------------------------Draw Rays and Walls--------------------------------
float distance(ax,ay,bx,by,ang){ return cos(degToRad(ang))*(bx-ax)-sin(degToRad(ang))*(by-ay);}

void drawRays2D()
{
// glColor3f(0,1,1); glBegin(GL_QUADS); glVertex2i(5260,  0); glVertex2i(1006,  0); glVertex2i(1006,160); glVertex2i(526,160); glEnd();	
 //glColor3f(0,0,1); glBegin(GL_QUADS); glVertex2i(5260,160); glVertex2i(1006,160); glVertex2i(1006,320); glVertex2i(526,320); glEnd();	
 int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
 
 ra=FixAng(pa+30);                                                              //ray set back 30 degrees
 
 for(r=0;r<120;r++)
 {
  //---Vertical--- 
  dof=0; side=0; disV=100000;
  float Tan=tan(degToRad(ra));
       if(cos(degToRad(ra))> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
  else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
  else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit  

  while(dof<8) 
  { 
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                     
   if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
   else{ rx+=xo; ry+=yo; dof+=1;}  
                                              //check next horizontal
  } 
  vx=rx; vy=ry;

  //---Horizontal---
  dof=0; disH=100000;
  Tan=1.0/Tan; 
       if(sin(degToRad(ra))> 0.001){ ry=(((int)py>>6)<<6) -0.0001; rx=(py-ry)*Tan+px; yo=-64; xo=-yo*Tan;}//looking up 
  else if(sin(degToRad(ra))<-0.001){ ry=(((int)py>>6)<<6)+64;      rx=(py-ry)*Tan+px; yo= 64; xo=-yo*Tan;}//looking down
  else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right
 
  while(dof<8) 
  { 
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                          
   if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
   else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
  } 
  
  glColor3f(0,0.8,0);
  if(disV<disH){ rx=vx; ry=vy; disH=disV; glColor3f(0,0.6,0);}                  //horizontal hit first
 // glLineWidth(2); glBegin(GL_LINES); glVertex2i(px,py); glVertex2i(rx,ry); glEnd();//draw 2D ray comment Out If you Want to disable the rayCast From Drawing
    
  int ca=FixAng(pa-ra); disH=disH*cos(degToRad(ca));                            //fix fisheye 
  int lineH = (mapS*640)/(disH); if(lineH>640){ lineH=640;}                     //line height and limit
  int lineOff = 320 - (lineH>>1);                                               //line offset
  
  glLineWidth(8);glBegin(GL_LINES);glVertex2i(r*8,lineOff);glVertex2i(r*8,lineOff+lineH);glEnd();//draw vertical wall  

   
  ra=FixAng(ra-0.5);                                                              //go to next ray
 }
}//-----------------------------------------------------------------------------


void drawSky()
{
	int x,y;
	for(y=0;y<90;y++)
    {
      for(x=0;x<120;x++)
      {
      	int xo=(int)pa*2-x; if(xo<0){ xo+=120;} xo=xo % 120;
      	
      	int pixel=(y*120+xo)*3;
 		int red   =Sky[pixel+0];
 	    int green =Sky[pixel+1];
        int blue  =Sky[pixel+2];
 	   glPointSize(8); glColor3ub(red,green,blue); glBegin(GL_POINTS); glVertex2i(x*8,y*8); glEnd();
 	    
     }
   }
	
}

void init()
{                                                             
 glClearColor(0.3,0.3,0.3,0);                                 
 gluOrtho2D(0,960,640,0);
 px=488; py=1700; pa=90; // Spawn Area
 pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa)); 
}

float frame1,frame2,fps;

void display()
{   
  
  frame2=glutGet(GLUT_ELAPSED_TIME); fps=(frame2-frame1); frame1=glutGet(GLUT_ELAPSED_TIME); 
  
 if(Keys.a==1){ pa+=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 	
 if(Keys.d==1){ pa-=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
  
 int xo=0; if(pdx<0){ xo=-20;} else{ xo=20;}                                    //x offset to check map
 int yo=0; if(pdy<0){ yo=-20;} else{ yo=20;}                                    //y offset to check map
 int ipx=px/64.0, ipx_add_xo=(px+xo)/64.0, ipx_sub_xo=(px-xo)/64.0;             //x position and offset
 int ipy=py/64.0, ipy_add_yo=(py+yo)/64.0, ipy_sub_yo=(py-yo)/64.0;             //y position and offset
 
if(Keys.w==1)                                                                  //move forward
 {  
  if(map[ipy*mapX        + ipx_add_xo]==0){ px+=pdx*0.2*fps;}
  if(map[ipy_add_yo*mapX + ipx       ]==0){ py+=pdy*0.2*fps;}
 }
 if(Keys.s==1)                                                                  //move backward
 { 
  if(map[ipy*mapX        + ipx_sub_xo]==0){ px-=pdx*0.2*fps;}
  if(map[ipy_sub_yo*mapX + ipx       ]==0){ py-=pdy*0.2*fps;}
 } 
 glutPostRedisplay();

 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 //drawMap2D();
// drawPlayer2D();
  drawSky();
 drawRays2D();
 glutSwapBuffers();  
}

void ButtonDown(unsigned char key,int x,int y)      
{
 if(key=='a'){ Keys.a=1;} 	
 if(key=='d'){ Keys.d=1;} 
 if(key=='w'){ Keys.w=1;}
 if(key=='s'){ Keys.s=1;}
 glutPostRedisplay();
}
void ButtonUp(unsigned char key,int x,int y)                   
{
 if(key=='a'){ Keys.a=0;} 	
 if(key=='d'){ Keys.d=0;} 
 if(key=='w'){ Keys.w=0;}
 if(key=='s'){ Keys.s=0;}
 glutPostRedisplay();
}

void Resize(int w,int h)
{
	glutReshapeWindow(960,640);
}

int main(int argc, char* argv[])
{

 glutInit(&argc, argv); 
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize(960,640);
 glutInitWindowPosition( glutGet(GLUT_SCREEN_WIDTH)/2-960/2 ,glutGet(GLUT_SCREEN_HEIGHT)/2-640/2);
 glutCreateWindow("3D Maze");
 init();
 glutDisplayFunc(display);
 glutReshapeFunc(Resize);
 glutKeyboardFunc(ButtonDown);
 glutKeyboardUpFunc(ButtonUp);
 glutMainLoop();
 
}


