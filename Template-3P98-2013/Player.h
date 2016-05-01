//holds all the information for a player
#pragma once
class Player
{
public:
	Player();
	~Player();

	float getHeight();
	void update();

	void setJumping(bool);
	void setSliding(bool);
	void reachedGround();

	void setModelNum(int, int, int, int);
	int getModelNum();
	bool isSliding();
	bool isJumping();

private:
	
	float height; //height for jumping
	int jumpTimer; //timer for how high player can jump
	int slideTimer; //timer for how long players can slide
	int modelJump; //the location of the jump model in the display list
	int modelSlide; //location of the slide model in the display list
	int modelRun; //location of the run model
	int modelRunSize; //size of the animation for running
	int modelRunAnimate = 0; //which frame the animation is currently on
	bool jumping; //flag for jumping
	bool sliding; //flag for sliding
	bool falling; //flag for falling
};

