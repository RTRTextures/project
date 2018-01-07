#include <RendererRegistrar.h>
#include <mutex>

#include "Test/TestRenderer.h"

namespace Features
{
    using namespace Interfaces;
    static std::once_flag s_flag;

    SceneToRenderersMap& GetRenderers()
    {
        // helper macros for constructing renderer list
#define INIT_RENDERER_MAP(renderer_map)   std::call_once(s_flag, [] (SceneToRenderersMap& map) { SceneType currentSceneType;
#define BEGIN_SCENE_TYPE(scene)           currentSceneType = (scene);
#define END_SCENE_TYPE()
#define REGISTER_RENDERER(renderer)       map[currentSceneType].push_back(IRendererPtr(new (renderer)));
#define END_RENDERER_MAP(renderer_map)    }, (renderer_map));

        static SceneToRenderersMap renderers;
        INIT_RENDERER_MAP(renderers)

            BEGIN_SCENE_TYPE(SCENE_TYPE_TEST0)
                REGISTER_RENDERER(TestRenderer)
                REGISTER_RENDERER(TestRenderer1)
                // other renderer?
            END_SCENE_TYPE()

            BEGIN_SCENE_TYPE(SCENE_TYPE_TEST1)
                REGISTER_RENDERER(TestRenderer2)
                // other renderer?
            END_SCENE_TYPE()

            // register new renderer classes here
        END_RENDERER_MAP(renderers)
        return renderers;
    }
}