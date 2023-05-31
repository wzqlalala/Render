#include "mPostColorTableRender.h"
#include "mFontRender.h"
#include "mPostRendStatus.h"

#include "mPostColorTableData.h"
#include "mPostOneFrameRendData.h"

#include <renderpch.h>
#include "mShaderManage.h"

using namespace mxr;
namespace MPostRend
{
	mPostColorTableRender::mPostColorTableRender(std::shared_ptr<MBaseRend::mFontRender> fontRender, mPostOneFrameRendData * rendData, std::shared_ptr<mPostRendStatus> rendStatus, std::shared_ptr<mxr::Group> parent, mxr::Texture *texture)
	{
		_fontRender = fontRender;
		_rendStatus = rendStatus;
		_parent = parent;
		_geode = MakeAsset<mxr::Geode>();
		_parent->addChild(_geode);
		_colorTableDrawable = nullptr;
		_colorTableLineDrawable = nullptr;

		Shader *ShaderTable = mShaderManage::GetInstance()->GetShader("PostTable");
		Shader *ShaderLine = mShaderManage::GetInstance()->GetShader("PostTableLine");

		_colorTableState = MakeAsset<StateSet>();
		_colorTableLineState = MakeAsset<StateSet>();

		_colorTableState->setShader(ShaderTable);
		_colorTableLineState->setShader(ShaderLine);

		_colorTableLineState->setDrawMode(GL_LINES);
		_colorTableState->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_colorTableState->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_colorTableState->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);
		_colorTableState->setUniform(MakeAsset<Uniform>("textureCoordRatio", float(1)));
		_colorTableState->setTexture("Texture", texture);

		_colorTableLineState->setAttributeAndModes(MakeAsset<PolygonMode>(), 0);
		_colorTableLineState->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_colorTableLineState->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		updateText(rendData);
	}
	mPostColorTableRender::~mPostColorTableRender()
	{
		if (_geode)
		{
			_geode->removeAllChild();
			_parent->removeChild(_geode);
		}
	}
	void mPostColorTableRender::updatePostColorTable(float textureCoordRatio)
	{		
		_colorTableState->setUniform(MakeAsset<Uniform>("textureCoordRatio", textureCoordRatio));
	}
	void mPostColorTableRender::updateText(mPostOneFrameRendData *rendData)
	{
		mPostColorTableData *colorTableData = rendData->getRendColorTable();
		colorTableData->setPostColorRowRatio(_rendStatus->_postColorTableRatio);
		colorTableData->setPostColorTableNum(_rendStatus->_cloudDiscreteNum);

		if (_colorTableDrawable)
		{
			_geode->removeChild(_colorTableDrawable);
			_geode->removeChild(_colorTableLineDrawable);
		}

		_colorTableDrawable = MakeAsset<mxr::Drawable>();
		_colorTableLineDrawable = MakeAsset<mxr::Drawable>();
		_colorTableDrawable->setVertexAttribArray(0, MakeAsset<Vec2Array>(colorTableData->getColorTableVertex()));
		_colorTableDrawable->setVertexAttribArray(1, MakeAsset<FloatArray>(colorTableData->getColorTableTexture()));
		_colorTableDrawable->setStateSet(_colorTableState);

		_colorTableLineDrawable->setVertexAttribArray(0, MakeAsset<Vec2Array>(colorTableData->getColorTableLineMarkVertex()));
		_colorTableLineDrawable->setStateSet(_colorTableLineState);
		_geode->addChild(_colorTableDrawable);
		_geode->addChild(_colorTableLineDrawable);


		QVector<MViewBasic::FontText> fontTexts = colorTableData->getColorTableNumVertex();
		QVector<QVector2D> pos; QVector<QString> value;
		for (auto fontText : fontTexts)
		{
			pos.append(fontText.pos);
			value.append(fontText.value);
		}
		pos.append(colorTableData->getColorTableMaxVertex());
		value.append(QString("max:%1").arg(rendData->getCurrentMaxData()));
		pos.append(colorTableData->getColorTableMinVertex());
		value.append(QString("min:%1").arg(rendData->getCurrentMinData()));
		pos.append(colorTableData->getColorTableTitleVertex());
		value.append(QString("%1").arg(rendData->getRendTitle()));
		pos.append(colorTableData->getColorTableMXVertex());
		value.append(QString("MxSim"));
		_fontRender->appendFixedFont("colorTable", pos, value, QVector3D(0,0,0), _rendStatus->_postColorTableFontSize);
	}
	void mPostColorTableRender::resizeWindow(mPostOneFrameRendData *rendData)
	{
		updateText(rendData);
	}
}