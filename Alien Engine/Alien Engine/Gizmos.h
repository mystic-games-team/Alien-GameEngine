#pragma once

#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include <vector>
#include "Color.h"
#include <tuple>
#include "ModuleObjects.h"

class ComponentMesh;
class ResourceMesh;
enum class PrimitiveType;

class __declspec(dllexport) Gizmos {
	friend class Gizmos;
	friend class ModuleObjects;
public:

	static void DrawCube(const float3& position, const float3& size, const Color& color);
	static void DrawWireCube(const float3& position, const float3& size, const Color& color, float line_width = 2.0F);
	static void DrawSphere(const float3& position, const float& radius, const Color& color);
	static void DrawWireSphere(const float3& position, const float& radius, const Color& color, float line_width = 2.0F);
	static void DrawLine(const float3& from, const float3& to, const Color& color, float line_width = 3.0F);
	static void DrawWireMesh(const ComponentMesh * mesh, const float4x4& global_transform, const Color& color, float line_width = 2.0F);
	// TODO: when UI available
	// static void DrawGUITexture();

private:

	static void DrawPoly(const ResourceMesh * mesh, const float4x4& matrix, const Color& color);
	static void DrawWire(const ResourceMesh * mesh, const float4x4& matrix, const Color& color, const float& line_width);
	static void RemoveGizmos();
	static void ClearAllCurrentGizmos();

private:
	struct Gizmo {
		Gizmo(ResourceMesh* mesh, bool control, PrimitiveType type) {
			this->mesh = mesh;
			controller = control;
			this->type = type;
		}
		ResourceMesh* mesh = nullptr;
		bool controller = true;
		PrimitiveType type = PrimitiveType::UNKONWN;
	};
	static std::vector<Gizmo> active_gizmos;
	static bool controller;
};
