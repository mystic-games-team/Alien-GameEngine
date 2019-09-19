#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
	

	
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[i].radius;
		wheel.height = info.wheels[i].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis.color = Green;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	chassis.Render();

	Cylinder aileron(info.aileron_radius, info.aileron_height);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron.transform);
	btQuaternion j = vehicle->getChassisWorldTransform().getRotation();
	aileron.color = Red;
	btVector3 Offset(info.aileron_offset.x, info.aileron_offset.y, info.aileron_offset.z);
	Offset = Offset.rotate(j.getAxis(), j.getAngle());

	aileron.transform.M[12] += Offset.getX();
	aileron.transform.M[13] += Offset.getY();
	aileron.transform.M[14] += Offset.getZ();

	aileron.Render();

	
	
	for (int i = 0; i < info.num_lights; ++i)
	{
		Cube light(info.lights[i].dimensions.x, info.lights[i].dimensions.y, info.lights[i].dimensions.z);
		light.color = Red;
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&light.transform);
		btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
		btVector3 offset(info.lights[i].offset.x, info.lights[i].offset.y, info.lights[i].offset.z);
		offset = offset.rotate(q.getAxis(), q.getAngle());

		light.transform.M[12] += offset.getX();
		light.transform.M[13] += offset.getY();
		light.transform.M[14] += offset.getZ();


		light.Render();


	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}