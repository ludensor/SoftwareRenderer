#include "Buffer.h"

#include <malloc.h>
#include <memory.h>

sr::Buffer::Buffer(const void* data, uint32_t byte_width)
	: byte_width_(byte_width)
{
	data_ = malloc(byte_width_);
	if (data_)
	{
		memcpy(data_, data, byte_width);
	}
}

sr::Buffer::~Buffer()
{
	free(data_);
}

void* sr::Buffer::GetData() const
{
	return data_;
}

uint32_t sr::Buffer::GetSize() const
{
	return byte_width_;
}

