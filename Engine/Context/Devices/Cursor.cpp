#include "Cursor.hpp"

#include <GLFW/glfw3.h>
#include "Bitmaps/Bitmap.hpp"

namespace acid {
Cursor::Cursor(const std::filesystem::path &filename, CursorHotspot hotspot) {
	Bitmap bitmap(filename);
	if (!bitmap) return;

	GLFWimage image[1];
	image[0].width = bitmap.GetSize().x;
	image[0].height = bitmap.GetSize().y;
	image[0].pixels = bitmap.GetData().get();

	switch (hotspot) {
	case CursorHotspot::UpperLeft:
		cursor = glfwCreateCursor(image, 0, 0);
		break;
	case CursorHotspot::UpperRight:
		cursor = glfwCreateCursor(image, image->width - 1, 0);
		break;
	case CursorHotspot::BottomLeft:
		cursor = glfwCreateCursor(image, 0, image->height - 1);
		break;
	case CursorHotspot::BottomRight:
		cursor = glfwCreateCursor(image, image->width - 1, image->height - 1);
		break;
	case CursorHotspot::Centered:
		cursor = glfwCreateCursor(image, image->width / 2, image->height / 2);
		break;
	}
}

Cursor::Cursor(CursorStandard standard) {
	cursor = glfwCreateStandardCursor(static_cast<int32_t>(standard));
}

Cursor::~Cursor() {
	glfwDestroyCursor(cursor);
}
}
