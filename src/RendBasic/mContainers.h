#pragma once
#include "rendbasic_global.h"

//QT
#include <QStringList>

namespace MBasicFunction
{
	//�洢�����Ϣ������
	extern RENDBASIC_EXPORT QStringList _outputMessages;

	//�洢Python�����е�����
	extern RENDBASIC_EXPORT QStringList _pythonCommands;

	//ȫ��Part ID
	extern RENDBASIC_EXPORT int _globalPartId;

	//ȫ��Solid ID
	extern RENDBASIC_EXPORT int _globalSolidId;

	//ȫ��Face ID
	extern RENDBASIC_EXPORT int _globalFaceId;

	//ȫ��Line ID
	extern RENDBASIC_EXPORT int _globalLineId;

	//ȫ��Point ID
	extern RENDBASIC_EXPORT int _globalPointId;

	//ȫ��Node ID
	extern RENDBASIC_EXPORT int _globalNodeId;

	//ȫ��Mesh ID
	extern RENDBASIC_EXPORT int _globalMeshId;

	//ȫ��MeshLine ID
	extern RENDBASIC_EXPORT int _globalMeshLineId;

	//ȫ��MeshFace ID
	extern RENDBASIC_EXPORT int _globalMeshFaceId;

	//��ʼ�����е�ȫ��ID
	extern RENDBASIC_EXPORT void initialGlobalID();
}