#pragma once

#include<cstdint>
#include<algorithm>

class OutputMemoryStream
{
public:
	OutputMemoryStream()
	{
		ReallocBuffer(65000);
		mHead = 0;
	}

	~OutputMemoryStream()
	{
		std::free(mBuffer);
	}

	const char* GetBufferPtr() const
	{
		return mBuffer;
	}

	uint32_t GetLength() const
	{
		return mHead;
	}


	void Write(const void* inData, size_t inByteCount)
	{
		uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);

		if (resultHead > mCapacity)
		{
			ReallocBuffer(65000);
		}

		std::memcpy(mBuffer + mHead, inData, inByteCount);
		mHead = resultHead;
	}

	template<typename T> void Write(T inData)
	{
		Write(&inData, sizeof(inData));
	}

private:
	void ReallocBuffer(uint32_t inNewLength)
	{
		mBuffer = static_cast<char*>(std::realloc(mBuffer, inNewLength));
		mCapacity = inNewLength;
	}

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};


class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount)
	{
		mBuffer = inBuffer;
		mCapacity = inByteCount;
		mHead = 0;
	}

	~InputMemoryStream()
	{
		std::free(mBuffer);
	}

	void Read(void* outData, uint32_t inByteCount)
	{
		uint32_t resultHead = mHead + inByteCount;

		std::memcpy(outData, mBuffer + mHead, inByteCount);

		mHead = resultHead;
	}

	template <typename T> void Read(T& outData)
	{
		Read(&outData, sizeof(outData));
	}

private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};