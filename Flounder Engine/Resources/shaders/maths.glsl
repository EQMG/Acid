mat2 finverse(mat2 m) {
	mat2 adj;
	adj[0][0] = m[1][1];
	adj[0][1] = -m[0][1];
	adj[1][0] = -m[1][0];
	adj[1][1] = m[0][0];
	float det = m[0][0] * m[1][1] - m[1][0] * m[0][1];
	return adj / det;
}

mat3 finverse(mat3 m) {
	mat3 adj;
	adj[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]);
	adj[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
	adj[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]);
	adj[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
	adj[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]);
	adj[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);
	adj[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]);
	adj[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
	adj[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]);

	float det = (+m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));

	return adj / det;
}

mat4 finverse(mat4 m) {
	float SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float SubFactor11 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	float SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	float SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	float SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	float SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	mat4 adj;

	adj[0][0] = +(m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02);
	adj[1][0] = -(m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04);
	adj[2][0] = +(m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05);
	adj[3][0] = -(m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05);

	adj[0][1] = -(m[0][1] * SubFactor00 - m[0][2] * SubFactor01 + m[0][3] * SubFactor02);
	adj[1][1] = +(m[0][0] * SubFactor00 - m[0][2] * SubFactor03 + m[0][3] * SubFactor04);
	adj[2][1] = -(m[0][0] * SubFactor01 - m[0][1] * SubFactor03 + m[0][3] * SubFactor05);
	adj[3][1] = +(m[0][0] * SubFactor02 - m[0][1] * SubFactor04 + m[0][2] * SubFactor05);

	adj[0][2] = +(m[0][1] * SubFactor06 - m[0][2] * SubFactor07 + m[0][3] * SubFactor08);
	adj[1][2] = -(m[0][0] * SubFactor06 - m[0][2] * SubFactor09 + m[0][3] * SubFactor10);
	adj[2][2] = +(m[0][0] * SubFactor11 - m[0][1] * SubFactor09 + m[0][3] * SubFactor12);
	adj[3][2] = -(m[0][0] * SubFactor08 - m[0][1] * SubFactor10 + m[0][2] * SubFactor12);

	adj[0][3] = -(m[0][1] * SubFactor13 - m[0][2] * SubFactor14 + m[0][3] * SubFactor15);
	adj[1][3] = +(m[0][0] * SubFactor13 - m[0][2] * SubFactor16 + m[0][3] * SubFactor17);
	adj[2][3] = -(m[0][0] * SubFactor14 - m[0][1] * SubFactor16 + m[0][3] * SubFactor18);
	adj[3][3] = +(m[0][0] * SubFactor15 - m[0][1] * SubFactor17 + m[0][2] * SubFactor18);

	float det = (+m[0][0] * adj[0][0] + m[0][1] * adj[1][0] + m[0][2] * adj[2][0] + m[0][3] * adj[3][0]);

	return adj / det;
}

mat4 frotationMatrix(float angle, float xAxis, float yAxis, float zAxis) {
	vec3 axis = normalize(vec3(xAxis, yAxis, zAxis));
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;
	return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0, oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0, oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0, 0, 0, 0, 1);
}

float fsmoothlyStep(float edge0, float edge1, float x) {
	float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}
