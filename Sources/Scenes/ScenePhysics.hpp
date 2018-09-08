#pragma once

#include <memory>
#include "Maths/Vector3.hpp"

class btCollisionConfiguration;

class btBroadphaseInterface;

class btCollisionDispatcher;

class btSequentialImpulseConstraintSolver;

class btDiscreteDynamicsWorld;

namespace acid
{
	class ACID_EXPORT ScenePhysics
	{
	private:
		std::unique_ptr<btCollisionConfiguration> m_collisionConfiguration;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;
		std::unique_ptr<btCollisionDispatcher> m_dispatcher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	public:
		ScenePhysics();

		~ScenePhysics();

		void Update();

		Vector3 GetGravity() const;

		void SetGravity(const Vector3 &gravity);

		float GetAirDensity() const;

		void SetAirDensity(const float &airDensity);

		btDiscreteDynamicsWorld *GetDynamicsWorld() { return m_dynamicsWorld.get(); }
	};
}
