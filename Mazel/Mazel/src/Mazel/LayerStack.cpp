#include "mzpch.h"
#include "LayerStack.h"

namespace Mazel
{
	LayerStack::LayerStack()
	{}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	/// <summary>
	/// Pops the layer out of the layer stack. This layer won't be deleted and caller is responsible to manually delete this layer.
	/// </summary>
	/// <param name="layer">ptr to Layer</param>
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			MZ_ASSERT(m_LayerInsertIndex >= 0, "Index ('m_LayerInsertIndex') in LayerStack is out of range.")
		}
	}

	/// <summary>
	/// Pops the overlay out of the layer stack. This overlay layer won't be deleted and caller is responsible to manually delete this overlay layer.
	/// </summary>
	/// <param name="overlay">ptr to overlay layer</param>
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}