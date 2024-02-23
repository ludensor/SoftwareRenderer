#pragma once

#include <stdint.h>

namespace sr
{
	class Texture2D
	{
	public:
		Texture2D(const void* data, uint32_t width, uint32_t height);
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
