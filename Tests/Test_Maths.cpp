#include <Engine/Log.hpp>
#include <Maths/Time.hpp>
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

int main(int argc, char **argv) {
	{
		Log::Out("Time Size: ", sizeof(Time), '\n');
		Log::Out("Colour Size: ", sizeof(Colour), '\n');
		Log::Out("Matrix2 Size: ", sizeof(Matrix2), '\n');
		Log::Out("Matrix3 Size: ", sizeof(Matrix3), '\n');
		Log::Out("Matrix4 Size: ", sizeof(Matrix4), '\n');
		Log::Out("Quaternion Size: ", sizeof(Quaternion), '\n');
		Log::Out("Vector2f Size: ", sizeof(Vector2f), '\n');
		Log::Out("Vector2d Size: ", sizeof(Vector2d), '\n');
		Log::Out("Vector2i Size: ", sizeof(Vector2i), '\n');
		Log::Out("Vector2ui Size: ", sizeof(Vector2ui), '\n');
		Log::Out("Vector3f Size: ", sizeof(Vector3f), '\n');
		Log::Out("Vector3d Size: ", sizeof(Vector3d), '\n');
		Log::Out("Vector3i Size: ", sizeof(Vector3i), '\n');
		Log::Out("Vector3ui Size: ", sizeof(Vector3ui), '\n');
		Log::Out("Vector4f Size: ", sizeof(Vector4f), '\n');
		Log::Out("Vector4d Size: ", sizeof(Vector4d), '\n');
		Log::Out("Vector4i Size: ", sizeof(Vector4i), '\n');
		Log::Out("Vector4ui Size: ", sizeof(Vector4ui), '\n');
		//Log::Out("Vector4uc Size: ", sizeof(Vector4<uint8_t>), '\n');
		Log::Out("Transform Size: ", sizeof(Transform), '\n');
		Log::Out("\n");
	}
	{
		Time a = 348.991s;
		Time b = 9683ms;
		Time c = a + b;

		Time d = 5s - 2s;
		Time e = 10s;
		double f = d / e;
		//double g = d.Mod(e); // d % e;

		Log::Out("Seconds: ", c.AsSeconds(), '\n');
		Log::Out("Milliseconds: ", c.AsMilliseconds(), '\n');
		Log::Out("Microseconds: ", c.AsMicroseconds(), '\n');
		Log::Out('\n');
	}
	{
		Vector3f original(90.0f, 0.0f, 0.0f);
		Quaternion quaternion(original);
		Vector3f restored = quaternion.ToEuler();

		Log::Out("Original: (", original, ")\n");
		Log::Out("Quaternion: (", quaternion, ")\n");
		Log::Out("Restored: (", restored, ")\n");
		Log::Out('\n');
	}
	{
		Log::Out("Vector2f:\n");
		Vector2f a(3.0f, -7.2f);
		Vector2f b(-1.74f, 15.4f);
		Vector2ui c(5, 2);

		Log::Out("(", a, ") + ", 20.0f, " = (", a + 20.0f, ")\n");
		Log::Out(-1.11f, " * (", a, ") = (", -1.11f * a, ")\n");
		Log::Out("(", a, ") + (", b, ") = (", a + b, ")\n");
		Log::Out("(", a, ") - (", b, ") = (", a - b, ")\n");
		Log::Out("(", a, ") * (", b, ") = (", a * b, ")\n");
		Log::Out("(", a, ") / (", b, ") = (", a / b, ")\n");
		Log::Out("(", a, ") angle (", b, ") = ", a.Angle(b), "\n");
		Log::Out("(", a, ") dot (", b, ") = ", a.Dot(b), "\n");
		Log::Out("(", a, ") lerp (", b, ") % ", 0.5f, " = (", a.Lerp(b, 0.5f), ")\n");
		Log::Out("(", a, ") scale ", 10.0f, " = (", a.Scale(10.0f), ")\n");
		Log::Out("(", a, ") rotate ", 1.5708f, " = (", a.Rotate(1.5708f), ")\n");
		Log::Out("-(", a, ") = (", -a, ")\n");
		Log::Out("normalize (", a, ") = (", a.Normalize(), ")\n");
		Log::Out("length (", a, ") = (", a.Length(), ")\n");
		Log::Out("(", a, ") distance (", b, ") = (", a.Distance(b), ")\n");
		Log::Out('\n');

		Log::Out("(", c, ") & ", 9, " = (", c & 9, ")\n");
		Log::Out("(", c, ") | ", 9, " = (", c | 9, ")\n");
		//Log::Out("(", c, ") ^ ", 9, " = (", c ^ 9, ")\n");
		Log::Out("~(", c, ") = (", ~c, ")\n");
		Log::Out("(", c, ") >> ", 1, " = (", c >> 1, ")\n");
		Log::Out("(", c, ") << ", 1, " = (", c << 1, ")\n");
		Log::Out('\n');
	}
	{
		Log::Out("Vector3:\n");
		Vector3f a(12.9f, -2.0f, 6.7f);
		Vector3f b(-9.7f, 15.9f, -13.8f);
		Vector3ui c(5, 2, 1);

		Log::Out("(", a, ") + ", 20.0f, " = (", a + 20.0f, ")\n");
		Log::Out(-1.11f, " * (", a, ") = (", -1.11f * a, ")\n");
		Log::Out("(", a, ") + (", b, ") = (", a + b, ")\n");
		Log::Out("(", a, ") - (", b, ") = (", a - b, ")\n");
		Log::Out("(", a, ") * (", b, ") = (", a * b, ")\n");
		Log::Out("(", a, ") / (", b, ") = (", a / b, ")\n");
		Log::Out("(", a, ") angle (", b, ") = ", a.Angle(b), "\n");
		Log::Out("(", a, ") cross (", b, ") = ", a.Cross(b), "\n");
		Log::Out("(", a, ") dot (", b, ") = ", a.Dot(b), "\n");
		Log::Out("(", a, ") lerp (", b, ") % ", 0.5f, " = (", a.Lerp(b, 0.5f), ")\n");
		Log::Out("(", a, ") scale ", 10.0f, " = (", a.Scale(10.0f), ")\n");
		Log::Out("-(", a, ") = (", -a, ")\n");
		Log::Out("normalize (", a, ") = (", a.Normalize(), ")\n");
		Log::Out("length (", a, ") = (", a.Length(), ")\n");
		Log::Out("(", a, ") distance (", b, ") = (", a.Distance(b), ")\n");
		//Log::Out("(", a, ") rotate (", Vector3f(1.22f, 0.0f, 0.0f), ") = (", a.Rotate(Vector3f(1.22f, 0.0f, 0.0f)), ")\n");
		Log::Out('\n');

		Log::Out("(", c, ") & ", 9, " = (", c & 9, ")\n");
		Log::Out("(", c, ") | ", 9, " = (", c | 9, ")\n");
		//Log::Out("(", c, ") ^ ", 9, " = (", c ^ 9, ")\n");
		Log::Out("~(", c, ") = (", ~c, ")\n");
		Log::Out("(", c, ") >> ", 1, " = (", c >> 1, ")\n");
		Log::Out("(", c, ") << ", 1, " = (", c << 1, ")\n");
		Log::Out('\n');
	}
	{
		Log::Out("Vector4:\n");
		Vector4f a(12.9f, -2.0f, 6.7f, 11.11f);
		Vector4f b(-9.7f, 15.9f, -13.8f, -2.246f);
		Vector4ui c(5, 2, 1, 3);

		Log::Out("(", a, ") + ", 20.0f, " = (", a + 20.0f, ")\n");
		Log::Out(-1.11f, " * (", a, ") = (", -1.11f * a, ")\n");
		Log::Out("(", a, ") + (", b, ") = (", a + b, ")\n");
		Log::Out("(", a, ") - (", b, ") = (", a - b, ")\n");
		Log::Out("(", a, ") * (", b, ") = (", a * b, ")\n");
		Log::Out("(", a, ") / (", b, ") = (", a / b, ")\n");
		Log::Out("(", a, ") angle (", b, ") = ", a.Angle(b), "\n");
		Log::Out("(", a, ") dot (", b, ") = ", a.Dot(b), "\n");
		Log::Out("(", a, ") lerp (", b, ") % ", 0.5f, " = (", a.Lerp(b, 0.5f), ")\n");
		Log::Out("(", a, ") scale ", 10.0f, " = (", a.Scale(10.0f), ")\n");
		Log::Out("-(", a, ") = (", -a, ")\n");
		Log::Out("normalize (", a, ") = (", a.Normalize(), ")\n");
		Log::Out("length (", a, ") = (", a.Length(), ")\n");
		Log::Out("(", a, ") distance (", b, ") = (", a.Distance(b), ")\n");
		Log::Out('\n');

		Log::Out("(", c, ") & ", 9, " = (", c & 9, ")\n");
		Log::Out("(", c, ") | ", 9, " = (", c | 9, ")\n");
		//Log::Out("(", c, ") ^ ", 9, " = (", c ^ 9, ")\n");
		Log::Out("~(", c, ") = (", ~c, ")\n");
		Log::Out("(", c, ") >> ", 1, " = (", c >> 1, ")\n");
		Log::Out("(", c, ") << ", 1, " = (", c << 1, ")\n");
		Log::Out('\n');
	}

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
