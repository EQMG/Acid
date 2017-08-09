#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::string coreProj = "";
std::string gameProj = "";

std::string corePrefix = "";
std::string gamePrefix = "";

std::string readFile(const std::string &filepath)
{
	std::ifstream ifs = std::ifstream(filepath.c_str());

	if (!ifs.is_open())
	{
		std::cout << "Could not find file: " << filepath << std::endl;
	}

	std::string content(
		std::istreambuf_iterator<char>(ifs.rdbuf()),
		std::istreambuf_iterator<char>()
	);
	return content;
}

std::vector<std::string> split(const std::string &str, const std::string &sep)
{
	char *cstr = const_cast<char*>(str.c_str());
	char *current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());

	while (current != NULL)
	{
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}

	return arr;
}

std::string trim(const std::string &str, const std::string &whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);

	if (strBegin == std::string::npos)
	{
		return ""; // no content
	}

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	std::string result = str;
	result = result.substr(strBegin, strRange);
	return result;
}

bool startsWith(const std::string &text, const std::string &token)
{
	if (text.length() < token.length())
	{
		return false;
	}

	return (text.compare(0, token.length(), token) == 0);
}

bool contains(const std::string &text, const std::string &token)
{
	return text.find(token) != std::string::npos;
}

std::string substring(const std::string &str, const int &start, const int &end)
{
	std::string result = str;
	result = result.substr(start, end - start);
	return result;
}

std::string replace(const std::string &str, const std::string &from, const std::string &to)
{
	std::string result = str;
	size_t start_pos = result.find(from);

	if (start_pos == std::string::npos)
	{
		return NULL;
	}

	result.replace(start_pos, from.length(), to);
	return result;
}

std::string getPrefix(const std::string file)
{
	std::string redef = file;
	std::replace(redef.begin(), redef.end(), '\\', '/');
	std::vector<std::string> prefixSplit = split(std::string(redef), "/");
	std::string folderPrefix = substring(redef, 0, redef.length() - prefixSplit.back().length());
	return folderPrefix;
}

void input()
{
	std::cout << "Input the Flounder .vcxproj file used for the project: " << std::endl << "> ";
	coreProj = "";
	std::cin >> coreProj;
	corePrefix = getPrefix(coreProj);

	std::cout << std::endl << "Input the game .vcxproj file to compile the emscripten file for: " << std::endl << "> ";
	gameProj = "";
	std::cin >> gameProj;
	gamePrefix = getPrefix(gameProj);
}

std::vector<std::string> getSourceFiles(const std::string file, std::string prefix)
{
	std::string fileLoaded = readFile(file);
	std::vector<std::string> lines = split(fileLoaded, "\n");

	std::vector<std::string> result = std::vector<std::string>();

	for (std::vector<std::string>::iterator it = lines.begin(); it < lines.end(); it++)
	{
		std::string line = trim(*it);

		if (startsWith(line, "<ClCompile Include="))
		{
			if (!contains(line, ".h"))
			{
				std::string found = line;
				found = replace(found, "<ClCompile Include=\"", "");
				found = replace(found, "\" />", "");
				found = trim(found);
				// std::replace(found.begin(), found.end(), '\\', '/');
				result.push_back("\"" + prefix + found + "\"");
			}
		}
	}

	return result;
}

void generate()
{
	std::string coreHeaders = "\"" + corePrefix + "src\"";
	std::string coreSources = "";
	std::string gameSources = "";

	for (auto i : getSourceFiles(coreProj, corePrefix))
	{
		coreSources += i + " ";
	}

	for (auto i : getSourceFiles(gameProj, gamePrefix))
	{
		gameSources += i + " ";
	}

	std::string gameRes = gamePrefix + "res";
	std::string outputDir = gamePrefix + "bin/Emscripten";
	std::string commandFolder = "mkdir \"" + outputDir + "\"";
	std::cout << std::endl << commandFolder << std::endl;
	system(commandFolder.c_str());

	std::string command = "emcc -std=c++11 -I" + coreHeaders + " " + coreSources + gameSources +
		" -s USE_GLFW=3 -s FULL_ES3=1 -DFLOUNDER_API_WEB=1 -DGLFW_INCLUDE_ES3=1 --memory-init-file 0 --embed-file " + gameRes +
		" --post-js SoundManager.js -O3 -o " + outputDir + "/floundergame.html";
	std::cout << std::endl << command << std::endl;
	system(command.c_str());

	std::string outputBat = gamePrefix + "bin/emscripten_build.bat";
	std::ofstream out(outputBat);
	out << command;
	out.close();

	std::ofstream outSM(gamePrefix + "/SoundManager.js");
	outSM << "function _SoundManagerAdd(name, filename)";
	outSM << "{";
	outSM << "	Module.SoundManagerAdd(Pointer_stringify(name), Pointer_stringify(filename));";
	outSM << "}";// TODO
	outSM.close();
}

bool load()
{
	if (coreProj.empty() || gameProj.empty())
	{
		input();
	}
	else
	{
		std::cout << std::endl << "Change inputs (" << coreProj << ", " << gameProj << ") (true/false):" << std::endl << "> ";
		std::string change = "";
		std::cin >> change;

		if (change == "true")
		{
			input();
		}
	}

	generate();

	std::cout << std::endl << "Exit application (true/false):" << std::endl << "> ";
	std::string exit = "";
	std::cin >> exit;

	if (exit == "true")
	{
		return false;
	}

	system("cls");
	return true;
}

int main()
{
	std::cout << "This is a tool that is used to compile emscripten apps running on Flounder." << std::endl;
	bool running = true;

	while (running)
	{
		running = load();
	}

	return 0;
}
