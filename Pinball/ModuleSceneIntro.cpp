#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ballCenter = player = box = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	closeGate = false;
	closeGateBody = nullptr;
	App->player->countBall = 3;
	score = 0;
	Font();

	App->renderer->camera.x = App->renderer->camera.y = 0;

	font = App->textures->Load("pinball/nums_score.png");

	ballCenter = App->textures->Load("pinball/bolaCentre.png");
	player = App->textures->Load("pinball/player.png");
	smallBall = App->textures->Load("pinball/smallBall.png");
	movingRectangle = App->textures->Load("pinball/moving_rectangle.png");
	leftCenterTriangleTexture = App->textures->Load("pinball/textura_triangle2.png");
	rightCenterTriangleTexture = App->textures->Load("pinball/textura_triangle1.png");
	smallTriangle1Texture = App->textures->Load("pinball/triangle_petit1.png");
	smallTriangle2Texture = App->textures->Load("pinball/triangle_petit2.png");
	smallTriangle3Texture = App->textures->Load("pinball/triangle_petit3.png");
	LShapeLeftTexture = App->textures->Load("pinball/textura_vaca1.png");
	LShapeRightTexture = App->textures->Load("pinball/textura_vaca2.png");
	herraduraTexture = App->textures->Load("pinball/herradura.png");
	pinballTexture = App->textures->Load("pinball/pinball_background.png");
	box = App->textures->Load("pinball/quadrats.png");
	springTexture = App->textures->Load("pinball/muelle.png");
	canonTexture = App->textures->Load("pinball/cano.png");
	gateTexture = App->textures->Load("pinball/gate.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	CreateSensor(App->physics->CreateRectangleSensor(185.5f, 750, 125, 10, 0), Sensor::DEATH, false);
	CreateSensor(App->physics->CreateRectangleSensor(348, 196, 16, 4, 2), Sensor::CLOSE_GATE, false);

	// Pivot 2, -751
	int pinball_shape[66] = {
		248, -52,
		348, -111,
		348, -30,
		373, -30,
		373, -544,
		368, -579,
		356, -607,
		337, -633,
		312, -656,
		287, -671,
		260, -684,
		224, -693,
		189, -696,
		156, -693,
		122, -687,
		97, -675,
		72, -660,
		45, -636,
		28, -610,
		13, -584,
		3, -549,
		3, -512,
		10, -482,
		22, -452,
		32, -424,
		32, -326,
		2, -282,
		2, -42,
		34, -42,
		34, -102,
		123, -51,
		123, 20,
		248, 20
	};

	pinballShape.add(App->physics->CreateChainStatic(0, 750, pinball_shape, 66, 0));

	//KICKER JOINT
	kicker = App->physics->CreateRectangleStatic(361, 710, 20, 10, 0, 0);
	App->physics->CreatePrismaticJoint(App->physics->CreateRectangleDynamic(361, 700, 20, 10, 0)->body, kicker->body);

	//Squares At Left-Side
	boxes.add(App->physics->CreateRectangleStatic(30, 340, 10, 10, 1, 0));
	boxes.add(App->physics->CreateRectangleStatic(30, 360, 10, 10, 1, 0));
	boxes.add(App->physics->CreateRectangleStatic(30, 380, 10, 10, 1, 0));
	boxes.add(App->physics->CreateRectangleStatic(30, 400, 10, 10, 1, 0));

	//Rectangles At Left-Down-Side
	spring = App->physics->CreateRectangleStatic(20, 680, 20, 30, 2, 0);

	//Separation Betwenn Ball Spawn and Map
	App->physics->CreateRectangleStatic(348, 440, 2, 440, 0, NULL);

	int triangleShape[6] = {
		5, 400,
		40, 427,
		1, 340,
	};
	// Pivot 0, 0
	int triangleShape1[6] = {
		11, 400,
		-40, 427,
		21, 340,
	};
	int triangleShape2[6] = {
		483, 579,
		501, 568,
		484, 556
	};
	int triangleShape3[6] = {
		483, 569,
		502, 578,
		500, 556
	};
	int triangleShape4[6] = {
		483, 576,
		502, 576,
		492, 556
	};

	int triangleShape5[6] = {
		10, 175,
		50, 165,
		1, 150,
	};

	int triangleShape6[6] = {
		30, 175,
		39, 150,
		-10, 165,
	};

	// CENTER TRIANGLES
	leftCenterTriangle = App->physics->CreateChainStatic(70, 180, triangleShape, 6, 1);
	rightCenterTriangle = App->physics->CreateChainStatic(275, 180, triangleShape1, 6, 1);
	/*triangle_Shape.add(App->physics->CreateChainStatic(70, 180, triangleShape, 6, 1));
	triangle_Shape.add(App->physics->CreateChainStatic(275, 180, triangleShape1, 6, 1));*/

	// THREE TRINAGLES
	smallTriangle1 = App->physics->CreateChainStatic(-200, -245, triangleShape2, 6, 1);
	smallTriangle2 = App->physics->CreateChainStatic(-160, -260, triangleShape3, 6, 1);
	smallTriangle3 = App->physics->CreateChainStatic(-175, -210, triangleShape4, 6, 1);
	/*triangle_Shape.add(App->physics->CreateChainStatic(-200, -245, triangleShape2, 6, 1));
	triangle_Shape.add(App->physics->CreateChainStatic(-160, -260, triangleShape3, 6, 1));
	triangle_Shape.add(App->physics->CreateChainStatic(-175, -210, triangleShape4, 6, 1));*/

	// PALANCAS
	/*triangle_Shape.add(App->physics->CreateChainStatic(110, 500, triangleShape5, 6, 0.5f));
	triangle_Shape.add(App->physics->CreateChainStatic(200, 500, triangleShape6, 6, 0.5f));*/

	int line1[12] = {
		420, 705,
		420, 691,
		363, 645,
		346, 546,
		345, 546,
		345, 652
	};

	int line2[12] = {
		270, 691,
		270, 705,
		363, 652,
		363, 546,
		362, 546,
		345, 645
	};

	/*int line3[4] = {
		348, -111,
		348, -511
	};*/

	LShapeLeft = App->physics->CreateChainStatic(-305, -45, line1, 12, 0);
	LShapeRight = App->physics->CreateChainStatic(-35, -45, line2, 12, 0);
	/*line_Shape.add(App->physics->CreateChainStatic(-305, -45, line1, 12, 0));
	line_Shape.add(App->physics->CreateChainStatic(-35, -45, line2, 12, 0));*/
	//line_Shape.add(App->physics->CreateChainStatic(0, 750, line3, 4)); //S'ha de declarar com a edge

	// Pivot 0, -1024
	int herradura1[72] = {
		366, 895,
		373, 895,
		379, 899,
		382, 903,
		385, 909,
		388, 917,
		402, 905,
		401, 896,
		394, 901,
		388, 885,
		382, 878,
		373, 871,
		361, 871,
		350, 875,
		341, 882,
		334, 889,
		326, 901,
		322, 912,
		319, 925,
		320, 938,
		322, 947,
		327, 953,
		334, 960,
		345, 962,
		350, 964,
		348, 970,
		356, 971,
		362, 958,
		362, 952,
		353, 952,
		343, 946,
		337, 934,
		336, 924,
		339, 911,
		347, 900,
		355, 896
	};

	herradura = App->physics->CreateChainStatic(-250, -660, herradura1, 72, 1);

	int vaquero[34] = {
		356, -607,
		337, -633,
		312, -656,
		273, -640,
		250, -620,
		233, -592,
		230, -580,
		240, -592,
		256, -610,
		267, -615,
		274, -615,
		290, -615,
		320, -610,
		340, -600,
		362, -570,
		373, -544,
		368, -579
	};

	herradura_Shape.add(App->physics->CreateChainStatic(0, 750, vaquero, 34, 0));

	int down_triangles[14] = {
		348,-300,
		348, -380,
		340, -360,
		330, -350,
		315, -340,
		285, -330,
		315, -320
	};

	herradura_Shape.add(App->physics->CreateChainStatic(0, 750, down_triangles, 14, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(pinballTexture);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	//---------------------------------------------------------------------------------------ShootPlatformMovement (KICKER)
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->physics->pJoint->SetMotorSpeed(-0.05*App->physics->prismDef.motorSpeed);
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE)
		App->physics->pJoint->SetMotorSpeed(App->physics->prismDef.motorSpeed);

	//----------------------------------------------------------CloseGate
	if (closeGate && closeGateBody == nullptr)
	{
		closeGateBody = App->physics->CreateRectangleStatic(356, 208, 20, 4, 0, 2);
	}
	else if (!closeGate && closeGateBody != nullptr)
	{
		closeGateBody->body->GetWorld()->DestroyBody(closeGateBody->body);
		closeGateBody = nullptr;
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = pinballShape.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(pinballTexture, 0, 0, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(player, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		/*if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}*/
		c = c->next;
	}



	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	PrintFont();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::Font()
{
	for (int x = 0; x < 10; x++) {
		nums[x] = { x * 22, 0, 23,44 };
	}
}

void ModuleSceneIntro::PrintFont() {

	int score_temp = score;
	for (int i = 8; i >= 0; i--) {

		int temp = score_temp % 10;
		App->renderer->Blit(font, i * 22, 0, &nums[temp]);
		score_temp = score_temp / 10;
	}

}

void ModuleSceneIntro::CreateSensor(PhysBody* sensor, Sensor::sensorValue sensorType, bool isActive)
{
	Sensor* newSensor = new Sensor;
	newSensor->sensor = sensor;
	newSensor->sensor->listener = this;
	newSensor->value = sensorType;
	newSensor->isActive = isActive;
	sensors.add(newSensor);
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	// Sensors
	p2List_item<Sensor*>* s = sensors.getFirst();
	while (s != NULL)
	{

		if (bodyA == s->data->sensor && bodyB->listener == (Module*)App->player)
		{
			App->audio->PlayFx(bonus_fx);

			switch (s->data->value)
			{
			case Sensor::DEATH:
			{
				p2List_item<Sensor*>* reset;
				App->audio->PlayFx(bonus_fx);
				App->player->isDead = true;
				closeGate = false;
				reset = sensors.getFirst();
				return;
			}
			case Sensor::CLOSE_GATE:
			{
				p2List_item<Sensor*>* reset;
				closeGate = true;
			}
				default:
					return;
			}
		}
		s = s->next;
	}

	/*
if(bodyA)
{
	bodyA->GetPosition(x, y);
	App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
}

if(bodyB)
{
	bodyB->GetPosition(x, y);
	App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
}*/
}


