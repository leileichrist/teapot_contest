#include "teapot.h"

using namespace std;

teapot::teapot()
{
    scale=1.0;
}

teapot::teapot(float scale_t ,const char* pathname)
{
    scale=scale_t;
    glm::vec3 vertex=glm::vec3(0.0,0.0,0.0);
    glm::vec2 tex=glm::vec2(0.0,0.0);
    vertices.push_back(vertex);
    texgen.push_back(tex);

    initializeFromFile(pathname);

}
 
void teapot::vfParser(FILE* file)
{
    	while( 1 )
    	{
    		char lineHeader[128];
   	     int res = fscanf(file, "%s", lineHeader);
    		if (res == EOF)
       		break; 
       	else
       	{
       		if ( strcmp( lineHeader, "v" ) == 0 )	// if it is a vertex
       		{
       			glm::vec3 vertex;
    			 	fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
    				vertices.push_back(vertex);
       		}
       		else if(strcmp(lineHeader, "f") == 0)
       		{
       			glm::vec3 face;
       			fscanf(file, "%f %f %f\n", &face.x, &face.y, &face.z);
                    faces.push_back(face);
       		}
       		else
       		{
				char stupidBuffer[1000];
				fgets(stupidBuffer, 1000, file);
       		}	
       	}
     }
     for(int i=0;i<(int)vertices.size();i++)
     {
          vNormals.push_back(glm::vec3(0.0, 0.0, 0.0));
          //printVector(vNormals.back());
     }
}

bool teapot::initializeFromFile(const char* path)
{
	FILE * file = fopen(path, "r");
	if( file == NULL ){
   		printf("Impossible to open the file !\n");
    		return false;
	}
	vfParser(file);
	return true;
}

void teapot::printVector(const glm::vec3 & myVec)
{
     cout<<"("<< myVec.x<< ","<< myVec.y<<"," << myVec.z <<")"<<endl;
}

void teapot::printVector_2(const glm::vec2 & myVec)
{
     cout<<"("<< myVec.x<< ","<< myVec.y<<")"<<endl;
}

void teapot::printData() 
{
	int num_vertices=(int)(vertices.size());
     int num_faces=(int)(faces.size());

	for(int i=0;i<(int)num_vertices;i++)
	{
          printVector(vertices[i]);
	}

     for(int i=0; i<(int)num_faces;i++)
     {
          printVector(faces[i]); cout<< "with a face normal: " ;
          printVector(fNormals[i]);
     }

	cout<<"num of vertices are "<< num_vertices<<endl;
     cout<<"num of faces are "<<num_faces<<endl;
     cout<<"num of face normals are "<< fNormals.size()<<endl;
}

void teapot::getFaceNormal()
{
     for(int i=0;i<(int)faces.size();i++)
     {
          glm::vec3 v1=vertices[faces[i].x];
          glm::vec3 v2=vertices[faces[i].y];
          glm::vec3 v3=vertices[faces[i].z];

          glm::vec3 left=v2-v1;
          glm::vec3 right=v3-v1;

          glm::vec3 normal=glm::cross(left,right);
          normal=glm::normalize(normal);
          fNormals.push_back(normal);
     }
}

void teapot::getVertexNormal()
{
     for(int i=0;i<(int)faces.size();i++)
     {
          glm::vec3 face=faces[i];
          glm::vec3 fnormal=fNormals[i];
          vNormals[face.x]+=fnormal;
          vNormals[face.y]+=fnormal;
          vNormals[face.z]+=fnormal;
     }
     for(int i=1;i<(int)vNormals.size();i++)
     {
          vNormals[i]= glm::normalize(vNormals[i]);
          //cout<< "the "<< i<<"th vertex normal is: ";
        //  printVector(vNormals[i]);
     }
}

void teapot::get_Ymax()
{
     y_max=0.0;
     for(int i=1;i<(int)vertices.size();i++)
     {
          if(vertices[i].y>y_max)
               y_max=vertices[i].y;
     }
   //  cout<< "the bigggest y is "<<y_max<<endl;
}


void teapot::getTextureCoord()
{
     for(int i=1;i<(int)vertices.size();i++)
     {
          float x=vertices[i].x; float y=vertices[i].y; float z=vertices[i].z;
          //float r=sqrt(x*x+y*y+z*z);
          float theta=atan2(z,x);
          float s=(theta+PI)/(2*PI);
          float t=y/y_max;
          texgen.push_back(glm::vec2(s,t));
        //  cout<< "tex coordinate for vertex "<< i << "is " <<endl;
          //printVector_2(texgen.back());
     }
}


void teapot::initializePara()
{
     get_Ymax();
     getFaceNormal();
     getVertexNormal();
     getTextureCoord();
}

void  teapot::drawTeapot_multiTex()
{
     glActiveTexture(GL_TEXTURE1);
     glEnable(GL_TEXTURE_2D);
     glEnable(GL_TEXTURE_GEN_S);
     glEnable(GL_TEXTURE_GEN_T);

     glActiveTexture(GL_TEXTURE0);
     glEnable(GL_TEXTURE_2D);

     for(int i=0;i<(int)faces.size();i++)
     {
          int pos1=faces[i].x;
          int pos2=faces[i].y;
          int pos3=faces[i].z;

          //cout<< "(" <<texgen[pos1].x <<"," <<texgen[pos1].y<<")"<<endl;

          glBegin(GL_TRIANGLE_STRIP);
         // glBegin(GL_TRIANGLES); 
               glTexCoord2f(texgen[pos1].x,texgen[pos1].y);
               glNormal3f( vNormals[pos1].x, vNormals[pos1].y, vNormals[pos1].z );  
               glVertex3f(vertices[pos1].x,vertices[pos1].y,vertices[pos1].z);

               glTexCoord2f(texgen[pos2].x,texgen[pos2].y);
               glNormal3f( vNormals[pos2].x, vNormals[pos2].y, vNormals[pos2].z );  
               glVertex3f(vertices[pos2].x,vertices[pos2].y,vertices[pos2].z);

               glTexCoord2f(texgen[pos3].x,texgen[pos3].y);
               glNormal3f( vNormals[pos3].x, vNormals[pos3].y, vNormals[pos3].z );  
               glVertex3f(vertices[pos3].x,vertices[pos3].y,vertices[pos3].z);

          glEnd();
     }
}


