#include "TimeConstBuffer.h"

TimeConstBuffer* TimeConstBuffer::Get()
{
	static TimeConstBuffer t;
	
	return &t;

}

void TimeConstBuffer::Init(ID3D12Device* device_)
{
	buffer.CreateAndMapping(device_);
}

