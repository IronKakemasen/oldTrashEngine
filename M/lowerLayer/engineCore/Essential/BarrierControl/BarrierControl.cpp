#include "BarrierControl.h"
#include "../CommandControl/CommandControl.h"

#pragma comment(lib,"d3d12.lib")


D3D12_RESOURCE_BARRIER BarrierControl::Create(
	ID3D12Resource* dst_resource_,
	D3D12_RESOURCE_BARRIER_TYPE Type,
	D3D12_RESOURCE_BARRIER_FLAGS Flags,
	D3D12_RESOURCE_STATES before_,
	D3D12_RESOURCE_STATES after_,
	UINT transitionSubResource_)
{
	D3D12_RESOURCE_BARRIER ret_barrier{};
	
	//バリアの設定
	ret_barrier.Type = Type;
	//Noneにしておく
	ret_barrier.Flags = Flags;
	//バリアを張る対象のリソース。
	ret_barrier.Transition.pResource = dst_resource_;

	ret_barrier.Transition.Subresource = transitionSubResource_;

	//バリアステートを遷移
	ret_barrier.Transition.StateBefore = before_;
	ret_barrier.Transition.StateAfter = after_;



	return ret_barrier;
}

void BarrierControl::Pitch(ID3D12GraphicsCommandList* cList_, D3D12_RESOURCE_BARRIER* barrier_)
{
	//TransitionBarrierを張る
	cList_->ResourceBarrier(1, barrier_);

}

