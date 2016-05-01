/*
Adam Balint
5141619
2016/01/11
*/

#if !defined(Linux)
#include <windows.h>           //Not Linux must be windows
#else
#include <stdlib.h>
#endif
#include <GL/gl.h>
#include <stdio.h>
#include <freeglut.h>
#include <iostream>
#include "Model.h"
#include "ModelLoader.h"
#include "Entity.h"
#include "Player.h"
#include <time.h>

//defines global variables
struct glob {
	GLuint list; //list pointing to models
	std::vector<Entity> entList; //list of enemies
	Entity pig; //stores the model number for the pig (used later)
	Entity cloud;//stores the model number for the cloud (used later)
	Player per; //stores the person
	bool running = true; //the game running flag
	int counter, nextSpawn = 200; //spawn timer for the enemy, and count limit for when to spawn enemy
	int score = 0; //set the score of the player
	int levelUp = 0; //set the level up counter
	int level = 0; //set the game level
	int pauseTime = 0; //the time to sleep before the next frame. Used to keep some animation consistancy on the school computers
	//models for the game
	Model* m_player;
	Model* m_pig;
	Model* m_sheep;
	Model* m_bird;
	Model* m_cloud;
} global;

//resets the game
void resetGame(){
	global.entList = std::vector<Entity>();
	global.nextSpawn = 200;
	global.counter = 0;
	global.score = 0;
	global.level = 0;
	global.levelUp = 0;
	global.pauseTime = 10;
	global.nextSpawn = 200;
	global.running = true;
}

//checks if the player leveled up
void checkLevelUp(){
	if (global.levelUp == 20){
		global.levelUp = 0;
		global.level++;
		printf("----------LEVEL UP!----------\n");
	}
}

//check for collisions with the enemy
bool checkCollision(){

	for (int i = global.entList.size() - 1; i >= 0; i--){
		float* entLoc = global.entList.at(i).getLocation();
		float playerHeight = global.per.getHeight();
		int modelNum = global.entList.at(i).getModelNum();
		if (entLoc[2] < -40){ //checks if it can delete animals. 
			//if an animal is deleted, the score goes up. essentially the player gets 1 score per animal they jumped over
			global.entList.erase(global.entList.begin() + i);
			global.score++;
			global.levelUp++;
			global.nextSpawn -= 10;
			global.counter = 0;
			if (global.nextSpawn < 80)
				global.nextSpawn = 80;
			global.pauseTime--;
			if (global.pauseTime < 5)
				global.pauseTime = 5;

			printf("Score: %d\n", global.score);
		}
		if (std::abs(0 - entLoc[2]) < 0.001){ //sheep collision
			if (modelNum == 0 && playerHeight < 0.8)
				return true;
			else if (modelNum == 2 && !global.per.isSliding()) // pig & bird collision
				return true;
			else if (modelNum == 1 && (playerHeight < 3.5 && !global.per.isSliding())) //pig collision
				return true;
		}


	}

	return false;
}


//adds an enemy to the list
void addEnemy(){
	int num = rand() % 3;
	if (num == 0){
		Entity sheep; //adds sheep to enemy list
		sheep.setModelNum(0);
		sheep.initTranslate(0, 0, 50);
		sheep.setVelocity(0, 0, -0.25);
		global.entList.push_back(sheep);
	}
	else if (num == 1){
		Entity pig; //adds a pig to the enemy list
		pig.setModelNum(1);
		pig.initTranslate(0, 0, 50);
		pig.setVelocity(0, 0, -0.25);
		global.entList.push_back(pig);
	}
	else {
		Entity bird; //adds the pig with the bird to the enemy list
		bird.setModelNum(2);
		bird.initTranslate(0, 0, 50);
		bird.setVelocity(0, 0, -0.25);
		global.entList.push_back(bird);
	}
}

//checks if an enemy will be added
void checkSpawning(){
	
	if (global.counter == global.nextSpawn){
		addEnemy();
		global.counter = 0;
	}
	global.counter++; //increments enemy spawning counter
}

