#pragma once

#include "Component.h"

class ComponentMesh : public Component {
public:

	ComponentMesh();
	virtual ~ComponentMesh();

	void Update();

public:

	// buffers id
	uint id_index = 0;
	uint id_vertex = 0;
	uint id_normals = 0;
	uint id_uv = 0;
	// buffers size
	uint num_index = 0;
	uint num_vertex = 0;
	uint num_faces = 0;
	// buffers
	uint* index = nullptr;
	float* vertex = nullptr;
	float* normals = nullptr;
	float* uv_cords = nullptr;
	float* center_point_normal = nullptr;
	float* center_point = nullptr;

};