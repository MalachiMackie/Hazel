#include "hzpch.h"
#include "Scene2D.h"

#include <glm/gtx/rotate_vector.hpp>

namespace Cheezy
{
	/*
		Get a collection of direction edges form a list of vertices
	*/
	static std::vector<glm::vec2> EdgesOf(const std::vector<glm::vec2>& verts)
	{
		std::vector<glm::vec2> edges;
		int n = verts.size();

		for (int i = 0; i < n; ++i)
			edges.push_back(verts[(i + 1) % n] - verts[i]);

		return edges;
	}

	/*
		Get the Orthogonal vector from a vector
		Orthogonal: Perpendicular direction
	*/
	static inline glm::vec2 GetOrthogonal(const glm::vec2& vec)
	{
		return { -vec.y, vec.x };
	}

	/*
		Check if a given axis separates the vertices of two objects
	*/
	static bool IsSeparatingAxis(glm::vec2 axis, std::vector<glm::vec2> verts1, std::vector<glm::vec2> verts2)
	{
		float min1 = FLT_MAX;
		float max1 = FLT_MIN;

		float min2 = FLT_MAX;
		float max2 = FLT_MIN;

		for (glm::vec2 vec : verts1)
		{
			float proj = glm::dot(vec, axis);

			min1 = std::min(min1, proj);
			max1 = std::max(max1, proj);
		}

		for (glm::vec2 vec : verts2)
		{
			float proj = glm::dot(vec, axis);

			min2 = std::min(min2, proj);
			max2 = std::max(max2, proj);
		}

		return max1 < min2 || max2 < min1;
	}

	/*
		Get the collection of collider vertices, moved, scaled and rotated by the transform
	*/
	static std::vector<glm::vec2> GetRotatedVertices(Ref<BoxCollider2DComponent>& collider, Ref<Transform2DComponent>& transform)
	{
		std::vector<glm::vec2> verts;

		glm::vec2 origin1 = collider->GetVertex(0);
		for (int i = 1; i < 5; ++i)
		{
			glm::vec2 vert = collider->GetVertex(i);
			glm::vec2 position = transform->GetPosition();
			glm::vec2 scale = transform->GetSize();
			position *= scale;

			vert += position;
			vert -= origin1;

			vert = glm::rotateZ(glm::vec3(vert, 0.0f), glm::radians(transform->GetRotation()));

			verts.push_back(vert + origin1);
		}

		return verts;
	}

	void Scene2D::OnUpdate(Timestep ts)
	{
		CZ_PROFILE_FUNCTION();
		CheckCollisions();

		m_CameraController.OnUpdate(ts);
		for (Ref<CheezyObject> obj : m_Objects)
			obj->OnUpdate(ts);
	}

	void Scene2D::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void Scene2D::CheckCollisions() const
	{
		CZ_PROFILE_FUNCTION();
		std::vector<Ref<CheezyObject>> objectsWithCollider = GetAllObjectsWithComponent<BoxCollider2DComponent>();

		for (int i = 0; i < objectsWithCollider.size(); ++i)
		{
			Ref<CheezyObject> obj1 = objectsWithCollider[i];
			auto obj1Collider = obj1->GetComponent<BoxCollider2DComponent>();
			auto obj1Transform = obj1->GetComponent<Transform2DComponent>();

			for (int j = i + 1; j < objectsWithCollider.size(); ++j)
			{
				Ref<CheezyObject> obj2 = objectsWithCollider[j];
				auto obj2Collider = obj2->GetComponent<BoxCollider2DComponent>();
				auto obj2Transform = obj2->GetComponent<Transform2DComponent>();

				bool areColliding = obj1Transform->GetRotation() == 0.0f && obj2Transform->GetRotation() == 0.0f
					? NoRotationCollisionCheck(obj1Collider, obj1Transform, obj2Collider, obj2Transform)
					: RotationCollisionCheck(obj1Collider, obj1Transform, obj2Collider, obj2Transform);

				CZ_CORE_TRACE("Are Colliding: {0}", areColliding);
			}
		}
	}

	bool Scene2D::NoRotationCollisionCheck(
		Ref<BoxCollider2DComponent>& collider1, Ref<Transform2DComponent>& transform1,
		Ref<BoxCollider2DComponent>& collider2, Ref<Transform2DComponent>& transform2) const
	{
		CZ_PROFILE_FUNCTION();
		float x1 = transform1->GetPosition().x;
		float y1 = transform1->GetPosition().y;
		float width1 = transform1->GetSize().x * collider1->GetDimensions().x;
		float height1 = transform1->GetSize().y * collider1->GetDimensions().y;

		float x2 = transform2->GetPosition().x;
		float y2 = transform2->GetPosition().y;
		float width2 = transform2->GetSize().x * collider2->GetDimensions().x;
		float height2 = transform2->GetSize().y * collider2->GetDimensions().y;

		return x1 < x2 + width2
			&& x1 + width1 > x2
			&& y1 < y2 + height2
			&& y1 + height1 > y2;
	}

	/*
	 * Check collision between two box colliders that are rotated
	 * using Separating Axis Theorem. Algorithm found here: 
	 * https://hackmd.io/@US4ofdv7Sq2GRdxti381_A/ryFmIZrsl?type=view
	 */
	bool Scene2D::RotationCollisionCheck(
		Ref<BoxCollider2DComponent>& collider1, Ref<Transform2DComponent>& transform1,
		Ref<BoxCollider2DComponent>& collider2, Ref<Transform2DComponent>& transform2) const
	{
		CZ_PROFILE_FUNCTION();
		std::vector<glm::vec2> vertices1 = GetRotatedVertices(collider1, transform1);
		std::vector<glm::vec2> vertices2 = GetRotatedVertices(collider2, transform2);

		std::vector<glm::vec2> edges;

		// Get the edges form the vertices
		// and filter them to unique edges
		for (glm::vec2 edge1 : EdgesOf(vertices1))
		{
			if (!std::any_of(edges.begin(), edges.end(),
				[&edge1](const glm::vec2& curr)
			{
				return (edge1.x ==  curr.x && edge1.y ==  curr.y);
			}))
			{
				edges.push_back(edge1);
			}
		}
		for (glm::vec2 edge2 : EdgesOf(vertices2))
		{
			if (!std::any_of(edges.begin(), edges.end(),
				[&edge2](const glm::vec2& curr)
			{
				return (edge2.x ==  curr.x && edge2.y ==  curr.y);
			}))
			{
				edges.push_back(edge2);
			}
		}

		//Othogonal: perpendicular to edge
		std::vector<glm::vec2> orthogonals;
		for (glm::vec2 edge : edges)
			orthogonals.push_back(GetOrthogonal(edge));

		//if any orthogonal separates the vertices of two polygons, then they are not colliding
		for (glm::vec2 orthogonal : orthogonals)
		{
			if (IsSeparatingAxis(orthogonal, vertices1, vertices2))
			{
				return false;
			}
		}

		//to gaps were found, so colliders are colliding
		return true;
	}
}