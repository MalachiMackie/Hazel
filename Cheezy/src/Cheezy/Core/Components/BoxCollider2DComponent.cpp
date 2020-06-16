#include "hzpch.h"
#include "BoxCollider2DComponent.h"

#include "Cheezy/Core/Application.h"

#include "Cheezy/Core/Components/ScriptComponent.h"
#include "Cheezy/Core/Components/Transform2DComponent.h"
#include "Cheezy/Core/Components/CameraComponent.h"

#include <glm/gtx/rotate_vector.hpp>

#include <future>

namespace Cheezy
{
#pragma region staticHelperFunctions
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
		size_t n = verts.size();

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
		std::array<glm::vec2, 4> colliderVerts = collider->GetVertices();
		float xScale = colliderVerts[0].x - colliderVerts[1].x;
		float yScale = colliderVerts[1].y - colliderVerts[2].y;

		for (int i = 0; i < colliderVerts.size(); i++)
		{
			glm::vec2 colliderVert{colliderVerts[i]};

			colliderVert.x += (xScale / 2);
			colliderVert.y += (yScale / 2);

			colliderVerts[i] = colliderVert;
		}

		glm::vec2& origin1 = GetCenter(colliderVerts);
		for (int i = 0; i < 4; ++i)
		{
			glm::vec2 vert = colliderVerts[i];
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

	/*
		Get the orientation of ordered triplet
		https://www.geeksforgeeks.org/orientation-3-ordered-points/
		0 -> p, q and r are colinear
		1 -> Clockwise
		2 -> AntiClockwise
	*/
	static int orientation(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
	{
		float val =	(p2.y - p1.y) * (p3.x - p2.x) -
					(p3.y - p2.y) * (p2.x - p1.x);

		if (val == 0.0f) return 0;  // colinear

		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}

	/*
		Given three colinear points p, q and r
		Check if point q lines on line segment pr
	*/
	static bool onSegment(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r)
	{
		if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
			q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
			return true;

		return false;
	}

	/*
		Check if line segment p1q1 intersects with p2q2
		https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
	*/
	static bool doIntersect(const glm::vec2& p1, const glm::vec2& q1, const glm::vec2& p2, const glm::vec2& q2)
	{
		// Find the four orientations needed for general and  special cases 
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case 
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases 
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;

		return false; // Doesn't fall in any of the above cases 
	}

	/*
		Check if a point lies inside a polygon
		https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
	*/
	static bool IsPointInsidePolygon(std::vector<glm::vec2> vertices, const glm::vec2& point)
	{
		glm::vec2 extreme = { INT_MAX, point.y };

		// Count intersections of the above line with sides of polygon 
		int count = 0;
		int i = 0;
		do
		{
			int next = (i + 1) % vertices.size();

			// Check if the line segment from 'p' to 'extreme' intersects 
			// with the line segment from 'polygon[i]' to 'polygon[next]' 
			if (doIntersect(vertices[i], vertices[next], point, extreme))
			{
				// If the point 'p' is colinear with line segment 'i-next', 
				// then check if it lies on segment. If it lies, return true, 
				// otherwise false 
				if (orientation(vertices[i], point, vertices[next]) == 0)
					return onSegment(vertices[i], point, vertices[next]);

				count++;
			}
			i = next;
		} while (i != 0);

		// Return true if count is odd, false otherwise 
		return count & 1;  // Same as (count%2 == 1) 
	}

#pragma endregion staticHelperFunctions

	BoxCollider2DComponent::BoxCollider2DComponent(bool isTrigger, const glm::vec2& dimensions, const char* tag)
		: m_IsTrigger(isTrigger),
		m_Dimensions(dimensions),
		m_Tag(tag),
		m_Vertices({ glm::vec2(0, 0), {dimensions.x, 0}, {dimensions.x, dimensions.y}, {0, dimensions.y} })
	{

	}

	void BoxCollider2DComponent::OnUpdate(Timestep timestep)
	{
		std::vector<glm::vec2> verts = GetObjectVertices(Get(), m_CheezyObject->GetComponent<Transform2DComponent>()->GetTransform());
		
		Ref<CheezyObject> cameraObject;
		for (const Ref<CheezyObject>& obj : Application::Get()->GetScene()->GetObjects())
		{
			if (obj->ContainsComponent<CameraComponent>())
			{
				cameraObject = obj;
				break;
			}
		}

		Ref<CameraComponent> cameraComponent = cameraObject->GetComponent<CameraComponent>();

		auto [mouseX, mouseY] = Input::GetMousePos();
		glm::vec2 mouseInWorld = cameraComponent->CameraToWorldSpace(glm::vec2(mouseX, mouseY));

		if (IsPointInsidePolygon(verts, mouseInWorld))
		{
			if (!m_MouseHovering)
			{
				m_CheezyObject->OnMouseEnter();
				m_MouseHovering = true;
			}
			m_CheezyObject->OnMouseHover();
		}
		else if (m_MouseHovering)
		{
			m_CheezyObject->OnMouseExit();
			m_MouseHovering = false;
		}
	}

	void BoxCollider2DComponent::OnCollision(Collision2D collision)
	{

	}

	void BoxCollider2DComponent::OnCollisionEnter(Collision2D collision)
	{
		const auto& found = std::find(m_CollidingWith.begin(), m_CollidingWith.end(), collision.OtherCollider);
		if (found == m_CollidingWith.end())
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

#pragma region collisionDetection
	/*
	 * Check collision between two box colliders
	 * using Separating Axis Theorem. Algorithm found here:
	 * https://hackmd.io/@US4ofdv7Sq2GRdxti381_A/ryFmIZrsl?type=view
	 */
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

		if (!obj1Collider)
			return;

		std::vector<Collision2D> collisions;

		for (auto& obj2 = otherObjects.begin() + index; obj2 != otherObjects.end(); ++obj2)
		{
			const auto& obj2Collider = (*obj2)->GetComponent<BoxCollider2DComponent>();
			const auto& obj2Transform = (*obj2)->GetComponent<Transform2DComponent>()->GetTransform();

			if (!obj2Collider)
				continue;

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
				if (otherCollider->IsTrigger())
				{
					(*cheezyObject)->OnTriggerExit(Collision2D{glm::vec2(0.0f), otherCollider});
					otherCollider->GetCheezyObject()->OnCollisionExit(Collision2D{ glm::vec2(0.0f), obj1Collider });
				}
				else
				{
					(*cheezyObject)->OnCollisionExit(Collision2D{ glm::vec2(0.0f), otherCollider });
					otherCollider->GetCheezyObject()->OnCollisionExit(Collision2D{ glm::vec2(0.0f), obj1Collider });
				}
			}
			else
			{
				if (otherCollider->IsTrigger())
				{
					(*cheezyObject)->OnTrigger(*existingCollision);
					otherCollider->GetCheezyObject()->OnTrigger({ existingCollision->PushVector * -1.0f, obj1Collider });
				}
				else
				{
					(*cheezyObject)->OnCollision(*existingCollision);
					otherCollider->GetCheezyObject()->OnCollision({ existingCollision->PushVector * -1.0f, obj1Collider });
				}
				collisions.erase(existingCollision);
			}
		}

		for (const Collision2D& collision : collisions)
		{
			if (collision.OtherCollider->IsTrigger())
			{
				(*cheezyObject)->OnTriggerEnter(collision);
				collision.OtherCollider->GetCheezyObject()->OnTriggerEnter({ collision.PushVector * -1.0f, obj1Collider });
			}
			else
			{
				(*cheezyObject)->OnCollisionEnter(collision);
				collision.OtherCollider->GetCheezyObject()->OnCollisionEnter({ collision.PushVector * -1.0f, obj1Collider });
			}
		}
	}

	std::vector<Collision2D> BoxCollider2DComponent::CheckCollisionForObjectWithTransform(const Ref<CheezyObject>& cheezyObject, const Transform2D& transform)
	{
		const std::vector<Ref<CheezyObject>>& objects = Application::Get()->GetScene()->GetObjects();

		const Ref<BoxCollider2DComponent>& collider = cheezyObject->GetComponent<BoxCollider2DComponent>();
		std::vector<Collision2D> collisions;

		if (!collider)
			return collisions;


		for (const Ref<CheezyObject>& other : objects)
		{
			if (other == cheezyObject)
				continue;

			const auto& obj2Collider = other->GetComponent<BoxCollider2DComponent>();
			const auto& obj2Transform = other->GetComponent<Transform2DComponent>()->GetTransform();

			if (!obj2Collider)
				continue;

			auto [collision, minPushVector] = BoxCollider2DComponent::CheckCollisionBetweenColliders(collider, transform, obj2Collider, obj2Transform);

			if (collision)
			{
				collisions.push_back(Collision2D{ minPushVector, obj2Collider });
			}
		}

		return collisions;
	}

#pragma endregion collisionDetection
}
