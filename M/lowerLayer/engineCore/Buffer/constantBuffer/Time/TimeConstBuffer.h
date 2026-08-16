#pragma once

#include "../bufferAndMap.h"

struct TimePara
{
	float passedTime = 0.0f;

};

struct TimeConstBuffer
{
	static TimeConstBuffer* Get();
	void Init(ID3D12Device* device_);

	BufferAndMap<TimePara> buffer;

	TimeConstBuffer() = default;
	~TimeConstBuffer() = default;

	// コピー・ムーブ禁止
	TimeConstBuffer(const TimeConstBuffer&) = delete;
	TimeConstBuffer& operator=(const TimeConstBuffer&) = delete;



};
