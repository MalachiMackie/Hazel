function OnUpdate(timestep)
	position = MyCheezyObject.GetTransform(MyCheezyObject).Position

	if IsKeyDown(CZ_KEY_A) then
	 	position.x = position.x - (1 * timestep)
	end
	if IsKeyDown(CZ_KEY_D) then
		position.x = position.x + (1 * timestep)
	end
	if IsKeyDown(CZ_KEY_W) then
		position.y = position.y + (1 * timestep)
	end
	if IsKeyDown(CZ_KEY_S) then
		position.y = position.y - (1 * timestep)
	end
end