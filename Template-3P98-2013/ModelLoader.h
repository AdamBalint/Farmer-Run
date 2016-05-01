//Loads a model from an obj and mtl file
#pragma once
#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();
	Model* loadModel(char*);
	
private:
	std::ifstream file; //the file to use
	std::string matLoc; //the material location to use
	Model *model; //the model being generated
	Material *nMat; //the material being generated
	void loadVertices(std::ifstream&);
	void loadMaterials(std::ifstream&);

};

