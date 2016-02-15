#include "stdafx.h"
#include "SFML/OpenGL.hpp"
#include "Terrain.h"
#include <cmath>

Terrain::Terrain(void)
{
	gridWidth=100;
	gridDepth=100;


	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	vertices=NULL;
	colors=NULL;	
	texCoords = NULL;
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;
	 tallestPoint = 0;
	 currentTris = 0;
	 loadMap();
}

void Terrain::loadMap() {
	map.loadFromFile("c.bmp");
}

Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
	delete [] texCoords;
}

//interpolate between two values
float lerp(float start, float end, float t){
	return start+(end-start)*t;
}


void Terrain::setVector3f(vector3f v, float x, float y, float z){

		v[0]=x;
		v[1]=y;
		v[2]=z;
		
}
void Terrain::setVector2f(vector2f v, float x, float y){
	v[0] = x;
	v[1] = y;
}
//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(float x, float y){

	//for the sample we will calculate height based on distance form origin
	//float dist=sqrt(x*x+y*y);
	float dist = 0;
	//center will be the highest point
	dist=30-dist;
	//put a nice curve in it
	dist*=dist;
	dist*=dist;
	//whoah, way too high, make it smaller
	dist/=50000;
	if (tallestPoint < dist){
		tallestPoint = dist;
	}
	return dist;
}
void Terrain::NormalVector(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat n[3]){

	GLfloat v1[3], v2[3]; // two vectors

	//calculate two vectors lying on the surface
	// v1=p2-p1
	// v2=p3-p2

	for (int i = 0; i<3; i++){
		v1[i] = p2[i] - p1[i];
		v2[i] = p3[i] - p2[i];
	}
	
	// calculate cross product of two vectors ( n= v1 x v2)
	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	n[2] = v1[0] * v2[1] - v2[0] * v1[1];

} //done
void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices = new vector3f[numVerts];
	delete [] colors;
	colors = new vector3f[numVerts];
	delete [] texCoords;
	texCoords = new vector2f[numVerts];

	//interpolate along the edges to generate interior points
	for(int i=0;i<gridWidth-1;i++){ //iterate left to right
		for(int j=0;j<gridDepth-1;j++){//iterate front to back

			float scale = 15;
			//sf::Color colors;
			sf::Color mapColors = map.getPixel(i, j);
			float rValue = mapColors.r;
			float gsValue = rValue / 255;
			float height1 = gsValue * scale;

			sf::Color mapColors2 = map.getPixel(i + 1, j );
			float rValue2 = mapColors2.r;
			float gsValue2 = rValue2 / 255;
			float height2 = gsValue2 * scale;

			sf::Color mapColors3 = map.getPixel(i, j + 1);
			float rValue3 = mapColors3.r;
			float gsValue3 = rValue3 / 255;
			float height3 = gsValue3 * scale;

			sf::Color mapColors4 = map.getPixel(i + 1, j + 1);
			float rValue4 = mapColors4.r;
			float gsValue4 = rValue4 / 255;
			float height4 = gsValue4 * scale;

			if (tallestPoint < height2){
				tallestPoint = height2;
			}
			int sqNum=(j+i*gridDepth);
			int vertexNum=sqNum*3*2; //6 vertices per square (2 tris)
			float front=lerp(-terrDepth/2,terrDepth/2,(float)j/gridDepth);
			float back =lerp(-terrDepth/2,terrDepth/2,(float)(j+1)/gridDepth);
			float left=lerp(-terrWidth/2,terrWidth/2,(float)i/gridDepth);
			float right=lerp(-terrDepth/2,terrDepth/2,(float)(i+1)/gridDepth);

			
			/*
			back   +-----+	looking from above, the grid is made up of regular squares
		height4    |tri1/|	'left & 'right' are the x cooded of the edges of the square
				   |   / |	'back' & 'front' are the y coords of the square
				   |  /  |	each square is made of two trianlges (1 & 2)
				   | /   |	
				   |/tri2|
			front  +-----+
			height     left   right
				 */

			//tri2
			setVector2f(texCoords[vertexNum], 0, 1);
			setVector3f(colors[vertexNum], 1, 1, 1);
			setVector3f(vertices[vertexNum++], left, height1, front); //bottom left

			setVector2f(texCoords[vertexNum], 1, 1);
			setVector3f(colors[vertexNum], 1, 1, 1);
			setVector3f(vertices[vertexNum++], right, height2, front);//bottom right

			setVector2f(texCoords[vertexNum], 1, 0);
			setVector3f(colors[vertexNum], 1, 1, 1);
			setVector3f(vertices[vertexNum++], right, height4, back); //top right
		
			//tri1/8
			setVector2f(texCoords[vertexNum], 1, 0);
			setVector3f(colors[vertexNum], 1, 1, 1);
			setVector3f(vertices[vertexNum++], right, height4, back); //top right

			setVector2f(texCoords[vertexNum], 0, 0);
			setVector3f(colors[vertexNum], 1, 1, 1);
			setVector3f(vertices[vertexNum++], left, height3, back); //top left

			setVector2f(texCoords[vertexNum], 0, 1);
			setVector3f(colors[vertexNum], 1, 1, 1);
			setVector3f(vertices[vertexNum++], left, height1, front); //bottom left 
		}
	}

}


void Terrain::Draw(){
	glBegin(GL_TRIANGLES);
	int counter =0; 
	bool once = true;
	for(int i =0;i<numVerts;i++){
			if (i == currentTris){
				NormalVector(vertices[i], vertices[i + 2], vertices[i + 1], normal);
				currentTris += 3;
				glNormal3fv(normal);
				counter++;

			}

			glColor3fv(colors[i]);
			glTexCoord2fv(texCoords[i]);

			//NormalVector(vertices[i - 2], vertices[i], vertices[i - 1], normal);
			glVertex3fv(vertices[i]);
		
	}
	currentTris = 0;
	int i = counter;
	glEnd();
}