#pragma once
#include "../core/rendbase_global.h"
namespace mxr
{
	class RENDBASE_EXPORT Callback
	{

	public:
		Callback() {}
		virtual Callback* asCallback() { return this; }
		virtual const Callback* asCallback() const { return this; }
	protected:
		virtual ~Callback() {}

	};
}
