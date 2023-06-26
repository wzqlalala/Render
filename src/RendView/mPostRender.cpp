#include "mPostRender.h"
#include "mPostRend.h"
#include "mPostRendStatus.h"
#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"
#include "mPostAnimationRender.h"
#include "mPostHighLightRender.h"
#include "mPostTempHighLightRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
#include "mPostFrameText.h"
#include "mPostDragRender.h"

#include <renderpch.h>
#include "texture.h"
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>

//MDataPost
#include "mDataPost1.h"
#include "mOneFrameData1.h"
#include "mPostOneFrameRendData.h"
#include "mPostAnimationRendData.h"
#include "mPostColorTableData.h"
#include "mPostMeshPickThread.h"
#include "mPostMeshPickData.h"
#include "mPostMeshData1.h"
#include "mPostMeshNodeData1.h"
#include "mPostMeshPartData1.h"

#include <qmath.h>

using namespace mxr;
using namespace std;
namespace MPostRend
{
	mPostRender::mPostRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPostRend *postRend):mBaseRender(app, parent, postRend)
	{
		this->makeCurrent();

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "mPostRender" << QString::number(long long int(context), 16);
		_dataPost = nullptr;
		_texture = nullptr;
		_oneFrameRender = nullptr;
		_oneFrameAnimationRender = nullptr;
		_currentDragRender = nullptr;
		_postFrameText = new mPostFrameText(postRend->getFontRender());
		////_animationId = 0;
		////_animationRender = nullptr;

		_rendStatus = make_shared<mPostRendStatus>();

		/**********************************************************模型**********************************************************/
		//face
		_faceStateSet = MakeAsset<StateSet>();
		mxr::Shader *faceshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceWithDeformation");
		_faceStateSet->setShader(faceshader);
		_faceStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_faceStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_faceStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_faceStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));

		//face透明
		_faceTransparentStateSet = MakeAsset<StateSet>();
		mxr::Shader * facetransparentshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceTransparentWithDeformation");
		_faceTransparentStateSet->setShader(facetransparentshader);
		_faceTransparentStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_faceTransparentStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceTransparentStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_faceTransparentStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));

		//face透明无变形
		_faceTransparentNodeformationStateSet = MakeAsset<StateSet>();
		mxr::Shader * facetransparentNodeformationshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceTransparentWithOutDeformation");
		_faceTransparentNodeformationStateSet->setShader(facetransparentNodeformationshader);
		_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceTransparentNodeformationStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));

		//edgeline
		_edgelineStateSet = MakeAsset<StateSet>();
		mxr::Shader * meshlineshader = mShaderManage::GetInstance()->GetShader("PostEdgeLineWithDeformation");
		_edgelineStateSet->setShader(meshlineshader);
		_edgelineStateSet->setDrawMode(GL_LINES);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_edgelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("lineWidth", 1.0f));

		//faceline
		_facelineStateSet = MakeAsset<StateSet>();
		mxr::Shader * facelineshader = mShaderManage::GetInstance()->GetShader("PostMeshFaceLineWithDeformation");
		_facelineStateSet->setShader(facelineshader);
		_facelineStateSet->setDrawMode(GL_TRIANGLES);
		_facelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_facelineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_facelineStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("isAllColor", float(1)));
		//glLineWidth(10.0);

		//line
		_lineStateSet = MakeAsset<StateSet>();
		mxr::Shader * lineshader = mShaderManage::GetInstance()->GetShader("PostMeshLineWithDeformation");
		_lineStateSet->setShader(lineshader);
		_lineStateSet->setDrawMode(GL_LINES);
		_lineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_lineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_lineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_lineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_lineStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_lineStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_lineStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_lineStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_lineStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));

		//point
		_pointStateSet = MakeAsset<StateSet>();
		mxr::Shader * pointshader = mShaderManage::GetInstance()->GetShader("PostMeshPointWithDeformation");
		_pointStateSet->setShader(pointshader);
		_pointStateSet->setDrawMode(GL_POINTS);
		_pointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_pointStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_pointStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_pointStateSet->setUniform(MakeAsset<Uniform>("deformationScale", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("PointSize", _rendStatus->_pointSize));

		/**********************************************************切面**********************************************************/
		//cuttingplane
		_cuttingPlaneStateSet = MakeAsset<StateSet>();
		mxr::Shader * cuttingplaneshader = mShaderManage::GetInstance()->GetShader("PostCuttingPlaneWithOutDeformation");
		_cuttingPlaneStateSet->setShader(cuttingplaneshader);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(1, 1), 1);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[0]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[1]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[2]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[3]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[4]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[5]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[6]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("planes[7]", QVector4D()), false);
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));
		_cuttingPlaneStateSet->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(0)));

		//transparentPlane
		_transparentPlaneStateSet = MakeAsset<StateSet>();
		mxr::Shader * transparentplaneshader = mShaderManage::GetInstance()->GetShader("PostTransparentPlaneWithOutDeformation");
		_transparentPlaneStateSet->setShader(transparentplaneshader);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_transparentPlaneStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_transparentPlaneStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));

		/**********************************************************等值线等值面**********************************************************/
		//等值线
		_contourLineStateSet = MakeAsset<StateSet>();
		mxr::Shader * countourlineshader = mShaderManage::GetInstance()->GetShader("PostContourLineWithOutDeformation");
		_contourLineStateSet->setShader(countourlineshader);
		_contourLineStateSet->setDrawMode(GL_LINES);
		_contourLineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_contourLineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_contourLineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_contourLineStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_contourLineStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_contourLineStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_contourLineStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_contourLineStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_contourLineStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));

		//等值面
		_contourFaceStateSet = MakeAsset<StateSet>();
		mxr::Shader * contourfaceshader = mShaderManage::GetInstance()->GetShader("PostContourFaceWithOutDeformation");
		_contourFaceStateSet->setShader(contourfaceshader);
		_contourFaceStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_contourFaceStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_contourFaceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_contourFaceStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_contourFaceStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));

		//流线的点
		_streamlinePointStateSet = MakeAsset<StateSet>();
		mxr::Shader * streamlinepointshader = mShaderManage::GetInstance()->GetShader("PostStreamLinePointWithOutDeformation");
		_streamlinePointStateSet->setShader(streamlinepointshader);
		_streamlinePointStateSet->setDrawMode(GL_POINTS);
		_streamlinePointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_streamlinePointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_streamlinePointStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("minValue", float(0)));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("maxValue", float(0)));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("PointSize", _rendStatus->_pointSize));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_streamlinePointStateSet->setUniform(MakeAsset<Uniform>("isEquivariance", int(0)));

		//初始化计时器
		_aniTimer = new QTimer;
		_aniTimer->setInterval(0);
		connect(_aniTimer, SIGNAL(timeout()), this, SLOT(slot_aniTimer()));

		//连接拖拽信号
		connect(this, SIGNAL(finishedDragSig()), this, SLOT(slot_finsishedDrag()));

		//初始化高亮渲染
		_pickData = new mPostMeshPickData;
		_highLightRender = make_shared<mPostHighLightRender>(_rendStatus, _pickData);

		//初始化临时高亮渲染
		_tempHighLightRender = make_shared<mPostTempHighLightRender>(_rendStatus);

		//添加积分球
		shared_ptr<mPostSphereRender> sphereRender = MakeAsset<mPostSphereRender>("积分球", _app, _parent, _rendStatus);
		_dragRenders.insert("积分球",sphereRender);

		shared_ptr<mPostMinMaxRender> minRender = MakeAsset<mPostMinMaxRender>("最小值", _app, _parent, _rendStatus);
		_dragRenders.insert("最小值", minRender);
		minRender->setColor(QVector3D(0, 0, 1));
		minRender->setIsShow(false);

		shared_ptr<mPostMinMaxRender> maxRender = MakeAsset<mPostMinMaxRender>("最大值", _app, _parent, _rendStatus);
		_dragRenders.insert("最大值", maxRender);
		maxRender->setColor(QVector3D(1, 0, 0));
		maxRender->setIsShow(false);

		//this->doneCurrent();
	}
	bool mPostRender::getIsDragSomething(QVector2D pos)
	{
		this->makeCurrent();
		float depth = this->getDepth(pos);
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		QMatrix4x4 matrix = _baseRend->getCamera()->getPVMValue();
		//判断是否有物体被拖拽
		for (auto render : _dragRenders)
		{
			if (render->pointIsIn(pos, depth, matrix, _baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT))
			{
				_currentDragRender = render;
				return true;
			}
		}
		return false;
	}
	void mPostRender::dragSomething(QVector2D pos)
	{
		this->makeCurrent();
		float depth = this->getDepth(pos);
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
		QMatrix4x4 matrix = _baseRend->getCamera()->getPVMValue();
		//拖拽物体并更新物体
		if (_currentDragRender)
		{
			_currentDragRender->move(pos, matrix, _baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT);
			emit finishedDragSig();
		}
	}
	QTime time;
	void mPostRender::startPick(QVector<QVector2D> poses)
	{
		if (_currentDragRender != nullptr)
		{
			//emit finishedDragSig();
			_currentDragRender = nullptr;
			return;
		}
		QTime time;
		time.start();
		makeCurrent();
		//开始拾取操作
		if (!_oneFrameRender)
		{
			return;
		}
		_pickData->setMeshPickFunction(int(_baseRend->getPickFuntion()));
		_thread->setCurrentFrameRend(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
		_thread->setMatrix(_baseRend->getCamera()->getPVMValue());
		_thread->setWidget(_baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT);
		_thread->setPickMode(*_baseRend->getCurrentPickMode(), *_baseRend->getMultiplyPickMode());
		_thread->setPickAngleValue(_baseRend->getPickAngle());
		if (*_baseRend->getCurrentPickMode() == PickMode::SoloPick)
		{		
			float depth = this->getDepth(poses.first());
			GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
			if (error != 0)
			{
				qDebug() << error;
			}
			_thread->setLocation(poses.first(), depth);
		}
		else
		{
			_thread->setLocation(poses, (_baseRend->getCamera()->_Center - _baseRend->getCamera()->_Eye).normalized());
		}
		QFuture<void> future; 
		future = QtConcurrent::run(_thread, &mPostMeshPickThread::startPick);
		QObject::connect(&w, &QFutureWatcher<void>::finished, [this, time] {
			//qDebug() << "拾取消耗时间" << time.elapsed();
			_highLightRender->updateHighLightRender(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
			//this->
			//set<int> ids = _pickData->getPickNodeIDs();
			//qDebug() << "拾取完成";
			QObject::disconnect(&w, 0, 0, 0);//断开信号
			emit update();
			emit finishedPickSig();
		});
		w.setFuture(future);

	}

	mPostFrameText * mPostRender::getPostFrameText()
	{
		return _postFrameText;
	}

	void mPostRender::updateHighLightRender()
	{
		this->makeCurrent();
		if (_highLightRender)
			_highLightRender->updateHighLightRender(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());
	}
	void mPostRender::updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>> postModelOperates)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->updateOneModelOperate(postModelOperates);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->updateOneModelOperate(postModelOperates);
		}
		for (auto rend : _animationRender)
		{
			rend->updateOneModelOperate(postModelOperates);
		}

		if (postModelOperates.first == HideOnePartOperate || postModelOperates.first == ShowOnePartOperate)
		{
			if (_rendStatus->_postMode == OneFrame || _rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
			{
				this->getMinMaxLocation();
			}
		}
	}
	void mPostRender::updateAllModelOperate(MViewBasic::PostModelOperateEnum postModelOperate)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->updateAllModelOperate(postModelOperate);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->updateAllModelOperate(postModelOperate);
		}
		for (auto rend : _animationRender)
		{
			rend->updateAllModelOperate(postModelOperate);
		}
		if (postModelOperate == HideAllPartOperate || postModelOperate == ShowAllPartOperate)
		{
			if (_rendStatus->_postMode == OneFrame || _rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
			{
				this->getMinMaxLocation();
			}
		}
	}
	Space::AABB mPostRender::getCurrentModelData()
	{
		this->calculateMinMaxLinePosition();
		if (_rendStatus->_postMode == No)
		{

		}
		else if (_rendStatus->_postMode == OneFrame || _rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
		{
			return this->getOneFrameRender()->getModelRender()->getModelAABB();
		}
		else
		{
			Space::AABB aabb;
			for (auto render : _animationRender)
			{
				aabb.push(render->getModelRender()->getModelAABB());
			}
			return aabb;
		}
		return Space::AABB();
	}
	void mPostRender::setPostMode(PostMode postMode)
	{
		_rendStatus->_postMode = postMode;
	}
	void mPostRender::clearRender()
	{
		this->makeCurrent();
		_dataPost.reset();
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "deletecontext" << QString::number(long long int(context), 16);
		if (_oneFrameRender)
		{
			_oneFrameRender->deleteThieFrame();
			_oneFrameRender.reset();
		}
		deleteAnimation();
		_baseRend->getFontRender()->clearAllRender();
		_baseRend->getArrowRender()->clearAllRender();
		_pickData->clearAllPickData();
		//_highLightRender->updateHighLightRender();
		//_tempHighLightRender->setAllMeshData();
	}
	void mPostRender::setRendCurrentFrameData(mPostOneFrameRendData* postOneFrameRendData)
	{
		this->makeCurrent();
		if (!_dataPost || nullptr == postOneFrameRendData)
		{
			return;
		}
		int id = postOneFrameRendData->getRendID();
		mOneFrameData1 *oneFrameData = _dataPost->getOneFrameData(id);
		if (_oneFrameRender)
		{
			_oneFrameRender.reset();
		}
		if (!_texture)
		{
			mPostColorTableData *table = postOneFrameRendData->getRendColorTable();
			_texture = new Texture(GL_TEXTURE_1D, table->getPostColorTableNum(), 0, 0, GL_RGB8, 1);
			_texture->SetData(0, 0, table->getPostColorTableNum(), table->getColorTable());
			_faceStateSet->setTexture("texture", _texture);
			_facelineStateSet->setTexture("texture", _texture);
			_lineStateSet->setTexture("texture", _texture);
			_pointStateSet->setTexture("texture", _texture);
			_cuttingPlaneStateSet->setTexture("texture", _texture);
			_contourFaceStateSet->setTexture("texture", _texture);
			_contourLineStateSet->setTexture("texture", _texture);
			_streamlinePointStateSet->setTexture("texture", _texture);
		}
		_oneFrameRender = make_shared<mPostOneFrameRender>(_app, _rendStatus, oneFrameData, postOneFrameRendData);
		_oneFrameRender->setFaceStateSet(_faceStateSet);
		_oneFrameRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
		_oneFrameRender->setFaceTransparentStateSet(_faceTransparentStateSet);
		_oneFrameRender->setEdgeLineStateSet(_edgelineStateSet);
		_oneFrameRender->setFaceLineStateSet(_facelineStateSet);
		_oneFrameRender->setLineStateSet(_lineStateSet);
		_oneFrameRender->setPointStateSet(_pointStateSet);
		//_oneFrameRender->setTexture(_texture);
		_oneFrameRender->updateAllModelOperate(ImportOperate);
		this->setDispersed(true);
		this->initialPickThreads();
		this->getMinMaxLocation();
	}

	void mPostRender::setShowFuntion(ShowFuntion showFuntion)
	{
		this->makeCurrent();
		_rendStatus->_showFunction = showFuntion;
		if (_oneFrameRender)
		{
			_oneFrameRender->getModelRender()->setShowFuntion(showFuntion);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->getModelRender()->setShowFuntion(showFuntion);
		}
		for (auto rend : _animationRender)
		{
			rend->getModelRender()->setShowFuntion(showFuntion);
		}
	}

	void mPostRender::setDispersed(bool isdispersed)
	{
		this->makeCurrent();
		_texture->Bind(_texture->ID());
		if (isdispersed)
		{
			_texture->setTexParameteri(GL_NEAREST, GL_NEAREST);
		}
		else
		{
			_texture->setTexParameteri(GL_LINEAR, GL_LINEAR);
		}
	}

	void mPostRender::setDeformationScale(QVector3D deformationScale)
	{
		this->makeCurrent();
		_rendStatus->_deformFactor = deformationScale;
		if (_oneFrameRender)
		{
			_oneFrameRender->setDeformationScale(deformationScale);
		}
		for (auto rend : _animationRender)
		{
			rend->setDeformationScale(deformationScale);
		}
	}

	void mPostRender::setIsShowInitialShape(bool isShowInitialShape)
	{
		this->makeCurrent();
		_rendStatus->_isShowInitialShape = isShowInitialShape;
		if (_oneFrameRender)
		{
			_oneFrameRender->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
		for (auto rend : _animationRender)
		{
			rend->getModelRender()->setIsShowInitialShape(isShowInitialShape);
		}
	}

	void mPostRender::setTextureCoordScale(float textureCoordScale)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setTextureCoordScale(textureCoordScale);
		}
		for (auto rend : _animationRender)
		{
			rend->setTextureCoordScale(textureCoordScale);
		}
	}

	void mPostRender::setMinMaxData(float maxValue, float minValue)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setMinMaxData(maxValue, minValue);
		}
	}

	void mPostRender::setRangeType(int index)
	{
		this->makeCurrent();
		_rendStatus->_rangeType = index;
		if (_oneFrameRender)
		{
			_oneFrameRender->updateAllModelOperate(UpdateMinMax);
		}
		this->getMinMaxLocation();
	}

	void mPostRender::setDispersIsEquivariance(bool isEquivariance)
	{
		_rendStatus->_isEquivariance = isEquivariance;
	}

	void mPostRender::setPointSize(int size)
	{
		_rendStatus->_pointSize = size;
		_pointStateSet->getUniform("PointSize")->SetData(_rendStatus->_pointSize);
	}

	void mPostRender::setEdgeLineWidth(float size)
	{
		_rendStatus->_lineWidth = size;
		_edgelineStateSet->getUniform("lineWidth")->SetData(_rendStatus->_lineWidth);
	}

	void mPostRender::setFaceLineColor(QVector4D color)
	{
		_rendStatus->_faceLineColor = color;
		_facelineStateSet->getUniform("showColor")->SetData(_rendStatus->_faceLineColor);
	}

	void mPostRender::setLightIsOn(bool isOn)
	{
		_faceStateSet->getUniform("lightIsOn")->SetData(int(isOn));
		_faceTransparentStateSet->getUniform("lightIsOn")->SetData(int(isOn));
		_faceTransparentNodeformationStateSet->getUniform("lightIsOn")->SetData(int(isOn));
		_pointStateSet->getUniform("lightIsOn")->SetData(int(isOn));
		_cuttingPlaneStateSet->getUniform("lightIsOn")->SetData(int(isOn));
		_streamlinePointStateSet->getUniform("lightIsOn")->SetData(int(isOn));
	}

	void mPostRender::setLightIsDependentCamera(bool lightIsDependOnCamera)
	{
		_rendStatus->_lightIsDependOnCamera = lightIsDependOnCamera;
	}

	void mPostRender::setLightPosition(QVector3D position)
	{
		_rendStatus->_postLight.lightPosition = position;
		_faceStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		_faceTransparentStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		_faceTransparentNodeformationStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		_pointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		_cuttingPlaneStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
		_streamlinePointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);

	}

	void mPostRender::setLightParameters(QVector3D ambient, QVector3D diffuse, QVector3D specular, float shiness)
	{
		_rendStatus->_postLight.ambient = ambient;
		_rendStatus->_postLight.diffuse = diffuse;
		_rendStatus->_postLight.specular = specular;
		_rendStatus->_postLight.shiness = shiness;
		_faceStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
		_faceStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
		_faceStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
		_faceStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);

		_faceTransparentStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
		_faceTransparentStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
		_faceTransparentStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
		_faceTransparentStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);

		_faceTransparentNodeformationStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
		_faceTransparentNodeformationStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
		_faceTransparentNodeformationStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
		_faceTransparentNodeformationStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);

		_pointStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
		_pointStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
		_pointStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
		_pointStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);

		_cuttingPlaneStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
		_cuttingPlaneStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
		_cuttingPlaneStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
		_cuttingPlaneStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);

		_streamlinePointStateSet->getUniform("light.ambient")->SetData(_rendStatus->_postLight.ambient);
		_streamlinePointStateSet->getUniform("light.diffuse")->SetData(_rendStatus->_postLight.diffuse);
		_streamlinePointStateSet->getUniform("light.specular")->SetData(_rendStatus->_postLight.specular);
		_streamlinePointStateSet->getUniform("light.shiness")->SetData(_rendStatus->_postLight.shiness);
	}

	void mPostRender::deleteCuttingPlane(int num)
	{
		this->makeCurrent();
		bool hasDeleteCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasDeleteCuttingPlane = _oneFrameRender->deleteCuttingPlane(num);
		}
		if (_oneFrameAnimationRender)
		{
			hasDeleteCuttingPlane = _oneFrameAnimationRender->deleteCuttingPlane(num);
		}
		for (auto rend : _animationRender)
		{
			hasDeleteCuttingPlane = rend->deleteCuttingPlane(num);
		}
		if (!hasDeleteCuttingPlane)
		{
			return;
		}
		_rendStatus->_cuttingPlanes.removeAt(num);
		this->updateCuttingPlaneUniform();
	}

	void mPostRender::reverseCuttingPlaneNormal(int num)
	{
		this->makeCurrent();
		bool hasReverseCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasReverseCuttingPlane = _oneFrameRender->reverseCuttingPlaneNormal(num);
		}
		if (_oneFrameAnimationRender)
		{
			hasReverseCuttingPlane = _oneFrameAnimationRender->reverseCuttingPlaneNormal(num);
		}
		for (auto rend : _animationRender)
		{
			hasReverseCuttingPlane = rend->reverseCuttingPlaneNormal(num);
		}
		if (!hasReverseCuttingPlane)
		{
			return;
		}
		_rendStatus->_cuttingPlanes[num] = -_rendStatus->_cuttingPlanes[num];
		updateCuttingPlaneUniform();
	}

	void mPostRender::setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane)
	{
		_rendStatus->_isOnlyShowCuttingPlane = isOnlyShowCuttingPlane;
		this->makeCurrent();

		if (_oneFrameRender)
		{
			_oneFrameRender->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
		for (auto rend : _animationRender)
		{
			rend->setOnlyShowCuttingPlane(isOnlyShowCuttingPlane);
		}
	}

	void mPostRender::setIsShowCuttingPlane(int num, bool isShow)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setIsShowCuttingPlane(num, isShow);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setIsShowCuttingPlane(num, isShow);
		}
		for (auto rend : _animationRender)
		{
			rend->setIsShowCuttingPlane(num, isShow);
		}
	}

	void mPostRender::createCuttingPlane(int num, QVector3D normal, QVector3D vertex, bool hasVector)
	{
		this->makeCurrent();
		bool hasCreateCuttingPlane = false;
		if (_oneFrameRender)
		{
			hasCreateCuttingPlane = _oneFrameRender->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
		if (_oneFrameAnimationRender)
		{
			hasCreateCuttingPlane = _oneFrameAnimationRender->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
		for (auto rend : _animationRender)
		{
			hasCreateCuttingPlane = rend->createCuttingPlane(_cuttingPlaneStateSet, _transparentPlaneStateSet, num, normal, vertex, hasVector);
		}
		if (!hasCreateCuttingPlane)
		{
			return;
		}
		QVector4D plane = QVector4D(normal, -QVector3D::dotProduct(normal, vertex));
		if (num < _rendStatus->_cuttingPlanes.size())
		{
			_rendStatus->_cuttingPlanes.replace(num, plane);
		}
		else if (num == _rendStatus->_cuttingPlanes.size())
		{
			_rendStatus->_cuttingPlanes.append(plane);
		}
		this->updateCuttingPlaneUniform();
	}

	void mPostRender::setPlaneData(int num, QVector3D normal, QVector3D centervertex, float maxR)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setPlaneData(num, normal, centervertex, maxR);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setPlaneData(num, normal, centervertex, maxR);
		}
		for (auto rend : _animationRender)
		{
			rend->setPlaneData(num, normal, centervertex, maxR);
		}
	}

	void mPostRender::setIsShowPlane(bool isShow)
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->setIsShowPlane(isShow);
		}
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->setIsShowPlane(isShow);
		}
		for (auto rend : _animationRender)
		{
			rend->setIsShowPlane(isShow);
		}
	}

	void mPostRender::setAnimationFrame(int start, int end, int current)
	{
		_rendStatus->_aniStartFrame = start;
		_rendStatus->_aniEndFrame = end;
		_rendStatus->_aniCurrentFrame = current;
	}

	void mPostRender::setAnimationFrameRate(int fr)
	{
		_rendStatus->_aniFrameRate = fr;
	}

	void mPostRender::setAnimationFrameInterval(int f)
	{
		_rendStatus->_aniFrameInterval = f;
	}

	void mPostRender::setAnimationLoopPlay(bool istrue)
	{
		_rendStatus->_aniLoopPlay;
	}

	void mPostRender::setRendAnimationFrame(std::shared_ptr<mPostAnimationRendData> allFrameRendData)
	{
		this->makeCurrent();
		if (!_dataPost || !allFrameRendData)
		{
			return;
		}
		deleteAnimation();
		_rendAnimationData = allFrameRendData;
		set<int> ids = _rendAnimationData->getRendAnimationIds();
		QVector<QFuture<void>> futures;
		for (int id : ids)
		{
			std::shared_ptr<mPostOneFrameRender> oneFrameRender = make_shared<mPostOneFrameRender>(_app, _rendStatus, _dataPost->getOneFrameData(id), _rendAnimationData->getRendOneFrameData(id));
			oneFrameRender->setFaceStateSet(_faceStateSet);
			oneFrameRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
			oneFrameRender->setFaceTransparentStateSet(_faceTransparentStateSet);
			oneFrameRender->setEdgeLineStateSet(_edgelineStateSet);
			oneFrameRender->setFaceLineStateSet(_facelineStateSet);
			oneFrameRender->setLineStateSet(_lineStateSet);
			oneFrameRender->setPointStateSet(_pointStateSet);
			oneFrameRender->setTextureCoordScale(1.0);
			oneFrameRender->setDeformationScale(_rendStatus->_deformFactor);
			//oneFrameRender->updateAllModelOperate(ImportOperate);
			//oneFrameRender->bufferThisFrame(_app->GLContext());
			_animationRender.insert(id, oneFrameRender);
			futures.append(QtConcurrent::run([oneFrameRender]
			{
				oneFrameRender->updateAllModelOperate(ImportOperate);
			}));
		}
		while (!futures.empty())
		{
			futures.back().waitForFinished();
			//futures.back().result()->bufferThisFrame(_app->GLContext());
			futures.takeLast();
		}
		for (auto rend : _animationRender)
		{
			//QtConcurrent::run(rend.get(), &mPostOneFrameRender::bufferThisFrame,_app->GLContext());
			rend->bufferThisFrame(_app->GLContext());
		}
		_rendStatus->_postMode = Animation;
	}

	void mPostRender::setAnimationFrameRange(bool isAgreement)
	{
		if (_rendStatus->_postMode == Animation)
		{
			if (_rendAnimationData == nullptr)
			{
				return;
			}
			_rendAnimationData->setAnimationFrameRange(isAgreement);
			QHashIterator<int, std::shared_ptr<mPostOneFrameRender>> iter(_animationRender);
			while (iter.hasNext())
			{
				iter.next();
				auto oneFrameRend = iter.value();
				if (oneFrameRend)
				{
					oneFrameRend->updateAllModelOperate(UpdateMinMax);
				}
			}
		}
	}

	void mPostRender::setAnimationFrameRange(float maxValue, float minValue)
	{
		if (_rendStatus->_postMode == Animation)
		{
			if (_rendAnimationData == nullptr)
			{
				return;
			}
			QHashIterator<int, std::shared_ptr<mPostOneFrameRender>> iter(_animationRender);
			while (iter.hasNext())
			{
				iter.next();
				auto oneFrameRend = iter.value();
				if (oneFrameRend)
				{
					oneFrameRend->setMinMaxData(maxValue, minValue);
				}
			}
		}
	}

	void mPostRender::createLinearAnimation(PostMode postMode)
	{
		this->makeCurrent();
		if (!_dataPost || nullptr == _oneFrameRender)
		{
			return;
		}
		deleteAnimation();
		mPostOneFrameRendData* postOneFrameRendData = _oneFrameRender->getOneFrameRendData();
		int id = postOneFrameRendData->getRendID();

		mOneFrameData1 *oneFrameData = _dataPost->getOneFrameData(id);

		mPostOneFrameRendData *newFrameRendData = new mPostOneFrameRendData(*postOneFrameRendData);
		_oneFrameAnimationRender = make_shared<mPostOneFrameRender>(_app, _rendStatus, oneFrameData, newFrameRendData);
		_oneFrameAnimationRender->setFaceStateSet(_faceStateSet);
		_oneFrameAnimationRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
		_oneFrameAnimationRender->setFaceTransparentStateSet(_faceTransparentStateSet);
		_oneFrameAnimationRender->setEdgeLineStateSet(_edgelineStateSet);
		_oneFrameAnimationRender->setFaceLineStateSet(_facelineStateSet);
		_oneFrameAnimationRender->setLineStateSet(_lineStateSet);
		_oneFrameAnimationRender->setPointStateSet(_pointStateSet);
		_oneFrameAnimationRender->setTextureCoordScale(0.0);
		_oneFrameAnimationRender->setDeformationScale(QVector3D(0,0,0));
		_oneFrameAnimationRender->updateAllModelOperate(ImportOperate);
		_oneFrameAnimationRender->bufferThisFrame(_app->GLContext());
		_rendStatus->_aniCurrentFrame = 1;
		_rendStatus->_postMode = postMode;

		/*
		int ids = 8;
		QVector<QFuture<void>> futures;
		for (int i = 0; i < ids; i++)
		{
			mPostOneFrameRendData *newFrameRendData = new mPostOneFrameRendData(*postOneFrameRendData);
			std::shared_ptr<mPostOneFrameRender> oneFrameRender = make_shared<mPostOneFrameRender>(_rendStatus, oneFrameData, newFrameRendData);
			float scale = postMode == OneFrameLinearAnimation ? i / float(ids - 1) : sin(2 * M_PI * i / float(ids - 1));

			oneFrameRender->setFaceStateSet(_faceStateSet);
			oneFrameRender->setFaceTransparentNoDeformationStateSet(_faceTransparentNodeformationStateSet);
			oneFrameRender->setFaceTransparentStateSet(_faceTransparentStateSet);
			oneFrameRender->setEdgeLineStateSet(_edgelineStateSet);
			oneFrameRender->setFaceLineStateSet(_facelineStateSet);
			oneFrameRender->setLineStateSet(_lineStateSet);
			oneFrameRender->setPointStateSet(_pointStateSet);
			oneFrameRender->setTextureCoordScale(scale);
			oneFrameRender->setDeformationScale(deformationScale*scale);
			_animationRender.insert(i + 1, oneFrameRender);
			futures.append(QtConcurrent::run([oneFrameRender]
			{
				oneFrameRender->updateAllModelOperate(ImportOperate);
			}));

		}
		while (!futures.empty())
		{
			futures.back().waitForFinished();
			//futures.back().result()->bufferThisFrame(_app->GLContext());
			futures.takeLast();
		}
		for (auto rend : _animationRender)
		{
			//QtConcurrent::run(rend.get(), &mPostOneFrameRender::bufferThisFrame,_app->GLContext());
			rend->bufferThisFrame(_app->GLContext());
		}
		//for (auto rend : _animationRender)
		//{
		//	rend->getOneFrameRendData()->deleteValueAndDisplacementData();
		//}
		*/

	}

	void mPostRender::deleteAnimation()
	{
		this->makeCurrent();
		setTimerOn(false);
		if (_oneFrameAnimationRender)
		{
			_oneFrameAnimationRender->deleteThieFrame();
			_oneFrameAnimationRender.reset();
		}
		for (auto render : _animationRender)
		{
			render->deleteThieFrame();
			render.reset();
		}
		_animationRender.clear();
	}

	void mPostRender::setTimerOn(bool ison)
	{
		mxr::time->start();
		_rendStatus->_aniIsStart = ison;
		if (ison)
		{
			int interval = 1000 / _rendStatus->_aniFrameRate;
			_aniTimer->start(interval);
			_rendStatus->_aniIsStart = true;
		}
		else
		{
			_rendStatus->_aniIsStart = false;
			_aniTimer->stop();
		}
	}
	
	void mPostRender::slot_aniTimer()
	{
		//qDebug() << __LINE__ << mxr::time->elapsed();
		//mxr::time->start();
		int interval = 1000 / _rendStatus->_aniFrameRate;
		_aniTimer->setInterval(interval);

		//判断是否到达最后一帧
		if (_rendStatus->_aniCurrentFrame >= _rendStatus->_aniEndFrame)
		{
			//判断是否循环播放
			if (_rendStatus->_aniLoopPlay)
			{
				//把当前帧设置为起始帧
				_rendStatus->_aniCurrentFrame = _rendStatus->_aniStartFrame;
			}
			else
			{
				//停止动画
				_rendStatus->_aniIsStart = false;
				_aniTimer->stop();
			}
		}
		else
		{
			//根据帧间隔运行
			_rendStatus->_aniCurrentFrame += _rendStatus->_aniFrameInterval;
		}
		
		if (_rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
		{
			//切换帧
			int nf = _rendStatus->_aniLinearCount;
			float scale = _rendStatus->_postMode == OneFrameLinearAnimation ? _rendStatus->_aniCurrentFrame / float(nf - 1) : sin(2 * M_PI * _rendStatus->_aniCurrentFrame / float(nf - 1));
			_oneFrameAnimationRender->setDeformationScale(scale * _rendStatus->_deformFactor);
			_oneFrameAnimationRender->setTextureCoordScale(scale);
		}
		else
		{
			//this->setAnimationFrameID(_frameGUIData->_aniCurrentFrame);
		}
		updateAnimationPanelSig(_rendStatus->_aniCurrentFrame);

		emit update();
	}

	void mPostRender::createVectorGraph(QVector<QPair<QString, QVector3D>> type_color, double size)
	{
		this->makeCurrent();
		_rendStatus->_vectorArrowTypeColor = type_color;
		_rendStatus->_vectorArrowSize = size;
		_rendStatus->_vectorArrowMethod = "";
		if (_oneFrameRender)
		{
			_oneFrameRender->createVectorGraph(type_color, size);
		}
	}

	void mPostRender::createVectorGraph(std::set<int> nodeIDs, QVector<QPair<QString, QVector3D>> type_color, double size)
	{
		this->makeCurrent();
		_rendStatus->_vectorArrowNodeIDs = nodeIDs;
		_rendStatus->_vectorArrowTypeColor = type_color;
		_rendStatus->_vectorArrowSize = size;
		_rendStatus->_vectorArrowMethod = "";
		if (_oneFrameRender)
		{
			_oneFrameRender->createVectorGraph(nodeIDs, type_color, size);
		}
	}

	void mPostRender::deleteVectorGraph()
	{
		this->makeCurrent();
		if (_oneFrameRender)
		{
			_oneFrameRender->deleteVectorGraph();
		}
	}

	void mPostRender::deleteContourGraph()
	{
		this->makeCurrent();
		if (_oneFrameRender != nullptr)
		{
			_oneFrameRender->deleteContourGraph();
		}
	}

	void mPostRender::deleteContourGraph(int i)
	{
		this->makeCurrent();
		if (_oneFrameRender != nullptr)
		{
			_oneFrameRender->deleteContourGraph(i);
		}
	}

	void mPostRender::createContourGraph()
	{
		this->makeCurrent();
		if (_oneFrameRender != nullptr)
		{
			_oneFrameRender->createContourGraph(_contourLineStateSet, _contourFaceStateSet);
		}
	}

	void mPostRender::createContourGraph(int i, double value, bool isshow)
	{
		this->makeCurrent();
		if (_oneFrameRender != nullptr)
		{
			_oneFrameRender->createContourGraph(_contourLineStateSet, _contourFaceStateSet, i, value, isshow);
		}
	}

	void mPostRender::setContourGraph(int i, bool isshow)
	{
		this->makeCurrent();
		if (_oneFrameRender != nullptr)
		{
			_oneFrameRender->setContourGraph(i, isshow);
		}
	}

	void mPostRender::setSphereData(QVector3D center, float radius)
	{
		_rendStatus->_sphereCenter = center;
		_rendStatus->_sphereRadius = radius;
		auto render = _dragRenders["积分球"];
		if (render)
		{
			auto sphereRender = dynamic_pointer_cast<mPostSphereRender>(render);
			if (sphereRender)
			{
				sphereRender->setSphereData(center, radius);
			}
		}
	}

	void mPostRender::deleteStreamLine()
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->deleteStreamLine();
		}
	}

	void mPostRender::setStreamLineShowForm(int streamLineShowForm)
	{
		_rendStatus->_streamLineStyle = streamLineShowForm;
		if (_oneFrameRender)
		{
			_oneFrameRender->setStreamLineShowForm(streamLineShowForm);
		}
	}

	void mPostRender::setIntergrateDirection(int intergrateDirection)
	{
		_rendStatus->_streamLineDirection = intergrateDirection;
		if (_oneFrameRender)
		{
			_oneFrameRender->setIntergrateDirection(intergrateDirection);
		}
		for (auto render : _animationRender)
		{
			render->setIntergrateDirection(intergrateDirection);
		}
	}

	void mPostRender::setIsShowSphere(bool isShow)
	{
		_rendStatus->_streamLineSphere = isShow;
		auto render = _dragRenders["积分球"];
		if (render)
		{
			auto sphereRender = dynamic_pointer_cast<mPostSphereRender>(render);
			if (sphereRender)
			{
				sphereRender->setIsShow(isShow);
			}
		}
	}

	QVector3D mPostRender::getDragSphereCenter()
	{
		return _rendStatus->_sphereCenter;
	}

	void mPostRender::createStreamLine(QVector3D center, float radius, int streamLineNum, float ratio)
	{
		if (_oneFrameRender)
		{
			_oneFrameRender->createStreamLine(_contourLineStateSet, _streamlinePointStateSet, center, radius, streamLineNum, ratio);
		}
		for (auto render : _animationRender)
		{
			render->createStreamLine(_contourLineStateSet, _streamlinePointStateSet, center, radius, streamLineNum, ratio);
		}
	}

	void mPostRender::setMinIsShow(bool isshow)
	{
		_rendStatus->_isShowMinLine = isshow;
		shared_ptr<mPostMinMaxRender> minRender = dynamic_pointer_cast<mPostMinMaxRender>(_dragRenders["最小值"]);
		if (isshow)
		{
			_baseRend->getFontRender()->appendCommonFont<int>("min", minRender->_vertexs, minRender->_ids);
			_baseRend->getFontRender()->appendCommonFont<float>("最小值", QVector<QVector3D>{minRender->_pos}, QVector<float>{_oneFrameRender->getOneFrameRendData()->getCurrentMinData()}, QVector3D(0,0,1));
			if (_oneFrameRender->getOneFrameRendData()->getNodeOrElement() == PostElement)
			{
				_tempHighLightRender->setAddMeshData(set<int>{minRender->_ids.begin(), minRender->_ids.end()});
			}
			else
			{
				_tempHighLightRender->setAddNodeData(set<int>{minRender->_ids.begin(), minRender->_ids.end()});
			}
		}
		else
		{
			if (_oneFrameRender->getOneFrameRendData()->getNodeOrElement() == PostElement)
			{
				_tempHighLightRender->setReduceMeshData(set<int>{minRender->_ids.begin(), minRender->_ids.end()});
			}
			else
			{
				_tempHighLightRender->setReduceNodeData(set<int>{minRender->_ids.begin(), minRender->_ids.end()});
			}
		}
		minRender->setData();
		_tempHighLightRender->updateHighLightRender(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());

		_dragRenders["最小值"]->setIsShow(isshow);
		_baseRend->getFontRender()->setCommonFontIsShow("min", isshow);
		_baseRend->getFontRender()->setCommonFontIsShow("最小值", isshow);
	}

	void mPostRender::setMaxIsShow(bool isshow)
	{
		_rendStatus->_isShowMaxLine = isshow;
		shared_ptr<mPostMinMaxRender> maxRender = dynamic_pointer_cast<mPostMinMaxRender>(_dragRenders["最大值"]);
		if (isshow)
		{
			_baseRend->getFontRender()->appendCommonFont<int>("max", maxRender->_vertexs, maxRender->_ids);
			_baseRend->getFontRender()->appendCommonFont<float>("最大值", QVector<QVector3D>{maxRender->_pos}, QVector<float>{_oneFrameRender->getOneFrameRendData()->getCurrentMaxData()}, QVector3D(1, 0, 0));
			if (_oneFrameRender->getOneFrameRendData()->getNodeOrElement() == PostElement)
			{
				_tempHighLightRender->setAddMeshData(set<int>{maxRender->_ids.begin(), maxRender->_ids.end()});
			}
			else
			{
				_tempHighLightRender->setAddNodeData(set<int>{maxRender->_ids.begin(), maxRender->_ids.end()});
			}
		}
		else
		{
			if (_oneFrameRender->getOneFrameRendData()->getNodeOrElement() == PostElement)
			{
				_tempHighLightRender->setReduceMeshData(set<int>{maxRender->_ids.begin(), maxRender->_ids.end()});
			}
			else
			{
				_tempHighLightRender->setReduceNodeData(set<int>{maxRender->_ids.begin(), maxRender->_ids.end()});
			}
		}
		maxRender->setData();
		_tempHighLightRender->updateHighLightRender(_oneFrameRender->getOneFrameData(), _oneFrameRender->getOneFrameRendData());

		_dragRenders["最大值"]->setIsShow(isshow);
		_baseRend->getFontRender()->setCommonFontIsShow("max", isshow);
		_baseRend->getFontRender()->setCommonFontIsShow("最大值", isshow);
	}

	void mPostRender::getMinMaxLocation()
	{
		if (!_oneFrameRender)
		{
			return;
		}
		shared_ptr<mPostMinMaxRender> minRender = dynamic_pointer_cast<mPostMinMaxRender>(_dragRenders["最小值"]);
		shared_ptr<mPostMinMaxRender> maxRender = dynamic_pointer_cast<mPostMinMaxRender>(_dragRenders["最大值"]);

		_tempHighLightRender->setAllMeshData();
		_tempHighLightRender->setAllNodeData();
		_oneFrameRender->getMinMaxIDs(minRender->_ids, maxRender->_ids);
		if (_oneFrameRender->getOneFrameRendData()->getNodeOrElement() == PostElement)
		{
			_oneFrameRender->getVertexs(PostElement, minRender->_ids, maxRender->_ids, minRender->_vertexs, maxRender->_vertexs);
		}
		else
		{
			_oneFrameRender->getVertexs(PostNode, minRender->_ids, maxRender->_ids, minRender->_vertexs, maxRender->_vertexs);
		}

		minRender->setData();
		maxRender->setData();
		setMinIsShow(_rendStatus->_isShowMinLine);
		setMaxIsShow(_rendStatus->_isShowMinLine);
	}

	void mPostRender::calculateMinMaxLinePosition()
	{
		Space::AABB aabb = this->getOneFrameRender()->getModelRender()->getModelAABB();
		QVector3D center = (aabb.maxEdge + aabb.minEdge) / 2.0;
		QVector3D vec = aabb.maxEdge - aabb.minEdge;

		shared_ptr<mPostMinMaxRender> minRender = dynamic_pointer_cast<mPostMinMaxRender>(_dragRenders["最小值"]);
		shared_ptr<mPostMinMaxRender> maxRender = dynamic_pointer_cast<mPostMinMaxRender>(_dragRenders["最大值"]);
		if (!aabb.ContainPoint(minRender->_pos))
		{
			minRender->_pos = center - 11.0* vec / 20.0;
		}
		if (!aabb.ContainPoint(maxRender->_pos))
		{
			maxRender->_pos = center + 11.0 * vec / 20.0;
		}
	}

	mPostRender::~mPostRender()
	{
		this->makeCurrent();
		_oneFrameRender.reset();
		_oneFrameAnimationRender.reset();
		_highLightRender.reset();
		_tempHighLightRender.reset();
		//_animationRender.reset();
		//delete _postFrameText;
	}

	QVector3D mPostRender::getCurrentNodeVertex(int id)
	{
		if (_oneFrameRender)
		{
			mPostOneFrameRendData* renddata = _oneFrameRender->getOneFrameRendData();
			if (renddata == nullptr)
			{
				return QVector3D();
			}
			QHash<int, QVector3D> dispdata = renddata->getNodeDisplacementData();
			QVector3D dfactor = renddata->getDeformationScale();
			mOneFrameData1* oneframe = _oneFrameRender->getOneFrameData();
			if (oneframe == nullptr)
			{
				return QVector3D();
			}
			mPostMeshNodeData1* nodedata = oneframe->getNodeDataByID(id);
			if (nodedata != nullptr)
			{
				QVector3D nodeCoord = nodedata->getNodeVertex();
				QVector3D nodeDisp = dispdata.value(id);
				QVector3D dfactor = renddata->getDeformationScale();
				return (nodeCoord + nodeDisp * dfactor);
			}
		}
		return QVector3D();
	}

	set<int> mPostRender::getMeshIDsByPartNames(MeshType meshType, set<QString> partNames)
	{
		set<int> resMeshIDs;
		if (_oneFrameRender != nullptr)
		{
			mOneFrameData1 *oneFrameData = _oneFrameRender->getOneFrameData();
			if (oneFrameData != nullptr)
			{
				if (meshType == MeshBeam)//一维
				{
					for (QString partName : partNames)
					{
						mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
						if (partData == nullptr)
						{
							continue;
						}
						set<int> meshIDs = partData->getMeshIDs1();
						for (int meshID : meshIDs)
						{
							mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
							if (meshData == nullptr)
							{
								continue;
							}
							if (!meshData->getMeshVisual())
							{
								continue;
							}
							if (_rendStatus->_pickElementTypeFilter.find(meshData->getElementType()) != _rendStatus->_pickElementTypeFilter.end())
							{
								resMeshIDs.insert(meshID);
							}
						}
					}
				}
				else if (meshType == MeshTri || meshType == MeshQuad)//二维
				{
					for (QString partName : partNames)
					{
						mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
						if (partData == nullptr)
						{
							continue;
						}
						set<int> meshIDs = partData->getMeshIDs2();
						for (int meshID : meshIDs)
						{
							mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
							if (meshData == nullptr)
							{
								continue;
							}
							if (!meshData->getMeshVisual())
							{
								continue;
							}
							if (_rendStatus->_pickElementTypeFilter.find(meshData->getElementType()) != _rendStatus->_pickElementTypeFilter.end())
							{
								resMeshIDs.insert(meshID);
							}
						}
					}
				}
				else
				{
					for (QString partName : partNames)
					{
						mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
						if (partData == nullptr)
						{
							continue;
						}
						set<int> meshIDs = partData->getMeshIDs3();
						for (int meshID : meshIDs)
						{
							mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
							if (meshData == nullptr)
							{
								continue;
							}
							if (!meshData->getMeshVisual())
							{
								continue;
							}
							if (_rendStatus->_pickElementTypeFilter.find(meshData->getElementType()) != _rendStatus->_pickElementTypeFilter.end())
							{
								resMeshIDs.insert(meshID);
							}
						}
					}
				}
			}
		}
		return resMeshIDs;
	}

	set<int> mPostRender::getAllMeshIDs(MeshType meshType)
	{
		set<int> resMeshIDs;
		if (_oneFrameRender)
		{
			mOneFrameData1 *oneFrameData = _oneFrameRender->getOneFrameData();

			if (oneFrameData != nullptr)
			{
				set<QString> partNames = oneFrameData->getAllPartNames();
				if (meshType == MeshBeam)//一维
				{
					for (QString partName : partNames)
					{
						mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
						if (partData == nullptr)
						{
							continue;
						}
						set<int> meshIDs = partData->getMeshIDs1();
						for (int meshID : meshIDs)
						{
							mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
							if (meshData == nullptr)
							{
								continue;
							}
							if (!meshData->getMeshVisual())
							{
								continue;
							}
							if (_rendStatus->_pickElementTypeFilter.find(meshData->getElementType()) != _rendStatus->_pickElementTypeFilter.end())
							{
								resMeshIDs.insert(meshID);
							}
						}
					}
				}
				else if (meshType == MeshTri || meshType == MeshQuad)//二维
				{
					for (QString partName : partNames)
					{
						mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
						if (partData == nullptr)
						{
							continue;
						}
						set<int> meshIDs = partData->getMeshIDs2();
						for (int meshID : meshIDs)
						{
							mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
							if (meshData == nullptr)
							{
								continue;
							}
							if (!meshData->getMeshVisual())
							{
								continue;
							}
							if (_rendStatus->_pickElementTypeFilter.find(meshData->getElementType()) != _rendStatus->_pickElementTypeFilter.end())
							{
								resMeshIDs.insert(meshID);
							}
						}
					}
				}
				else
				{
					for (QString partName : partNames)
					{
						mPostMeshPartData1 *partData = oneFrameData->getMeshPartDataByPartName(partName);
						if (partData == nullptr)
						{
							continue;
						}
						set<int> meshIDs = partData->getMeshIDs3();
						for (int meshID : meshIDs)
						{
							mPostMeshData1 *meshData = oneFrameData->getMeshDataByID(meshID);
							if (meshData == nullptr)
							{
								continue;
							}
							if (!meshData->getMeshVisual())
							{
								continue;
							}
							if (_rendStatus->_pickElementTypeFilter.find(meshData->getElementType()) != _rendStatus->_pickElementTypeFilter.end())
							{
								resMeshIDs.insert(meshID);
							}
						}
					}
				}
			}
		}
		return resMeshIDs;
	}

	QVector<QVector3D> mPostRender::getNodeVertexByMeshID(int meshID)
	{
		QVector<QVector3D> res;
		if (_oneFrameRender)
		{
			mPostOneFrameRendData* renddata = _oneFrameRender->getOneFrameRendData();
			if (renddata == nullptr)
			{
				return res;
			}
			QHash<int, QVector3D> dispdata = renddata->getNodeDisplacementData();
			QVector3D dfactor = renddata->getDeformationScale();
			mOneFrameData1* oneframe = _oneFrameRender->getOneFrameData();
			if (oneframe == nullptr)
			{
				return res;
			}
			//获取单元数据
			mPostMeshData1* meshdata = oneframe->getMeshDataByID(meshID);
			if (meshdata == nullptr)
			{
				return res;
			}
			//获取单元节点
			QVector<int> nodeIDs = meshdata->getNodeIndex();
			for (int nodeID : nodeIDs)
			{
				mPostMeshNodeData1* nodedata = oneframe->getNodeDataByID(nodeID);
				if (nodedata == nullptr)
				{
					continue;
				}
				res.append(nodedata->getNodeVertex() + dispdata.value(nodeID) * dfactor);
			}
		}
		return res;
	}

	QVector<QVector3D> mPostRender::getCuttingPlaneData(int cuttingPlaneIndex)
	{
		QVector<QVector3D> result;
		if (cuttingPlaneIndex >=  _rendStatus->_cuttingPlanes.size())
		{
			return result;
		}
		if (_oneFrameRender != nullptr)
		{
			result = _oneFrameRender->getCuttingPlaneData(cuttingPlaneIndex);
		}
		return result;
	}

	QVector3D mPostRender::getCuttingPlaneNormal(int cuttingPlaneIndex)
	{
		QVector3D result;
		if (cuttingPlaneIndex >= _rendStatus->_cuttingPlanes.size())
		{
			return result;
		}
		result = _rendStatus->_cuttingPlanes.at(cuttingPlaneIndex).toVector3D();

		return result;
	}

	void mPostRender::updateCuttingPlaneUniform()
	{
		int cuttingPlaneSize = _rendStatus->_cuttingPlanes.size();
		for (int i = 0; i < 7; i++)
		{
			string str = QString("planes[%1]").arg(i).toStdString();
			if (i < cuttingPlaneSize)
			{
				_cuttingPlaneStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_cuttingPlaneStateSet->getUniform(str)->SetEnable(true);
				_faceStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_faceStateSet->getUniform(str)->SetEnable(true);
				_faceTransparentStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_faceTransparentStateSet->getUniform(str)->SetEnable(true);
				_faceTransparentNodeformationStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_faceTransparentNodeformationStateSet->getUniform(str)->SetEnable(true);
				_edgelineStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_edgelineStateSet->getUniform(str)->SetEnable(true);
				_facelineStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_facelineStateSet->getUniform(str)->SetEnable(true);
				_lineStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_lineStateSet->getUniform(str)->SetEnable(true);
				_pointStateSet->getUniform(str)->SetData(_rendStatus->_cuttingPlanes[i]);
				_pointStateSet->getUniform(str)->SetEnable(true);
				_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_faceStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_faceTransparentStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_edgelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_facelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_lineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
				_pointStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 1);
			}
			else
			{
				_cuttingPlaneStateSet->getUniform(str)->SetEnable(false);
				_cuttingPlaneStateSet->getUniform(str)->SetData(QVector4D());
				_faceStateSet->getUniform(str)->SetEnable(false);
				_faceStateSet->getUniform(str)->SetData(QVector4D());
				_faceTransparentStateSet->getUniform(str)->SetEnable(false);
				_faceTransparentStateSet->getUniform(str)->SetData(QVector4D());
				_faceTransparentNodeformationStateSet->getUniform(str)->SetEnable(false);
				_faceTransparentNodeformationStateSet->getUniform(str)->SetData(QVector4D());
				_edgelineStateSet->getUniform(str)->SetEnable(false);
				_edgelineStateSet->getUniform(str)->SetData(QVector4D());
				_facelineStateSet->getUniform(str)->SetEnable(false);
				_facelineStateSet->getUniform(str)->SetData(QVector4D());
				_lineStateSet->getUniform(str)->SetEnable(false);
				_lineStateSet->getUniform(str)->SetData(QVector4D());
				_pointStateSet->getUniform(str)->SetEnable(false);
				_pointStateSet->getUniform(str)->SetData(QVector4D());
				_cuttingPlaneStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_faceStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_faceTransparentStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_faceTransparentNodeformationStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_edgelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_facelineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_lineStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
				_pointStateSet->setAttributeAndModes(MakeAsset<ClipDistance>(i), 0);
			}
		}
	}

	void mPostRender::initialPickThreads()
	{
		if (!_dataPost)
		{
			return;
		}

		//初始化部件拾取多线程
		set<QString> partNames = _dataPost->getAllPostPartNames();
		_thread = new mPostMeshPickThread(_pickData);
		_thread->setPickFilter(_baseRend->getPickFilter());
		for (QString partName : partNames)
		{
			_thread->appendPartSpaceTree(partName, _oneFrameRender->getModelRender()->getPartSpaceTree(partName));
		}	
	}

	void mPostRender::slot_finsishedDrag()
	{
		if (_currentDragRender != nullptr)
		{
			if (_currentDragRender->getName() == "最小值" || _currentDragRender->getName() == "最大值")
			{
				float txt = _currentDragRender->getName() == "最小值" ? _oneFrameRender->getOneFrameRendData()->getCurrentMinData() : _oneFrameRender->getOneFrameRendData()->getCurrentMaxData();
				QVector3D color = _currentDragRender->getName() == "最小值" ? QVector3D(0, 0, 1) : QVector3D(1, 0, 0);
				shared_ptr<mPostMinMaxRender> render = dynamic_pointer_cast<mPostMinMaxRender>(_currentDragRender);
				if (render)
				{
					//render->setData();
					//更新文字
					_baseRend->getFontRender()->appendCommonFont<float>(_currentDragRender->getName(), QVector<QVector3D>{render->_pos}, QVector<float>{txt}, color);
				}
			}
		}
	}

	void mPostRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		if (_faceStateSet)
		{
			_faceStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceStateSet->getUniform("view")->SetData(modelView->_view);
			_faceStateSet->getUniform("model")->SetData(modelView->_model);
			_faceTransparentStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceTransparentStateSet->getUniform("view")->SetData(modelView->_view);
			_faceTransparentStateSet->getUniform("model")->SetData(modelView->_model);
			_faceTransparentNodeformationStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceTransparentNodeformationStateSet->getUniform("view")->SetData(modelView->_view);
			_faceTransparentNodeformationStateSet->getUniform("model")->SetData(modelView->_model);
			_edgelineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_edgelineStateSet->getUniform("view")->SetData(modelView->_view);
			_edgelineStateSet->getUniform("model")->SetData(modelView->_model);
			_facelineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_facelineStateSet->getUniform("view")->SetData(modelView->_view);
			_facelineStateSet->getUniform("model")->SetData(modelView->_model);
			_lineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_lineStateSet->getUniform("view")->SetData(modelView->_view);
			_lineStateSet->getUniform("model")->SetData(modelView->_model);
			_pointStateSet->getUniform("projection")->SetData(modelView->_projection);
			_pointStateSet->getUniform("view")->SetData(modelView->_view);
			_pointStateSet->getUniform("model")->SetData(modelView->_model);
			_cuttingPlaneStateSet->getUniform("projection")->SetData(modelView->_projection);
			_cuttingPlaneStateSet->getUniform("view")->SetData(modelView->_view);
			_cuttingPlaneStateSet->getUniform("model")->SetData(modelView->_model);
			_transparentPlaneStateSet->getUniform("projection")->SetData(modelView->_projection);
			_transparentPlaneStateSet->getUniform("view")->SetData(modelView->_view);
			_transparentPlaneStateSet->getUniform("model")->SetData(modelView->_model);
			_contourLineStateSet->getUniform("projection")->SetData(modelView->_projection);
			_contourLineStateSet->getUniform("view")->SetData(modelView->_view);
			_contourLineStateSet->getUniform("model")->SetData(modelView->_model);
			_contourFaceStateSet->getUniform("projection")->SetData(modelView->_projection);
			_contourFaceStateSet->getUniform("view")->SetData(modelView->_view);
			_contourFaceStateSet->getUniform("model")->SetData(modelView->_model);
			_streamlinePointStateSet->getUniform("projection")->SetData(modelView->_projection);
			_streamlinePointStateSet->getUniform("view")->SetData(modelView->_view);
			_streamlinePointStateSet->getUniform("model")->SetData(modelView->_model);

			_faceStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_faceTransparentStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_pointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_cuttingPlaneStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_contourFaceStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_streamlinePointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);

			if (_rendStatus->_lightIsDependOnCamera)
			{
				_faceStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_faceTransparentStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_faceTransparentNodeformationStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_pointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_cuttingPlaneStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_contourFaceStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_streamlinePointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
			}

			_facelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
			_edgelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
		}
		if (_rendStatus->_postMode == OneFrame)
		{
			if (_oneFrameRender)
			{
				_oneFrameRender->updateUniform(modelView, commonView);
			}
		}
		else if (_rendStatus->_postMode == OneFrameLinearAnimation || _rendStatus->_postMode == OneFrameSinAnimation)
		{
			if (_oneFrameAnimationRender)
			{
				_oneFrameAnimationRender->updateUniform(modelView, commonView);
			}
		}
		else if (!_animationRender.empty())
		{
			_animationRender.value(_rendStatus->_aniCurrentFrame)->updateUniform(modelView, commonView);
		}

		for (auto render : _dragRenders)
		{
			render->updateUniform(modelView);
		}

		_highLightRender->updateUniform(modelView, commonView);
		_tempHighLightRender->updateUniform(modelView, commonView);
	}
	void mPostRender::resizeWindow(int w, int h)
	{
		//自适应文字大小
		if (h > 600)
		{
			_rendStatus->_postColorTableFontSize = 1.0;
		}
		else if (h < 600 && h > 350)
		{
			_rendStatus->_postColorTableFontSize = h / 1000.0f + 0.4;
		}
		else
		{
			_rendStatus->_postColorTableFontSize = 0.75;
		}

		//自适应颜色表的位置
		_rendStatus->_postColorTableRatio = 0.05f;//一行的高度比例
		if (h < 600 && h > 350)
		{
			_rendStatus->_postColorTableRatio = -h / 5000.0f + 0.17;
		}
		else if (h <= 350)
		{
			_rendStatus->_postColorTableRatio = 0.1*350.0 / h;
		}
		if (_postFrameText)
		{
			_postFrameText->resizeWindow(w, h);
		}
		if (_oneFrameRender)
		{
			_oneFrameRender->resizeWindow(w, h);
		}

	}
}