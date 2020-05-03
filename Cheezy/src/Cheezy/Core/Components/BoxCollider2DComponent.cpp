#include "hzpch.h"
#include "BoxCollider2DComponent.h"

#include "Cheezy/Core/Application.h"

#include "Cheezy/Core/Components/ScriptComponent.h"
#include "Cheezy/Core/Components/Transform2DComponent.h"

#include <glm/gtx/rotate_vector.hpp>

#include <future>

namespace Cheezy
{
	template<int ARRAYSIZE>
	static glm::vec2 GetCenter(const std::array<glm::vec2, ARRAYSIZE>& verts)
	{
		float sumX{ 0 };
		float sumY{ 0 };
		for (const glm::vec2& vert : verts)
		{
			sumX += vert.x;
			sumY += vert.y;
		}

		return { sumX / ARRAYSIZE, sumY / ARRAYSIZE };
	}

	static glm::vec2 GetCenter(const std::vector<glm::vec2>& verts)
	{
		float sumX{ 0 };
		float sumY{ 0 };
		for (const glm::vec2& vert : verts)
		{
			sumX += vert.x;
			sumY += vert.y;
		}

		return { sumX / verts.size(), sumY / verts.size() };
	}

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
	static std::pair<bool, glm::vec2> IsSeparatingAxis(const glm::vec2& axis, const std::vector<glm::vec2>& verts1, const std::vector<glm::vec2>& verts2)
	{
		float min1 = FLT_MAX;
		float max1 = FLT_MIN;

		float min2 = FLT_MAX;
		float max2 = FLT_MIN;

		for (const glm::vec2& vec : verts1)
		{
			float proj = glm::dot(vec, axis);

			min1 = std::min(min1, proj);
			max1 = std::max(max1, proj);
		}

		for (const glm::vec2& vec : verts2)
		{
			float proj = glm::dot(vec, axis);

			min2 = std::min(min2, proj);
			max2 = std::max(max2, proj);
		}

		if (max1 >= min2 && max2 >= min1)
		{
			float dist = std::min(max2 - min1, max1 - min2);
			float pushVector = (dist / glm::dot(axis, axis)) + 0.00000000001f;
			return std::pair(false, axis * pushVector);
		}
		return std::pair(true, glm::vec2(0.0f));
	}

	/*
		Get the collection of collider vertices, moved, scaled and rotated by the transform
	*/
	static std::vector<glm::vec2> GetObjectVertices(const Ref<BoxCollider2DComponent>& collider, const Transform2D& transform)
	{
		std::vector<glm::vec2> verts;

		glm::vec2& origin1 = GetCenter(collider->GetVertices());// collider->GetVertex(0);
		for (int i = 0; i < 4; ++i)
		{
			glm::vec2 vert = collider->GetVertex(i);
			const glm::vec2& position = transform.Position;
			const glm::vec2& scale = transform.Scale;

			vert += position;
			vert -= origin1;

			vert *= scale;

			if (transform.Rotation != 0.0f)
				vert = glm::rotateZ(glm::vec3(vert, 0.0f), glm::radians(transform.Rotation));

			verts.push_back(vert + origin1);
		}

		return verts;
	}

	BoxCollider2DComponent::BoxCollider2DComponent(const glm::vec2& dimensions, const char* tag)
		: m_Dimensions(dimensions),
		m_Tag(tag),
		m_Vertices({ glm::vec2(0, 0), {dimensions.x, 0}, {dimensions.x, dimensions.y}, {0, dimensions.y} })
	{
		
	}

	void BoxCollider2DComponent::OnUpdate(Timestep timestep)
	{

	}

	void BoxCollider2DComponent::OnCollision(Collision2D collision)
	{

	}


	void BoxCollider2DComponent::OnCollisionEnter(Collision2D collision)
	{
		const auto& found = std::find(m_CollidingWith.begin(), m_CollidingWith.end(), collision.OtherCollider);
		if (found != m_CollidingWith.end())
		{
			m_CollidingWith.push_back(collision.OtherCollider);
		}
	}

	void BoxCollider2DComponent::OnCollisionExit(Collision2D collision)
	{
		const auto& found = std::find(m_CollidingWith.begin(), m_CollidingWith.end(), collision.OtherCollider);
		if (found != m_CollidingWith.end())
		{
			m_CollidingWith.erase(found);
		}
	}

