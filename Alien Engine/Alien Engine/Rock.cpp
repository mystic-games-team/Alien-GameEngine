#include "Rock.h"
#include "Primitive.h"
#include "Shapes.h"

Rock::Rock( ) :Primitive()
{
	shape = par_shapes_create_rock(seed, subdivisions);
	SetPosition( 0, 0, 0);
	MemCpy();
	InitBuffers();
}

Rock::~Rock()
{
}
