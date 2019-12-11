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
	friend class ModuleObjects;
public:

	static void DrawCube(float3 position, float3 size, Color color);
	static void DrawSphere(float3 position, float radius, Color color);

private:

	static void DrawPoly(ResourceMesh* mesh, const float4x4& matrix, const Color& color);
	static void RemoveGizmos();
	static void ClearAllCurrentGizmos();

private:
	struct Gizmo {
		ResourceMesh* mesh = nullptr;
		bool controller = true;
		PrimitiveType type = PrimitiveType::UNKONWN;
	};
	static std::vector<Gizmo> active_gizmos;
	static bool controller;
};
