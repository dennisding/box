#ifndef _VISIBLE_ITEM_HPP_
#define _VISIBLE_ITEM_HPP_

#include "primitive/index_buffer_mgr.hpp"
#include "primitive/vertex_buffer_mgr.hpp"
#include "primitive/vertex_shader_mgr.hpp"

#include "utils/smart_ptr.hpp"

struct VisualItem : public Object
{
public:
	bool has_alpha_;
	IndexBufferPtr index_buffer_;
	VertexBufferPtr vertext_buffer_;
	VertexShaderPtr vertex_shader_;
};

typedef SmartPtr< VisualItem > VisibleItemPtr;

#endif
