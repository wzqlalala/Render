#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

#include <QVector3D>
#include <QVector>
#include <QList>
#include <QHash>
#include <QSet>
#include <set>

#include "mBasicEnum.h"
#include "mBasicStructor.h"

using namespace std;
namespace MDataPost
{
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshNodeData1;
	class mPostMeshData1;
	class mPostMeshLineData1;
	class mPostMeshFaceData1;
	class mPostOneFrameRendData;
	class RENDDATA_EXPORT mOneFrameData1
	{

	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mOneFrameData1();

		/*
		 *Ĭ����������
		 */
		~mOneFrameData1();

		/*
		 * ģ�Ͳ���
		 */

		 //��ȡ���еĽڵ�ID����ȥ���ص�����
		set<int> getAllNodeIDs();
		//��ȡһ�����������нڵ�ID(��ȥ���ص�����)
		set<int> getAllNodeIDsInPart(QString partName);

		//��ȡ���е�����ID����ȥ���ص�����
		set<int> getAllMeshIDs();
		//��ȡһ�����������е�����ID(��ȥ���ص�����)
		set<int> getAllMeshIDsInPart(QString partName);
		//��ȡ���е���ά����ID����ȥ���ص�����
		QVector<mPostMeshData1*> getAll3DMeshIDs();
		//��ȡ���еĶ�ά����ID����ȥ���ص�����
		QVector<mPostMeshData1*> getAll2DMeshIDs();

		//��ȡ���еĵ�Ԫ��ID����ȥ���ص�����
		set<int> getAllMeshFaceIDs();
		//��ȡһ�����������еĵ�Ԫ��ID(��ȥ���ص�����)
		set<int> getAllMeshFaceIDsInPart(QString partName);
		//��ȡ���еĵ�Ԫ��ָ�루��ȥ���ص�����
		QVector<mPostMeshFaceData1*> getAllMeshFaceDatas();

		//�ж�ID��Ӧ�Ľڵ��Ԫ�Ƿ����
		bool isIdExistInModel(int id, const QString& type);

		//ͨ��ID(����)��ȡ��Ӧ������
		mPostMeshPartData1 *getMeshPartDataByPartName(QString partName);
		mPostMeshLineData1* getMeshLineDataByID(int ID);
		mPostMeshFaceData1* getMeshFaceDataByID(int ID);
		mPostMeshData1* getMeshDataByID(int ID);
		mPostMeshNodeData1* getNodeDataByID(int ID);

		//��ȡ��ǰ֡�����ĵ�����
		QHashIterator<QString, mPostMeshPartData1*> getMeshPartIterator();

		//��ȡ��ǰ֡��������
		const QHash<QString, mPostMeshPartData1*> getMeshParts();

		//�������
		void appendMeshPartData(QString partName, mPostMeshPartData1 *data);
		void appendMeshFaceData(int ID, mPostMeshFaceData1 *data);
		void appendMeshLineData(int ID, mPostMeshLineData1 *data);
		void appendMeshData(int ID, mPostMeshData1 *data);
		void appendNodeData(int ID, mPostMeshNodeData1 *data);



		//��ȡģ�͵����뾶������
		void getModelSize(MBasicFunction::ModelSize & modelSize);

		//�ж��Ƿ����ģ��
		bool isExistModel();

		//��ȡģ�͵�ģ�͵ĵ�Ԫ���³ߴ�
		float getModeMeshSize();

		/*
		* �ж��Ƿ��иò���ͨ������
		*/
		bool isExistMeshPart(QString partName);

		/*
		 *��������
		 */

		//��ȡ���еĲ�������(ȥ�����صĲ���)
		std::set<QString> getAllPartNames();

		//��ȡ���еĲ�������(ȥ�����صĲ���)
		QStringList getAllPartNameList();

		//���ļ�������ݣ����Զ�������Ⱦ
		void importMeshPartData(QString partName, mPostMeshPartData1 *data);

		//�滻����
		void replaceMeshPartData(QString partName, mPostMeshPartData1* data);

		//�������񲿼�����ɫ
		void setMeshPartColor(QString partName, QVector3D color);

		//�������񲿼�������
		void setMeshPartVisual(QString partName, bool isShow);

