#include "RendererComponentHandler.h"

namespace Learning2DEngine
{
    namespace Render
    {
		RendererComponentHandler::RendererComponentHandler()
			: renderers(), renderData(), renderDataMapping(), rendererMutex(), dataMutex(), activeRenderersAndLayers()
		{
		}

		bool RendererComponentHandler::IsRendererExist(const std::string& id, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				return renderers.find(id) != renderers.end();
			}
			else
			{
				return renderers.find(id) != renderers.end();
			}
		}

		void RendererComponentHandler::AddRenderer(const std::string& id, IRenderer* renderer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				renderers[id] = renderer;
			}
			else
			{
				renderers[id] = renderer;
			}
		}

		void RendererComponentHandler::RemoveRenderer(const std::string& id, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(rendererMutex);
				renderers.erase(id);
			}
			else
			{
				renderers.erase(id);
			}
		}

		void RendererComponentHandler::AddData(const std::string& id, RendererMode rendererMode, RenderData* data, int layer)
		{
			renderData[id][rendererMode][layer].push_back(data);
			renderDataMapping[data] = std::make_tuple(id, rendererMode, layer);
		}

		void RendererComponentHandler::AddData(const std::string& id, RendererMode rendererMode, RenderData* data, int layer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(dataMutex);
				AddData(id, rendererMode, data, layer);
			}
			else
			{
				AddData(id, rendererMode, data, layer);
			}
		}

		void RendererComponentHandler::ChangeLayer(RenderData* data, int newLayer)
		{
			bool isFound = renderDataMapping.find(data) != renderDataMapping.end();
			if (!isFound)
				return;

			const std::string& id = std::get<0>(renderDataMapping[data]);
			const RendererMode rendererMode = std::get<1>(renderDataMapping[data]);
			const int oldLayer = std::get<2>(renderDataMapping[data]);

			renderData[id][rendererMode][oldLayer].erase(
				std::remove(renderData[id][rendererMode][oldLayer].begin(), renderData[id][rendererMode][oldLayer].end(), data), 
				renderData[id][rendererMode][oldLayer].end()
			);
			renderData[id][rendererMode][oldLayer].push_back(data);
			renderDataMapping[data] = std::make_tuple(id, rendererMode, newLayer);
		}

		void RendererComponentHandler::ChangeLayer(RenderData* data, int newLayer, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(dataMutex);
				ChangeLayer(data, newLayer);
			}
			else
			{
				ChangeLayer(data, newLayer);
			}
		}

		void RendererComponentHandler::RemoveData(RenderData* data)
		{
			bool isFound = renderDataMapping.find(data) != renderDataMapping.end();
			if (!isFound)
				return;

			const std::string& id = std::get<0>(renderDataMapping[data]);
			const RendererMode rendererMode = std::get<1>(renderDataMapping[data]);
			const int layer = std::get<2>(renderDataMapping[data]);

			renderData[id][rendererMode][layer].erase(
				std::remove(renderData[id][rendererMode][layer].begin(), renderData[id][rendererMode][layer].end(), data),
				renderData[id][rendererMode][layer].end()
			);
			if (renderData[id][rendererMode][layer].size() == 0)
			{
				renderData[id][rendererMode].erase(layer);
				if (renderData[id][rendererMode].size() == 0)
				{
					renderData[id].erase(rendererMode);
					if (renderData[id].size() == 0)
					{
						renderData.erase(id);
					}
				}

			}

			renderDataMapping.erase(data);
		}

		void RendererComponentHandler::RemoveData(RenderData* data, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(dataMutex);
				RemoveData(data);
			}
			else
			{
				RemoveData(data);
			}
		}

		void RendererComponentHandler::SetDataToRenderers()
		{
			activeRenderersAndLayers.clear();
			std::map<RendererMode, std::map<int, std::vector<RenderData*>>> activeData;
			for (auto& data : renderData)
			{
				for (auto& modeData : data.second)
				{
					for (auto& layerData : modeData.second)
					{
						size_t activeDataCount = 0;
						for (const RenderData* renderData : layerData.second)
						{
							if (renderData->component->isActive && renderData->component->gameObject->isActive)
								activeDataCount++;
						}
						if (activeDataCount > 0)
						{
							std::get<0>(activeRenderersAndLayers[modeData.first]).insert(renderers[data.first]);
							std::get<1>(activeRenderersAndLayers[modeData.first]).insert(layerData.first);

							if (activeDataCount == layerData.second.size())
								activeData[modeData.first][layerData.first] = layerData.second;
							else
							{
								activeData[modeData.first][layerData.first].reserve(activeDataCount);
								for (RenderData* renderData : layerData.second)
								{
									if (renderData->component->isActive && renderData->component->gameObject->isActive)
										activeData[modeData.first][layerData.first].push_back(renderData);
								}
							}
						}
					}
				}

				if (activeData.size() > 0)
				{
					renderers[data.first]->SetData(activeData);
					activeData.clear();
				}
			}
		}

		void RendererComponentHandler::Run(RendererMode rendererMode)
		{
			//Render
			for (int layer : std::get<1>(activeRenderersAndLayers[rendererMode]))
			{
				for (auto renderer : std::get<0>(activeRenderersAndLayers[rendererMode]))
				{
					renderer->Draw(rendererMode, layer);
				}
			}

		}

		void RendererComponentHandler::Clear()
		{
			renderers.clear();
			renderData.clear();
			renderDataMapping.clear();
			activeRenderersAndLayers.clear();
		}
    }
}