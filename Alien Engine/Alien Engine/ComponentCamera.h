#pragma once

#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "ComponentMesh.h"

class __declspec(dllexport) ComponentCamera : public Component {
	friend class GameObject;
	friend class ModuleCamera3D;
	friend class ReturnZ;
	friend class CompZ;
	friend class ModuleRenderer3D;
	friend class PanelRender;
	friend class PanelScene;
	friend class ModuleObjects;
	friend class RayCreator;
	friend class Octree;
	friend class OctreeNode;
public:

	ComponentCamera(GameObject* attach);
	virtual ~ComponentCamera();

	void Look(const float3& position_to_look);

	float* GetProjectionMatrix() const;

	float* GetViewMatrix() const;

	void SetVerticalFov(const float& vertical_fov);
	float GetVerticalFov() const;
	void SetHorizontalFov(const float& horitzontal_fov);
	float GetHorizontalFov() const;

	void SetFarPlane(const float& far_plane);
	void SetNearPlane(const float& near_plane);
	float GetFarPlane() const;
	float GetNearPlane() const;

	void SetCameraPosition(const float3& position);
	float3 GetCameraPosition() const;

private:
	void AspectRatio(int width_ratio, int height_ratio, bool fov_type = 0);

	bool DrawInspector();

	void DrawFrustum();
	void DrawIconCamera();

	void Clone(Component* clone);

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void Reset();
	void SetComponent(Component* component);

private:
	Frustum frustum;

	float vertical_fov = 60.0f;
	float horizontal_fov = 91.0f;

	bool projection_changed = false;

	float far_plane = 200.f;

	float near_plane = 0.1f;

	int is_fov_horizontal = 0;

	ComponentMesh* mesh_camera = nullptr;

	bool print_icon = true;
	Color camera_icon_color = { 0.85f,0.85f,0.85f, 1.0F };

	float4x4 ViewMatrix = float4x4::identity();
	float4x4 ViewMatrixInverse = float4x4::identity();

public:
	Color camera_color_background{ 0.05f, 0.05f, 0.05f, 1.0f };
};
