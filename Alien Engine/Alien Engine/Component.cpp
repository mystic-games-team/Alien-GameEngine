#include "Component.h"

Component::Component(GameObject* attach)
{
	game_object_attached = attach;
}

Component::~Component()
{
}

void Component::Enable()
{
	enabled = true;
}

void Component::Disable()
{
	enabled = false;
}

bool Component::IsEnabled()
{
	return enabled;
}

const ComponentType& Component::GetType() const
{
	return type;
}
