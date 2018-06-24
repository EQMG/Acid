#include "Compute.hpp"

namespace fl
{
	Compute::Compute(const std::string &shader, const VertexInput &vertexInput) :
		m_shader(shader),
		m_vertexInput(vertexInput),
		m_shaderProgram(new ShaderProgram(shader)),
		m_descriptorSetLayout(VK_NULL_HANDLE),
		m_descriptorPool(VK_NULL_HANDLE),
		m_pipeline(VK_NULL_HANDLE),
		m_pipelineLayout(VK_NULL_HANDLE)
	{
	}

	Compute::~Compute()
	{
		delete m_shaderProgram;
	}
}
