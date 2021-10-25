#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;

    b2Vec2 a = { -0.44, 0 };
    b2Vec2 b = { 0, 0 };

    //Implementació dels flippers
    Flipper* flipperL = new Flipper;
    flipperL->Circle = App->physics->CreateCircle(110, 500, 4, b2_staticBody);
    flipperL->Rect = App->physics->CreateRectangleDynamic(72 + rectSect.w / 2, 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, 0);
    flipperL->rightSide = false;
    App->physics->CreateRevoluteJoint(flipperL->Rect, a, flipperL->Circle, b, 35.0f);
    flippers.add(flipperL);

    a = { 0.44,0 };

    Flipper* flipperR = new Flipper;
    flipperR->Circle = App->physics->CreateCircle(PIXEL_TO_METERS(110), PIXEL_TO_METERS(600), 4, 0);
    flipperR->Rect = App->physics->CreateRectangleDynamic(PIXEL_TO_METERS(216 - rectSect.w / 2), 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, 0);
    flipperR->rightSide = true;
    App->physics->CreateRevoluteJoint(flipperR->Rect, a, flipperR->Circle, b, 35.0f);
    flippers.add(flipperR);
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
    // Flippers --------------------------------------------------
    if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
    {
        p2List_item<Flipper*> *f = flippers.getFirst();
        while (f != NULL)
        {
            if (f->data->rightSide == false)
            {
                f->data->Rect->body->ApplyForce({ -3,0 }, { 0,0 }, true);
            }
            f = f->next;
        }
    }
    if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
    {
        p2List_item<Flipper*> *f = flippers.getFirst();
        while (f != NULL)
        {
            if (f->data->rightSide == true)
            {
                f->data->Rect->body->ApplyForce({ 3,0 }, { 0,0 }, true);
            }
            f = f->next;
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
    {
        p2List_item<Flipper*> *f = flippers.getFirst();
        while (f != NULL)
        {
            if (f->data->rightSide == true)
            {
                f->data->Rect->body->ApplyForce({ 3,0 }, { 0,0 }, true);
            }
            f = f->next;
        }
    }
    return UPDATE_CONTINUE;
}



