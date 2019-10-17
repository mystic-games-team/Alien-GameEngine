#include "Component.h"

Component::Component(GameObject* attach)
{
	game_object_attached = attach;
}

Component::~Component()
{
}

bool Component::IsEnabled()
{
	return enabled;
}

const ComponentType& Component::GetType() const
{
	return type;
}
