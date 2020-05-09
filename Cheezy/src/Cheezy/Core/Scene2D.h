#pragma once

#include "Cheezy/Core/CheezyObject.h"
#include "Cheezy/Renderer/OrthographicCameraController.h"

namespace Cheezy
{
	class Scene2D
	{
	public:
		Scene2D() = default;

		void OnStart();
		/*
			Runs 50 times per second.
			Will run multiple times per rendered frame if it has to
		*/
		void OnFixedUpdate();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void AddObject(const Ref<CheezyObject>& obj) { m_Objects.push_back(obj); }

		const std::vector<Ref<CheezyObject>>& GetObjects() const { return m_Objects; }

	private:
		template<typename T>
		std::vector<Ref<CheezyObject>> GetAllObjectsWithComponent() const
		{
			static_assert(std::is_base_of<CheezyComponent, T>::value, "Type is not a cheezy component");
			std::vector<Ref<CheezyObject>> objectsWithComponent;
			for (const Ref<CheezyObject>& obj : m_Objects)
			{
				if (obj->ContainsComponent<T>())
				{
					objectsWithComponent.push_back(obj);
				}
			}

			return objectsWithComponent;
		}

	private:
		std::vector<Ref<CheezyObject>> m_Objects;
		
	};
}