	std::pair<bool, glm::vec2> BoxCollider2DComponent::CheckCollisionBetweenColliders(
		const Ref<BoxCollider2DComponent>& collider1, const Transform2D& transform1,
		const Ref<BoxCollider2DComponent>& collider2, const Transform2D& transform2)
	{
		CZ_PROFILE_FUNCTION();

		const std::vector<glm::vec2>& vertices1 = GetObjectVertices(collider1, transform1);
		const std::vector<glm::vec2>& vertices2 = GetObjectVertices(collider2, transform2);

		std::vector<glm::vec2> edges;

		// Get the edges form the vertices
		// and filter them to unique edges
		for (const glm::vec2& edge1 : EdgesOf(vertices1))
		{
			if (!std::any_of(edges.begin(), edges.end(),
				[&edge1](const glm::vec2& curr)
				{
					return (edge1.x == curr.x && edge1.y == curr.y);
				}))
			{
				edges.push_back(edge1);
			}
		}
		for (const glm::vec2& edge2 : EdgesOf(vertices2))
		{
			if (!std::any_of(edges.begin(), edges.end(),
				[&edge2](const glm::vec2& curr)
				{
					return (edge2.x == curr.x && edge2.y == curr.y);
				}))
			{
				edges.push_back(edge2);
			}
		}

		//Othogonal: perpendicular to edge
		std::vector<glm::vec2> orthogonals;
		for (const glm::vec2& edge : edges)
			orthogonals.push_back(GetOrthogonal(edge));

		std::vector<glm::vec2> pushVectors;
		//if any orthogonal separates the vertices of two polygons, then they are not colliding
		for (const glm::vec2& orthogonal : orthogonals)
		{
			auto [isSeparating, pushVector] = IsSeparatingAxis(orthogonal, vertices1, vertices2);
			if (isSeparating)
			{
				return { false, glm::vec2(0.0f) };
			}
			pushVectors.push_back(pushVector);
		}

		float min = FLT_MAX;
		glm::vec2 minPushVector;
		for (const glm::vec2& vec : pushVectors)
		{
			float dot = glm::dot(vec, vec);
			if (dot < min)
			{
				min = dot;
				minPushVector = vec;
			}
		}

		const glm::vec2& center1 = GetCenter(vertices1);
		const glm::vec2& center2 = GetCenter(vertices2);

		const glm::vec2& centerDisplacement = center2 - center1; // direction frum center1 to center2
		if (glm::dot(centerDisplacement, minPushVector) > 0) // if its the same direction then invert;
			minPushVector *= -1;

		if (minPushVector.x == -0.0f)
			minPushVector.x = 0.0f;
		if (minPushVector.y == -0.0f)
			minPushVector.y = 0.0f;

		//no gaps were found, so colliders are colliding
		return { true, minPushVector };
	}

	void BoxCollider2DComponent::CheckCollisionsForObjects(const std::vector<Ref<CheezyObject>>& cheezyObjects)
	{
		CZ_PROFILE_FUNCTION();
		std::vector<std::future<void>> futures;

		for (int i = 0; i < cheezyObjects.size() - 1; ++i)
		{
			const auto& cheezyObject = cheezyObjects[i];
			const auto& obj1Collider = cheezyObject->GetComponent<BoxCollider2DComponent>();
			const auto& obj1Transform = cheezyObject->GetComponent<Transform2DComponent>();

			futures.push_back(std::async(std::launch::async, &BoxCollider2DComponent::CheckCollisionsForObject, &cheezyObject, cheezyObjects, i + 1));
		}
	}

	void BoxCollider2DComponent::CheckCollisionsForObject(const Ref<CheezyObject>* cheezyObject, const std::vector<Ref<CheezyObject>>& otherObjects, int index)
	{
		const Transform2D& obj1Transform = (*cheezyObject)->GetComponent<Transform2DComponent>()->GetTransform();
		const Ref<BoxCollider2DComponent>& obj1Collider = (*cheezyObject)->GetComponent<BoxCollider2DComponent>();

		std::vector<Collision2D> collisions;

		for (auto& obj2 = otherObjects.begin() + index; obj2 != otherObjects.end(); ++obj2)
		{
			const auto& obj2Collider = (*obj2)->GetComponent<BoxCollider2DComponent>();
			const auto& obj2Transform = (*obj2)->GetComponent<Transform2DComponent>()->GetTransform();

			auto [collision, minPushVector] = BoxCollider2DComponent::CheckCollisionBetweenColliders(obj1Collider, obj1Transform, obj2Collider, obj2Transform);

			if (collision)
			{
				collisions.push_back(Collision2D{ minPushVector, obj2Collider });
			}
		}

		const std::vector<Ref<BoxCollider2DComponent>>& collidingWith = obj1Collider->GetCollidingWith();

		for (const Ref<BoxCollider2DComponent>& otherCollider : collidingWith)
		{
			auto& existingCollision = std::find_if(collisions.begin(), collisions.end(),
				[&otherCollider](Collision2D& collision) {return collision.OtherCollider == otherCollider; });

			if (existingCollision == collisions.end())
			{
				(*cheezyObject)->OnCollisionExit(Collision2D{ glm::vec2(0.0f), otherCollider });
				otherCollider->GetCheezyObject()->OnCollisionExit(Collision2D{ glm::vec2(0.0f), obj1Collider });
			}
			else
			{
				(*cheezyObject)->OnCollision(*existingCollision);
				otherCollider->GetCheezyObject()->OnCollision({ existingCollision->PushVector * -1.0f, obj1Collider });
				collisions.erase(existingCollision);
			}
		}
		for (const Collision2D& collision : collisions)
		{
			(*cheezyObject)->OnCollisionEnter(collision);
			collision.OtherCollider->GetCheezyObject()->OnCollisionEnter({ collision.PushVector * -1.0f, obj1Collider });
		}
	}

	std::vector<Collision2D> BoxCollider2DComponent::CheckCollisionForObjectWithTransform(const Ref<CheezyObject>& cheezyObject, const Transform2D& transform)
	{
		const std::vector<Ref<CheezyObject>>& objects = Application::Get()->GetScene()->GetObjects();
		
		const Ref<BoxCollider2DComponent>& collider = cheezyObject->GetComponent<BoxCollider2DComponent>();

		std::vector<Collision2D> collisions;

		for (const Ref<CheezyObject>& other : objects)
		{
			if (other == cheezyObject)
				continue;

			const auto& obj2Collider = other->GetComponent<BoxCollider2DComponent>();
			const auto& obj2Transform = other->GetComponent<Transform2DComponent>()->GetTransform();

			auto [collision, minPushVector] = BoxCollider2DComponent::CheckCollisionBetweenColliders(collider, transform, obj2Collider, obj2Transform);

			if (collision)
			{
				collisions.push_back(Collision2D{ minPushVector, obj2Collider });
			}
		}

		return collisions;
	}
}
