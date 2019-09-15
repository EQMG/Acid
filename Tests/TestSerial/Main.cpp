#include <Files/File.hpp>
#include <Files/Files.hpp>
#include <Helpers/EnumClass.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Vector2.hpp>
#include <Serialized/Node.hpp>
#include <Serialized/Json/Json.hpp>
#include <Zip/ZipArchive.h>
//#include <Serialized/Xml/Xml.hpp>
//#include <Serialized/Yaml/Yaml.hpp>

using namespace acid;

enum class ExampleType {
	A = 1,
	B = 2,
	C = 4,
	D = 8
};

// TODO: Allow bitmask macro to work in namespaces outside of acid.
namespace acid {
ENABLE_BITMASK_OPERATORS(ExampleType);
}

namespace test {
class Example1 {
public:
	class XML {
	public:
		std::vector<std::vector<std::string>> data = {{"clunky"}, {"uses more words than necessary"}};
		std::optional<float> optional0;
		std::optional<std::string> optional1 = "Hello optional string!";

		friend const Node &operator>>(const Node &node, XML &xml) {
			node["data"].Get(xml.data);
			node["optional0"].Get(xml.optional0);
			node["optional1"].Get(xml.optional1);
			return node;
		}

		friend Node &operator<<(Node &node, const XML &xml) {
			node["data"].Set(xml.data);
			node["optional0"].Set(xml.optional0);
			node["optional1"].Set(xml.optional1);
			return node;
		}
	} xml;

	class Objects {
	public:
		std::string key = "value";
		std::vector<float> values = {190.0f, 11.0f, -0.001f};
		std::vector<Vector3f> vectors = {Vector3f::Left, Vector3f::Right, Vector3f::Up, Vector3f::Down};

		friend const Node &operator>>(const Node &node, Objects &objects) {
			node["key"].Get(objects.key);
			node["values"].Get(objects.values);
			node["vectors"].Get(objects.vectors);
			return node;
		}

		friend Node &operator<<(Node &node, const Objects &objects) {
			node["key"].Set(objects.key);
			node["values"].Set(objects.values);
			node["vectors"].Set(objects.vectors);
			return node;
		}
	} objects;

	std::string paragraph = "Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit,\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n";
	std::unique_ptr<std::string> content = std::make_unique<std::string>(
		"Ut enim ad minim veniam,\nquis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.");

	Time timeNow = Time::Now();
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::vector<std::string> json = {"rigid", "better for data interchange"};
	std::vector<std::string> yaml = {"slim and flexible", "better for configuration", "supports comments"};
	std::map<int32_t, std::string> map = {{10, "Hello World"}, {-2, "Negative Keys"}, {400, "Larger Key"}};
	std::map<int32_t, std::vector<std::string>> vectorMap = {{-1, {"A", "B", "C"}}, {8, {"1", "2.00", "3.00"}}, {700, {"%", "$", "#", "&", "#"}}};
	std::vector<std::pair<std::string, BitMask<ExampleType>>> types = {
		{"AB", ExampleType::A | ExampleType::B}, {"C", ExampleType::C},
		{"ABD", ExampleType::A | ExampleType::B | ExampleType::D}
	};
	//std::vector<std::unique_ptr<float>> uniqueVector = {std::make_unique<float>(10.0f), std::make_unique<float>(-2.1111f)};
	//std::map<Vector2f, Matrix4> vectorMatrixMap = {{Vector2f(-0.91f, 5998.1f), Matrix4(1.0f)}, {Vector2f(75.559f, 1.2433f), Matrix4(0.0f)}}; // Not allowed by Json.
	//std::array<double, 5> array = {-9.1, 10932.0, 1.111, 64634.324324234, -7436.0043}; // TODO
	//float cArray[3] = {0.0f, 10.0f, -33.3f}; // TODO: By converting into a vector for saving?

	friend const Node &operator>>(const Node &node, Example1 &example1) {
		node["timeNow"].Get(example1.timeNow);
		node["currentPath"].Get(example1.currentPath);
		node["paragraph"].Get(example1.paragraph);
		node["content"].Get(example1.content);
		node["xml"].Get(example1.xml);
		node["json"].Get(example1.json);
		node["yaml"].Get(example1.yaml);
		node["map"].Get(example1.map);
		node["vectorMap"].Get(example1.vectorMap);
		//node["array"].Get(example1.array);
		//node["cArray"].Get(example1.cArray);
		//node["vectorMatrixMap"].Get(example1.vectorMatrixMap);
		node["types"].Get(example1.types);
		//node["uniqueVector"].Get(example1.uniqueVector);
		node["objects"].Get(example1.objects);
		return node;
	}

	friend Node &operator<<(Node &node, const Example1 &example1) {
		node["timeNow"].Set(example1.timeNow);
		node["currentPath"].Set(example1.currentPath);
		node["paragraph"].Set(example1.paragraph);
		node["content"].Set(example1.content);
		node["xml"].Set(example1.xml);
		node["json"].Set(example1.json);
		node["yaml"].Set(example1.yaml);
		node["map"].Set(example1.map);
		node["vectorMap"].Set(example1.vectorMap);
		//node["array"].Set(example1.array);
		//node["cArray"].Set(example1.cArray);
		//node["vectorMatrixMap"].Set(example1.vectorMatrixMap);
		node["types"].Set(example1.types);
		//node["uniqueVector"].Set(example1.uniqueVector);
		node["objects"].Set(example1.objects);
		return node;
	}
};
}

int main(int argc, char **argv) {
	{
		auto source = R"({"message":"hello world","value":3})";
		Json json;
		json.Load(source);

		auto value = json["value"];
		value.Set(3 * value->Get<int32_t>() + 2);

		json["values"] = std::vector{10, 11, -1, 2};

		Log::Out(json.Write(Node::Format::Minified), '\n');
		//Log::Out(Xml("source", &json).Write(Node::Format::Minified), '\n');
	}

	test::Example1 example1;
	Node node;
	node = example1;

	File("Serial/Example1.json", std::make_unique<Json>(node)).Write();
	//File("Serial/Example1.xml", std::make_unique<Xml>("Example", node)).Write();
	//File("Serial/Example1.yaml", std::make_unique<Yaml>(node)).Write();

	File jsonLoader("Serial/Example1.json", std::make_unique<Json>());
	jsonLoader.Load();
	//jsonLoader.GetNode()["objects"]["key"]->SetValue("modified");
	jsonLoader.SetFilename("Serial/Example2.json");
	jsonLoader.Write();

	test::Example1 example2;
	*jsonLoader.GetNode() >> example2;

	/*ZipArchive zip0("Serial.zip");
	zip0.AddEntry("hello.txt", "Hello World!");
	zip0.Write();
	zip0.Close();*/
	ZipArchive zip("Serial.zip");
	for (const auto &entry : zip.GetEntryNames())
		Log::Out(entry, '\n');
	zip.ExtractAll("Extracted");
	//zip.ExtractDir("Serial", "Extracted");

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