		//��ȡ���񲿼�������
		bool getMeshPartVisual(QString partName);

		//����ȫ�����񲿼�������
		void setAllMeshPartVisual(bool isShow);

		//����ȫ�����񲿼�����ɫ
		void setAllMeshPartColor(QVector3D color);

		//��ȡ���񲿼�����ɫ
		QVector3D getMeshPartColor(QString partName);

		/*
		* ��ȡ��ǰ֡�ڵ�����
		*/
		int getOneFrameNodeAmount();

		/*
		 * ��ӵ�ǰ֡��ģ��
		 */
		void appendPartData(QString partName, mPostMeshPartData1* onePartData);

		/*
		 * ��Ԫ����
		 */

		 //��������
		void createMesh(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType elementType, 
			QVector<int> index, mPostMeshPartData1 *partData, QHash<QVector<int>, mPostMeshFaceData1*> &_meshFace);

		//��������
		void createMesh(int ID, MBasicFunction::MeshType meshType, MBasicFunction::ElementType elementType,
			QVector<int> index, mPostMeshPartData1 *partData, QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>> &_meshFace);

		//��������
		void maskMesh(set<int> meshIDs);

		//��ʾ����
		void showMesh(set<int> meshIDs);

		//������������
		void reverseMaskMesh();

		//��ʾ��������
		void showAllMesh();

		//���ݵ�Ԫ���ͻ�ȡ����
		int getNumByElementType(MBasicFunction::ElementType elementType);

		//��ȡ���������
		QVector3D getMeshCenter(mPostMeshData1 *meshData, const QHash<int, QVector3D> &dis, QVector3D deformationScale);

		//��ȡ����Ľڵ㣨�������߽׽ڵ㣩
		QVector<QVector3D> getMeshVertexs(mPostMeshData1 *meshData, const QHash<int, QVector3D> &dis, QVector3D deformationScale);

		//ɾ������
		//void deleteMesh(set<int> meshIDs);

		/*
		 * ��Ԫ�����
		 */

		 //�����ж��Ƿ��Ǳ��棬����ԭʼ�ļ���˵
		bool MeshFaceIsSurfaceIDPointer(mPostMeshFaceData1* mMeshFaceData1);

		//�ж������Ԫ���ǲ��Ǳ��棬����ԭʼ�ļ���˵,���ҷ��������Ԫ�������ĵ�Ԫ��ָ��,�߽��߿��ܴ��������������У���������������������������У���Ҳ������һ����Ԫ��ָ��
		bool MeshFaceIsSurface(mPostMeshFaceData1* mMeshFaceData1, mPostMeshData1 **meshData, int isInTwoMesh, mPostMeshData1 **meshData1);

		//�ж������Ԫ���ǲ��Ǳ��棬����һ�������˵
		bool MeshFaceIsSurface(mPostMeshFaceData1 *mMeshFaceData1);

		//�ж������Ԫ���ǲ��Ǳ��棬����һ�������˵,���������Ԫ�����������ĵ�ԪID
		int MeshFaceIsSurface1(mPostMeshFaceData1 *mMeshFaceData1);

		 //������Ԫ��
		void createMeshFace(QVector<int>  set, QVector<int> index, mPostMeshData1* meshData, int order,
			QString partName, mPostMeshPartData1* partData, QHash<QVector<int>, mPostMeshFaceData1*> &_meshFace);

		//������Ԫ��
		void createMeshFace(QVector<int> set,  mPostMeshData1* meshData, int order,
			QString partName, QVector<QVector<QPair<QPair<int, int>, mPostMeshFaceData1*>>> &_meshFace);

		//�Ƴ������Ǳ���ĵ�Ԫ��
		void eraseNotMeshFace();

		//�жϵ�Ԫ���Ƿ���Ⱦ
		void judgeMeshFaceIsShow();

		//�����Ƿ���ӵ�����,���ݵ�Ԫ���жϵĵ�Ԫ
		bool getMeshFaceShow(mPostMeshFaceData1 *mMeshFaceData1,mPostMeshData1* meshData);

		//������洢��������
		void setMeshFaceShow();

