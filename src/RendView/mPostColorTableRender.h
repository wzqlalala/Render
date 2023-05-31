#pragma once
#pragma execution_character_set("utf-8")

#include <memory>

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
}
namespace MDataPost
{
	class mPostColorTableData;
}
namespace MBaseRend
{
	class mFontRender;
}
using namespace MDataPost;
namespace MPostRend
{
	class mPostColorTableRender
	{

	public:
		mPostColorTableRender(std::shared_ptr<MBaseRend::mFontRender>, mPostColorTableData *colorTableData, std::shared_ptr<mxr::Group> parent, mxr::Texture *texture);

		~mPostColorTableRender();

		void updatePostColorTable(float textureCoordRatio);

		void updateText(mPostColorTableData *colorTableData);
	protected:

		std::shared_ptr<MBaseRend::mFontRender> _fontRender;//Œƒ◊÷‰÷»æ

		std::shared_ptr<mxr::Group> _parent;
		std::shared_ptr<mxr::Geode> _geode;

		std::shared_ptr<mxr::StateSet> _colorTableState;
		std::shared_ptr<mxr::StateSet> _colorTableLineState;

		std::shared_ptr<mxr::Drawable> _colorTableDrawable;
		std::shared_ptr<mxr::Drawable> _colorTableLineDrawable;
	};
}

