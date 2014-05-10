#include "teapot.h"
#include "SOIL/SOIL.h"

glm::vec3 eye;
glm::vec3 lookAtVec;
glm::vec3 center;
glm::vec3 rightVec;
glm::vec3 upDir;

int nFPS = 60;
float fRotateAngle = 0.f;
float fRotateIncrement=0.8;
float angle=0.02f;

GLuint textureID;
GLuint textureID_alt;
GLuint enviTexID;
GLuint enviTexID_alt;

char* curTexImage;
char* curEnviImage;
bool  disableEnvi;
bool lightOn;

teapot myTeapot(1.0,"teapot_0.obj");

void mapTexture(char* filename)
{
     int width, height;
     unsigned char* pixel =SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
     SOIL_free_image_data(pixel);
} 

void setupReflection()
{
     glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
     glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
     glTexGeni(GL_R, GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
     glEnable(GL_TEXTURE_GEN_S);
     glEnable(GL_TEXTURE_GEN_T);
     glEnable(GL_TEXTURE_GEN_R);
 }   

void setUpMultiTexture(char* file1, char* file2, GLuint* texID1,  GLuint* texID2)
{
     glGenTextures(1, texID1);
     glGenTextures(1, texID2);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, *texID1);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     mapTexture(file1);
     curTexImage=file1;
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, *texID2);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     mapTexture(file2);
     if(strlen(file2)>0)
          curEnviImage=file2;
     glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
     glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); 
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

 void initializeMaterial()
{
     GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
     GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
     GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
     GLfloat mat_shininess[] = { 50.0 };
     glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
     glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
     glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
     glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); 
}

 void setUpLighting()
 {
     glEnable(GL_COLOR_MATERIAL);
     glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
     GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
     GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
     GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
     GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

     GLfloat light_position2[] = { 3.0, 1.0, 1.0, 0.0 };

     glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
     glLightfv(GL_LIGHT0, GL_POSITION, light_position);

     glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
     glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
     glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
     glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
 }


//parameters: vec: (x,y,z) axis: (u,v,w) and rotate angle
//rotate vector vec about arbituray axis axis (assuming axis is a unit vector)
//return: the vector after rotation. 
glm::vec3 rotateAboutArbi(glm::vec3 vec, glm::vec3 axis, float angle)
{
     float x=vec.x; float y=vec.y; float z=vec.z;
     float u=axis.x; float v=axis.y; float w=axis.z; 
/********************This equation is cited from outside source--see README.txt **********************/
    float newX=u*(u*x+v*y+w*z)*(1-cos(angle))+x*cos(angle)+(-w*y+v*z)*sin(angle);
    float newY=v*(u*x+v*y+w*z)*(1-cos(angle))+y*cos(angle)+( w*x-u*z)*sin(angle);
    float newZ=w*(u*x+v*y+w*z)*(1-cos(angle))+z*cos(angle)+(-v*x+u*y)*sin(angle);
/*****************************************************************************************************/
    glm::vec3 result=glm::vec3(newX,newY,newZ);
    return result ;
}

void init(void)
{
     eye.x=0.f;eye.y=5.f;eye.z=5.f;
     center.x=0;center.y=0;center.z=0;
     upDir.x=0.f; upDir.y=1.f; upDir.z=0.f;

     lookAtVec=glm::normalize( glm::vec3(center.x-eye.x,center.y-eye.y,center.z-eye.z) );
     rightVec=glm::normalize( glm::cross( lookAtVec , upDir ) );

     glClearColor(1.9,0.9,2.9,1.0); // clear color is gray       
     glPointSize(9.0);
     glEnable(GL_DEPTH_TEST);

     //  multitexture mapping: 
     setUpMultiTexture( "texture1.jpg" , "norway.jpg" , &textureID , &enviTexID );
     myTeapot.setTexID(&textureID);
     myTeapot.setEnviID(&enviTexID);
     disableEnvi=false;
     setupReflection();

     glEnable(GL_LIGHT0);
     glEnable(GL_LIGHT1);
     glEnable(GL_LIGHTING);
     lightOn=true;

     initializeMaterial();
     setUpLighting();

     cout<< "initialization complete !"<<endl;
}

void display()
{
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

     // reset OpenGL transformation matrix
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity(); // reset transformation matrix to identity

     // setup look at transformation 
     gluLookAt(eye.x,eye.y,eye.z,
               center.x,center.y,center.z,
               upDir.x,upDir.y,upDir.z);
     glRotatef(fRotateAngle,0.f,1.f,0.f);
     lookAtVec=glm::normalize( glm::vec3(center.x-eye.x,center.y-eye.y,center.z-eye.z) );
     rightVec=glm::normalize( glm::cross( lookAtVec , upDir ) );

     myTeapot.drawTeapot_multiTex();
     glFlush();
     glutSwapBuffers();  // swap front/back framebuffer to avoid flickering 
}

void reset()
{
     eye.x=0.f;eye.y=5.f;eye.z=5.f;
     center.x=0;center.y=0;center.z=0;
     upDir.x=0.f; upDir.y=1.f; upDir.z=0.f;

     lookAtVec=glm::normalize( glm::vec3(center.x-eye.x,center.y-eye.y,center.z-eye.z) );
     rightVec=glm::normalize( glm::cross( lookAtVec , upDir ) );
}

