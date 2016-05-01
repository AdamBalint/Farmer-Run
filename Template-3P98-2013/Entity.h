//Holds an enemy that moves along the screen
#pragma once
#include "Model.h"
class Entity
{
public:
	Entity();
	~Entity();

	void initTranslate(float, float, float);
	void initRotate(float, float, float, float);
	void setVelocity(float, float, float);
	void setModelNum(int);
	int getModelNum();
	void update();
	float* getLocation();

private:
	float loc[3]; //the location of the entity
	float vel[3]; //the velocity of the entity
	float rot[3]; //the rotation of the entity
	int modelNum; //the model number

};

