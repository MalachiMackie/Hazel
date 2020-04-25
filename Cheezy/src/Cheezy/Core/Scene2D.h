#pragma once

#include "Cheezy/Core/CheezyObject.h"
#include "Cheezy/Core/Components/BoxCollider2DComponent.h"
#include "Cheezy/Core/Shapes/Shape2D.h"
#include "Cheezy/Renderer/OrthographicCameraController.h"

namespace Cheezy
{
	class Scene2D
	{
	public:
		Scene2D(OrthographicCameraController cameraController = OrthographicCameraController(1280.0f/720.0f))
			: m_CameraController(cameraController)
		{
		}

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void AddObject(const Ref<CheezyObject>& obj) { m_Objects.push_back(obj); }

		OrthographicCameraController GetCameraController() const { return m_CameraController; }

		const std::vector<Ref<CheezyObject>>& GetObjects() const { return m_Objects; }

	private:
		template<typename T>
		std::vector<Ref<CheezyObject>> GetAllObjectsWithComponent() const
		{
			static_assert(std::is_base_of<CheezyComponent, T>::value, "Type is not a cheezy component");
			std::vector<Ref<CheezyObject>> objectsWithComponent;
			for (Ref<CheezyObject> obj : m_Objects)
			{
				if (obj->ContainsComponent<T>())
				{
					objectsWithComponent.push_back(obj);
				}
			}

			return objectsWithComponent;
		}

		void CheckCollisions() const;

		bool NoRotationCollisionCheck(
			Ref<BoxCollider2DComponent>& collider1, Ref<Transform2DComponent>& transform1,
			Ref<BoxCollider2DComponent>& collider2, Ref<Transform2DComponent>& transform2) const;

		bool RotationCollisionCheck(
			Ref<BoxCollider2DComponent>& collider1, Ref<Transform2DComponent>& transform1,
			Ref<BoxCollider2DComponent>& collider2, Ref<Transform2DComponent>& transform2) const;

	private:
		OrthographicCameraController m_CameraController;

		std::vector<Ref<CheezyObject>> m_Objects;
		
	};
}