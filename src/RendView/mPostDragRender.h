#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QObject>

namespace MPostRend
{

	class RENDVIEW_EXPORT mPostDragRender :public QObject
	{
		Q_OBJECT

	public:
		mPostDragRender();

		~mPostDragRender();

	};
}

