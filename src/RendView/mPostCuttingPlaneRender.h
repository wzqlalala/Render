#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "rendview_global.h"

#include <memory>

#include <QVector3D>
#include <QVector4D>
#include <QVector>

#include "mMeshViewEnum.h"



namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace MDataPost
{
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace std;
namespace MPostRend
{
	class RENDVIEW_EXPORT mPostCuttingPlaneData 
	{
	public:
		mPostCuttingPlaneData(mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData = nullptr, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);
		~mPostCuttingPlaneData();

		void reverseCuttingPlaneNormal();

		void setCuttingData(QVector3D normal, QVector3D vertex);

		QVector4D getCuttingPlane();

		QVector<QVector3D> getTriVertex();

		QVector<float> getTriTexture();

		QVector<QVector3D> getTriVectorValue();

		void setPlaneData(QVector3D normal, QVector3D centervertex, float maxR);

		QVector3D getCuttingPlaneVertex();

		QVector3D getCuttingPlaneNormal();

		/*
		* �������ͼ�ϵĵ������ʸ��ֵ��û������Ҳû�����ǻ���������ͼ�ϵĵ㲻��Ӧ��
		*/
		QPair<QVector<QVector3D>, QVector<QVector3D>> getCuttingNodeData();

		/*
		* ͨ��ָ���������������������
		*/
		QPair<QVector<QVector3D>, QVector<double>> getCuttingNodeData(QHash<int, double> values, MViewBasic::NodeOrElement nodeOrElement);

		/*
		* ͨ��ָ��ʸ�����������������
		*/
		QPair<QVector<QVector3D>, QVector<QVector3D>> getCuttingNodeData(QHash<int, QVector3D> vectorValues, MViewBasic::NodeOrElement nodeOrElement);

		//�Ƿ���
		bool isCutting;
		//ƽ������
		float theta;//�Ƕ�
		QVector3D axis;//��ת��
		QVector3D centerVertex;//���ĵ�
		QVector<QVector3D> _planeVertex;//ƽ���ĸ��������
	private:

		//�������ƽ��
		void caculateCuttingPlaneData();

		//���ڽڵ�������жϿռ�ֱ�ߺ�ƽ��Ľ��㣬vertex����ֱ�ߵ������˵㣬value���������˵��ֵ��isInVertex�����ཻ�ĵ�����غϵ������˵�
		template <class T>
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<T> value, QVector<QPair<QVector3D, T>> &isInVertex)
		{
			//ƽ�淨����
			QVector3D vp = _normal;

			//ƽ��������һ��
			QVector3D n = _vertex;

			//�߶η�������
			QVector3D v = vertex.last() - vertex.first();

			//�߶���ʼ��
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			T t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				QVector3D jiaodian = m + v * t;
				T jiaodianvalue = value.first() + (value.last() - value.first()) * t;
				isInVertex.append(QPair<QVector3D, T>{jiaodian, jiaodianvalue});
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains({ vertex.first(),value.first() }))
				{
					isInVertex.append({ vertex.first(),value.first() });
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains({ vertex.last(),value.last() }))
				{
					isInVertex.append({ vertex.last(),value.last() });
				}
			}
			return false;
		};

		//���ڽڵ�ʸ�����жϿռ�ֱ�ߺ�ƽ��Ľ��㣬vertex����ֱ�ߵ������˵㣬value���������˵��ֵ,vectorValue���������˵��ʸ��ֵ��isInVertex�����ཻ�ĵ�����غϵ������˵㣨�������ֵ��
		template <class T>
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<T> value, QVector<QVector3D> vectorValue, QVector<QPair<QVector3D, T>> &isInVertex, QVector<QVector3D> &isInvectorValue)
		{
			//ƽ�淨����
			QVector3D vp = _normal;

			//ƽ��������һ��
			QVector3D n = _vertex;

			//�߶η�������
			QVector3D v = vertex.last() - vertex.first();

			//�߶���ʼ��
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			T t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				QVector3D jiaodian = m + v * t;
				T jiaodianvalue = value.first() + (value.last() - value.first()) * t;
				QVector3D jiaodianvector = vectorValue.first() + (vectorValue.last() - vectorValue.first()) * t;

				isInVertex.append(QPair<QVector3D, T>{jiaodian, jiaodianvalue});
				isInvectorValue.append(jiaodianvector);
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains({ vertex.first(),value.first() }))
				{
					isInVertex.append({ vertex.first(),value.first() });
					isInvectorValue.append(vectorValue.first());
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains({ vertex.last(),value.last() }))
				{
					isInVertex.append({ vertex.last(),value.last() });
					isInvectorValue.append(vectorValue.last());
				}
			}
			return false;
		};

		//���ڽڵ�ʸ�����жϿռ�ֱ�ߺ�ƽ��Ľ��㣬vertex����ֱ�ߵ������˵㣬vectorValue���������˵��ʸ��ֵ��isInVertex�����ཻ�ĵ�����غϵ������˵㣨���������ֵ��
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<QVector3D> vectorValue, QVector<QVector3D> &isInVertex, QVector<QVector3D> &isInvectorValue)
		{
			//ƽ�淨����
			QVector3D vp = _normal;

			//ƽ��������һ��
			QVector3D n = _vertex;

			//�߶η�������
			QVector3D v = vertex.last() - vertex.first();

			//�߶���ʼ��
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			float t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				QVector3D jiaodian = m + v * t;
				QVector3D jiaodianvector = vectorValue.first() + (vectorValue.last() - vectorValue.first()) * t;

				isInVertex.append(jiaodian);
				isInvectorValue.append(jiaodianvector);
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains(vertex.first()))
				{
					isInVertex.append(vertex.first());
					isInvectorValue.append(vectorValue.first());
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains(vertex.last()))
				{
					isInVertex.append(vertex.last());
					isInvectorValue.append(vectorValue.last());
				}
			}

			return false;
		};

		//���ڵ�Ԫ�������жϿռ�ֱ�ߺ�ƽ��Ľ��㣬vertex����ֱ�ߵ������˵㣬isInVertex�����ཻ�ĵ�����غϵ������˵�
		bool judgeLineIsCuttingByPlane(QVector<QVector3D> vertex, QVector<QVector3D> &isInVertex)
		{
			QVector3D jiaodian;

			//ƽ�淨����
			QVector3D vp = _normal;

			//ƽ��������һ��
			QVector3D n = _vertex;

			//�߶η�������
			QVector3D v = vertex.last() - vertex.first();

			//�߶���ʼ��
			QVector3D m = vertex.first();

			float vpt = QVector3D::dotProduct(v, vp);

			if (vpt == 0)
			{
				return false;
			}

			float t = QVector3D::dotProduct((n - m), vp) / vpt;

			if (t > 0 && t < 1)
			{
				jiaodian = m + v * t;
				isInVertex.append(jiaodian);
				return true;
			}
			else if (t == 0)
			{
				if (!isInVertex.contains(vertex.first()))
				{
					isInVertex.append(vertex.first());
				}
			}
			else if (t == 1)
			{
				if (!isInVertex.contains(vertex.last()))
				{
					isInVertex.append(vertex.last());
				}
			}
			return false;
		};

		//�������εĶ���˳��a��b�������ε�������
		bool judgeVertexOrder(QVector3D a, QVector3D b, QVector3D center, QVector3D OO)
		{
			QVector3D OA = a - center;
			QVector3D OB = b - center;
			//����OA������OB�Ĳ��
			QVector3D voa = QVector3D::crossProduct(a - center, OO);
			QVector3D vob = QVector3D::crossProduct(b - center, OO);

			float cosA = QVector3D::dotProduct(OA, OO) / (OA.length());
			float cosB = QVector3D::dotProduct(OB, OO) / (OB.length());

			if (QVector3D::dotProduct(voa, _normal) < 0)
			{
				cosA = -(cosA + 2);
			}
			if (QVector3D::dotProduct(vob, _normal) < 0)
			{
				cosB = -(cosB + 2);
			}
			return cosA < cosB;
		};
	private:
		mPostOneFrameRendData *_currentFrameRendData;
		bool _isInitial;

		//�ж��Ƿ����ʸ������
		bool _hasVector;

		QVector4D _cuttingPlane;
		QVector3D _normal;
		QVector3D _vertex;

		//������
		QVector<QVector3D> _triPlane;
		QVector<float> _triValue;
		QVector<float> _triTexture;
		QVector<QVector3D> _triVectorValue;
		QVector<float> _triMeshID;
		//ģ������
		MDataPost::mOneFrameData1* _oneFrameData;

	};
	class RENDVIEW_EXPORT mPostCuttingPlaneRender
	{
	public:

		mPostCuttingPlaneRender(shared_ptr<mxr::Group> parent, std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet);

		~mPostCuttingPlaneRender();

		//����ͼ����ţ����������㣬�Ƿ����ʸ��ֵ
		void createCuttingPlane(mOneFrameData1 *oneFramData, mPostOneFrameRendData* currentFrameRendData,int index, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);
		void setPlaneData(QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);
		QVector4D getCuttingPlane();
		void setIsShowCuttingPlane(bool isShow);
		void reverseCuttingPlaneNormal();
		void updateCuttingPlaneIndex(int index);

	private:
		void UpdateCuttingPlane(int index);

	private:

		mPostCuttingPlaneData* _postCuttingPlaneData;
		std::shared_ptr<mxr::Group> _parent;

		//����ͼ
		std::shared_ptr<mxr::Drawable> _cuttingPlaneDrawable;
		std::shared_ptr<mxr::StateSet> _cuttingPlaneStateSet;

		//͸��ƽ��
		std::shared_ptr<mxr::Drawable> _transparentPlaneDrawable;
		std::shared_ptr<mxr::StateSet> _transparentPlaneStateSet;

		bool _isShowCuttingPlane;
	};
}
