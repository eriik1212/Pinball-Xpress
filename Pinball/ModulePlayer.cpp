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
   
    //----------------------------------------Ball
    isDead = false;
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
       

        // Game Overs ----------------------------------------------------
        if (isDead)
        {
            isDead = false;
            if (App->physics->mouse_joint != nullptr)
            {
                App->physics->mouse_joint->GetBodyA()->GetWorld()->DestroyJoint(App->physics->mouse_joint);
                App->physics->mouse_joint = nullptr;
            }
            p2List_item<PhysBody*>* c = App->scene_intro->circles.getFirst();
            while (c != NULL)
            {
                c->data->body->GetWorld()->DestroyBody(c->data->body);
                c = c->next;
            }
            App->scene_intro->circles.clear();

            countBall--;
            App->audio->PlayFx(App->scene_intro->lostLife, 0);
            if (countBall >= 0)
            {
                App->scene_intro->circles.add(App->physics->CreateCircle(360, 630, 8, 0));
                App->scene_intro->circles.getLast()->data->listener = (Module*)App->player;
            }

            if (countBall<0)
                App->audio->PlayFx(App->scene_intro->lostGame, 0);
        }

        return UPDATE_CONTINUE;

}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
    //--------------------------------------------------------------------------------------------------- SCORE INCREMENT
    //BIG CIRCLE
    if (bodyA == App->scene_intro->circles.getLast()->data
        && bodyB == App->physics->bigCercle)
    {
        App->audio->PlayFx(App->scene_intro->bonus_fx);
        App->scene_intro->score += 125;
    }


    //ALL THREE SMALL CIRCLES
    if (bodyA == App->scene_intro->circles.getLast()->data
        && (bodyB == App->physics->smallCercle1
            || bodyB == App->physics->smallCercle2
            || bodyB == App->physics->smallCercle3))
    {
        App->audio->PlayFx(App->scene_intro->bonus_fx);
        App->scene_intro->score += 175;

        //CoMbOOOO!
        if (bodyB == App->physics->smallCercle1)
            App->scene_intro->comboBall1 = true;

        if (bodyB == App->physics->smallCercle2)
            App->scene_intro->comboBall2 = true;

        if (bodyB == App->physics->smallCercle3)
            App->scene_intro->comboBall3 = true;
    }
        

    //MOVING PLATFORM
    if (bodyA == App->scene_intro->circles.getLast()->data
        && bodyB == App->physics->pbody)
    {
        App->audio->PlayFx(App->scene_intro->bonus_fx);
        App->scene_intro->score += 50;
    }


    //ALL FOUR LEFT-SIDE BOXES
    p2List_item<PhysBody*>* c = App->scene_intro->boxes.getFirst();
   
    while (c != NULL)
    {
        if (bodyA == App->scene_intro->circles.getLast()->data
            && bodyB == c->data)
        {
            App->audio->PlayFx(App->scene_intro->bonus_fx);
            App->scene_intro->score += 200;
        }
        c = c->next;
    }

    //HERRADURA
    if (bodyA == App->scene_intro->circles.getLast()->data
        && bodyB == App->scene_intro->herradura)
    {
        App->audio->PlayFx(App->scene_intro->bonus_fx);
        App->scene_intro->score += 100;
    }
  
    //ALL THREE TRIANGLES
    if (bodyA == App->scene_intro->circles.getLast()->data
        && (bodyB == App->scene_intro->smallTriangle1
            || bodyB == App->scene_intro->smallTriangle2
            || bodyB == App->scene_intro->smallTriangle3))
    {
        App->audio->PlayFx(App->scene_intro->bonus_fx);
        App->scene_intro->score += 150;

        //Let's Make a CombOOooOOoOoO
        if (bodyB == App->scene_intro->smallTriangle1)
            App->scene_intro->comboTrian1 = true;

        if (bodyB == App->scene_intro->smallTriangle2)
            App->scene_intro->comboTrian2 = true;
    
        if (bodyB == App->scene_intro->smallTriangle3)
            App->scene_intro->comboTrian3 = true;
    }

    //TWO DOWN-CENTER TRIANGLES
    if (bodyA == App->scene_intro->circles.getLast()->data
        && (bodyB == App->scene_intro->leftCenterTriangle
            || bodyB == App->scene_intro->rightCenterTriangle))
    {
        App->audio->PlayFx(App->scene_intro->bonus_fx);
        App->scene_intro->score += 100;
    }
    if (bodyA == App->scene_intro->circles.getLast()->data
        && bodyB == App->scene_intro->spring)
    {
        App->audio->PlayFx(App->scene_intro->springSound);
    }

    //--------------------------------------------------------------------------------------------------- AUDIO
    //App->audio->PlayFx(App->scene_intro->bonus_fx);
}
