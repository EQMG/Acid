#pragma once

#include <string>

#include "helperstring.h"

class file
{
private:
	std::string path;
	std::string name;

public:
	static const std::string FILE_SEPARATOR;
	static file *const RES_FOLDER;

	/// <summary>
	/// Constructor for file.
	/// </summary>
	/// <param name="path"> The path for this file to represent. </param>
	file(std::string &path);

	/// <summary>
	/// Constructor for file.
	/// </summary>
	/// <param name="paths"> Paths for this file to represent. </param>
	file(std::vector<std::string> paths);

	/// <summary>
	/// Constructor for file.
	/// </summary>
	/// <param name="file"> The file for this file to represent. </param>
	/// <param name="subFile"> The name of the represented sub file. </param>
	file(file *file, std::string &subFile);

	/// <summary>
	/// Constructor for file.
	/// </summary>
	/// <param name="file"> The file for this file to represent. </param>
	/// <param name="subFiles"> Names of the represented sub file. </param>
	file(file *file, std::vector<std::string> subFiles);

	/// <summary>
	/// Deconstructor for the file.
	/// </summary>
	~file();

	/// <summary>
	/// Creates a reader for the file.
	/// </summary>
	/// <returns> A buffered reader for the file. </returns>
	// virtual BufferedReader *getReader();

	/// <summary>
	/// Gets a input steam to the file path.
	/// </summary>
	/// <returns> Returns a input steam to the file path. </returns>
	// virtual InputStream *getInputStream();

	/// <summary>
	/// Gets the name / sub-file of this file.
	/// </summary>
	/// <returns> The name / sub-file of this file. </returns>
	virtual std::string getName();

	/// <summary>
	/// Gets the path to the represented file(s)
	/// </summary>
	/// <returns> The path to the represented file(s). </returns>
	virtual std::string getPath();
};

