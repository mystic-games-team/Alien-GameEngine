#include "ComponentScript.h"

ComponentScript::ComponentScript(GameObject* attach) : Component(attach)
{
	type = ComponentType::SCRIPT;
}

ComponentScript::~ComponentScript()
{
}

void ComponentScript::Reset()
{
}

void ComponentScript::SetComponent(Component* component)
{
}

bool ComponentScript::DrawInspector()
{
	return true;
}

void ComponentScript::SaveComponent(JSONArraypack* to_save)
{
}

void ComponentScript::LoadComponent(JSONArraypack* to_load)
{
}
