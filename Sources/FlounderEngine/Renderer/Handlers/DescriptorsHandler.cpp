#include "DescriptorsHandler.hpp"

#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	DescriptorsHandler::DescriptorsHandler() :
		m_shaderProgram(nullptr),
		m_descriptorSet(nullptr),
		m_descriptors(new std::vector<IDescriptor *>()),
		m_changed(false)
	{
	}

	DescriptorsHandler::~DescriptorsHandler()
	{
		delete m_descriptorSet;
		delete m_descriptors;
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, IDescriptor *descriptor)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		int location = m_shaderProgram->GetDescriptorLocation(descriptorName);

		if (location == -1)
		{
			return;
		}

		if (m_descriptors->at(location) != descriptor)
		{
			m_descriptors->at(location) = descriptor;
			m_changed = true;
		}
	}

	void DescriptorsHandler::Push(const std::string &descriptorName, UniformHandler *uniformHandler)
	{
		if (m_shaderProgram == nullptr)
		{
			return;
		}

		uniformHandler->Update(m_shaderProgram->GetUniformBlock(descriptorName));
		Push(descriptorName, uniformHandler->GetUniformBuffer());
	}

	bool DescriptorsHandler::Update(const Pipeline &pipeline)
	{
		if (m_shaderProgram != pipeline.GetShaderProgram())
		{
			m_descriptors->clear();
			delete m_descriptorSet;

			m_shaderProgram = pipeline.GetShaderProgram();
			m_descriptors->resize(pipeline.GetShaderProgram()->GetDescriptors()->size());
			m_descriptorSet = new DescriptorSet(pipeline);
			m_changed = false;
			return false;
		}

		if (m_changed)
		{
			m_descriptorSet->Update(*m_descriptors);
			m_changed = false;
		}

		return true;
	}
}
