#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QFutureWatcher>

//MViewBasic
#include "mMeshViewEnum.h"

//MViewBasic
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "SpaceTree.h"

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace MDataGeo
{
	class mGeoModelData1;
	class mGeoPickData1;
	class mPreGeoPickThread;
}
namespace MDataPre
{
	class mPreMeshPickData1;
}
using namespace MViewBasic;
using namespace MBaseRend;
using namespace MDataGeo;
using namespace MDataPre;
using namespace std;
namespace MPreRend
{
	class mPreRend;
	class mPreRendStatus;
	class mPreGeoModelRender;
	class mPreGeoHighLightRender;
	class mPreMeshModelRender;
	class mPreMeshHighLightRender;
	class mPreMeshPickThread;
	class RENDVIEW_EXPORT mPreRender :public mBaseRender
	{
		Q_OBJECT
	public:

		mPreRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPreRend *preRend);

		bool getIsDragSomething(QVector2D pos) override;

		void dragSomething(QVector2D pos) override;

		void startPick(QVector<QVector2D> poses) override;

		bool getModelAABB(Space::AABB &aabb);

		//��ȡ����ģ����Ⱦ����
		mGeoModelData1 *getGeoModelData() { return _geoModelData; }; 

		//��ȡ����ģ��ʰȡ����
		mGeoPickData1 *getGeoPickData() { return _geoPickData; };

		//��ȡ����ģ��ʰȡ����
		mPreMeshPickData1 *getMeshPickData() { return _meshPickData; };
		 
		//���¸�����Ⱦ
		void updateHighLightRender();

		//���²�������
		void updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>> modelOperates);
		
		//���ȫ������Ⱦ
		void clearRender();
		
		//���õ�ǰ����ʾģʽ	
		void setShowFuntion(ShowFuntion shoFuntion);

		//���ýڵ�Ĵ�С
		void setNodeSize(int size);

		//���õ�Ĵ�С
		void setPointSize(int size);

		//���ñ߽��ߵĿ��
		void setEdgeLineWidth(float size);

		//�����ߵ���ɫ
		void setFaceLineColor(QVector4D color);


		~mPreRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

	private:
		//���������Ƿ�����
		void setLineVisual(float l_r);

	signals:
		//ʰȡ����ź�
		void finishedPickSig();

	protected:

		shared_ptr<mPreRendStatus> _rendStatus;

		QFutureWatcher<void> w;//���ʰȡ���߳�

		shared_ptr<mPreGeoModelRender> _geoModelRender;//������Ⱦ
		mGeoModelData1 *_geoModelData;//����ģ������
		mPreGeoPickThread *_geoPickThread;//����ʰȡ
		mGeoPickData1 *_geoPickData;//����ʰȡ����
		shared_ptr<mPreGeoHighLightRender> _geoHighLightRender;//���θ�������

		shared_ptr<mPreMeshModelRender> _meshModelRender;//������Ⱦ
		mPreMeshPickData1 *_meshPickData;//��������
		mPreMeshPickThread *_meshPickThread;//����ʰȡ
		shared_ptr<mPreMeshHighLightRender> _meshHighLightRender;//���θ�������

		mxr::Texture *_pointTexture;//���ε������

		/*********************************ģ��*****************************************************/
		std::shared_ptr<mxr::StateSet> _faceStateSet;//��Ⱦ���״̬
		std::shared_ptr<mxr::StateSet> _independentlineStateSet;//��Ⱦ�����ߵ�״̬
		std::shared_ptr<mxr::StateSet> _dotlineStateSet;//��Ⱦ���ߵ�״̬
		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//��Ⱦ�߽��ߵ�״̬
		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ�������״̬
		std::shared_ptr<mxr::StateSet> _trifacelineStateSet;//��Ⱦ���������ߵ�״̬
		std::shared_ptr<mxr::StateSet> _quadfacelineStateSet;//��Ⱦ�ı������ߵ�״̬
		std::shared_ptr<mxr::StateSet> _nodeStateSet;

		//��ʱ��
		QTimer* _aniTimer;

		//ʰȡ�߳�
		////ǰ��Ĳ���������
		//mPostMeshPickThread *_thread; QFutureWatcher<void> w;
		//mPostMeshPickData *_pickData;
		//shared_ptr<mPreHighLightRender> _highLightRender;
		
	};
}
