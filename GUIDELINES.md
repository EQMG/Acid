# Flounder Guidelines 
March 9, 2018
 
This document is a rough outline for guidelines for Flounder. This document covers the languages of C++, C#, and GLSL. Flounder is licenced on the MIT Licence, read more on our [LICENSE](LICENSE) file. For more about the project read our read more on our [README](README) file. 
 
# Introduction 
This is a set of guidelines for Flounder C++17, .NET 4.6.2, and GLSL 450. Our guideline is setup for the best code performance, documentation, uniformaty, and readability. 

# Example: Example.hpp
```cpp
#include <vector>

#include "ProjectClass.hpp"

namespace Examples
{
	enum Enable
	{
		EnableOn = 0,
		EnableOff = 1,
	}

	/// <summary>
	/// A example class.
	/// </summary>
	class Example
	{
	private:
		int m_x;
		std::vector<int> *m_list;
	public:
		float m_y;
		
		Example(const int &x = 0);

		~Example();
	
		/// <summary>
		/// Does stuff!
		/// </summary>
		/// <param name="doThing"> If a thing will be done. </param>
		/// <returns> If there was a error (result does not equal 0). </returns>
		int DoStuff(const bool &doThing);

		int GetX() const { return m_x; }
	
		void SetX(const int &x) { m_x = x; }

		bool IsEmpty() const { return m_list->Size() == 0; }
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
		m_list(new std::vector<int>()),
		m_y(-1.0f)
	{
	}
	
	Example::~Example()
	{
		delete m_list;
	}

	int Example::DoStuff(const bool &doThing)
	{
		m_list->push_back(m_x + m_list->size());
		Logger->Warning("X: %i", m_x);

		if (doThing)
		{
			for (auto item : *m_list)
			{
				Logger->Log("Item: %i", m_x);
			}
		}
		else
		{
			Logger->Error("doThing is false!");
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

layout(binding = 0) uniform UboObject 
{
	float passedValue;
} object;

layout(location = 0) out vec4 outColour;

void main(void) 
{
	outColour = vec4(passedValue, 0.0, 0.0, 1.0);
}
```