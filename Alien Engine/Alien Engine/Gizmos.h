#pragma once

#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>
#include "Color.h"
#include <tuple>
#include "ModuleObjects.h"

class ResourceMesh;
enum class PrimitiveType;

class __declspec(dllexport) Gizmos {
	friend class Gizmos;
public:


	// TODO: after calling all scripts OnDrawGizmos && OnDrawGizmosSelected, iterate and look for the bool to erase
	// TODO: clear gizmos in change scene
	static void DrawCube(float3 position, float3 size, Color color);

private:

	static void DrawPoly(ResourceMesh* mesh, const float4x4& matrix, const Color& color);

private:
	struct Gizmo {
		ResourceMesh* mesh = nullptr;
		bool is_in_use = false;
		PrimitiveType type = PrimitiveType::UNKONWN;
	};
	static std::vector<Gizmo> active_gizmos;
};
