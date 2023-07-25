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

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

using namespace std;
namespace MDataMesh
{
	class mMeshPartData;
	class mMeshData;
	class mMeshNodeData;
	class mMeshLineData;
	class mMeshFaceData;
	class RENDDATA_EXPORT mMeshModelData
	{

	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mMeshModelData();

		/*
		 *Ĭ����������
		 */
		~mMeshModelData();

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
		QVector<std::shared_ptr<mMeshData> > getAll3DMeshIDs();
		//��ȡ���еĶ�ά����ID����ȥ���ص�����
		QVector<std::shared_ptr<mMeshData> > getAll2DMeshIDs();

		//��ȡ���еĵ�Ԫ��ID����ȥ���ص�����
		set<int> getAllMeshFaceIDs();
		//��ȡһ�����������еĵ�Ԫ��ID(��ȥ���ص�����)
		set<int> getAllMeshFaceIDsInPart(QString partName);
		//��ȡ���еĵ�Ԫ��ָ�루��ȥ���ص�����
		QVector<std::shared_ptr<mMeshFaceData>> getAllMeshFaceDatas();

		//�ж�ID��Ӧ�Ľڵ��Ԫ�Ƿ����
		bool isIdExistInModel(int id, const QString& type);

		//ͨ��ID(����)��ȡ��Ӧ������
		std::shared_ptr<mMeshPartData> getMeshPartDataByPartName(QString partName);
		//mPostMeshLineData1* getMeshLineDataByID(int ID);
		//std::shared_ptr<mMeshFaceData> getMeshFaceDataByID(int ID);
		std::shared_ptr<mMeshData>  getMeshDataByID(int ID);
		std::shared_ptr<mMeshNodeData>  getNodeDataByID(int ID);

		//��ȡ��ǰ֡�����ĵ�����
		QHashIterator<QString, std::shared_ptr<mMeshPartData>> getMeshPartIterator();

		//��ȡ��ǰ֡��������
		const QHash<QString, std::shared_ptr<mMeshPartData>> getMeshParts();

		//�������
		void appendMeshPartData(QString partName, std::shared_ptr<mMeshPartData>data);
		//void appendMeshFaceData(int ID, std::shared_ptr<mMeshFaceData> *data);
		//void appendMeshLineData(int ID, mPostMeshLineData1 *data);
		void appendMeshData(int ID, std::shared_ptr<mMeshData>data);
		void appendNodeData(int ID, std::shared_ptr<mMeshNodeData> data);



		//��ȡģ�͵����뾶������
		void getModelSize(MViewBasic::ModelSize & modelSize);

		//�ж��Ƿ����ģ��
		bool isExistModel();

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
		void importMeshPartData(QString partName, std::shared_ptr<mMeshPartData>data);

		//�滻����
		void replaceMeshPartData(QString partName, std::shared_ptr<mMeshPartData> data);

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
		void appendPartData(QString partName, std::shared_ptr<mMeshPartData> onePartData);

		/*
		 * ��Ԫ����
		 */

		 //��������
		void createMesh(int ID, MViewBasic::MeshType meshType,
			QVector<int> index, std::shared_ptr<mMeshPartData>partData, QHash<QVector<int>, std::shared_ptr<mMeshFaceData>> &_meshFace);

		//��������
		void createMesh(int ID, MViewBasic::MeshType meshType,
			QVector<int> index, std::shared_ptr<mMeshPartData>partData, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>> &_meshFace);

		//��������
		void maskMesh(set<int> meshIDs);

		//��ʾ����
		void showMesh(set<int> meshIDs);

		//������������
		void reverseMaskMesh();

		//��ʾ��������
		void showAllMesh();

		//���ݵ�Ԫ���ͻ�ȡ����
		int getNumByElementType(MViewBasic::ElementType elementType);

		//��ȡ���������
		QVector3D getMeshCenter(std::shared_ptr<mMeshData>meshData, const QHash<int, QVector3D> &dis, QVector3D deformationScale, QVector3D explodeDis);

		//��ȡ����Ľڵ㣨�������߽׽ڵ㣩
		QVector<QVector3D> getMeshVertexs(std::shared_ptr<mMeshData>meshData, const QHash<int, QVector3D> &dis, QVector3D deformationScale, QVector3D explodeDis);

		//ɾ������
		//void deleteMesh(set<int> meshIDs);

		/*
		 * ��Ԫ�����
		 */

		 //�����ж��Ƿ��Ǳ��棬����ԭʼ�ļ���˵
		bool MeshFaceIsSurfaceIDPointer(std::shared_ptr<mMeshFaceData> mMeshFaceData1);

