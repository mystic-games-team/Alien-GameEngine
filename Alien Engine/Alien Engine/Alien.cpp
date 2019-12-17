#include "Alien.h"

Alien::Alien()
{
}

Alien::~Alien()
{
}

bool Alien::IsScriptEnabled() const
{
	return (enabled == nullptr) ? false : *enabled;
}

void Alien::SetScriptEnable(const bool& enable)
{
	if (enable != *enabled) {
		*enabled = enable;
		if (enable) {
			OnEnable();
		}
		else {
			OnDisable();
		}
	}
}
