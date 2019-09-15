# Acid Guidelines 
September 14, 2019
 
This document is a rough outline for guidelines for Acid. This document covers the languages of C++, C#, and GLSL. Acid is licenced on the MIT Licence, read more on our [LICENSE.md](../LICENSE.md) file. For more about the project read our read more on our [README.md](../README.md) file.

You may also import default settings for [CLion](CLion-Settings.jar), or [Visual Studio Resharper](Resharper.DotSettings).

# Introduction 
This is a set of guidelines for Acid C++17, and GLSL 450. Our guideline is setup for the best code performance, documentation, uniformaty, and readability.

# Example: Example.hpp
```cpp
#include <vector>

#include <LibraryClass.hpp>
#include "ProjectClass.hpp"

namespace Examples {
	enum class Enable {
		Null, On, Off
	}

	/**
	 * @brief A example class.
	 */
	class Example {
	public:
		explicit Example(int x = 0);

		~Example();
	
		/**
		 * Does stuff!
		 * @param doThing If a thing will be done.
		 * @return If there was a error (result does not equal 0).
		 */
		int32_t DoStuff(bool doThing);

		uint32_t GetX() const { return m_x; }
		void SetX(uint32_t x) { m_x = x; }
		float GetY() const { return m_y; }
		void SetY(float y) { m_y = y; }

		const std::vector<std::string> &GetList() const { return m_list; }
		void AddListItem(const std::string &item) {
			m_list.emplace_back(item);
		}

	private:
		uint32_t m_x;
		float m_y;
		std::vector<std::string> m_list;
	}
}
```

# Example: Example.cpp
```cpp
#pragma once

#include "Example.hpp"

namespace Examples {
	Example::Example(int x) : m_x(x), m_y(-1.0f) {
	}
	
	Example::~Example() {
	}

	int32_t Example::DoStuff(bool doThing) {
		m_list.emplace_back(m_x + m_list.size());
		printf("X: %i\n", m_x);

		if (doThing) {
			for (auto item : m_list) {
				printf("Item: %i\n", m_x);
			}
		} else {
			printf("doThing is false!\n");
			return 1;
		}

		for (int x = 0; x < 10; x++) {
			// Do something!
		}

		int i = 0;

		do {
			i++;
		} while (i < 8);

		return 0;
	}
}
```

# Styling Guide
Classes, all methods, functions, and files are camelcase + first letter upper. Member variables start with m_, paramaters are normal camelcased. Tabs are used for indentations, and braces are required for all blocks. Use the member initializer list no matter what. Avoid compiler macros where possible! Use multiple lines instead of long lines, const as much as possible. Line length is usualy never wrapped, VS wraps lines for readability.

# Date Guide
Our date format is day.month.year, for example January 20th 2019 is written as '20.1.19'. 

# GLSL
```glsl
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform UniformObject {
	float passedValue;
} object;

layout(location = 0) out vec4 outColour;

void main(void) {
	outColour = vec4(object.passedValue, 0.0f, 0.0f, 1.0f);
}
```
