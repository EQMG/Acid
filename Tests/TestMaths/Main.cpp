#include <iostream>
#include <Maths/Maths.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Matrix2.hpp>
#include <Maths/Matrix3.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Quaternion.hpp>
#include <Maths/Vector2.hpp>
#include <Maths/Vector3.hpp>
#include <Maths/Vector4.hpp>
#include <Maths/Transform.hpp>

using namespace acid;

int main(int argc, char **argv)
{
	{
		fprintf(stdout, "Colour Size: %i\n", (int) sizeof(Colour));
		fprintf(stdout, "Matrix2 Size: %i\n", (int) sizeof(Matrix2));
		fprintf(stdout, "Matrix3 Size: %i\n", (int) sizeof(Matrix3));
		fprintf(stdout, "Matrix4 Size: %i\n", (int) sizeof(Matrix4));
		fprintf(stdout, "Vector2 Size: %i\n", (int) sizeof(Vector2));
		fprintf(stdout, "Vector3 Size: %i\n", (int) sizeof(Vector3));
		fprintf(stdout, "Vector4 Size: %i\n", (int) sizeof(Vector4));
		fprintf(stdout, "\n");
	}
	{
	// Transform transform = Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f));

		Vector3 original = Vector3(90.0f, 0.0f, 0.0f);
		Quaternion quaternion = original.ToQuaternion();
		Vector3 restored = quaternion.ToEuler();

		fprintf(stdout, "Original: %s\n", original.ToString().c_str());
		fprintf(stdout, "Quaternion: %s\n", quaternion.ToString().c_str());
		fprintf(stdout, "Restored: %s\n", restored.ToString().c_str());
		fprintf(stdout, "\n");
	}
	{
		fprintf(stdout, "Vector2:\n");
		Vector2 a{3.0f, -7.2f};
		Vector2 b{-1.74f, 15.4f};
		fprintf(stdout, "  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a + b).ToString().c_str());
		fprintf(stdout, "  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a - b).ToString().c_str());
		fprintf(stdout, "  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a * b).ToString().c_str());
		fprintf(stdout, "  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a / b).ToString().c_str());
		fprintf(stdout, "  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		fprintf(stdout, "  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		fprintf(stdout, "  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		fprintf(stdout, "  %s rot %f = %s\n", a.ToString().c_str(), 90.0f, a.Rotate(90.0f).ToString().c_str());
		fprintf(stdout, "  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		fprintf(stdout, "  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		fprintf(stdout, "  len %s = %f\n", a.ToString().c_str(), a.Length());
		fprintf(stdout, "  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		fprintf(stdout, "\n");
	}
	{
		fprintf(stdout, "Vector3:\n");
		Vector3 a{12.9f, -2.0f, 6.7f};
		Vector3 b{-9.7f, 15.9f, -13.8f};
		fprintf(stdout, "  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a + b).ToString().c_str());
		fprintf(stdout, "  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a - b).ToString().c_str());
		fprintf(stdout, "  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a * b).ToString().c_str());
		fprintf(stdout, "  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), Vector2(a / b).ToString().c_str());
		fprintf(stdout, "  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		fprintf(stdout, "  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		fprintf(stdout, "  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		//	fprintf(stdout, "  %s rot %f = %s\n", a.ToString().c_str(), 90.0f, a.Rotate(90.0f).ToString().c_str());
		fprintf(stdout, "  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		fprintf(stdout, "  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		fprintf(stdout, "  len %s = %f\n", a.ToString().c_str(), a.Length());
		fprintf(stdout, "  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		fprintf(stdout, "\n");
	}

	// Pauses the console.
	std::cin.get();
	return EXIT_SUCCESS;
}
