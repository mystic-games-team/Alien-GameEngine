#pragma once

typedef unsigned int uint;

enum class ComponentType {
	TRANSFORM,
	MESH,
	MATERIAL,

	UNKNOWN
};

class Component {
public:

	Component();	
	virtual ~Component();

	virtual void Update() {};
	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled();

	const ComponentType& GetType() const;

protected:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;

};
