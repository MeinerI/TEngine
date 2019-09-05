#include "StackAllocator.h"
#include <stdlib.h>
#include <iostream>
#include <string>

namespace TEngine
{
	StackAllocator::StackAllocator(const size stackSizeInBytes)
		: stackSizeInBytes(stackSizeInBytes)
	{
		topRawPtr = malloc(stackSizeInBytes);
		topPtr = reinterpret_cast<uintptr>(topRawPtr);
		std::cout << std::to_string(topPtr) << std::endl;
	}

	StackAllocator::~StackAllocator()
	{
		free(topRawPtr);
	}

	void* StackAllocator::Alloc(const size sizeInBytes, const uint8 alignment)
	{
		if ((alignment & (alignment - 1)) != 0)
			throw std::invalid_argument("Alignment was not a power of 2");

		uintptr testAddress = topPtr + currentMarker;
		uintptr misalign = testAddress & (alignment - 1); // check for 1's to right of power of 2
		ptrdiff correction = alignment - misalign;

		correction = correction & (alignment - 1); // stops skipping block if misalign = 0

		size newMarker = currentMarker + correction + sizeInBytes;

		if (newMarker > topPtr + stackSizeInBytes)
			throw std::overflow_error("StackAllocator::Alloc not enough memory to allocate");

		currentMarker = newMarker;
		
		uintptr alignedAddress = testAddress + correction;
		return reinterpret_cast<void*>(alignedAddress);
	}

	void StackAllocator::FreeUpTo(const size marker)
	{
		currentMarker = marker;
	}

	void StackAllocator::Clear()
	{
		currentMarker = 0;
	}
}
