#include <bitmask/bitmask.hpp>

#include <Files/File.hpp>
#include <Files/Files.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Vector2.hpp>
#include <Files/Node.hpp>
#include <Files/Json/Json.hpp>
#include <Files/Xml/Xml.hpp>
//#include <Files/Yaml/Yaml.hpp>
#include <Files/Zip/ZipArchive.hpp>

using namespace acid;

namespace test {
enum class ExampleType {
	A = 1,
	B = 2,
	C = 4,
	D = 8
};

class Example1 {
public:
	class XML {
	public:
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
		
		std::vector<std::vector<std::string>> data = {{"clunky"}, {"uses more words than necessary"}};
		std::optional<float> optional0;
		std::optional<std::string> optional1 = "Hello optional string!";
	} xml;

	class Objects {
	public:
		friend const Node &operator>>(const Node &node, Objects &objects) {
			node["url"].Get(objects.url);
			node["key"].Get(objects.key);
			node["values"].Get(objects.values);
			node["vectors"].Get(objects.vectors);
			return node;
		}

		friend Node &operator<<(Node &node, const Objects &objects) {
			node["url"].Set(objects.url);
			node["key"].Set(objects.key);
			node["values"].Set(objects.values);
			node["vectors"].Set(objects.vectors);
			return node;
		}
		
		std::string url = R"(<a href=\"http://twitter.com/download/iphone\" rel=\"nofollow\">Twitter for iPhone</a>)";
		std::string key = "value";
		std::vector<float> values = {190.0f, 11.0f, -0.001f};
		std::vector<Vector3f> vectors = {Vector3f::Left, Vector3f::Right, Vector3f::Up, Vector3f::Down};
	} objects;

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
		//node["types"].Get(example1.types);
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
		//node["types"].Set(example1.types);
		//node["uniqueVector"].Set(example1.uniqueVector);
		node["objects"].Set(example1.objects);
		return node;
	}

	std::string paragraph = "Lorem ipsum dolor sit amet,\nconsectetur adipiscing elit,\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n";
	std::unique_ptr<std::string> content = std::make_unique<std::string>(
		"Ut enim ad minim veniam,\nquis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.");

	Time timeNow = Time::Now();
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::vector<std::string> json = {"rigid", "better for data interchange"};
	std::vector<std::string> yaml = {"slim and flexible", "better for configuration", "supports comments"};
	std::map<int32_t, std::string> map = {{10, "Hello World"}, {-2, "Negative Keys"}, {400, "Larger Key"}};
	std::map<int32_t, std::vector<std::string>> vectorMap = {{-1, {"A", "B", "C"}}, {8, {"1", "2.00", "3.00"}}, {700, {"%", "$", "#", "&", "#"}}};
	//std::vector<std::pair<std::string, bitmask::bitmask<ExampleType>>> types = {
	//	{"AB", ExampleType::A | ExampleType::B}, {"C", ExampleType::C},
	//	{"ABD", ExampleType::A | ExampleType::B | ExampleType::D}
	//};
	//std::vector<std::unique_ptr<float>> uniqueVector = {std::make_unique<float>(10.0f), std::make_unique<float>(-2.1111f)};
	//std::map<Vector2f, Matrix4> vectorMatrixMap = {{Vector2f(-0.91f, 5998.1f), Matrix4(1.0f)}, {Vector2f(75.559f, 1.2433f), Matrix4(0.0f)}}; // Not allowed by Json.
	//std::array<double, 5> array = {-9.1, 10932.0, 1.111, 64634.324324234, -7436.0043}; // TODO
	//float cArray[3] = {0.0f, 10.0f, -33.3f}; // TODO: By converting into a vector for saving?
};

class User {
public:
	friend const Node &operator>>(const Node &node, User &user) {
		node["_username"].Get(user.username);
		node["_fullname"].Get(user.fullname);
		node["_description"].Get(user.description);
		node["_employed"].Get(user.employed);
		node["_birthday"].Get(user.birthday);
		return node;
	}

