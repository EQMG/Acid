#include <iostream>

#include "maths.h"
#include "colour.h"
#include "vector2.h"
#include "vector3.h"

using namespace flounder;

#define LOG(x) (std::cout << x << std::endl)

int main() {
	vector3 *v1 = new vector3(0.2f, -0.9f, 0.1f);
	vector3 *v2 = new vector3(0.9f, 0.1f, -0.6f);
	vector3 *vr = vector3::cross(v1, v2, NULL);
	LOG(vr->x);
	LOG(vr->y);
	LOG(vr->z);
	system("PAUSE");
	return 0;
}