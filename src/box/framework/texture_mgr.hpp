#ifndef _TEXTURE_MGR_HPP_
#define _TEXTURE_MGR_HPP_

#include "utils/smart_ptr.hpp"

#include "d3d11.h"
#include <string>

class Texture : public Object
{
public:
	Texture( ID3D11Texture2D *texture );
	~Texture();
private:
	ID3D11Texture2D *texture_;
};

typedef SmartPtr< Texture > TexturePtr;

class TextureMgr
{
public:
	static TexturePtr create( int width, int height );
	static TexturePtr get( const std::string &name );
};

// inline
inline Texture::Texture( ID3D11Texture2D *texture ) : texture_( texture )
{

}

inline Texture::~Texture()
{
	texture_->Release();
}

#endif
