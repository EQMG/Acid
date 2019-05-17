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
	Log::Out("Hello out!\n");
	Log::Out(Log::Style::Underlined, "Hello underlined!\n");
	Log::Info("Hello info!\n");
	Log::Success("Hello success!\n");
	Log::Warning("Hello warning!\n");
	Log::Error("Hello error!\n");

	{
		RingBuffer<int32_t> buffer(4);

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
		//Log::Out("Vector3d Size: %i\n", static_cast<int>(sizeof(Vector3d)));
		//Log::Out("Vector3i Size: %i\n", static_cast<int>(sizeof(Vector3i)));
		Log::Out("Vector4f Size: %i\n", static_cast<int>(sizeof(Vector4f)));
		//Log::Out("Vector4d Size: %i\n", static_cast<int>(sizeof(Vector4d)));
		//Log::Out("Vector4i Size: %i\n", static_cast<int>(sizeof(Vector4i)));
		Log::Out("Transform Size: %i\n", static_cast<int>(sizeof(Transform)));
		Log::Out("\n");
	}
	{
		Time a = Time::Seconds(348.991f);
		Time b = Time::Milliseconds(9683);
		Time c = a + b;

		Log::Out("Seconds: %f\n", c.AsSeconds());
		Log::Out("Milliseconds: %i\n", c.AsMilliseconds());
		Log::Out("Microseconds: %i\n", c.AsMicroseconds());
		Log::Out("\n");
	}
	{
		Vector3f original(90.0f, 0.0f, 0.0f);
		Quaternion quaternion = Quaternion(original);
		Vector3f restored = quaternion.ToEuler();

		Log::Out("Original: %s\n", original.ToString().c_str());
		Log::Out("Quaternion: %s\n", quaternion.ToString().c_str());
		Log::Out("Restored: %s\n", restored.ToString().c_str());
		Log::Out("\n");
	}
	{
		Log::Out("Vector2f:\n");
		Vector2f a(3.0f, -7.2f);
		Vector2f b(-1.74f, 15.4f);
		Vector2ui c(5, 2);

		Log::Out("  %s + %f = %s\n", a.ToString().c_str(), 20.0f, (a + 20.0f).ToString().c_str());
		Log::Out("  %f * %s = %s\n", -1.11f, a.ToString().c_str(), (-1.11f * a).ToString().c_str());
		Log::Out("  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a + b).ToString().c_str());
		Log::Out("  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a - b).ToString().c_str());
		Log::Out("  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a * b).ToString().c_str());
		Log::Out("  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a / b).ToString().c_str());
		Log::Out("  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		Log::Out("  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		Log::Out("  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		Log::Out("  %s rot %f = %s\n", a.ToString().c_str(), 90.0f, a.Rotate(90.0f).ToString().c_str());
		Log::Out("  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		Log::Out("  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		Log::Out("  len %s = %f\n", a.ToString().c_str(), a.Length());
		Log::Out("  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		Log::Out("\n");

		Log::Out("  %s & %i = %s\n", c.ToString().c_str(), 9, (c & 9).ToString().c_str());
		Log::Out("  %s | %i = %s\n", c.ToString().c_str(), 9, (c & 9).ToString().c_str());
		Log::Out("  %s ^ %i = %s\n", c.ToString().c_str(), 9, (c & 9).ToString().c_str());
		Log::Out("  ~%s = %s\n", c.ToString().c_str(), (~c).ToString().c_str());
		Log::Out("  %s >> %i = %s\n", c.ToString().c_str(), 1, (c >> 1).ToString().c_str());
		Log::Out("  %s << %i = %s\n", c.ToString().c_str(), 1, (c << 1).ToString().c_str());
		Log::Out("\n");
	}
	{
		Log::Out("Vector3:\n");
		Vector3f a(12.9f, -2.0f, 6.7f);
		Vector3f b(-9.7f, 15.9f, -13.8f);

		Log::Out("  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a + b).ToString().c_str());
		Log::Out("  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a - b).ToString().c_str());
		Log::Out("  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a * b).ToString().c_str());
		Log::Out("  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a / b).ToString().c_str());
		Log::Out("  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		Log::Out("  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		Log::Out("  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		//Log::Out("  %s rot %s = %s\n", a.ToString().c_str(), Vector3f(1.22f, 0.0f, 0.0f).ToString().c_str(), a.Rotate(Vector3f(1.22f, 0.0f, 0.0f)).ToString().c_str());
		Log::Out("  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		Log::Out("  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		Log::Out("  len %s = %f\n", a.ToString().c_str(), a.Length());
		Log::Out("  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		Log::Out("\n");

		//Log::Out << "  " << a << " + " << b << " = " << a + b << Log::Endl;
	}

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
