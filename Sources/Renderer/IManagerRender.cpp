#include "IManagerRender.hpp"

namespace acid
{
	IManagerRender::IManagerRender(const std::vector<RenderpassCreate> &renderpassCreates) :
		m_renderpassCreates(renderpassCreates),
		m_started(false)
	{
	}
}
