#include <iostream>
#include <Engine/Log.hpp>
#include <Maths/Maths.hpp>
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


#include <Engine/Engine.hpp>
#include <Files/File.hpp>
#include <Serialized/Json/Json.hpp>
#include <Serialized/Xml/Xml.hpp>
#include <Files/Files.hpp>
#include <Serialized/Yaml/Yaml.hpp>

using namespace acid;

int main(int argc, char **argv)
{
	auto engine = std::make_unique<Engine>(argv[0], true);
	engine->GetModuleManager().Add<Files>(Module::Stage::Always);
	Files::Get()->AddSearchPath("Resources/Engine");

	{
		auto dataYaml = File("Example0.yaml", new Yaml());
		dataYaml.Read();

		auto dataJson = File("Example0.json", new Json(dataYaml.GetMetadata()));
		dataJson.Write();
	}
	{
		auto dataJson = File("Example1.json", new Json());
		dataJson.Read();

		auto dataYaml = File("Example1.yaml", new Yaml(dataJson.GetMetadata()));
		dataYaml.Write();

		auto dataXml = File("Example1.xml", new Xml("EntityDefinition", dataJson.GetMetadata()));
		dataXml.Write();

		auto dataJson2 = File("Example1.json", new Json(dataJson.GetMetadata()));
		dataJson2.Write();

	}

	/*{
		Log::Out("Time Size: %i\n", static_cast<int>(sizeof(Time)));
		Log::Out("Colour Size: %i\n", static_cast<int>(sizeof(Colour)));
		Log::Out("Matrix2 Size: %i\n", static_cast<int>(sizeof(Matrix2)));
		Log::Out("Matrix3 Size: %i\n", static_cast<int>(sizeof(Matrix3)));
		Log::Out("Matrix4 Size: %i\n", static_cast<int>(sizeof(Matrix4)));
		Log::Out("Quaternion Size: %i\n", static_cast<int>(sizeof(Quaternion)));
		Log::Out("Vector2 Size: %i\n", static_cast<int>(sizeof(Vector2)));
		Log::Out("Vector3 Size: %i\n", static_cast<int>(sizeof(Vector3)));
		Log::Out("Vector4 Size: %i\n", static_cast<int>(sizeof(Vector4)));
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
		Vector3 original{ 90.0f, 0.0f, 0.0f };
		Quaternion quaternion = original.ToQuaternion();
		Vector3 restored = quaternion.ToEuler();

		Log::Out("Original: %s\n", original.ToString().c_str());
		Log::Out("Quaternion: %s\n", quaternion.ToString().c_str());
		Log::Out("Restored: %s\n", restored.ToString().c_str());
		Log::Out("\n");
	}
	{
		Log::Out("Vector2:\n");
		Vector2 a{ 3.0f, -7.2f };
		Vector2 b{ -1.74f, 15.4f };
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
	}
	{
		Log::Out("Vector3:\n");
		Vector3 a{ 12.9f, -2.0f, 6.7f };
		Vector3 b{ -9.7f, 15.9f, -13.8f };
		Log::Out("  %s + %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a + b).ToString().c_str());
		Log::Out("  %s - %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a - b).ToString().c_str());
		Log::Out("  %s * %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a * b).ToString().c_str());
		Log::Out("  %s / %s = %s\n", a.ToString().c_str(), b.ToString().c_str(), (a / b).ToString().c_str());
		Log::Out("  %s ang %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Angle(b));
		Log::Out("  %s dot %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Dot(b));
		Log::Out("  %s sca %f = %s\n", a.ToString().c_str(), 10.0f, a.Scale(10.0f).ToString().c_str());
		//	Log::Out("  %s rot %f = %s\n", a.ToString().c_str(), 90.0f, a.Rotate(90.0f).ToString().c_str());
		Log::Out("  -%s = %s\n", a.ToString().c_str(), (-a).ToString().c_str());
		Log::Out("  nor %s = %s\n", a.ToString().c_str(), a.Normalize().ToString().c_str());
		Log::Out("  len %s = %f\n", a.ToString().c_str(), a.Length());
		Log::Out("  %s dist %s = %f\n", a.ToString().c_str(), b.ToString().c_str(), a.Distance(b));
		Log::Out("\n");
	}

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();*/
	return EXIT_SUCCESS;
}
