#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "ModuleSceneIntro.h"


#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	//FirstBall
	App->scene_intro->circles.add(CreateCircleBullet(360, 630, 8, 0));
	App->scene_intro->circles.getLast()->data->listener = (Module*)App->player;

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	//Els cercles es mouen amb la camara CANVIAR
	//CERCLE 1 (GRAN)
	bigCercle = CreateCircleStatic(SCREEN_WIDTH/2, 375, 35.5, 1.5f);

	//CERCLE 2 (petit)
	smallCercle1 = CreateCircleStatic(METERS_TO_PIXELS(6.4f), METERS_TO_PIXELS(5.0f), 5, 1);

	//CERCLE 3 (petit)
	smallCercle2 = CreateCircleStatic(METERS_TO_PIXELS(5.6f), METERS_TO_PIXELS(5.15f), 5, 1);

	//CERCLE 4 (petit)
	smallCercle3 = CreateCircleStatic(METERS_TO_PIXELS(4.8f), METERS_TO_PIXELS(5.5f), 5, 1);

	//--------------------------------------------------------------------------MovingRectangle
	bodyRectKine.type = b2_kinematicBody;
	bodyRectKine.position.Set(PIXEL_TO_METERS(70), PIXEL_TO_METERS(475));
	bodyRectKine.linearVelocity = b2Vec2(1.0f, 0.0f);

	bRectKine = world->CreateBody(&bodyRectKine);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(50) * 0.5f, PIXEL_TO_METERS(10) * 0.5f);

	b2FixtureDef fixtureRectKine;
	fixtureRectKine.shape = &box;
	fixtureRectKine.density = 1.0f;
	fixtureRectKine.restitution = 1;

	bRectKine->CreateFixture(&fixtureRectKine);

	bRectKine->SetLinearVelocity(b2Vec2(bodyRectKine.linearVelocity.x, 0));

	pbody = new PhysBody();
	pbody->body = bRectKine;
	bRectKine->SetUserData(pbody);
	pbody->width = 50 * 0.5f;
	pbody->height = 10 * 0.5f;


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && debug == true)
	{
		App->scene_intro->circles.add(CreateCircleBullet(360, 630, 8, 0));
		App->scene_intro->circles.getLast()->data->listener = (Module*)App->player;
	}


	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && debug == true)
	{
		App->scene_intro->circles.add(CreateCircleBullet(App->input->GetMouseX(), App->input->GetMouseY(), 8, 0));
		App->scene_intro->circles.getLast()->data->listener = (Module*)App->player;
	}



	if ((bRectKine->GetPosition().x < PIXEL_TO_METERS(70) && bRectKine->GetLinearVelocity().x < 0) || (bRectKine->GetPosition().x > PIXEL_TO_METERS(300) && bRectKine->GetLinearVelocity().x > 0)) {
		bodyRectKine.linearVelocity.x *= -1;
		bRectKine->SetLinearVelocity(b2Vec2(bodyRectKine.linearVelocity.x, 0));
	}


	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, int restitution)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleStatic(int x, int y, int radius, int restitution)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleBullet(int x, int y, int radius, int restitution)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.bullet = true;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleDynamic(int x, int y, int width, int height, int restitution)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleStatic(int x, int y, int width, int height, int restitution, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	
	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	b->SetTransform(b->GetPosition(), angle);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleKinematic(int x, int y, int width, int height, b2Vec2 linearVelocity)
{
	b2BodyDef body;
	body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.linearVelocity = linearVelocity;


	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b->SetTransform(b->GetPosition(), angle);


	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChainStatic(int x, int y, int* points, int size, int restitution)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateChainDynamic(int x, int y, int* points, int size, int restitution)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}



b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float angle, bool collideConnected, bool enableLimit)
{
	b2RevoluteJointDef RevJointDef;
	RevJointDef.bodyA = A->body;
	RevJointDef.bodyB = B->body;
	RevJointDef.collideConnected = collideConnected;
	RevJointDef.localAnchorA.Set(anchorA.x, anchorA.y);
	RevJointDef.localAnchorA.Set(anchorB.x, anchorB.y);
	RevJointDef.referenceAngle = 0;
	RevJointDef.enableLimit = enableLimit;
	RevJointDef.lowerAngle = -DEG_TO_RAD(angle);
	RevJointDef.upperAngle = DEG_TO_RAD(angle);

	return (b2RevoluteJoint*)world->CreateJoint(&RevJointDef);


}
// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;


	// ------------------------------------------------------------------------------------- Blit Objects
	// Positions
	int bigCercleX, bigCercleY,
		smallCercle1X, smallCercle1Y,
		smallCercle2X, smallCercle2Y,
		smallCercle3X, smallCercle3Y,
		movingRectangleX, movingRectangleY,
		leftCenterTriangleX, leftCenterTriangleY,
		rightCenterTriangleX, rightCenterTriangleY,
		smallTriangle1X, smallTriangle1Y,
		smallTriangle2X, smallTriangle2Y,
		smallTriangle3X, smallTriangle3Y,
		LShapeLeftX, LShapeLeftY,
		LShapeRightX, LShapeRightY,
		herraduraX, herraduraY,
		springX, springY,
		kickerX, kickerY,
		gateX = NULL, gateY = NULL;

	bigCercle->GetPosition(bigCercleX, bigCercleY);
	App->renderer->Blit(App->scene_intro->ballCenter, bigCercleX, bigCercleY, NULL, 1.0f, 0);

	smallCercle1->GetPosition(smallCercle1X, smallCercle1Y);
	smallCercle2->GetPosition(smallCercle2X, smallCercle2Y);
	smallCercle3->GetPosition(smallCercle3X, smallCercle3Y);
	App->renderer->Blit(App->scene_intro->smallBall, smallCercle1X, smallCercle1Y, NULL, 1.0f, 0);
	App->renderer->Blit(App->scene_intro->smallBall, smallCercle2X, smallCercle2Y, NULL, 1.0f, 0);
	App->renderer->Blit(App->scene_intro->smallBall, smallCercle3X, smallCercle3Y, NULL, 1.0f, 0);

	pbody->GetPosition(movingRectangleX, movingRectangleY);
	App->renderer->Blit(App->scene_intro->movingRectangle, movingRectangleX, movingRectangleY, NULL, 1.0f, 0);

	App->scene_intro->leftCenterTriangle->GetPosition(leftCenterTriangleX, leftCenterTriangleY);
	App->renderer->Blit(App->scene_intro->leftCenterTriangleTexture, leftCenterTriangleX, leftCenterTriangleY + 340, NULL, 1.0f, 0);

	App->scene_intro->rightCenterTriangle->GetPosition(rightCenterTriangleX, rightCenterTriangleY);
	App->renderer->Blit(App->scene_intro->rightCenterTriangleTexture, rightCenterTriangleX - 40, leftCenterTriangleY + 340, NULL, 1.0f, 0);

	App->scene_intro->smallTriangle1->GetPosition(smallTriangle1X, smallTriangle1Y);
	App->scene_intro->smallTriangle2->GetPosition(smallTriangle2X, smallTriangle2Y);
	App->scene_intro->smallTriangle3->GetPosition(smallTriangle3X, smallTriangle3Y);
	App->renderer->Blit(App->scene_intro->smallTriangle1Texture, smallTriangle1X + 524, smallTriangle1Y + 540, NULL, 1.0f, 0);
	App->renderer->Blit(App->scene_intro->smallTriangle2Texture, smallTriangle2X + 444, smallTriangle2Y + 570, NULL, 1.0f, 0);
	App->renderer->Blit(App->scene_intro->smallTriangle3Texture, smallTriangle3X + 482, smallTriangle3Y + 556, NULL, 1.0f, 0);

	App->scene_intro->LShapeLeft->GetPosition(LShapeLeftX, LShapeLeftY);
	App->scene_intro->LShapeRight->GetPosition(LShapeRightX, LShapeRightY);
	App->renderer->Blit(App->scene_intro->LShapeLeftTexture, LShapeLeftX + 345, LShapeLeftY + 546, NULL, 1.0f, 0);
	App->renderer->Blit(App->scene_intro->LShapeRightTexture, LShapeRightX + 270, LShapeRightY + 546, NULL, 1.0f, 0);

	App->scene_intro->herradura->GetPosition(herraduraX, herraduraY);
	App->renderer->Blit(App->scene_intro->herraduraTexture, herraduraX + 319, herraduraY + 872, NULL, 1.0f, 0);

	App->scene_intro->spring->GetPosition(springX, springY);
	App->renderer->Blit(App->scene_intro->springTexture, springX, springY, NULL, 1.0f, 0);

	App->scene_intro->kicker->GetPosition(kickerX, kickerY);
	App->renderer->Blit(App->scene_intro->canonTexture, kickerX - 10, kickerY - 60, NULL, 1.0f, 0);

	if (App->scene_intro->closeGate)
	{
		App->scene_intro->closeGateBody->GetPosition(gateX, gateY);
		App->renderer->Blit(App->scene_intro->gateTexture, gateX, gateY, NULL, 1.0f, 114.0);

	}

	if (!debug)
	{
		if (mouse_joint != nullptr)
		{
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
		}
		return UPDATE_CONTINUE;
	}

	

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				p = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
				if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
				{
					mouseBody = b;

					b2Vec2 mousePosition;
					mousePosition.x = p.x;
					mousePosition.y = p.y;

					b2MouseJointDef def;
					def.bodyA = ground;
					def.bodyB = mouseBody;
					def.target = mousePosition;
					def.dampingRatio = 0.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * mouseBody->GetMass();

					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}
		}

	}
	if (mouseBody != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			b2Vec2 mousePosition;
			mousePosition.x = PIXEL_TO_METERS(App->input->GetMouseX());
			mousePosition.y = PIXEL_TO_METERS(App->input->GetMouseY());
			mouse_joint->SetTarget(mousePosition);

			App->renderer->DrawLine(METERS_TO_PIXELS(mouseBody->GetPosition().x), METERS_TO_PIXELS(mouseBody->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		{
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
			mouseBody = nullptr;
		}
	}

	if (App->player->countBall < 0) {
		App->renderer->Blit(App->scene_intro->lose_screen, 60, 110, &App->scene_intro->looseScreen);
		App->scene_intro->PrintFont(150,200);
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::CreatePrismaticJoint(b2Body* bodyA, b2Body* bodyB)
{
	
	//-----------------------------------------------------------------------------PrismaticJoint

	prismDef.Initialize(bodyA, bodyB, bodyA->GetPosition(), b2Vec2(0, 1));
	prismDef.lowerTranslation = 0;
	prismDef.upperTranslation = 1;
	prismDef.enableLimit = true;
	prismDef.maxMotorForce = 20;
	prismDef.motorSpeed = 20.0;
	prismDef.enableMotor = true;


	pJoint = (b2PrismaticJoint*) world->CreateJoint(&prismDef);

}