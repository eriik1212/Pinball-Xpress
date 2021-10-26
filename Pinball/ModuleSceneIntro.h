#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void imprimir_fuente();
	void fuente();

public:

	//SCORE
	SDL_Rect nums[10];
	uint score = 000000;
	SDL_Texture* font;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> pinballShape;
	p2List<PhysBody*> herradura_Shape;

	PhysBody* leftCenterTriangle;
	PhysBody* rightCenterTriangle;
	PhysBody* smallTriangle1;
	PhysBody* smallTriangle2;
	PhysBody* smallTriangle3;
	PhysBody* LShapeLeft;
	PhysBody* LShapeRight;
	PhysBody* herradura;
	PhysBody* spring;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* ballCenter;
	SDL_Texture* player;
	SDL_Texture* smallBall;
	SDL_Texture* smallTriangle1Texture;
	SDL_Texture* smallTriangle2Texture;
	SDL_Texture* smallTriangle3Texture;
	SDL_Texture* movingRectangle;
	SDL_Texture* leftCenterTriangleTexture;
	SDL_Texture* rightCenterTriangleTexture;
	SDL_Texture* LShapeLeftTexture;
	SDL_Texture* LShapeRightTexture;
	SDL_Texture* herraduraTexture;
	SDL_Texture* box;
	SDL_Texture* springTexture;
	SDL_Texture* pinballTexture;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
