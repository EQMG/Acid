#pragma once

#include <Post/PostFilter.hpp>

using namespace acid;

namespace test
{
class FilterBlit :
	public PostFilter
{
public:
	explicit FilterBlit(const Pipeline::Stage &pipelineStage);

	void Record(const CommandBuffer &commandBuffer) override;

private:
	bool m_lastFilter;
};
}
