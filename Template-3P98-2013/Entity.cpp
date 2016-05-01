#include "Entity.h"


Entity::Entity()
{
}


Entity::~Entity()
{
}

//sets the initial translation
void Entity::initTranslate(float x, float y, float z){
	loc[0] = x;
	loc[1] = y;
	loc[2] = z;
}

//sets the initial rotation
void Entity::initRotate(float num, float x, float y, float z){
	rot[0] = num*x;
	rot[1] = num*y;
	rot[1] = num*z;
}

//sets the velocity
void Entity::setVelocity(float vx, float vy, float vz){
	vel[0] = vx;
	vel[1] = vy;
	vel[2] = vz;
}

//sets the model number
void Entity::setModelNum(int mNum){
	modelNum = mNum;
}

//return the model number
int Entity::getModelNum(){
	return modelNum;
}

//updates the entity
void Entity::update(){
	loc[0] += vel[0];
	loc[1] += vel[1];
	loc[2] += vel[2];
}

//returns the location
float* Entity::getLocation(){
	return loc;
}

