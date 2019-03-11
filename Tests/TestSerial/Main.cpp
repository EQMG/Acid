#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <Engine/Log.hpp>
#include <Files/File.hpp>
#include <Files/Files.hpp>
#include <Helpers/EnumClass.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Vector2.hpp>
#include <Serialized/Metadata.hpp>
#include <Serialized/Json/Json.hpp>
#include <Serialized/Xml/Xml.hpp>
#include <Serialized/Yaml/Yaml.hpp>

using namespace acid;

enum class ExampleType
{
	A = 1, B = 2, C = 4, D = 8
};
// TODO: Allow bitmask macro to work in namespaces outside of acid.
namespace acid
{
	ENABLE_BITMASK_OPERATORS(ExampleType)
}

namespace test
{
	struct Example1
	{
		std::string paragraph{ "Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit,\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n" };
		std::string content{ "Ut enim ad minim veniam,\nquis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat." };

		struct XML
		{
			std::vector<std::vector<std::string>> data{ { "clunky" }, { "uses more words than necessary" } };

			void Decode(const Metadata& metadata)
			{
				metadata.GetChild("data", data);
			}

			void Encode(Metadata& metadata) const
			{
				metadata.SetChild("data", data);
			}
		} xml;

		std::vector<std::string> json{ "rigid", "better for data interchange" };
		std::vector<std::string> yaml{ "slim and flexible", "better for configuration", "supports comments" };
		std::map<int32_t, std::string> map{ { 10, "Hello World" }, { -2, "Negaitive Keys" }, { 400, "Larger Key" } };
		std::map<int32_t, std::vector<std::string>> vectorMap{ { -1, { "A", "B", "C" } }, { 8, { "1", "2.00", "3.00" } }, { 700, { "%", "$", "#", "&", "#" } } };
		std::vector<std::pair<std::string, BitMask<ExampleType>>> types{ { "AB", ExampleType::A | ExampleType::B }, { "C", ExampleType::C },
			{ "ABD", ExampleType::A | ExampleType::B | ExampleType::D } };
	//	std::vector<std::unique_ptr<float>> uniqueVector{ std::make_unique<float>(10.0f), std::make_unique<float>(-2.1111f) }; // TODO
	//	std::map<Vector2, Matrix4> vectorMatrixMap{ { Vector2(-0.91f, 5998.1f), Matrix4(1.0f) }, { Vector2(75.559f, 1.2433f), Matrix4(0.0f) } }; // Not allowed by Json.
	//	std::array<double, 5> array{ -9.1, 10932.0, 1.111, 64634.324324234, -7436.0043 }; // TODO
	//	float cArray[3]{ 0.0f, 10.0f, -33.3f }; // TODO: By converting into a vector for saving?

		struct Objects
		{
			std::string key{ "value" };
			std::vector<float> values{ 190.0f, 11.0f, -0.001f };

			void Decode(const Metadata &metadata)
			{
				metadata.GetChild("key", key);
				metadata.GetChild("values", values);
			}

			void Encode(Metadata &metadata) const
			{
				metadata.SetChild("key", key);
				metadata.SetChild("values", values);
			}
		} objects;

		void Decode(const Metadata &metadata)
		{
			metadata.GetChild("paragraph", paragraph);
			metadata.GetChild("content", content);
			metadata.GetChild("xml", xml);
			metadata.GetChild("json", json);
			metadata.GetChild("yaml", yaml);
			metadata.GetChild("map", map);
			metadata.GetChild("vectorMap", vectorMap);
		//	metadata.GetChild("array", array);
		//	metadata.GetChild("cArray", cArray);
		//	metadata.GetChild("vectorMatrixMap", vectorMatrixMap);
			metadata.GetChild("types", types);
			metadata.GetChild("objects", objects);
		}

		void Encode(Metadata &metadata) const
		{
			metadata.SetChild("paragraph", paragraph);
			metadata.SetChild("content", content);
			metadata.SetChild("xml", xml);
			metadata.SetChild("json", json);
			metadata.SetChild("yaml", yaml);
			metadata.SetChild("map", map);
			metadata.SetChild("vectorMap", vectorMap);
		//	metadata.SetChild("array", array);
		//	metadata.SetChild("cArray", cArray);
		//	metadata.SetChild("vectorMatrixMap", vectorMatrixMap);
			metadata.SetChild("types", types);
			metadata.SetChild("objects", objects);
		}
	};
}

int main(int argc, char** argv)
{
	test::Example1 example1 = {};
	auto metadata = Metadata();
	example1.Encode(metadata);

	File("Serial/Example1.json", new Json(&metadata)).Write();
	File("Serial/Example1.xml", new Xml("Example", &metadata)).Write();
	File("Serial/Example1.yaml", new Yaml(&metadata)).Write();

	auto jsonLoader = File("Serial/Example1.json", new Json());
	jsonLoader.Read();
	jsonLoader.SetFilename("Serial/Example2.json");
	jsonLoader.Write();

	test::Example1 example2;
	example2.Decode(*jsonLoader.GetMetadata());

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
