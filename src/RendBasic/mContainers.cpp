#include "mContainers.h"
namespace MBasicFunction
{
	 //�洢�����Ϣ������
	 QStringList _outputMessages;

	 //�洢Python�����е�����
	 QStringList _pythonCommands;

	 //ȫ��Part ID
	 int _globalPartId = 0;

	 //ȫ��Solid ID
	 int _globalSolidId = 0;

	 //ȫ��Face ID
	 int _globalFaceId = 100000000;

	 //ȫ��Line��Edge�� ID
	 int _globalLineId = 100000000;

	 //ȫ��Point��Vertex�� ID
	 int _globalPointId = 0;

	 //ȫ��Node ID
	 int _globalNodeId = 0;

	 //ȫ��Mesh ID
	 int _globalMeshId = 0;

	 //ȫ��MeshLine ID
	 int _globalMeshLineId = 100000000;

	 //ȫ��MeshFace ID
	 int _globalMeshFaceId = 100000000;


	 //��ʼ�����е�ȫ��ID
	 void initialGlobalID()
	 {
		 _globalPartId = 0;
		 _globalSolidId = 0;
		 _globalFaceId = 100000000;
		 _globalLineId = 100000000;
		 _globalPointId = 0;
		 _globalNodeId = 0;
		 _globalMeshId = 0;
		 _globalMeshLineId = 100000000;
		 _globalMeshFaceId = 100000000;
	 }
}