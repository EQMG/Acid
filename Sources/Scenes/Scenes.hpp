#pragma once

#include "Engine/Engine.hpp"
#include "Objects/ComponentRegister.hpp"
#include "SceneStructure.hpp"
#include "IScene.hpp"

class btDefaultCollisionConfiguration;

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
		std::shared_ptr<IScene> m_scene;

		ComponentRegister m_componentRegister;

		btDefaultCollisionConfiguration *m_collisionConfiguration;
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

		std::shared_ptr<IScene> GetScene() const { return m_scene; }

		void SetScene(std::shared_ptr<IScene> scene) { m_scene = scene; }

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
		void DeregisterComponent(const std::string &name) { m_componentRegister.DeregisterComponent(name); }

		/// <summary>
		/// Creates a new component from the register.
		/// </summary>
		/// <param name="name"> The component name to create. </param>
		/// <returns> The new component. </returns>
		std::shared_ptr<IComponent> CreateComponent(const std::string &name) { return m_componentRegister.CreateComponent(name); }

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		std::shared_ptr<ICamera> GetCamera() const { return m_scene->GetCamera(); }

		/// <summary>
		/// Gets the GameObjects structure.
		/// </summary>
		/// <returns> The GameObjects structure. </returns>
		std::shared_ptr<SceneStructure> GetStructure() const { return m_scene->GetStructure(); }

		/// <summary>
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		bool IsGamePaused() { return m_scene->IsGamePaused(); }

		ACID_HIDDEN btDiscreteDynamicsWorld *GetDynamicsWorld() { return m_dynamicsWorld; }
	};
}
