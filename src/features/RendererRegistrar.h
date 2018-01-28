#pragma once

#include "Renderer.h"
#include <list>
#include <memory>
#include <map>

namespace Features
{
    using namespace Interfaces;

    typedef std::shared_ptr<IRenderer> IRendererPtr;
    typedef std::list<IRendererPtr> RenderersList;
    typedef std::map<SceneType, RenderersList> SceneToRenderersMap;

    typedef RenderersList::iterator RendererStateIter;
    typedef SceneToRenderersMap::iterator SceneToRenderersMapIter;

    // returns global instance of renderer states
	SceneToRenderersMap& GetRenderers();
}
