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
class Example1
{
public:
	class XML
	{
	public:
		std::vector<std::vector<std::string>> data{{"clunky"}, {"uses more words than necessary"}};
		std::optional<float> optional0{};
		std::optional<std::string> optional1{"Hello optional string!"};

		friend const Metadata &operator>>(const Metadata &metadata, XML &xml)
		{
			metadata.GetChild("data", xml.data);
			metadata.GetChild("optional0", xml.optional0);
			metadata.GetChild("optional1", xml.optional1);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const XML &xml)
		{
			metadata.SetChild("data", xml.data);
			metadata.SetChild("optional0", xml.optional0);
			metadata.SetChild("optional1", xml.optional1);
			return metadata;
		}
	} xml;

	class Objects
	{
	public:
		std::string key{"value"};
		std::vector<float> values{190.0f, 11.0f, -0.001f};

		friend const Metadata &operator>>(const Metadata &metadata, Objects &objects)
		{
			metadata.GetChild("key", objects.key);
			metadata.GetChild("values", objects.values);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const Objects &objects)
		{
			metadata.SetChild("key", objects.key);
			metadata.SetChild("values", objects.values);
			return metadata;
		}
	} objects;

	std::string paragraph{"Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit,\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"};
	std::unique_ptr<std::string> content{std::make_unique<std::string>("Ut enim ad minim veniam,\nquis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.")};

	std::vector<std::string> json{"rigid", "better for data interchange"};
	std::vector<std::string> yaml{"slim and flexible", "better for configuration", "supports comments"};
	std::map<int32_t, std::string> map{{10, "Hello World"}, {-2, "Negaitive Keys"}, {400, "Larger Key"}};
	std::map<int32_t, std::vector<std::string>> vectorMap{{-1, {"A", "B", "C"}}, {8, {"1", "2.00", "3.00"}}, {700, {"%", "$", "#", "&", "#"}}};
	std::vector<std::pair<std::string, BitMask<ExampleType>>> types{{"AB", ExampleType::A | ExampleType::B}, {"C", ExampleType::C},
	{"ABD", ExampleType::A | ExampleType::B | ExampleType::D}};
	//std::vector<std::unique_ptr<float>> uniqueVector{std::make_unique<float>(10.0f), std::make_unique<float>(-2.1111f)};
	//std::map<Vector2f, Matrix4> vectorMatrixMap{{Vector2f{-0.91f, 5998.1f}, Matrix4{1.0f}}, {Vector2f{75.559f, 1.2433f}, Matrix4{0.0f}}}; // Not allowed by Json.
	//std::array<double, 5> array{-9.1, 10932.0, 1.111, 64634.324324234, -7436.0043}; // TODO
	//float cArray[3]{0.0f, 10.0f, -33.3f}; // TODO: By converting into a vector for saving?

	friend const Metadata &operator>>(const Metadata &metadata, Example1 &example1)
	{
		example1.paragraph = metadata.GetChild<std::string>("paragraph");
		metadata.GetChild("content", example1.content);
		metadata.GetChild("xml", example1.xml);
		metadata.GetChild("json", example1.json);
		metadata.GetChild("yaml", example1.yaml);
		metadata.GetChild("map", example1.map);
		metadata.GetChild("vectorMap", example1.vectorMap);
		//metadata.GetChild("array", example1.array);
		//metadata.GetChild("cArray", example1.cArray);
		//metadata.GetChild("vectorMatrixMap", example1.vectorMatrixMap);
		metadata.GetChild("types", example1.types);
		//metadata.GetChild("uniqueVector", example1.uniqueVector);
		metadata.GetChild("objects", example1.objects);
		return metadata;
	}

	friend Metadata &operator<<(Metadata &metadata, const Example1 &example1)
	{
		metadata.SetChild("paragraph", example1.paragraph);
		metadata.SetChild("content", example1.content);
		metadata.SetChild("xml", example1.xml);
		metadata.SetChild("json", example1.json);
		metadata.SetChild("yaml", example1.yaml);
		metadata.SetChild("map", example1.map);
		metadata.SetChild("vectorMap", example1.vectorMap);
		//metadata.SetChild("array", example1.array);
		//metadata.SetChild("cArray", example1.cArray);
		//metadata.SetChild("vectorMatrixMap", example1.vectorMatrixMap);
		metadata.SetChild("types", example1.types);
		//metadata.SetChild("uniqueVector", example1.uniqueVector);
		metadata.SetChild("objects", example1.objects);
		return metadata;
	}
};
}

int main(int argc, char **argv)
{
	test::Example1 example1;
	Metadata metadata;
	metadata << example1;

	File{"Serial/Example1.json", std::make_unique<Json>(&metadata)}.Write();
	File{"Serial/Example1.xml", std::make_unique<Xml>("Example", &metadata)}.Write();
	File{"Serial/Example1.yaml", std::make_unique<Yaml>(&metadata)}.Write();

	File jsonLoader{"Serial/Example1.json", std::make_unique<Json>()};
	jsonLoader.Load();
	jsonLoader.SetFilename("Serial/Example2.json");
	jsonLoader.Write();

	test::Example1 example2;
	*jsonLoader.GetMetadata() >> example2;

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
