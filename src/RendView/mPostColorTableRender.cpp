#include "mPostColorTableRender.h"
#include "mFontRender.h"

#include "mPostColorTableData.h"

#include <renderpch.h>
#include "mShaderManage.h"

using namespace mxr;
namespace MPostRend
{
	mPostColorTableRender::mPostColorTableRender(std::shared_ptr<MBaseRend::mFontRender> fontRender, mPostColorTableData * colorTableData, std::shared_ptr<mxr::Group> parent, mxr::Texture *texture)
	{
		_fontRender = fontRender;
		_parent = parent;
		_geode = MakeAsset<mxr::Geode>();
		_parent->addChild(_geode);

		Shader *ShaderTable = mShaderManage::GetInstance()->GetShader("PostTable");
		Shader *ShaderLine = mShaderManage::GetInstance()->GetShader("PostTableLine");
		_colorTableDrawable = MakeAsset<mxr::Drawable>();
		_colorTableLineDrawable = MakeAsset<mxr::Drawable>();
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

		_colorTableDrawable->setVertexAttribArray(0, MakeAsset<Vec2Array>(colorTableData->getColorTableVertex()));
		_colorTableDrawable->setVertexAttribArray(1, MakeAsset<FloatArray>(colorTableData->getColorTableTexture()));
		_colorTableDrawable->setStateSet(_colorTableState);

		_colorTableLineDrawable->setVertexAttribArray(0, MakeAsset<Vec2Array>(colorTableData->getColorTableLineMarkVertex()));
		_colorTableLineDrawable->setStateSet(_colorTableLineState);

		_geode->addChild(_colorTableDrawable);
		_geode->addChild(_colorTableLineDrawable);

		updateText(colorTableData);
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
	void mPostColorTableRender::updateText(mPostColorTableData *colorTableData)
	{
		QVector<MViewBasic::FontText> fontTexts = colorTableData->getColorTableNumVertex();
		QVector<QVector2D> pos; QVector<QString> value;
		for (auto fontText : fontTexts)
		{
			pos.append(fontText.pos);
			value.append(fontText.value);
		}
		_fontRender->appendFixedFont("colorTable", pos, value, QVector3D(0,0,0));
	}
}