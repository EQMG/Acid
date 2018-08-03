#pragma once

#include "Engine/Engine.hpp"
#include "Objects/ComponentRegister.hpp"
#include "SceneStructure.hpp"
#include "IScene.hpp"

class btCollisionConfiguration;

class btBroadphaseInterface;

class btCollisionDispatcher;

class btSequentialImpulseConstraintSolver;

class btDiscreteDynamicsWorld;

namespace acid
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class ACID_EXPORT Scenes :
		public IModule
	{
	private:
		IScene *m_scene;

		ComponentRegister m_componentRegister;

		btCollisionConfiguration *m_collisionConfiguration;
		btBroadphaseInterface *m_broadphase;
		btCollisionDispatcher *m_dispatcher;
		btSequentialImpulseConstraintSolver *m_solver;
		btDiscreteDynamicsWorld *m_dynamicsWorld;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Scenes *Get()
		{
			return Engine::Get()->GetModule<Scenes>();
		}

		/// <summary>
		/// Creates a new Scenes module.
		/// </summary>
		Scenes();

		/// <summary>
		/// Deconstructor for the Scenes module.
		/// </summary>
		~Scenes();

		void Update() override;

		std::string GetName() const override { return "Scenes"; };

		IScene *GetScene() const { return m_scene; }

		void SetScene(IScene *scene);

		void SetGravity(const Vector3 &gravity);

		void SetAirDensity(const float &airDensity);

		/// <summary>
		/// Creates a new component from the register.
		/// </summary>
		/// <param name="name"> The component name to create. </param>
		/// <returns> The new component. </returns>
		IComponent *CreateComponent(const std::string &name) { return m_componentRegister.CreateComponent(name); }

		/// <summary>
		/// Registers a component with the register.
		/// </summary>
		/// <param name="update"> The components update type. </param>
		/// <param name="name"> The components name. </param>
		/// <param name="T"> The components type. </param>
		template<typename T>
		void RegisterComponent(const std::string &name) { m_componentRegister.RegisterComponent<T>(name); }

		/// <summary>
		/// Deregisters a component.
		/// </summary>
		/// <param name="name"> The components name. </param>
		/// <returns> If the component was deregistered. </returns>
		bool DeregisterComponent(const std::string &name) { return m_componentRegister.DeregisterComponent(name); }

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		ICamera *GetCamera() const { return m_scene->GetCamera(); }

		/// <summary>
		/// Gets the GameObjects structure.
		/// </summary>
		/// <returns> The GameObjects structure. </returns>
		SceneStructure *GetStructure() const { return m_scene->GetStructure(); }

		/// <summary>
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		bool IsGamePaused() { return m_scene->IsGamePaused(); }

		ACID_HIDDEN btDiscreteDynamicsWorld *GetDynamicsWorld() { return m_dynamicsWorld; }
	};
}
