#include <RendererRegistrar.h>
#include <mutex>

#include "TestRenderer.h"

namespace Features
{
    using namespace Interfaces;
    static std::once_flag s_flag;

    std::list<RendererState>& GetRenderers()
    {
        // helper macros for constructing renderer list
#define INIT_RENDERER_MAP(renderer_list)   std::call_once(s_flag, [] (std::list<RendererState>& list) {
#define REGISTER_RENDERER(renderer)        list.push_back(IRendererPtr(new (renderer)));
#define END_RENDERER_MAP(renderer_list)    }, (renderer_list));

        static std::list<RendererState> renderers;
        INIT_RENDERER_MAP(renderers)
            REGISTER_RENDERER(TestRenderer)
            REGISTER_RENDERER(TestRenderer2)
            // register new renderer classes here
        END_RENDERER_MAP(renderers)
        return renderers;
    }
}