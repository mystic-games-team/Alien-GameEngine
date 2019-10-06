#include "Globals.h"
#include "ModuleObjects.h"
#include "Objects.h"
#include "Primitive.h"
#include "glew/include/glew.h"

#include "Cube.h"
#include "Sphere_Alien.h"
#include "Rock.h"

ModuleObjects::ModuleObjects(bool start_enabled):Module(start_enabled)
{
	name.assign("ModuleObject");
}

ModuleObjects::~ModuleObjects()
{
}

bool ModuleObjects::Start()
{
	LOG("Starting Module Objects");
	bool ret = true;

	return ret;
}

update_status ModuleObjects::PreUpdate(float dt)
{
	std::vector<Object*>::iterator iter;
	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter) != nullptr)
		{
			
		}
		else
			++iter;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleObjects::Update(float dt)
{
	std::vector<Object*>::iterator iter;
	for (iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter) != nullptr)
		{

		}
		else
			++iter;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleObjects::PostUpdate(float dt)
{
	std::vector<Object*>::iterator item;
	for (item = objects.begin(); item != objects.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			if ((*item)->num_index == 0 || (*item)->num_vertex == 0 || !(*item)->IsEnabled())
				continue;
			(*item)->Draw();
		}

	}

	return UPDATE_CONTINUE;
}

bool ModuleObjects::CleanUp()
{
	std::vector<Object*>::iterator iter;
	for (iter=objects.begin(); iter!=objects.end(); ++iter)
	{
		if ((*iter) != nullptr)
		{
			delete (*iter);
			(*iter) = nullptr;
		}
	}

	objects.clear();

	return true;
}

Primitive* ModuleObjects::CreatePrimitive(const PrimitiveType& type, const float& position_x, const float& position_y, const float& position_z, const uint &subdivisions, const uint &extra)
{
	Primitive* ret = nullptr;
	switch (type)
	{
	case PrimitiveType::CUBE:
		ret = new Cube(position_x, position_y, position_z);
		ret->type = PrimitiveType::CUBE;
		objects.push_back(ret);
		break;
	case PrimitiveType::SPHERE_ALIEN:
		ret = new Sphere_Alien(position_x, position_y, position_z, subdivisions);
		ret->type = PrimitiveType::SPHERE_ALIEN;
		objects.push_back(ret);
		break;
	case PrimitiveType::ROCK:
		ret = new Rock(position_x, position_y, position_z, subdivisions, extra);
		ret->type = PrimitiveType::ROCK;
		objects.push_back(ret);
		break;
	}

	if (ret != nullptr)
	{
		static_cast<Object*>(ret)->type = ObjectType::PRIMITIVE;
	}

	return ret;
}


