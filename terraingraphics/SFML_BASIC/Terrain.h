#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector3f[3];

	typedef  GLfloat vector2f[2];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector3f *vertices;
	vector3f *colors;
	vector2f *texCoords;
	sf::Image map;
	GLfloat normal[3];
	int currentTris;
	int scale = 10;
	

	float getHeight(float x, float y);
	void setVector3f(vector3f, float, float, float);
	void setVector2f(vector2f v, float x, float y);
public:
	Terrain(void);
	~Terrain(void);
	void Terrain::NormalVector(GLfloat p1[], GLfloat p2[], GLfloat p3[], GLfloat n[]);
	float tallestPoint;

	void Init();
	void Draw();
	void loadMap();
};

