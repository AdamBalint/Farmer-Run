#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

//returns the height the player is currently at
float Player::getHeight(){
	return height;
}

//updates the player
void Player::update(){
	//if the player is jumping and not at the max limit or height, then add to the height, otherwise set falling to true
	if (jumping && !falling){
		if (!(jumpTimer == 30 || height + 0.0001 > 7.5)){
			height += 0.25;
			jumpTimer++;
		}else{
			falling = true;
		}
	// if the the player is falling then subtract from the height
	}else if (jumpTimer != 0 || falling){
		height -= 0.25;
	}
	//if the player is sliding then increase the slide timer, and if it is older the limit, make the player stop sliding
	else if (sliding){
		slideTimer++;
		if (slideTimer == 50){
			sliding = false;
		}
		//if the player is not sliding and the slide timer is not 0, then count down as the cool down
	}if (!sliding && slideTimer > 0){
		slideTimer -= 3;
		if (slideTimer < 0)
			slideTimer = 0;
	}
	//if the player is not jumping, or sliding or falling then increase the run animation
	//each frame is displayed 2 times to slow down the animation
	if (!sliding && !falling && !jumping){
		modelRunAnimate = (modelRunAnimate+1)%(modelRunSize*2);
	}

	//if they are on the ground then reset variables related to jumping
	if (height < 0){
		height = 0;
		jumpTimer = 0;
		jumping = false;
		falling = false;
	}


}

//set if the player is jumping or not
void Player::setJumping(bool jump){
	if (!jump){
		jumping = false;
		falling = true;
		modelRunAnimate = 0;
	}
	else if (!falling){
		jumping = true;
		falling = false;
		sliding = false;
		modelRunAnimate = 0;
	}
}

//set if the player is sliding or not
void Player::setSliding(bool slide){
	if (slide && slideTimer == 0 && (!jumping || !falling)){
		modelRunAnimate = 0;
		sliding = true;
	}
	else if (!slide)
		sliding = false;
}

//set the model number information for the player
void Player::setModelNum(int rnum, int rsize, int snum, int jnum){
	modelRun = rnum;
	modelRunSize = rsize;
	modelSlide = snum;
	modelJump = jnum;
}
//get the model number based on what the player is doing
int Player::getModelNum(){
	if (jumping || falling)
		return modelJump;//return jumping
	else if (sliding && !falling)
		return modelSlide;// return sliding
	return modelRun + (modelRunAnimate/2); //return running (divided by 2 because of the displaying 2 times)
}

// get if the player is sliding
bool Player::isSliding(){
	return sliding;
}

//get if the player is jumping
bool Player::isJumping(){
	return jumping;
}