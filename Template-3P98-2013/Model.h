//Holds all the information for a model
#pragma once

#include <vector>
#include "Material.h"
#include <unordered_map>
#include <freeglut.h>
#include <array>
class Model
{
public:
	Model();
	~Model();

	void addVertex(std::array<float, 3>);
	void addVertexNormal(std::array<float, 3>);
	void addVertexTexture(std::array<float, 3>);
	void addMaterial(std::string, Material*);
	void addMaterialToUse(std::string);
	void addFace(int, int, int, int, int, int);
	void nextFaceSet();
	void drawModel();
	void addToList(GLuint*, int);

private:
	std::vector<std::array<float, 3>> v; //the vertex location
	std::vector<std::array<float, 3>> vn; //the vertex normal
	std::vector<std::array<float, 3>> vt; //the vertex texture
	std::vector<std::vector<std::array<std::array<int, 3>,2>>> faces; //hold the faces for each object - separated into sets
	std::vector<std::string> matToUse; //holds the name of the material to use
	std::unordered_map<std::string, Material*> matTable; //holds the material lookup table

	float* getPoint(std::array<std::array<int, 3>,2>, int);
	float* getNormal(std::array<std::array<int, 3>, 2>, int);

	int faceSet = -1;

};

