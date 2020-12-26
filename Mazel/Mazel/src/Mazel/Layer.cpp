#include "mzpch.h"
#include "Layer.h"

namespace Mazel
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{}

	Layer::~Layer() {}
}