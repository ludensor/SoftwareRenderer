#pragma once

#include <stdint.h>

namespace sr
{
	class Buffer
	{
	public:
		Buffer(const void* data, uint32_t byte_width);
		~Buffer();

		void* GetData() const;
		uint32_t GetSize() const;

	private:
		void* data_;
		uint32_t byte_width_;
	};
}
