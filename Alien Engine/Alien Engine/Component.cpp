#include "Component.h"

Component::Component()
{
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
