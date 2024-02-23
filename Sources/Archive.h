#pragma once

#include <memory>

namespace sr
{
	class Archive
	{
	public:
		typedef int8_t Element;

		Archive();
		Archive(const Archive&) = delete;
		~Archive();

		Archive& operator=(const Archive&) = delete;

		Archive& operator>>(bool& value);
		Archive& operator>>(int8_t& value);
		Archive& operator>>(int16_t& value);
		Archive& operator>>(int32_t& value);
		Archive& operator>>(int64_t& value);
		Archive& operator>>(uint8_t& value);
		Archive& operator>>(uint16_t& value);
		Archive& operator>>(uint32_t& value);
		Archive& operator>>(uint64_t& value);
		Archive& operator>>(float& value);
		Archive& operator>>(double& value);
		Archive& operator>>(void*& value);

		Archive& operator<<(bool value);
		Archive& operator<<(int8_t value);
		Archive& operator<<(int16_t value);
		Archive& operator<<(int32_t value);
		Archive& operator<<(int64_t value);
		Archive& operator<<(uint8_t value);
		Archive& operator<<(uint16_t value);
		Archive& operator<<(uint32_t value);
		Archive& operator<<(uint64_t value);
		Archive& operator<<(float value);
		Archive& operator<<(double value);
		Archive& operator<<(void* value);

		int32_t Read(void* v, int32_t size);
		int32_t Write(const void* v, int32_t size);

		bool IsEnd() const;

	private:
		bool Overflow();

	private:
		static constexpr uint32_t MIN_SIZE = 32;

		std::allocator<Element> allocator_;
		Element* buffer_;
		int32_t buffer_count_;
		int32_t read_pos_;
		int32_t write_pos_;
	};
}

inline sr::Archive::Archive()
	: buffer_(nullptr)
	, buffer_count_(0)
	, read_pos_(0)
	, write_pos_(0)
{

}

inline sr::Archive::~Archive()
{
	if (buffer_)
	{
		allocator_.deallocate(buffer_, buffer_count_);
	}
}

inline sr::Archive& sr::Archive::operator>>(bool& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(int8_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(int16_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(int32_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(int64_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(uint8_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(uint16_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(uint32_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(uint64_t& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(float& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(double& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator>>(void*& value)
{
	Read(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(bool value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(int8_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(int16_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(int32_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(int64_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(uint8_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(uint16_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(uint32_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(uint64_t value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(float value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(double value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline sr::Archive& sr::Archive::operator<<(void* value)
{
	Write(&value, sizeof(value));
	return *this;
}

inline int32_t sr::Archive::Read(void* v, int32_t size)
{
	const int32_t start_size = size;
	while (size > 0)
	{
		int32_t count = write_pos_ - read_pos_;
		if (0 < count)
		{
			if (size < count)
			{
				count = size;
			}

			memcpy(v, &buffer_[read_pos_], count);
			v = (Element*)v + count;
			size -= count;
			read_pos_ += count;
		}
		else
		{
			break;
		}
	}

	return start_size - size;
}

inline int32_t sr::Archive::Write(const void* v, int32_t size)
{
	const int32_t start_size = size;
	while (size > 0)
	{
		int32_t count = buffer_count_ - write_pos_;
		if (0 < count)
		{
			if (size < count)
			{
				count = size;
			}

			memcpy(&buffer_[write_pos_], v, count);
			v = (Element*)v + count;
			size -= count;
			write_pos_ += count;
		}
		else if (!Overflow())
		{
			break;
		}
	}

	return start_size - size;
}

inline bool sr::Archive::IsEnd() const
{
	return read_pos_ == write_pos_;
}

inline bool sr::Archive::Overflow()
{
	const int32_t old_size = buffer_count_;
	int32_t new_size;
	if (old_size < MIN_SIZE)
	{
		new_size = MIN_SIZE;
	}
	else if (old_size < INT_MAX / 2)
	{
		new_size = old_size << 1;
	}
	else if (old_size < INT_MAX)
	{
		new_size = INT_MAX;
	}
	else
	{
		return false;
	}

	Element* old_buffer = buffer_;
	Element* new_buffer = allocator_.allocate(new_size);

	memcpy(new_buffer, old_buffer, old_size);
	buffer_count_ = new_size;
	buffer_ = new_buffer;

	if (old_buffer)
	{
		allocator_.deallocate(old_buffer, old_size);
	}

	return true;
}
