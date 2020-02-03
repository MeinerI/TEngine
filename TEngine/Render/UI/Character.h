#pragma once
#include "../../Core/PortableTypes.h"
#include "../../Core/Math/Vector2Int.h"
#ifdef DIRECTX
#include "../D3D11/Bindables/DXTexture.h"
#endif

namespace TEngine
{
	struct Character
	{
#ifdef DIRECTX
		DXTexture* dxTexture;
#elif defined(OPENGL)
		uint32 textureID;
#endif
		//Texture* texture;
		uint32 advance;
		Vector2Int bearing;
		Vector2Int size;
	};
}
