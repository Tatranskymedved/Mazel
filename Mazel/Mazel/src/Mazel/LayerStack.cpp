#include "mzpch.h"
#include "LayerStack.h"

namespace Mazel
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
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
			m_Layers.erase(it);
			m_LayerInsert--;
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
			m_Layers.erase(it);
	}
}