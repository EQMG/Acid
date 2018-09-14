# Acid Guidelines 
September 4, 2018
 
This document is a rough outline for guidelines for Acid. This document covers the languages of C++, C#, and GLSL. Acid is licenced on the MIT Licence, read more on our [LICENSE.md](LICENSE.md) file. For more about the project read our read more on our [README.md](README.md) file.

You may also import default settings for [CLion](Documents/CLion-Settings.jar), or [Visual Studio Resharper](Documents/Resharper.DotSettings).

# Introduction 
This is a set of guidelines for Acid C++17, and GLSL 450. Our guideline is setup for the best code performance, documentation, uniformaty, and readability.

# Example: Example.hpp
```cpp
#include <vector>

#include <LibraryClass.hpp>
#include "ProjectClass.hpp"

namespace Examples
{
	enum Enable
	{
		ENABLE_ON = 0,
		ENABLE_OFF = 1
	}

	/// <summary>
	/// A example class.
	/// </summary>
	class Example
	{
	private:
		uint32_t m_x;
		float m_y;
		std::vector<int16_t> m_list;
	public:
		Example(const int &x = 0);

		~Example();
	
		/// <summary>
		/// Does stuff!
		/// </summary>
		/// <param name="doThing"> If a thing will be done. </param>
		/// <returns> If there was a error (result does not equal 0). </returns>
		int DoStuff(const bool &doThing);

		uint32_t GetX() const { return m_x; }
	
		void SetX(const uint32_t &x) { m_x = x; }

		float GetY() const { return m_y; }

		void SetY(const float &y) { m_y = y; }

		bool IsListEmpty() const { return m_list.empty(); }
	}
}
```

# Example: Example.cpp
```cpp
#pragma once

#include "Example.hpp"

namespace Examples
{
	Example::Example(const int &x) :
		m_x(x),
        m_y(-1.0f),
		m_list(std::vector<int16_t>())
	{
	}
	
	Example::~Example()
	{
	}

	int Example::DoStuff(const bool &doThing)
	{
		m_list.emplace_back(m_x + m_list.size());
		fprintf(stdout, "X: %i\n", m_x);

		if (doThing)
		{
			for (auto item : m_list)
			{
				fprintf(stdout, "Item: %i\n", m_x);
			}
		}
		else
		{
			fprintf(stdout, "doThing is false!\n");
			return 1;
		}

		for (int x = 0; x < 10; x++)
		{
			// Do something!
		}

		int i = 0;

		do
		{
			i++;
		} while (i < 8);

		return 0;
	}
}
```

# Styling Guide
Classes, all methods, functions, and files are camelcase + first letter upper. Member variables start with m_, paramaters are normal camelcased. Tabs are used for indentations, and braces are required for all blocks. Use the member initializer list no matter what. Avoid compiler macros where possible! Use multiple lines instead of long lines, const as much as possible. Line length is usualy never wrapped, VS wraps lines for readability.

# Date Guide
Our date format is day.month.year, for example December 1st 2017 is written as '1.12.17'. 

# GLSL
```glsl
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UboObject
{
	float passedValue;
} object;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = vec4(object.passedValue, 0.0, 0.0, 1.0);
}
```
