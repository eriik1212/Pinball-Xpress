#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

enum Scene
{
	TITLE_SCREEN,
	PINBALL,
	GAME_OVER
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> pinballShape;
	p2List<PhysBody*> triangle_Shape;
	p2List<PhysBody*> line_Shape;
	p2List<PhysBody*> herradura_Shape;

	PhysBody* leftCenterTriangle;
	PhysBody* rightCenterTriangle;

	PhysBody* sensor;
	bool sensed;

	Scene currentScene;

	SDL_Texture* ballCenter;
	SDL_Texture* player;
	SDL_Texture* smallBall;
	SDL_Texture* smallTriangle1;
	SDL_Texture* movingRectangle;
	SDL_Texture* leftCenterTriangleTexture;
	SDL_Texture* box;
	SDL_Texture* pinballTexture;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
