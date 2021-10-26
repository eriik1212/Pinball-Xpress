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
    Flipper* f = new Flipper;
    f->Circle = App->physics->CreateCircle(SCREEN_WIDTH/2+20, 390, 50, b2_staticBody);
    f->Rect = App->physics->CreateRectangleDynamic(72 + rectSect.w / 2, 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, 0);
    f->rightSide = false;
    App->physics->CreateRevoluteJoint(f->Rect, a, f->Circle, b, 35.0f);
    flippers.add(f);

    a = { 0.44,0 };

    Flipper* f2 = new Flipper;
    f2->Circle = App->physics->CreateCircle(PIXEL_TO_METERS(110), PIXEL_TO_METERS(600), 4, 0);
    f2->Rect = App->physics->CreateRectangleDynamic(PIXEL_TO_METERS(216 - rectSect.w / 2), 858 + rectSect.h / 2, rectSect.w, rectSect.h - 10, 0);
    f2->rightSide = true;
    App->physics->CreateRevoluteJoint(f2->Rect, a, f2->Circle, b, 35.0f);
    flippers.add(f2);
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
            p2List_item<Flipper*>* f = flippers.getFirst();
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
            p2List_item<Flipper*>* f = flippers.getFirst();
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



