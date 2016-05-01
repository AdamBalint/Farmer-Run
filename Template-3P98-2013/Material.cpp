#include "Material.h"


Material::Material()
{
}


Material::~Material()
{
}

//stores the specular passed in
void Material::addSpecular(std::array<float, 3> in){
	specular[0] = in[0];
	specular[1] = in[1];
	specular[2] = in[2];
	specular[3] = 1.0;
}

//stores the diffuse passed in
void Material::addDiffuse(std::array<float, 3> in){
	diffuse[0] = in[0];
	diffuse[1] = in[1];
	diffuse[2] = in[2];
	diffuse[3] = 1.0;
}

//stores the ambient passed in
void Material::addAmbient(std::array<float, 3> in){
	ambient[0] = in[0];
	ambient[1] = in[1];
	ambient[2] = in[2];
	ambient[3] = 1.0;
}

//changes the alpha from the 1.0 to whatever was passed in
void Material::addDissolved(float f){
	specular[3] = f;
	diffuse[3] = f;
	ambient[3] = f;
}

//stores the shininess
void Material::addShininess(float f){
	shininess = f;
}

//returns the specular, diffuse, ambient and shininess
std::array<float, 4> Material::getSpecular(){
	return specular;
}
std::array<float, 4> Material::getDiffuse(){
	return diffuse;
}
std::array<float, 4> Material::getAmbient(){
	return ambient;
}
float Material::getShininess(){
	return shininess;
}

//prints what the material consists of
void Material::printMaterial(){
	printf("\nSpecular [%f,%f,%f,%f]\n", specular[0], specular[1], specular[2], specular[3]);
	printf("Diffuse [%f,%f,%f,%f]\n", diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	printf("Ambient [%f,%f,%f,%f]\n", ambient[0], ambient[1], ambient[2], ambient[3]);
	printf("Shininess [%f]\n\n", shininess);
}