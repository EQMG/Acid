mat4 rotationMatrix(float angle, vec3 axis)
{
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0f - c;

	return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0f,
				oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0f,
				oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 scaleMatrix(vec3 scale)
{
	return mat4(scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 modelMatrix(mat4 source, mat4 view, bool lockRotation, vec3 rotation)
{
	mat4 modelMatrix = source;

	if (lockRotation)
	{
		vec3 scale = vec3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2]);
		modelMatrix[0][0] = view[0][0];
		modelMatrix[0][1] = view[1][0];
		modelMatrix[0][2] = view[2][0];
		modelMatrix[1][0] = view[0][1];
		modelMatrix[1][1] = view[1][1];
		modelMatrix[1][2] = view[2][1];
		modelMatrix[2][0] = view[0][2];
		modelMatrix[2][1] = view[1][2];
		modelMatrix[2][2] = view[2][2];
		modelMatrix *= rotationMatrix(rotation.x, vec3(1.0f, 0.0f, 0.0f));
		modelMatrix *= rotationMatrix(rotation.y, vec3(0.0f, 1.0f, 0.0f));
		modelMatrix *= rotationMatrix(rotation.z, vec3(0.0f, 0.0f, 1.0f));
		modelMatrix *= scaleMatrix(scale);
	}

	return modelMatrix;
}