		//�жϵ�Ԫ���Ƿ���Ⱦ
		void judgeMeshFaceIsShow(set<int> meshIDs);

		//��ȡ��Ԫ��Ľڵ㣨�������߽׽ڵ㣩
		QVector<QVector3D> getMeshFaceVertexs(mPostMeshFaceData1 *meshData, const QHash<int, QVector3D> &dis, QVector3D deformationScale);

		/*
		 * ��Ԫ�߲���
		 */

		 //��������ı߽���
		void caculateMeshLine();

		 //�жϱ߽������ڵ�Ԫ�ĵڼ�����
		int judgeMeshLineOrder(mPostMeshData1 *meshData, std::set<int> index);

		/*
		 * ģ�Ͳ���
		 */

		/*
		 * ��ȡ��ǰ֡������ģ��
		 */
		QHash<QString, mPostMeshPartData1*> getAllPartData();

		/*
		* ��ȡ��ǰ֡���������
		*/
		QStringList getOneFramePartNames();

		/*
		 * ͨ�����ƻ�ȡ��ǰ֡��һ��ģ��
		 */
		mPostMeshPartData1 *getOnePartDataByName(QString partName);

		/*
		 * ɾ����һ֡����������
		 */
		void deleteOneFrameData();

		/*
		* ��ȡ��ǰ֡��Ԫ����
		*/
		int getOneFrameElementAmount();

		/*
		* ����Ƶ������
		*/
		void setOneFrameModalValue(double freq, double eig);

		/*
		* ��ȡ�Ƿ���ģ̬����
		*/
		bool isOneFrameModalValue();

		/*
		* ��ȡģ̬Ƶ��ֵ
		*/
		double getOneFrameModalFrequency();

		/*
		* ��ȡģ̬����ֵ
		*/
		double getOneFrameModalEigen();

		/*
		 * ��ȡ��ǰ֡�Ľڵ������
		 */
		QHashIterator<int, mPostMeshNodeData1*> getMeshNodeIterator();

		///*
		//* �洢����ǽ����ֹ����
		//*/
		//void setRigidWallEndPos(int ID, QVector3D pos);

		///*
		//* ��ȡ����ǽ����ֹ����
		//*/
		//QVector3D getRigidWallEndPos(QString name);
		
	public:
		//ģ�Ͳ���ö��
		MBasicFunction::ModelOperateEnum _meshOperateEnum;

		//��Ⱦ��Ҫʵʱ�ж�
		//����ģ�Ͳ����Ĳ�������Ⱦ����Ϊ��
		std::set<QString> _oneMeshOperatePartName;

		//�Ƿ����ȫ��ģ�Ͳ�������Ⱦ����Ϊfalse
		bool _allMeshOperatePart;

		void calculatePointCell();

		void resizeNodeSize(int size);
		void resizeCopyCellSize(int size);
		void calculateCellData();
	protected:

		//���㼸�����ĺͰ뾶
		void caculateMeshModelRadiusAndCenter();

		//��������ĳߴ�
		void caculateMeshSize();

		//���ɱ߽���
		void generateMeshLine(mPostMeshFaceData1* surfaceID1, mPostMeshFaceData1* surfaceID2, mPostMeshData1 *meshData, QString partName, mPostMeshPartData1* partData);

	private:

		//�Ƿ���Ƶ������
		bool _hasModalValue;

		//Ƶ������
		double _modalValue[2];

	protected:

		QVector<mPostMeshNodeData1*> _nodeData2;
		QHash<int, mPostMeshNodeData1*> _nodeData1;

		QVector<mPostMeshData1*> _meshData2;
		QHash<int, mPostMeshData1*> _meshData1;


		QHash<int, mPostMeshLineData1*> _meshLineData1;

		std::vector<mPostMeshFaceData1*> _meshFaceData1;

		QHash<QString, mPostMeshPartData1*> _partData1;
		QHash<MBasicFunction::ElementType, int> _eleNum;

		//ģ�ͳߴ�
		MBasicFunction::ModelSize _modelSize;

		//�Ƿ��ģ�ͽ���ɾ���������
		bool _isAddOrDeletePart_Mesh;

		//ģ�͵ĵ�Ԫ���³ߴ�
		float _meshSize;
		
	};
}