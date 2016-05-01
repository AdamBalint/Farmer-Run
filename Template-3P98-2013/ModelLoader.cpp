#include "ModelLoader.h"


ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

//loads a model
Model* ModelLoader::loadModel(char* filePath){
	//creates a new model and opens the obj file passed in
	model = new Model();
	std::ifstream file;
	file.open(filePath, std::ifstream::in);
	if (!file.is_open()){
		printf("Failed to open obj file\n");
		return NULL;
	}

	//first, the program loads the vertices and then closes the file
	loadVertices(file);
	file.close();

	//then, it uses the material file referenced in the obj, and reads in the materials
	std::string tmp = filePath;
	tmp.replace(tmp.length() - matLoc.length(), matLoc.length(), matLoc);

	file.open(tmp, std::ifstream::in);
	if (!file.is_open()){
		printf("Failed to open material file\n");
		return NULL;
	}
	
	//loads the materials, closes file and returns the model
	loadMaterials(file);
	file.close();

	return model;
}

//loads the vertices
void ModelLoader::loadVertices(std::ifstream& file){
	int count = 0;
	while (file.good() && !file.eof()){
		count++;
		//reads in the next line
		std::string line;
		std::getline(file, line);
		//if the line is empty then it breaks out of the loop
		if (line.size() == 0){
			break;
		}
		//check if the line is a vertex declaration
		//if it is, then it makes a vertex and adds it to the model
		if ((line.substr(0, 2).compare("v ")) == 0){
			std::string rest = line.substr(2);
			std::array<float, 3> val;
			std::stringstream ss;
			ss << rest;
			ss >> val[0] >> val[1] >> val[2];
			(*model).addVertex(val);
		}
		//check if the line is a vertex texture declaration
		//if it is, then it makes a vertex for the vertex texture and adds it to the model
		else if ((line.substr(0, 2).compare("vt")) == 0){
			std::string rest = line.substr(2);
			std::array<float, 3> val;
			std::stringstream ss;
			ss << rest;
			ss >> val[0] >> val[1] >> val[2];
			(*model).addVertexTexture(val);
		}
		//check if the line is a vertex normal declaration
		//if it is, then it makes a vertex normal and adds it to the model
		else if ((line.substr(0, 2).compare("vn")) == 0){
			std::string rest = line.substr(2);
			std::array<float, 3> val;
			std::stringstream ss;
			ss << rest;
			ss >> val[0] >> val[1] >> val[2];
			(*model).addVertexNormal(val);
		}
		//check if the line is a face declaration
		//if it is, then it makes a face and adds it to the model
		else if ((line.substr(0, 2).compare("f ")) == 0){
			std::string rest = line.substr(2);
			//variables for the references of the vertex, vertex normal and vertex texture
			int v1, v2, v3, vn1, vn2, vn3, vt1, vt2, vt3;
			bool texture = rest.find("//") == std::string::npos;
			std::stringstream ss;
			for (int i = 0; i < rest.size() - 1; i++){
				if (rest.at(i) == '/'){
					rest.replace(i, 1, " ");
				}
			}
			ss << rest;
			if (!texture)
				ss >> v1 >> vn1 >> v2 >> vn2 >> v3 >> vn3;
			else if(texture)
				ss >> v1 >> vt1 >> vn1 >> v2 >> vt2 >> vn2 >> v3 >> vt3 >> vn3;
			(*model).addFace(v1, v2, v3, vn1, vn2, vn3);
		}
		//check if the line is a material lib declaration
		//if it is, then it stores it for later use
		else if ((line.substr(0, 6).compare("mtllib")) == 0){
			matLoc = line.substr(7);
		}
		//check if the line is a material use declaration
		//if it is, then it adds what material to use to the model
		else if ((line.substr(0, 6).compare("usemtl")) == 0){
			std::string  matName = line.substr(7);
			(*model).addMaterialToUse(matName);
			(*model).nextFaceSet();
		}
		//otherwise it skips it
		else{
		}
	}
}

//loads the materials
void ModelLoader::loadMaterials(std::ifstream& file){
	std::string materialName;

	bool first = true; //flag to not add an empty material
	//reads in everything in the file
	while (file.good() && !file.eof()){
		std::string line;
		//reads in the line
		std::getline(file, line);
		//check if the line is a new material declaration
		//if it is, then it adds the previous material to the model and makes a new material
		if ((line.substr(0, 6).compare("newmtl")) == 0){
			if (!first){
				(*model).addMaterial(materialName, nMat);
			}
			first = false;
			nMat = new Material();
			materialName = line.substr(7);
		}
		//check if the line is a material shininess declaration
		//if it is, then it sets the shininess of the model (from 0-1000, so division by 1000 is needed)
		else if ((line.substr(0, 2).compare("Ns")) == 0){
			std::string rest = line.substr(2);
			float shin;
			std::stringstream ss;
			ss << rest;
			ss >> shin;
			(*nMat).addShininess(shin/1000);
		}
		//check if the line is a material ambient declaration
		//if it is, then it sets the ambient of the model
		else if ((line.substr(0, 2).compare("Ka")) == 0){
			std::string rest = line.substr(2);
			std::array<float, 3> amb;
			std::stringstream ss;
			ss << rest;
			ss >> amb[0] >> amb[1] >> amb[2];
			(*nMat).addAmbient(amb);
		}
		//check if the line is a material diffuse declaration
		//if it is, then it sets the diffuse of the model
		else if ((line.substr(0, 2).compare("Kd")) == 0){
			std::string rest = line.substr(2);
			std::array<float, 3> dif;
			std::stringstream ss;
			ss << rest;
			ss >> dif[0] >> dif[1] >> dif[2];
			(*nMat).addDiffuse(dif);
		}
		//check if the line is a material specular declaration
		//if it is, then it sets the specular of the model
		else if ((line.substr(0, 2).compare("Ks")) == 0){
			std::string rest = line.substr(2);
			std::array<float, 3> spec;
			std::stringstream ss;
			ss << rest;
			ss >> spec[0] >> spec[1] >> spec[2];
			(*nMat).addSpecular(spec);
		}
		//check if the line is a material dissolved(alpha) declaration
		//if it is, then it sets the dissolved of the model
		else if ((line.substr(0, 2).compare("d ")) == 0){
			std::string rest = line.substr(2);
			float dis;
			std::stringstream ss;
			ss << rest;
			ss >> dis;
			(*nMat).addDissolved(dis);
		}
		//if it is the end of the file, then it adds the last material made
		else if (file.eof()){
			(*model).addMaterial(materialName, nMat);
		}
	}
}