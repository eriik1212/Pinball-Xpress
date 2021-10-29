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

struct Sensor {
	enum sensorValue
	{
		NONE = -1,
		DEATH,
		CLOSE_GATE
	};
	PhysBody* sensor;
	sensorValue value;
	bool isActive;
	int sensorTimer = 0;
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
	void CreateSensor(PhysBody* sensor, Sensor::sensorValue sensorType, bool isActive);

	void PrintFont(int x, int y, uint score_);
	void Font();

public:
	//--------------------------SENSORS
	p2List<Sensor*> sensors;

	bool closeGate = false;

	SDL_Rect looseScreen;
	SDL_Rect cora_;	//--------------------------SCORE
	SDL_Rect High;
	SDL_Rect nums[10];
	uint score = 000000;
	int partida = 0;
	uint highscore = 000000;
	uint previous_score = 000000;
	SDL_Texture* font;

	//----------COMBO!
	bool comboTrian1 = false,
		comboTrian2 = false,
		comboTrian3 = false,
		comboBall1 = false,
		comboBall2 = false,
		comboBall3 = false;

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
	PhysBody* kicker;
	PhysBody* closeGateBody;

	PhysBody* sensor;
	bool sensed;

	Scene currentScene;

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
	SDL_Texture* canonTexture;
	SDL_Texture* gateTexture;
	SDL_Texture* lose_screen;
	SDL_Texture* highscore_;
	SDL_Texture* cora;
	SDL_Texture* flipperR;
	SDL_Texture* flipperL;

	uint bonus_fx,
		flipperSound,
		cannonStart,
		bonusSound,
		lostLife,
		lostGame,
		newLife,
		springSound;

	p2Point<int> ray;
	bool ray_on;
};
