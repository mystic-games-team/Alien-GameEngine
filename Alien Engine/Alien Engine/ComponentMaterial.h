#pragma once

#include "Component.h"
#include "Color.h"

class ComponentMaterial : public Component {
public:
	ComponentMaterial();
	virtual ~ComponentMaterial();
	
	void BindTexture();

public:

	uint id_texture = 0;
	uint material_index = 0;
	Color color{ 1,1,1 };

};