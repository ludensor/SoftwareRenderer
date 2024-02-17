#pragma once

#include <stdint.h>

namespace sr
{
	class Buffer
	{
	public:
		Buffer(uint32_t byte_width);
		~Buffer();

		void* GetData() const;
		uint32_t GetSize() const;

	private:
		void* data_;
		uint32_t byte_width_;
	};

	class Texture2D
	{
	public:
		Texture2D(uint32_t width, uint32_t height);
		~Texture2D();

		void* GetData() const;
		uint32_t GetPitch() const;
		uint32_t GetHeight() const;
		uint32_t GetSize() const;

	private:
		void* data_;
		uint32_t width_;
		uint32_t height_;
		uint32_t pitch_;
	};
}
