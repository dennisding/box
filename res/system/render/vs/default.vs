#include "vs.h"

struct vs_in
{
	float4 pos : POSITION;
};

struct vs_out
{
	float4 pos : POSITION;
};

float4x4 world_view;

vs_out main( vs_in info )
{
	vs_out result;
	result.pos = mul( world_view, info.pos );
	return result;
}
