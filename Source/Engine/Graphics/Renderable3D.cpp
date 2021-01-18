#include "Renderable3D.hpp"
#include "../System/MemoryManager.hpp"

CRenderable3D::CRenderable3D(ERenderableType x):
	Type(x)
{
	ADD_MEMORY_RECORD(this);
}
	
CRenderable3D::~CRenderable3D()
{
	ERASE_MEMORY_RECORD(this);
}