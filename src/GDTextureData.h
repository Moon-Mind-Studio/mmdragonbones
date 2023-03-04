#ifndef GDTEXTUREDATA_H
#define GDTEXTUREDATA_H

#include <memory>
#include "dragonBones/DragonBonesHeaders.h"
#include "dragonBones/model/TextureAtlasData.h"
#include <gdextension_interface.h>
#include <godot_cpp/classes/ref.hpp>

DRAGONBONES_USING_NAME_SPACE;

class GDTextureData : public TextureData
{
	BIND_CLASS_TYPE_B(GDTextureData);

public:	
    Rect2i				textureRect;

public:
	GDTextureData()
	{
		_onClear();
	}

	virtual ~GDTextureData()
	{
		_onClear();
	}

	void _onClear() override
	{
		TextureData::_onClear();
	}
};

#endif // GDTEXTUREDATA_H
                                
