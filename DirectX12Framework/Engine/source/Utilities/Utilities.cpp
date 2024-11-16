#include "pch.h"
#include "Utilities.h"


namespace Engine
{
	namespace Utils
	{
		UINT CalculateConstantbufferAlignment(const UINT allocation)
		{
			return (allocation + 255) & ~255; //this will wrap the allocation in multiples of 255 bytes (~255 is a bit shift)
		}
	}
}
