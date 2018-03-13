#pragma once

#include "../Maths/Maths.hpp"
#include "../Maths/Matrix4.hpp"

namespace Flounder
{
	class MeshAnimated
	{
	private:
		std::string m_filename;

	public:
		static const Matrix4 *S_CORRECTION;
		static const int MAX_WEIGHTS;

		MeshAnimated(const std::string &filename);

		~MeshAnimated();
	};
}