#pragma once
#include "Drawable.h"
#include "NodeVisitor.h"
#include "StateAttribute.h"

#include <QTime>

namespace mxr
{
	extern RENDBASE_EXPORT QTime *time;
	class RENDBASE_EXPORT Viewer
	{
	public:
		Viewer();
		void setSceneData(asset_ref<Node> _node) { _sceneData = _node; }
		void compile();
		void noClearRun();
		void run();
		void deleteAllData();
	protected:
		asset_ref<Node> _sceneData;
		asset_ref<NodeVisitor> _visitor;
		asset_ref<Clear> _clear;
	};

}