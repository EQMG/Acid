# Acid Guidelines 
March 1, 2020
 
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

		uint32_t GetX() const { return x; }
		void SetX(uint32_t x) { this->x = x; }
		float GetY() const { return y; }
		void SetY(float y) { this->y = y; }

		const std::vector<std::string> &GetList() const { return list; }
		void AddListItem(const std::string &item) {
			list.emplace_back(item);
		}

	private:
		uint32_t x;
		float y = -1.0f;
		std::vector<std::string> list;
	}
}
```

# Example: Example.cpp
```cpp
#pragma once

#include "Example.hpp"

namespace Examples {
	Example::Example(int x) : x(x) {
	}
	
	Example::~Example() {
	}

	int32_t Example::DoStuff(bool doThing) {
		printf("X: %i\n", x);

		if (doThing) {
			for (const auto &item : list) {
				printf("Item: %i\n", x);
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