		//�ж������Ԫ���ǲ��Ǳ��棬����ԭʼ�ļ���˵,���ҷ��������Ԫ�������ĵ�Ԫ��ָ��,�߽��߿��ܴ��������������У���������������������������У���Ҳ������һ����Ԫ��ָ��
		bool MeshFaceIsSurface(std::shared_ptr<mMeshFaceData> mMeshFaceData1, std::shared_ptr<mMeshData> meshData, int isInTwoMesh, std::shared_ptr<mMeshData> meshData1);

		//�ж������Ԫ���ǲ��Ǳ��棬����һ�������˵
		bool MeshFaceIsSurface(std::shared_ptr<mMeshFaceData>mMeshFaceData1);

		//�ж������Ԫ���ǲ��Ǳ��棬����һ�������˵,���������Ԫ�����������ĵ�ԪID
		int MeshFaceIsSurface1(std::shared_ptr<mMeshFaceData>mMeshFaceData1);

		 //������Ԫ��
		void createMeshFace(QVector<int>  set, QVector<int> index, std::shared_ptr<mMeshData>  meshData, int order,
			QString partName, std::shared_ptr<mMeshPartData> partData, QHash<QVector<int>, std::shared_ptr<mMeshFaceData>> &_meshFace);

		//������Ԫ��
		void createMeshFace(QVector<int> set,  std::shared_ptr<mMeshData>  meshData, int order,
			QString partName, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>> &_meshFace);

		//�Ƴ������Ǳ���ĵ�Ԫ��
		void eraseNotMeshFace();

		//�жϵ�Ԫ���Ƿ���Ⱦ
		void judgeMeshFaceIsShow();

		//�����Ƿ���ӵ�����,���ݵ�Ԫ���жϵĵ�Ԫ
		bool getMeshFaceShow(std::shared_ptr<mMeshFaceData>mMeshFaceData1,std::shared_ptr<mMeshData>  meshData);

		//������洢��������
		void setMeshFaceShow();

		//�жϵ�Ԫ���Ƿ���Ⱦ
		void judgeMeshFaceIsShow(set<int> meshIDs);

		//��ȡ��Ԫ��Ľڵ㣨�������߽׽ڵ㣩
		QVector<QVector3D> getMeshFaceVertexs(std::shared_ptr<mMeshFaceData>meshData, const QHash<int, QVector3D> &dis, QVector3D deformationScale, QVector3D explodeDis);

		/*
		 * ��Ԫ�߲���
		 */

		 //��������ı߽���
		void caculateMeshLine();

		 //�жϱ߽������ڵ�Ԫ�ĵڼ�����
		int judgeMeshLineOrder(std::shared_ptr<mMeshData> meshData, std::set<int> index);

		/*
		 * ģ�Ͳ���
		 */

		/*
		 * ��ȡ��ǰ֡������ģ��
		 */
		QHash<QString, std::shared_ptr<mMeshPartData>> getAllPartData();

		/*
		* ��ȡ��ǰ֡���������
		*/
		QStringList getOneFramePartNames();

		/*
		 * ͨ�����ƻ�ȡ��ǰ֡��һ��ģ��
		 */
		std::shared_ptr<mMeshPartData> getOnePartDataByName(QString partName);

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
		QHashIterator<int, std::shared_ptr<mMeshNodeData> > getMeshNodeIterator();

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
		MViewBasic::ModelOperateEnum _meshOperateEnum;

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
		void generateMeshLine(mMeshFaceData* surfaceID1, mMeshFaceData* surfaceID2, mMeshData *meshData, QString partName, mMeshPartData* partData);

	private:

		//�Ƿ���Ƶ������
		bool _hasModalValue;

		//Ƶ������
		double _modalValue[2];

	protected:

		QVector<std::shared_ptr<mMeshNodeData>> _nodeData2;
		QHash<int, std::shared_ptr<mMeshNodeData>> _nodeData1;

		QVector<std::shared_ptr<mMeshData>> _meshData2;
		QHash<int, std::shared_ptr<mMeshData>> _meshData1;


		//QHash<int, mMeshLineData*> _meshLineData1;

		//std::vector<std::shared_ptr<mMeshFaceData>> _meshFaceData1;

		QHash<QString, std::shared_ptr<mMeshPartData>> _partData1;

		//ģ�ͳߴ�
		MViewBasic::ModelSize _modelSize;

		//�Ƿ��ģ�ͽ���ɾ���������
		bool _isAddOrDeletePart_Mesh;
		
	};
}