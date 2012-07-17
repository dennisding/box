#include "vs.h"

struct vs_in
{
	float4 pos : POSITON;
};

struct vs_out
{
	float4 pos : POSITION;
};

vs_out main( vs_in info )
{
	return (vs_out)0;
}
