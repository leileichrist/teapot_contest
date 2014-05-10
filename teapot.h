#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

#define PI 3.14159265358979323846

#ifndef TEAPOT_H
#define TEAPOT_H

class teapot
{
/* functions */
public:
	 teapot();
	 teapot(const teapot& otherTeapot){cout<< "don't need copy constructor here"<<endl;}
	 teapot(float scale_t,const char* pathname );
	~teapot(){cout<<"Teapot destructed; Simulation complete !"<<endl;}

	void initializePara();
	
	void 	render();
	void 	textureMapping();
	void 	envirmtMapping();
	void 	printData() ;
	void 	drawTeapot_multiTex();
	void 	testDrawing();
	GLuint* getTextureID(){return currTexID;}
	GLuint* getEnviID(){return currEnviID;}
	void	setTexID(GLuint* texID)    {  currTexID=texID;    }
	void	setEnviID(GLuint* enviID)  {  currEnviID=enviID;  }

/* data */
private:
	float scale;
	float y_max;
	GLuint*  curr_texID;
	GLuint*  currTexID;
	GLuint*  currEnviID;

	//vector< unsigned int > vertexIndices, faceIndices, normalIndices;
	vector< glm::vec3 > vertices;
	vector< glm::vec3 > faces;
	vector< glm::vec3 > vNormals;
	vector< glm::vec3 > fNormals;
	vector< glm::vec2 > texgen;

	bool initializeFromFile(const char* path);
	void vfParser(FILE* filename);

	void getFaceNormal();
	void getVertexNormal();
	void getTextureCoord();
	void get_Ymax();

	void printVector(const glm::vec3 & myVec);
	void printVector_2(const glm::vec2 & myVec);
};

#endif