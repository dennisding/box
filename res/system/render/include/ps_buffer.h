

cbuffer per_frame
{
	matrix4x4 view_matrix : packoffset( c0 );
	matrix4x4 project_matrix : packoffset( c4 );
}

cbuffer per_instance
{

}