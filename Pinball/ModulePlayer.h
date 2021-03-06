#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct Flipper
{
	PhysBody* Rect;
	PhysBody* Circle;
	bool rightSide;
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	PhysBody* Rflipper;
	p2List<Flipper*> flippers;
	SDL_Rect rectSect = { 48, 250, 64, 20 };

	//---------------------------------------Ball
	int countBall = 3;
	bool isDead = false;
};