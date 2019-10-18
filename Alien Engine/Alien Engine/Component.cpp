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

void Component::OnDisable()
{
	enabled = false;
}

const ComponentType& Component::GetType() const
{
	return type;
}
