#include <iostream>
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

#include <Helpers/RingBuffer.hpp>

using namespace acid;

int main(int argc, char **argv)
{
	{
		Log::Out("Hello out!\n");
		Log::Out(Log::Style::Underlined, Log::Colour::Default, "Hello underlined!\n");
		Log::Debug("Hello debug!\n");
		Log::Info("Hello info!\n");
		Log::Warning("Hello warning!\n");
		Log::Error("Hello error!\n");
		Log::Out("\n");
	}

	{
		RingBuffer<int32_t> buffer{4};

		if (!buffer.push(1, 2, 3, 4))
		{
			return false;
		}

		buffer[1] = 11;

		for (const auto &i : buffer)
		{
			Log::Out("%i, ", i);
		}
		Log::Out("\n");

		if (buffer.front() != 1)
		{
			return false;
		}

		buffer.pop();

		if (buffer.front() != 11)
		{
			return false;
		}

		buffer.pop();

		if (buffer.front() != 3)
		{
			return false;
		}

		buffer.pop();

		if (!buffer.push(5))
		{
			return false;
		}

		buffer.resize(2);

		if (buffer.front() != 4)
		{
			return false;
		}

		buffer.pop();

		if (buffer.front() != 5)
		{
			return false;
		}

		buffer.pop();

		if (buffer.size() != 0)
		{
			return false;
		}
		Log::Out("\n");
	}
	{
		Log::Out("Time Size: %i\n", static_cast<int>(sizeof(Time)));
		Log::Out("Colour Size: %i\n", static_cast<int>(sizeof(Colour)));
		Log::Out("Matrix2 Size: %i\n", static_cast<int>(sizeof(Matrix2)));
		Log::Out("Matrix3 Size: %i\n", static_cast<int>(sizeof(Matrix3)));
		Log::Out("Matrix4 Size: %i\n", static_cast<int>(sizeof(Matrix4)));
		Log::Out("Quaternion Size: %i\n", static_cast<int>(sizeof(Quaternion)));
		Log::Out("Vector2f Size: %i\n", static_cast<int>(sizeof(Vector2f)));
		Log::Out("Vector2d Size: %i\n", static_cast<int>(sizeof(Vector2d)));
		Log::Out("Vector2i Size: %i\n", static_cast<int>(sizeof(Vector2i)));
		Log::Out("Vector2ui Size: %i\n", static_cast<int>(sizeof(Vector2ui)));
		Log::Out("Vector3f Size: %i\n", static_cast<int>(sizeof(Vector3f)));
		Log::Out("Vector3d Size: %i\n", static_cast<int>(sizeof(Vector3d)));
		Log::Out("Vector3i Size: %i\n", static_cast<int>(sizeof(Vector3i)));
		Log::Out("Vector3ui Size: %i\n", static_cast<int>(sizeof(Vector3ui)));
		Log::Out("Vector4f Size: %i\n", static_cast<int>(sizeof(Vector4f)));
		Log::Out("Vector4d Size: %i\n", static_cast<int>(sizeof(Vector4d)));
		Log::Out("Vector4i Size: %i\n", static_cast<int>(sizeof(Vector4i)));
		Log::Out("Vector4ui Size: %i\n", static_cast<int>(sizeof(Vector4ui)));
		Log::Out("Transform Size: %i\n", static_cast<int>(sizeof(Transform)));
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

		Log::Out("Seconds: %f\n", c.AsSeconds());
		Log::Out("Milliseconds: %i\n", c.AsMilliseconds());
		Log::Out("Microseconds: %i\n", c.AsMicroseconds());
		Log::Out("\n");
	}
	{
		Vector3f original(90.0f, 0.0f, 0.0f);
		Quaternion quaternion(original);
		Vector3f restored = quaternion.ToEuler();

		Log::Out("Original: %s\n", original.ToString());
		Log::Out("Quaternion: %s\n", quaternion.ToString());
		Log::Out("Restored: %s\n", restored.ToString());
		Log::Out("\n");
	}
	{
		Log::Out("Vector2f:\n");
		Vector2f a{3.0f, -7.2f};
		Vector2f b{-1.74f, 15.4f};
		Vector2ui c{5, 2};

		Log::Out("  %s + %f = %s\n", a.ToString(), 20.0f, (a + 20.0f).ToString());
		Log::Out("  %f * %s = %s\n", -1.11f, a.ToString(), (-1.11f * a).ToString());
		Log::Out("  %s + %s = %s\n", a.ToString(), b.ToString(), (a + b).ToString());
		Log::Out("  %s - %s = %s\n", a.ToString(), b.ToString(), (a - b).ToString());
		Log::Out("  %s * %s = %s\n", a.ToString(), b.ToString(), (a * b).ToString());
		Log::Out("  %s / %s = %s\n", a.ToString(), b.ToString(), (a / b).ToString());
		Log::Out("  %s ang %s = %f\n", a.ToString(), b.ToString(), a.Angle(b));
		Log::Out("  %s dot %s = %f\n", a.ToString(), b.ToString(), a.Dot(b));
		Log::Out("  %s lerp %s prog %f = %s\n", a.ToString(), b.ToString(), 0.5f, a.Lerp(b, 0.5f).ToString());
		Log::Out("  %s sca %f = %s\n", a.ToString(), 10.0f, a.Scale(10.0f).ToString());
		Log::Out("  %s rot %f = %s\n", a.ToString(), 90.0f, a.Rotate(90.0f).ToString());
		Log::Out("  -%s = %s\n", a.ToString(), (-a).ToString());
		Log::Out("  nor %s = %s\n", a.ToString(), a.Normalize().ToString());
		Log::Out("  len %s = %f\n", a.ToString(), a.Length());
		Log::Out("  %s dist %s = %f\n", a.ToString(), b.ToString(), a.Distance(b));
		Log::Out("\n");

		Log::Out("  %s & %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  %s | %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  %s ^ %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  ~%s = %s\n", c.ToString(), (~c).ToString());
		Log::Out("  %s >> %i = %s\n", c.ToString(), 1, (c >> 1).ToString());
		Log::Out("  %s << %i = %s\n", c.ToString(), 1, (c << 1).ToString());
		Log::Out("\n");
	}
	{
		constexpr auto compileTime = Vector3f{10.0f, -9.0f, 8.0f} + Vector3d{-7.0, -6.0, 5.0};

		Log::Out("Vector3:\n");
		Vector3f a{12.9f, -2.0f, 6.7f};
		Vector3f b{-9.7f, 15.9f, -13.8f};
		Vector3ui c{5, 2, 1};

		Log::Out("  %s + %s = %s\n", a.ToString(), b.ToString(), (a + b).ToString());
		Log::Out("  %s - %s = %s\n", a.ToString(), b.ToString(), (a - b).ToString());
		Log::Out("  %s * %s = %s\n", a.ToString(), b.ToString(), (a * b).ToString());
		Log::Out("  %s / %s = %s\n", a.ToString(), b.ToString(), (a / b).ToString());
		Log::Out("  %s ang %s = %f\n", a.ToString(), b.ToString(), a.Angle(b));
		Log::Out("  %s dot %s = %f\n", a.ToString(), b.ToString(), a.Dot(b));
		Log::Out("  %s cross %s = %f\n", a.ToString(), b.ToString(), a.Cross(b));
		Log::Out("  %s lerp %s prog %f = %s\n", a.ToString(), b.ToString(), 0.5f, a.Lerp(b, 0.5f).ToString());
		Log::Out("  %s sca %f = %s\n", a.ToString(), 10.0f, a.Scale(10.0f).ToString());
		//Log::Out("  %s rot %s = %s\n", a.ToString(), Vector3f(1.22f, 0.0f, 0.0f).ToString(), a.Rotate(Vector3f(1.22f, 0.0f, 0.0f)).ToString());
		Log::Out("  -%s = %s\n", a.ToString(), (-a).ToString());
		Log::Out("  nor %s = %s\n", a.ToString(), a.Normalize().ToString());
		Log::Out("  len %s = %f\n", a.ToString(), a.Length());
		Log::Out("  %s dist %s = %f\n", a.ToString(), b.ToString(), a.Distance(b));
		Log::Out("\n");

		Log::Out("  %s & %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  %s | %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  %s ^ %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  ~%s = %s\n", c.ToString(), (~c).ToString());
		Log::Out("  %s >> %i = %s\n", c.ToString(), 1, (c >> 1).ToString());
		Log::Out("  %s << %i = %s\n", c.ToString(), 1, (c << 1).ToString());
		Log::Out("\n");
	}
	{
		Log::Out("Vector4:\n");
		Vector4f a{12.9f, -2.0f, 6.7f, 11.11f};
		Vector4f b{-9.7f, 15.9f, -13.8f, -2.246f};
		Vector4ui c{5, 2, 1, 3};

		Log::Out("  %s + %s = %s\n", a.ToString(), b.ToString(), (a + b).ToString());
		Log::Out("  %s - %s = %s\n", a.ToString(), b.ToString(), (a - b).ToString());
		Log::Out("  %s * %s = %s\n", a.ToString(), b.ToString(), (a * b).ToString());
		Log::Out("  %s / %s = %s\n", a.ToString(), b.ToString(), (a / b).ToString());
		Log::Out("  %s ang %s = %f\n", a.ToString(), b.ToString(), a.Angle(b));
		Log::Out("  %s dot %s = %f\n", a.ToString(), b.ToString(), a.Dot(b));
		Log::Out("  %s lerp %s prog %f = %s\n", a.ToString(), b.ToString(), 0.5f, a.Lerp(b, 0.5f).ToString());
		Log::Out("  %s sca %f = %s\n", a.ToString(), 10.0f, a.Scale(10.0f).ToString());
		Log::Out("  -%s = %s\n", a.ToString(), (-a).ToString());
		Log::Out("  nor %s = %s\n", a.ToString(), a.Normalize().ToString());
		Log::Out("  len %s = %f\n", a.ToString(), a.Length());
		Log::Out("  %s dist %s = %f\n", a.ToString(), b.ToString(), a.Distance(b));
		Log::Out("\n");

		Log::Out("  %s & %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  %s | %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  %s ^ %i = %s\n", c.ToString(), 9, (c & 9).ToString());
		Log::Out("  ~%s = %s\n", c.ToString(), (~c).ToString());
		Log::Out("  %s >> %i = %s\n", c.ToString(), 1, (c >> 1).ToString());
		Log::Out("  %s << %i = %s\n", c.ToString(), 1, (c << 1).ToString());
		Log::Out("\n");
	}

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
