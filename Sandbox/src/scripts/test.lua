moveSpeed = 0.5
maxSpeed = 7.5


function OnFixedUpdate()
	position = MyCheezyObject.GetTransform(MyCheezyObject).Position
	rigidbody = MyCheezyObject.GetRigidBody(MyCheezyObject)
	
	currentMoveForce = Vec2(0.0, 0.0)

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

	velocity = rigidbody.GetVelocity(rigidbody)
	magnitude = velocity.GetMagnitude(velocity)
	if magnitude < maxSpeed then
		rigidbody.AddForce(rigidbody, currentMoveForce)
	end
end

function OnStart()

end