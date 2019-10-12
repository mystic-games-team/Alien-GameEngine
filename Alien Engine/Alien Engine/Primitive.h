#pragma once

#include "GameObject.h"
#include "Shapes.h"
#include "Color.h"
#include "glmath.h"

enum class PrimitiveType
{
	CUBE,
	SPHERE_ALIEN,
	ROCK,
	DODECAHEDRON,
	OCTAHEDRON,
	TORUS,
	ICOSAHEDRON,

	UNKONWN
};

class Primitive : public GameObject
{
public:
	Primitive();
	virtual ~Primitive();

	PrimitiveType type = PrimitiveType::UNKONWN;

	void SetPosition(const float& position_x, const float& position_y, const float& position_z);

	void SetSubdivisions(const int& subdivisions);

protected:

	void InitBuffers();
	void MemCpy();
	void RestartBuffers();

protected:
	par_shapes_mesh* shape = nullptr;
	int subdivisions = 5;

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