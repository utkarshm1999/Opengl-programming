#include <GL/glut.h>  
#include<bits/stdc++.h>

using namespace std; 
int magnification = 8; // magnification factor to change the rendered size
bool fullscreen = false;
bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

// command to compile: g++ 3dmesh-draw.cpp -lGL -lGLU -lglut

// class that handles .obj files ( draw,read)
class obj3dmodel
{
   struct vertex{
        double x;
        double y;
        double z;
   };
   struct face{
         unsigned int v1,v2,v3;
   };
   vector<vertex> vertexes;
   vector<face> faces;

public:
  void readfile(const char* filename);
  void draw();
};

void obj3dmodel::readfile(const char *filename) 
{
   string s;
   ifstream fin(filename);
   if(!fin)
         return;
   while(fin>>s)
   {
         switch(*s.c_str())
         {
         case 'v':
              {
                    vertex v;
                    fin>>v.x>>v.y>>v.z;
                    this->vertexes.push_back(v);
              }
              break;            
         case 'f':
              {
                    face f;
                    fin>>f.v1>>f.v2>>f.v3;
                    faces.push_back(f);
              }
              break;
        }
   }     
}

void obj3dmodel::draw()
{
   glBegin(GL_TRIANGLES);
   for(int i=0;i<faces.size();i++)
   {                         
      vertex v1= vertexes[faces[i].v1-1];
      vertex v2=vertexes[faces[i].v2-1];
      vertex v3=vertexes[faces[i].v3-1];
      if(i%3 == 0){
          glColor3f(1.0f, 0.0f, 0.0f); 
      }
      else if(i%3 == 1){
          glColor3f(0.0f, 1.0f, 0.0f); 
      }
      else if(i%3==2){
          glColor3f(0.0f, 0.0f, 1.0f); 
      }
        
      glVertex3f(v1.x*magnification-1.0,v1.y*magnification-1.0,v1.z*magnification-1.0);
      glVertex3f(v2.x*magnification-1.0,v2.y*magnification-1.0,v2.z*magnification-1.0);
      glVertex3f(v3.x*magnification-1.0,v3.y*magnification-1.0,v3.z*magnification-1.0);
   }
   glEnd();
}


char title[] = "BUNNY";
obj3dmodel * bunny;
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
   glClearDepth(1.0f);                   
   glEnable(GL_DEPTH_TEST);  
   glDepthFunc(GL_LEQUAL);    
   glShadeModel(GL_SMOOTH);   
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   glMatrixMode(GL_MODELVIEW);    
   glLoadIdentity();                
   glTranslatef(1.5f, 0.0f, -7.0f);
  
   gluLookAt(
   0.0f, 0.0f, 3.0f,
   0.0f, 0.0f, 0.0f,
   0.0f, 1.0f, 0.0f);

   glRotatef(xrot, 1.0f, 0.0f, 0.0f);
   glRotatef(yrot, 0.0f, 1.0f, 0.0f);    
   glBegin(GL_TRIANGLES); 
   bunny->draw();
   glEnd();   
   glFlush();
   glutSwapBuffers(); 
}
 
void reshape(GLsizei width, GLsizei height) {  
  
   if (height == 0) height = 1;               
   GLfloat aspect = (GLfloat)width / (GLfloat)height;  
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);  
   glLoadIdentity();            
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
void mouse(int button, int state, int x, int y){
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
      mouseDown = true;

      xdiff = x - yrot;
      ydiff = -y + xrot;
   }
   else if(button == 3 || button == 4){
      //cout<<"scroll:"<<button<<":"<<state<<endl;
      if(state == GLUT_UP){
         // do nothing
      }
      else{
         if(button == 3){
            magnification++;
           // cout<<"plus"<<endl;
            glutPostRedisplay();
         }
         else{
            if(magnification>1){
               magnification--;
              // cout<<"minus"<<endl;
               glutPostRedisplay();
            }
            
         }
      }
   }
   else{
      mouseDown = false;
   }
}

void mouseMotion(int x, int y){
   if (mouseDown){
      yrot = x - xdiff;
      xrot = y + ydiff;

      glutPostRedisplay();
   }
}
 
int main(int argc, char** argv) {
   glutInit(&argc, argv);           
   bunny = new obj3dmodel;
   bunny->readfile("lowpolybunny.obj");
   glutInitDisplayMode(GLUT_DOUBLE); 
   glutInitWindowSize(800, 600);   
   glutInitWindowPosition(50, 50); 
   glutCreateWindow(title);          
   glutDisplayFunc(display);       
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);      
   initGL();                      
   glutMainLoop();               
   return 0;
}
