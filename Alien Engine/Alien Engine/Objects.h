#pragma once

#include "Globals.h"
#include "glmath.h"
#include "Color.h"

enum class ObjectType
{
	PRIMITIVE,
	CAMERA,

	UNKONWN
};

class Object
{
public:
	Object();
	virtual ~Object();

	void ChangeEnable();
	bool IsEnabled();

	void DrawPolygon();
	void DrawMesh();
	void DrawVertexNormals();
	void DrawFaceNormals();

	vec3 GetPosition();

	void SetColor(Color color);

protected:
	bool enabled = true;

public:

	ObjectType type = ObjectType::UNKONWN;

	vec3 position = { 0,0,0 };

	uint id_index = 0;
	uint id_vertex = 0;

	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	float* vertex = nullptr;

	uint material_index = 0;

	float* normals = nullptr;
	uint id_normals = 0;

	float* center_point_normal = nullptr;
	float* center_point = nullptr;
	uint num_faces = 0;

	float* uv_cords = nullptr;
	uint id_uv = 0;

	int id_texture = -1;

	Color color{ 1,1,1 };
};