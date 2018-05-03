#include <iostream>
#include <Maths/Vector2.hpp>
#include <Maths/Vector3.hpp>
#include <Maths/Vector4.hpp>
#include <Maths/Maths.hpp>

using namespace fl;

int main(int argc, char **argv)
{
	{
		printf("Vector2 Size: %i\n", (int) sizeof(Vector2));
		printf("Vector3 Size: %i\n", (int) sizeof(Vector3));
		printf("Vector4 Size: %i\n", (int) sizeof(Vector4));
		printf("\n");
	}
	{
		printf("Vector2:\n");
		Vector2 a{3.0f, -7.2f};
		Vector2 b{-1.74f, 15.4f};
		printf("  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a + b).ToString().c_str());
		printf("  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a - b).ToString().c_str());
		printf("  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a * b).ToString().c_str());
		printf("  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a / b).ToString().c_str());
		printf("  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		printf("  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		printf("  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		printf("  %s rot %f = %s\n", a.ToString().c_str(), 90.0f, a.Rotate(90.0f).ToString().c_str());
		printf("  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		printf("  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		printf("  len %s = %f\n", a.ToString().c_str(), a.Length());
		printf("  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		printf("\n");
	}
	{
		printf("Vector3:\n");
		Vector3 a{12.9f, -2.0f, 6.7f};
		Vector3 b{-9.7f, 15.9f, -13.8f};
		printf("  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a + b).ToString().c_str());
		printf("  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a - b).ToString().c_str());
		printf("  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a * b).ToString().c_str());
		printf("  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a / b).ToString().c_str());
		printf("  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		printf("  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		printf("  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		//	printf("  %s rot %f = %s\n", a.ToString().c_str(), 90.0f, a.Rotate(90.0f).ToString().c_str());
		printf("  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		printf("  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		printf("  len %s = %f\n", a.ToString().c_str(), a.Length());
		printf("  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		printf("\n");
	}

	// Pauses the console.
	std::cin.get();
	return EXIT_SUCCESS;
}
