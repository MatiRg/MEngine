#include "Renderable3D.hpp"
#include "../System/MemoryManager.hpp"

IRenderable3D::IRenderable3D(ERenderableType x):
	Type(x)
{
	ADD_MEMORY_RECORD(this);
}
	
IRenderable3D::~IRenderable3D()
{
	ERASE_MEMORY_RECORD(this);
}