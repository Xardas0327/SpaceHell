#pragma once

#include <map>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>
#include <set>

#include "RendererMode.h"
#include "RenderData.h"
#include "IRenderer.h"

namespace Learning2DEngine
{
    namespace Render
    {
        class RendererComponentHandler final
        {
        private:
            std::map<std::string, IRenderer*> renderers;
            //The string is the render id.
            //The int is the layer.
            std::map<std::string, std::map<RendererMode, std::map<int, std::vector<RenderData*>>>> renderData;
            //Help for find the data faster.
            std::map<RenderData*, std::tuple<std::string, RendererMode, int>> renderDataMapping;
            std::mutex rendererMutex;
            std::mutex dataMutex;
			//The first set is the active renderers.
            //The second set is the active layers.
			std::map <RendererMode, std::tuple<std::set<IRenderer*>, std::set<int>>> activeRenderersAndLayers;

            void AddData(const std::string& id, RendererMode rendererMode, RenderData* data, int layer);
            void ChangeLayer(RenderData* data, int newLayer);
            void RemoveData(RenderData* data);
        public:
            RendererComponentHandler();
			~RendererComponentHandler() = default;

            bool IsRendererExist(const std::string& id, bool isThreadSafe);
            void AddRenderer(const std::string& id, IRenderer* renderer, bool isThreadSafe);
            void RemoveRenderer(const std::string& id, bool isThreadSafe);

            void AddData(const std::string& id, RendererMode rendererMode, RenderData* data, int layer, bool isThreadSafe);
            void ChangeLayer(RenderData* data, int newLayer, bool isThreadSafe);
            void RemoveData(RenderData* data, bool isThreadSafe);

            void SetDataToRenderers();
            void Run(RendererMode rendererMode);
            void Clear();
        };
    }
}