	friend Node &operator<<(Node &node, const User &user) {
		node["_username"].Set(user.username);
		node["_fullname"].Set(user.fullname);
		node["_description"].Set(user.description);
		node["_employed"].Set(user.employed);
		node["_birthday"].Set(user.birthday);
		return node;
	}
	
	std::string username;
	std::string fullname;
	std::string description;
	bool employed = false;
	std::string birthday;
};
}

int main(int argc, char **argv) {
	test::Example1 example1;
	Node node;
	node = example1;

	// Appends different types into a array.
	node["array1"]->Append("Hello", nullptr, 10, 4.8924f);

	// Creates a array, then appends values to the back of the array.
	node["array2"] = std::vector{1.0f, 10.0f, -5.55f, 9.3456f};
	node["array2"]->Append(64, 32.1f, -2.0);
	//node["array2"].SetName("array2_renamed");
	//auto array2Name{node["array2"].GetName()};
	//auto array2{node["array2"].Get<std::vector<float>>()};

	auto timeNow = node["timeNow"].Get<int64_t>(123456); // 123456
	node.RemoveProperty("timeNow");

	auto data00 = node["xml"]["data"][0][0].Get<std::string>(); // "clunky"
	auto data10 = node["xml"]["data"][1][0].Get<std::string>(); // "uses more words than necessary"

	auto mapN2 = node["map"]["-2"].Get<std::string>(); // TODO: Can names be numbers without searching with keys?
	auto map400 = node["map"]["400"].Get<std::string>(); // TODO: Can names be numbers without searching with keys?

	if (auto mapN2_ = node["map"]["-2"]) {
		auto value = mapN2_.Get<std::string>();
	}

	node["users"][0] = test::User{"user1", "User One", "Description 1...", false, "10/07/2000"};
	node["users"][1] = test::User{"user2", "User Two", "Description 2...", true, "11/05/1999"};
	node["users"][3] = test::User{"user3", "User Three", "Description 3...", false, "2/03/1996"};
	node["users"][6] = test::User{"user4", "User Four", "Description 4...", true, "30/04/2002"};
	auto users = node["users"].Get<std::vector<std::optional<test::User>>>();

	{
		// Test Json writer.
		File fileJson1(File::Type::Json, node);
		fileJson1.Write("Serial/Test1.json", Node::Format::Beautified);

		// Test Json reader.
		File fileJson2(File::Type::Json);
		fileJson2.Load("Serial/Test1.json");
		// Ensure Test1.json and Test2.json values are the same.
		fileJson2.Write("Serial/Test2.json", Node::Format::Beautified);
	}
	{
		// Test Xml writer.
		File fileXml1(File::Type::Xml, node);
		fileXml1.GetNode().SetName("root");
		fileXml1.Write("Serial/Test1.xml", Node::Format::Beautified);

		/*// Test Xml reader.
		File fileXml2(std::make_unique<Xml>("root"));
		fileXml2.Load("Serial/Test1.xml");
		// Ensure Test1.xml and Test2.xml values are the same.
		fileXml2.Write("Serial/Test2.xml", Node::Format::Beautified);*/
	}
	{
		std::string source = R"({"message":"hello world","value":3})";
		Node json;
		json.ParseString<Json>(source);

		auto value = json["value"];
		value.Set(3 * value->Get<int32_t>() + 2);

		json["values"] = std::vector{10, 11, -1, 2};

		Log::Out(json.WriteString<Json>(Node::Format::Minified), '\n');
	}
	
	/*ZipArchive zip0("Serial.zip");
	zip0.AddEntry("hello.txt", "Hello World!");
	zip0.Write();
	zip0.Close();*/
	/*ZipArchive zip("Serial.zip");
	for (const auto &entry : zip.GetEntryNames())
		Log::Out(entry, '\n');
	zip.ExtractAll("Extracted");*/
	//zip.ExtractDir("Serial", "Extracted");

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}

namespace bitmask {
template<>
struct enable_bitmask_operators<test::ExampleType> : std::true_type {
};
}
