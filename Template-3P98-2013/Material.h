//Hold a material - used for lighting
#pragma once
#include <array>
class Material
{
public:
	Material();
	~Material();

	void addSpecular(std::array<float, 3>);
	void addDiffuse(std::array<float, 3>);
	void addAmbient(std::array<float, 3>);
	void addShininess(float);
	void addDissolved(float);
	std::array<float, 4> getSpecular();
	std::array<float, 4> getDiffuse();
	std::array<float, 4> getAmbient();
	float getShininess();
	void printMaterial();

private:
	std::array<float, 4> specular;
	std::array<float, 4> diffuse;
	std::array<float, 4> ambient;
	float shininess;

};