//sets up the lighting
void setUpLight(){
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat specular[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat position[] = { 0.0, 50.0, 150.0, 0.0 };
	GLfloat lmodelAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat localView[] = { 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	GLfloat amb[] = { 0, 0, 0, 1 };
	GLfloat dif[] = { 0.3, 0.3, 0.3, 1 };
	GLfloat spec[] = { 0, 0, 0, 1 };
	GLfloat pos[] = { 200, 150, 0, 1 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodelAmbient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, localView);

	glEnable(GL_LIGHTING);   /* turns on lighting */
	glEnable(GL_LIGHT0);     /* turns on light 0  */
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
}

//draws the background, grass, path, sky and clouds
void drawBackground(){
	glColor3f(0, 0, 0);
	GLfloat ambient[] = { 44/255.0, 176/255.0, 55/255.0, 1.0 };
	GLfloat diffuse[] = { 44 / 255.0, 176 / 255.0, 55 / 255.0, 1.0 };
	GLfloat specular[] = { 0.0, 0.01, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 1);

	//draws grass
	glBegin(GL_QUADS);
	glVertex3d(300, 0, 200);
	glVertex3d(300, 0, -300);
	glVertex3d(-50, 0, -300);
	glVertex3d(-50, 0, 200);
	glEnd();

	GLfloat ambient2[] = { 0.99, 0.86, 0.60, 1.0 };
	GLfloat diffuse2[] = { 0.99, 0.86, 0.60, 1.0 };
	GLfloat specular2[] = { 0.0, 0.01, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	
	//draws path
	glBegin(GL_QUADS);
	glVertex3d(10, 0.01, 200);
	glVertex3d(10, 0.01, -200);
	glVertex3d(-10, 0.01, -200);
	glVertex3d(-10, 0.01, 200);
	glEnd();

	GLfloat ambient3[] = { 0, 0.7, 1, 1.0 };
	GLfloat diffuse3[] = { 0, 0.7, 1, 1.0 };
	GLfloat specular3[] = { 0.0, 0.01, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular3);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);

	//draws sky
	glBegin(GL_QUADS);
	glVertex3d(300, 300, 200);
	glVertex3d(300, 300, -300);
	glVertex3d(300, 0, -300);
	glVertex3d(300, 0, 200);
	glEnd();

	//draws clouds
	glPushMatrix();
	glTranslated(100,10,0);
	glRotated(90, 0, 1, 0);
	glCallList(global.list + global.cloud.getModelNum());
	glPopMatrix();

	glPushMatrix();
	glTranslated(20, 10, 35);
	glRotated(90, 0, 1, 0);
	glCallList(global.list + global.cloud.getModelNum());
	glPopMatrix();

	glPushMatrix();
	glTranslated(40, 15, -15);
	glRotated(90, 0, 1, 0);
	glCallList(global.list + global.cloud.getModelNum());
	glPopMatrix();
}

//main game loop
void draw(void) {
	//while the game is running, play the game
	if (global.running){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//draws the player at the player's height
		glPushMatrix();
		glTranslated(0, global.per.getHeight(), 0);
		glCallList(global.list + global.per.getModelNum());
		glPopMatrix();
		
		//draws and updates the animals
		for (int i = 0; i < global.entList.size(); i++){
			glPushMatrix();
			float *tmp = global.entList.at(i).getLocation();
			glTranslated(tmp[0], tmp[1], tmp[2]);
			int modelNum = global.entList.at(i).getModelNum();
			glRotated(modelNum == 0 ? 180 : 0, 0, 1, 0);
			glCallList(global.list + modelNum);
			global.entList.at(i).update();
			glPopMatrix();
		}
	
		//updates the player
		global.per.update();

		//draws a pig off the screen to keep the color of the background consistent
		glPushMatrix();
		glTranslated(global.pig.getLocation()[0], global.pig.getLocation()[1], global.pig.getLocation()[2]);
		glCallList(global.list + global.pig.getModelNum());
		
		drawBackground();
		glPopMatrix();
		
		//if there is a collision, then the game is stopped
		if (checkCollision()){
			global.running = false;
			printf("***************************************\n");
			printf("************You have lost**************\n");
			printf("**********Press R to restart***********\n");
			printf("***************************************\n");

		}
		//check for spawning and level up
		checkSpawning();
		checkLevelUp();

		Sleep(global.pauseTime); //sleeps to cause more consistent animation on school computers
		glutSwapBuffers();
	}
}

//loads all the models for the game
void loadModels (){
	ModelLoader ml;
	global.list = glGenLists(19);

	//loads the sheep model
	printf("Load Sheep\n");
	global.m_sheep = ml.loadModel("Models/sheep/sheep3.obj");
	(*global.m_sheep).addToList(&global.list, 0);

	//loads the pig model
	printf("Load Pig\n");
	global.m_pig = ml.loadModel("Models/pig/pig-2-4-stilts.obj");
	(*global.m_pig).addToList(&global.list, 1);

	//loads the pig & bird model
	printf("Load Pig & Bird\n");
	global.m_bird = ml.loadModel("Models/pig/pig-4-stilts-bird-7.obj");
	(*global.m_bird).addToList(&global.list, 2);
	
	//loads the cloud model
	printf("Load Cloud\n");
	global.m_cloud = ml.loadModel("Models/Background/Cloud.obj");
	(*global.m_cloud).addToList(&global.list, 3);
	global.cloud.setModelNum(3);

	//loads all models for the player 
	printf("Load Player\n");
	printf("Player Running\n");
	for (int i = 0; i < 12; i++){
		std::stringstream ss;
		ss << "Models/Person/Run/GuyRun" << i << ".obj"; //generates name of the running animation frame to load
		std::string loc;
		ss >> loc;
		
		char* c = new char[loc.size() + 1];
		strcpy_s(c, loc.size() + 1, loc.c_str());
		printf("%s\n",c);
		global.m_player = ml.loadModel(c);
		(*global.m_player).addToList(&global.list, (4+i));
		delete[] c;
	}
	printf("Player Sliding\n");
	global.m_player = ml.loadModel("Models/Person/Slide/GuySlide.obj");
	(*global.m_player).addToList(&global.list, 17);
	printf("Player Jumping\n");
	global.m_player = ml.loadModel("Models/Person/Jump/GuyJump.obj");
	(*global.m_player).addToList(&global.list, 18);
	global.per.setModelNum(4, 12, 17, 18);

}

//checks for keyboard input to restart the game
void keyboard(unsigned char key, int x, int y) {
	switch (key){
	case 27:
		exit(0);
		break;
	case 'r':
	case 'R':
		if (!global.running){
			printf("***************************************\n");
			printf("*************Game Started**************\n");
			printf("***************************************\n");
			resetGame();
		}
		break;
	}
	
}

//check for the up arrow and down arrow key pressed events
void keyboard_spcl(int key, int x, int y){
	//printf("Key Event Key: %d\n", key);
	switch (key)
	{
	case GLUT_KEY_UP:
		global.per.setJumping(true);
		break;
	case GLUT_KEY_DOWN:
		global.per.setSliding(true);
		break;
	default:
		break;
	}

}

//checks for the up arrow and down arrow key released events
void keyboardUp_spcl(int key, int x, int y){
	switch (key)
	{
	case GLUT_KEY_UP:
		global.per.setJumping(false);
		break;
	case GLUT_KEY_DOWN:
		global.per.setSliding(false);
		break;
	default:
		break;
	}
}


//initializes frame and sets up everything
int main(int argc, char **argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("Farmer Run");

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_spcl);
	glutSpecialUpFunc(keyboardUp_spcl);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(30, 2, 1, 5000);
	gluLookAt(20, 10, 40, 15, 5, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	setUpLight();

	glRotated(90, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	srand(time(NULL));
	loadModels();
	addEnemy();
	
	global.pig.setModelNum(0);
	global.pig.initTranslate(0,0,100);

	printf("***************************************\n");
	printf("*************Game Started**************\n");
	printf("***************************************\n");
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutMainLoop();
}
