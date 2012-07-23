#include "input_layout_mgr.hpp"
#include "device.hpp"

#include "utils/log.hpp"

#include <map>

typedef ID3D11InputLayout *(*InputLayoutCreator)( ID3DBlob *byte_code );
typedef std::map< std::string, InputLayoutPtr > InputLayoutMap;
typedef std::map< std::string, InputLayoutCreator > InputLayoutCreatorMap;

static InputLayoutMap input_layouts_;
static InputLayoutCreatorMap input_layout_creators_;

static ID3D11InputLayout *create_default_layout( ID3DBlob *byte_code )
{
	static D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int num = sizeof( desc )/ sizeof( D3D11_INPUT_ELEMENT_DESC);
	ID3D11InputLayout *layout = 0;
	HRESULT result = Device::device_->CreateInputLayout( desc, num, byte_code->GetBufferPointer(), byte_code->GetBufferSize(), &layout);
	if ( result != S_OK ) {
		return 0;
	}
	
	return layout;
}

void InputLayoutMgr::init()
{
	input_layout_creators_[ "default" ] = create_default_layout;
}

void InputLayoutMgr::fini()
{
	input_layouts_.clear();
	input_layout_creators_.clear();
}

InputLayoutPtr InputLayoutMgr::get( const std::string &name )
{
	InputLayoutMap::iterator it = input_layouts_.find( name );
	if ( it != input_layouts_.end() ) {
		return it->second;
	}

	return 0;
}

InputLayoutPtr InputLayoutMgr::create( ID3DBlob *byte_code, const std::string &name )
{
	InputLayoutCreatorMap::iterator it = input_layout_creators_.find( name );
	if ( it == input_layout_creators_.end() ) {
		log_error( "[InputLayoutMgr::create] invalid input layout creator '%s'", name.c_str() );
		return 0;
	}

	ID3D11InputLayout *origin_layout = it->second( byte_code );
	if ( !origin_layout ) {
		log_error( "[InputLayoutMgr::create] mismatch input layout '%s'", name.c_str() );
		return 0;
	}

	InputLayoutPtr layout = new InputLayout( origin_layout );
	input_layouts_.insert( std::make_pair( name, layout ) );
	
	return layout;
}