void reshape (int w, int h)
{
     // reset viewport ( drawing screen ) size
     glViewport(0, 0, w, h);
     float fAspect = ((float)w)/h; 
     // reset OpenGL projection matrix
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(70.f,fAspect,0.001f,30.f); 
}

void keyboard(unsigned char key, int x, int y)
{
     // put your keyboard control here
     if (key == 27) 
     {
          // ESC hit, so quit
          printf("demonstration finished.\n");
          exit(0);
     }
     else if(key=='t' || key=='T')
     {
          if(myTeapot.getTextureID()==&textureID)
          {
               setUpMultiTexture( "texture2.jpg" , curEnviImage , &textureID_alt , myTeapot.getEnviID());
               myTeapot.setTexID(&textureID_alt);
          }
          else
          {
               setUpMultiTexture( "texture1.jpg" , curEnviImage ,  &textureID ,    myTeapot.getEnviID() );
               myTeapot.setTexID(&textureID);
          }

     }
     else if(key=='e' || key=='E')
     {
          if(myTeapot.getEnviID()==&enviTexID)
          {
               setUpMultiTexture( curTexImage, "winter.jpg", myTeapot.getTextureID(), &enviTexID_alt );
               myTeapot.setEnviID( &enviTexID_alt);
          }
          else
          {
               setUpMultiTexture( curTexImage, "norway.jpg", myTeapot.getTextureID(), &enviTexID );
               myTeapot.setEnviID( &enviTexID);
          }  
          disableEnvi=false;
     }
     else if(key=='c' || key=='C')
     {
          if(!disableEnvi)
          {
               setUpMultiTexture( curTexImage , "" , myTeapot.getTextureID() , myTeapot.getEnviID() );
               disableEnvi=true;
          }
          else
          {
               setUpMultiTexture( curTexImage , curEnviImage , myTeapot.getTextureID() , myTeapot.getEnviID() );
               disableEnvi=false;
          }
     }
     else if(key=='l' || key=='L')
     {
          if(lightOn)
          {
               cout<<"light turned off!"<<endl;
               glDisable(GL_LIGHT0);
               glDisable(GL_LIGHT1);
               glDisable(GL_LIGHTING);
               lightOn=false;
          }
          else
          {    cout<<"light turned on!"<<endl;
               glEnable(GL_LIGHT0);
               glEnable(GL_LIGHT1);
               glEnable(GL_LIGHTING);
               lightOn=true;
          }
     }
     else if(key=='k' || key=='K')
     {
          reset();
          fRotateIncrement=0.8f;
     }
     else if(key=='+' || key=='=')
     {
          eye+=0.02f*lookAtVec;
     
     }
     else if(key=='-' || key=='_')
     {
          eye-=0.02f*lookAtVec;
     }
     else if(key=='r' || key=='R')
     {
          if(fRotateIncrement==0.0)
               fRotateIncrement=0.8f;
          else
               fRotateIncrement=0.0;
     } 
     else
          cout<<"Invalid keyboard input !"<<endl;    
}

//specialkeyboard function recognizes arrow keys
void  arrowKeyFunct(int key, int x, int y)
{
     if(fRotateIncrement>0.0) fRotateIncrement=0.0;
     switch (key) {
     case GLUT_KEY_LEFT:
     {
          eye=rotateAboutArbi(eye, upDir, angle);
          rightVec=glm::normalize( glm::cross( lookAtVec , upDir ) );
          break;
     }
     case GLUT_KEY_RIGHT:
     {
          eye=rotateAboutArbi(eye, upDir, -1*angle);
               rightVec=glm::normalize( glm::cross( lookAtVec , upDir ) );

          break;
     }
         
     case GLUT_KEY_UP:
     {
          eye=rotateAboutArbi(eye, rightVec, angle);
          upDir=glm::normalize(glm::cross(rightVec,lookAtVec) );
          break;
     }
     case GLUT_KEY_DOWN:
     {
          eye=rotateAboutArbi(eye, rightVec, -1*angle);
          upDir=glm::normalize(glm::cross(rightVec,lookAtVec )) ;
          break;
     }     
        default: break;
    }            
}

void timer(int v)
{
     fRotateAngle += fRotateIncrement; // change rotation angles
     glutPostRedisplay(); // trigger display function by sending redraw into message queue
     glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

int  main(int argc, char* argv[])
{
	myTeapot.initializePara();

	glutInit(&argc, (char**)argv);

	// set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize (1200, 900); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ((const char*)"mhu9_mp3: Hello Teapot!");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	if (GLEW_ARB_vertex_program)
		fprintf(stdout, "Status: ARB vertex programs available.\n");
	if (glewGetExtension("GL_ARB_fragment_program"))
		fprintf(stdout, "Status: ARB fragment programs available.\n");
	if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
		fprintf(stdout, "Status: ARB point sprites available.\n");
	
     // set up the call-back functions 
	init(); // setting up user data & OpenGL environment
	glutDisplayFunc(display);  // called when drawing 
	glutReshapeFunc(reshape);  // called when change window size
	glutKeyboardFunc(keyboard); // called when received keyboard interaction
     glutSpecialFunc(arrowKeyFunct); // called when user adjusts the eye's position
	glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation

	glutMainLoop(); // start the main message-callback loop

	return 0;
}