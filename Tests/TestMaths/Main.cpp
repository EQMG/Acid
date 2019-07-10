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
		RingBuffer<int32_t> buffer{4};

		if (!buffer.push(1, 2, 3, 4))
		{
			return false;
		}

		buffer[1] = 11;

		for (const auto &i : buffer)
		{
			std::cout << i << ", ";
		}
		std::cout << '\n';

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
		std::cout << '\n';
	}
	{
		std::cout << "Time Size: " << sizeof(Time) << '\n';
		std::cout << "Colour Size: " << sizeof(Colour) << '\n';
		std::cout << "Matrix2 Size: " << sizeof(Matrix2) << '\n';
		std::cout << "Matrix3 Size: " << sizeof(Matrix3) << '\n';
		std::cout << "Matrix4 Size: " << sizeof(Matrix4) << '\n';
		std::cout << "Quaternion Size: " << sizeof(Quaternion) << '\n';
		std::cout << "Vector2f Size: " << sizeof(Vector2f) << '\n';
		std::cout << "Vector2d Size: " << sizeof(Vector2d) << '\n';
		std::cout << "Vector2i Size: " << sizeof(Vector2i) << '\n';
		std::cout << "Vector2ui Size: " << sizeof(Vector2ui) << '\n';
		std::cout << "Vector3f Size: " << sizeof(Vector3f) << '\n';
		std::cout << "Vector3d Size: " << sizeof(Vector3d) << '\n';
		std::cout << "Vector3i Size: " << sizeof(Vector3i) << '\n';
		std::cout << "Vector3ui Size: " << sizeof(Vector3ui) << '\n';
		std::cout << "Vector4f Size: " << sizeof(Vector4f) << '\n';
		std::cout << "Vector4d Size: " << sizeof(Vector4d) << '\n';
		std::cout << "Vector4i Size: " << sizeof(Vector4i) << '\n';
		std::cout << "Vector4ui Size: " << sizeof(Vector4ui) << '\n';
		//std::cout << "Vector4uc Size: " << sizeof(Vector4<uint8_t>) << '\n';
		std::cout << "Transform Size: " << sizeof(Transform) << '\n';
		std::cout << "\n";
	}
	{
		Time a = 348.991s;
		Time b = 9683ms;
		Time c = a + b;

		Time d = 5s - 2s;
		Time e = 10s;
		double f{d / e};
		//double g{d.Mod(e)}; // d % e;

		std::cout << "Seconds: " << c.AsSeconds() << '\n';
		std::cout << "Milliseconds: " << c.AsMilliseconds() << '\n';
		std::cout << "Microseconds: " << c.AsMicroseconds() << '\n';
		std::cout << '\n';
	}
	{
		Vector3f original(90.0f, 0.0f, 0.0f);
		Quaternion quaternion(original);
		Vector3f restored = quaternion.ToEuler();

		std::cout << "Original: {" << original << "}\n";
		std::cout << "Quaternion: {" << quaternion << "}\n";
		std::cout << "Restored: {" << restored << "}\n";
		std::cout << '\n';
	}
	{
		std::cout << "Vector2f:\n";
		Vector2f a{3.0f, -7.2f};
		Vector2f b{-1.74f, 15.4f};
		Vector2ui c{5, 2};

		std::cout << "{" << a << "} + " << 20.0f << " = {" << (a + 20.0f) << "}\n";
		std::cout << -1.11f << " * {" << a << "} = {" << (-1.11f * a) << "}\n";
		std::cout << "{" << a << "} + {" << b << "} = {" << (a + b) << "}\n";
		std::cout << "{" << a << "} - {" << b << "} = {" << (a - b) << "}\n";
		std::cout << "{" << a << "} * {" << b << "} = {" << (a * b) << "}\n";
		std::cout << "{" << a << "} / {" << b << "} = {" << (a / b) << "}\n";
		std::cout << "{" << a << "} angle {" << b << "} = " << a.Angle(b) << "\n";
		std::cout << "{" << a << "} dot {" << b << "} = " << a.Dot(b) << "\n";
		std::cout << "{" << a << "} lerp {" << b << "} % " << 0.5f << " = {" << a.Lerp(b, 0.5f) << "}\n";
		std::cout << "{" << a << "} scale " << 10.0f << " = {" << a.Scale(10.0f) << "}\n";
		std::cout << "{" << a << "} rotate " << 1.5708f << " = {" << a.Rotate(1.5708f) << "}\n";
		std::cout << "-{" << a << "} = {" << (-a) << "}\n";
		std::cout << "normalize {" << a << "} = {" << a.Normalize() << "}\n";
		std::cout << "length {" << a << "} = {" << a.Length() << "}\n";
		std::cout << "{" << a << "} distance {" << b << "} = {" << a.Distance(b) << "}\n";
		std::cout << '\n';

		std::cout << "{" << c << "} & " << 9 << " = {" << (c & 9) << "}\n";
		std::cout << "{" << c << "} | " << 9 << " = {" << (c | 9) << "}\n";
		//std::cout << "{" << c << "} ^ " << 9 << " = {" << (c ^ 9) << "}\n";
		std::cout << "~{" << c << "} = {" << (~c) << "}\n";
		std::cout << "{" << c << "} >> " << 1 << " = {" << (c >> 1) << "}\n";
		std::cout << "{" << c << "} << " << 1 << " = {" << (c << 1) << "}\n";
		std::cout << '\n';
	}
	{
		constexpr auto compileTime = Vector3f{10.0f, -9.0f, 8.0f} + Vector3d{-7.0, -6.0, 5.0};

		std::cout << "Vector3:\n";
		Vector3f a{12.9f, -2.0f, 6.7f};
		Vector3f b{-9.7f, 15.9f, -13.8f};
		Vector3ui c{5, 2, 1};
		
		std::cout << "{" << a << "} + " << 20.0f << " = {" << (a + 20.0f) << "}\n";
		std::cout << -1.11f << " * {" << a << "} = {" << (-1.11f * a) << "}\n";
		std::cout << "{" << a << "} + {" << b << "} = {" << (a + b) << "}\n";
		std::cout << "{" << a << "} - {" << b << "} = {" << (a - b) << "}\n";
		std::cout << "{" << a << "} * {" << b << "} = {" << (a * b) << "}\n";
		std::cout << "{" << a << "} / {" << b << "} = {" << (a / b) << "}\n";
		std::cout << "{" << a << "} angle {" << b << "} = " << a.Angle(b) << "\n";
		std::cout << "{" << a << "} cross {" << b << "} = " << a.Cross(b) << "\n";
		std::cout << "{" << a << "} dot {" << b << "} = " << a.Dot(b) << "\n";
		std::cout << "{" << a << "} lerp {" << b << "} % " << 0.5f << " = {" << a.Lerp(b, 0.5f) << "}\n";
		std::cout << "{" << a << "} scale " << 10.0f << " = {" << a.Scale(10.0f) << "}\n";
		std::cout << "-{" << a << "} = {" << (-a) << "}\n";
		std::cout << "normalize {" << a << "} = {" << a.Normalize() << "}\n";
		std::cout << "length {" << a << "} = {" << a.Length() << "}\n";
		std::cout << "{" << a << "} distance {" << b << "} = {" << a.Distance(b) << "}\n";
		//std::cout << "{" << a << "} rotate {" << Vector3f{1.22f, 0.0f, 0.0f} << "} = {" << a.Rotate(Vector3f{1.22f, 0.0f, 0.0f}) << "}\n";
		std::cout << '\n';

		std::cout << "{" << c << "} & " << 9 << " = {" << (c & 9) << "}\n";
		std::cout << "{" << c << "} | " << 9 << " = {" << (c | 9) << "}\n";
		//std::cout << "{" << c << "} ^ " << 9 << " = {" << (c ^ 9) << "}\n";
		std::cout << "~{" << c << "} = {" << (~c) << "}\n";
		std::cout << "{" << c << "} >> " << 1 << " = {" << (c >> 1) << "}\n";
		std::cout << "{" << c << "} << " << 1 << " = {" << (c << 1) << "}\n";
		std::cout << '\n';
	}
	{
		std::cout << "Vector4:\n";
		Vector4f a{12.9f, -2.0f, 6.7f, 11.11f};
		Vector4f b{-9.7f, 15.9f, -13.8f, -2.246f};
		Vector4ui c{5, 2, 1, 3};

		std::cout << "{" << a << "} + " << 20.0f << " = {" << (a + 20.0f) << "}\n";
		std::cout << -1.11f << " * {" << a << "} = {" << (-1.11f * a) << "}\n";
		std::cout << "{" << a << "} + {" << b << "} = {" << (a + b) << "}\n";
		std::cout << "{" << a << "} - {" << b << "} = {" << (a - b) << "}\n";
		std::cout << "{" << a << "} * {" << b << "} = {" << (a *b) << "}\n";
		std::cout << "{" << a << "} / {" << b << "} = {" << (a / b) << "}\n";
		std::cout << "{" << a << "} angle {" << b << "} = " << a.Angle(b) << "\n";
		std::cout << "{" << a << "} dot {" << b << "} = " << a.Dot(b) << "\n";
		std::cout << "{" << a << "} lerp {" << b << "} % " << 0.5f << " = {" << a.Lerp(b, 0.5f) << "}\n";
		std::cout << "{" << a << "} scale " << 10.0f << " = {" << a.Scale(10.0f) << "}\n";
		std::cout << "-{" << a << "} = {" << (-a) << "}\n";
		std::cout << "normalize {" << a << "} = {" << a.Normalize() << "}\n";
		std::cout << "length {" << a << "} = {" << a.Length() << "}\n";
		std::cout << "{" << a << "} distance {" << b << "} = {" << a.Distance(b) << "}\n";
		std::cout << '\n';

		std::cout << "{" << c << "} & " << 9 << " = {" << (c & 9) << "}\n";
		std::cout << "{" << c << "} | " << 9 << " = {" << (c | 9) << "}\n";
		//std::cout << "{" << c << "} ^ " << 9 << " = {" << (c ^ 9) << "}\n";
		std::cout << "~{" << c << "} = {" << (~c) << "}\n";
		std::cout << "{" << c << "} >> " << 1 << " = {" << (c >> 1) << "}\n";
		std::cout << "{" << c << "} << " << 1 << " = {" << (c << 1) << "}\n";
		std::cout << '\n';
	}

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
