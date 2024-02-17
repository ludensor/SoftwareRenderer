#include "Resource.h"

#include <malloc.h>

sr::Buffer::Buffer(uint32_t byte_width)
	: byte_width_(byte_width)
{
	data_ = malloc(byte_width_);
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

sr::Texture2D::Texture2D(uint32_t width, uint32_t height)
	: width_(width)
	, height_(height)
	, pitch_(width * 4)
{
	data_ = malloc(pitch_ * height_);
}

sr::Texture2D::~Texture2D()
{
	free(data_);
}

void* sr::Texture2D::GetData() const
{
	return data_;
}

uint32_t sr::Texture2D::GetPitch() const
{
	return pitch_;
}

uint32_t sr::Texture2D::GetHeight() const
{
	return height_;
}

uint32_t sr::Texture2D::GetSize() const
{
	return pitch_ * height_;
}

