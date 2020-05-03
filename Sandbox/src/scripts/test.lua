moveSpeed = 10
maxSpeed = 7.5
jumpForce = 7.5

jumping = false


function OnFixedUpdate()
	local position = MyCheezyObject.GetTransform(MyCheezyObject).Position
	local rigidbody = MyCheezyObject.GetRigidBody(MyCheezyObject)
	
	local currentMoveForce = Vec2(0.0, 0.0)

	if IsKeyDown(CZ_KEY_A) then
		currentMoveForce.x = currentMoveForce.x - moveSpeed
	end
	if IsKeyDown(CZ_KEY_D) then
		currentMoveForce.x = currentMoveForce.x + moveSpeed
	end
	if IsKeyDown(CZ_KEY_W) then
		currentMoveForce.y = currentMoveForce.y + moveSpeed
	end
	if IsKeyDown(CZ_KEY_S) then
		currentMoveForce.y = currentMoveForce.y - moveSpeed
	end

	if IsKeyDown(CZ_KEY_SPACE) and not jumping then
		rigidbody.AddForce(rigidbody, Vec2(0, jumpForce))
	end

	local velocity = rigidbody.GetVelocity(rigidbody)
	local magnitude = velocity.GetMagnitude(velocity)
	if magnitude < maxSpeed then
		rigidbody.AddForce(rigidbody, Vec2(currentMoveForce.x * 0.02, currentMoveForce.y * 0.02))
	end
end

function OnCollisionEnter(collision)
	local otherCollider = collision.GetCollider(collision)
	if (otherCollider.Tag == "Floor") then
		jumping = false
	end
end

function OnCollisionExit(collision)
	local otherCollider = collision.GetCollider(collision)
	if (otherCollider.Tag == "Floor") then
		jumping = true
	end
end