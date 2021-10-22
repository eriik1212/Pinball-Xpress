#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include "Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, int restitution);
	PhysBody* CreateCircleBullet(int x, int y, int radius, int restitution);
	PhysBody* CreateRectangleDynamic(int x, int y, int width, int height, int restitution);
	PhysBody* CreateRectangleStatic(int x, int y, int width, int height, int restitution);
	PhysBody* CreateRectangleKinematic(int x, int y, int width, int height, b2Vec2 linearVelocity);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChainStatic(int x, int y, int* points, int size, int restitution);
	PhysBody* CreateChainDynamic(int x, int y, int* points, int size, int restitution);

	PhysBody* ballBullet;

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//MovingRectangle
	b2BodyDef bodyRectKine;
	b2Body* bRectKine;

	// CreateJoints
	// Prismatic Joint
	b2PrismaticJointDef prismDef;
	b2PrismaticJoint* pJoint;
	void CreatePrismaticJoint(b2Body* bodyA, b2Body* bodyB);
	void CreateRevoluteJoint(b2Body* bodyA, b2Body* bodyB);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};