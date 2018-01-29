#include <RendererRegistrar.h>
#include <mutex>

#include "Test/TestRenderer.h"
#include"SolarSystem\SolarRender.h"

namespace Features
{
    using namespace Interfaces;
    static std::once_flag s_flag;

    SceneToRenderersMap& GetRenderers()
    {
<<<<<<< HEAD
<<<<<<< HEAD
		// helper macros for constructing renderer list
		#define INIT_RENDERER_MAP(renderer_map)   std::call_once(s_flag, [] (SceneToRenderersMap& map) { SceneType currentSceneType;
		#define BEGIN_SCENE_TYPE(scene)           currentSceneType = (scene);
		#define END_SCENE_TYPE()
		#define REGISTER_RENDERER(renderer)       map[currentSceneType].push_back(IRendererPtr(new (renderer)));
		#define END_RENDERER_MAP(renderer_map)    }, (renderer_map));

		static SceneToRenderersMap renderers;
		INIT_RENDERER_MAP(renderers)
		#ifdef _TEST
			BEGIN_SCENE_TYPE(SCENE_TYPE_TEST0)
				REGISTER_RENDERER(TestRenderer)
				REGISTER_RENDERER(TestRenderer1)
					// other renderer?
			END_SCENE_TYPE()
		#endif
			BEGIN_SCENE_TYPE(SCENE_SOLAR_SYSTEM)
					REGISTER_RENDERER(SolarSystemRenderer)
			END_SCENE_TYPE()

			// register new renderer classes here, like Solar system added above.
		END_RENDERER_MAP(renderers)
	
		return renderers;
=======
        // helper macros for constructing renderer list
        #define INIT_RENDERER_MAP(renderer_map)   std::call_once(s_flag, [] (SceneToRenderersMap& map) { SceneType currentSceneType;
        #define BEGIN_SCENE_TYPE(scene)           currentSceneType = (scene);
        #define END_SCENE_TYPE()
        #define REGISTER_RENDERER(renderer)       map[currentSceneType].push_back(IRendererPtr(new (renderer)));
        #define END_RENDERER_MAP(renderer_map)    }, (renderer_map));

=======
        // helper macros for constructing renderer list
        #define INIT_RENDERER_MAP(renderer_map)   std::call_once(s_flag, [] (SceneToRenderersMap& map) { SceneType currentSceneType;
        #define BEGIN_SCENE_TYPE(scene)           currentSceneType = (scene);
        #define END_SCENE_TYPE()
        #define REGISTER_RENDERER(renderer)       map[currentSceneType].push_back(IRendererPtr(new (renderer)));
        #define END_RENDERER_MAP(renderer_map)    }, (renderer_map));

>>>>>>> bfa7d69bafc6d0e030340840a21794ac8b0a4fa9
        static SceneToRenderersMap renderers;
        INIT_RENDERER_MAP(renderers)
#ifdef _TEST
            BEGIN_SCENE_TYPE(SCENE_TYPE_TEST0)
                REGISTER_RENDERER(TestRenderer)
                REGISTER_RENDERER(TestRenderer1)
                    // other renderer?
            END_SCENE_TYPE()
#endif
            BEGIN_SCENE_TYPE(SCENE_SOLAR_SYSTEM)
                    REGISTER_RENDERER(SolarSystemRenderer)
            END_SCENE_TYPE()
            // register new renderer classes here, like Solar system added above.
        END_RENDERER_MAP(renderers)
        return renderers;
<<<<<<< HEAD
>>>>>>> 262f5631734c516bd6e17471498adc4118646d03
=======
>>>>>>> bfa7d69bafc6d0e030340840a21794ac8b0a4fa9
    }

}