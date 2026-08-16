#pragma once
#include < algorithm >
#include <vector>
#include "../ConvertToRadian/ConvertToRadian.h"

template<typename T>
bool DoesContain(const std::vector<T>& container_, T value_) {
	return std::find(container_.cbegin(), container_.cend(), value_) != container_.cend();
}

namespace Benri
{
	//MAXを超えたらMAXにする
	template<typename T>
	inline bool Max(T& t_, T max_t_)
	{
		if (t_ > max_t_)
		{
			t_ = max_t_;
			return 1;
		}
		return 0;
	}

	//値がMINを下回ったらMINにする
	template<typename T>
	inline bool Min(T& t_, T min_t_)
	{
		if (t_ < min_t_)
		{
			t_ = min_t_;
			return true;
		}

		return false;
	}

	//値をMINからMAXのとき特定の値にする
	template<typename T>
	inline void Adjust(T& t_, T min_t_, T max_t_, T dstNum_)
	{
		if (t_ >= min_t_ && t_ <= max_t_) t_ = dstNum_;
	}

	template<typename T>
	inline void AdjustMin(T& t_, T min_t_, T dst_)
	{
		if (t_ < min_t_) t_ = dst_;
	}

	template<typename T>
	inline void AdjustMax(T& t_, T max_t_, T dst_)
	{
		if (t_ > max_t_) t_ = dst_;
	}


	//符号を得る。return [0] or [-] or [+] 
	template<typename T>
	int GetSign(T num_)
	{
		return (num_ > 0) - (num_ < 0);
	}

	//配列の中身を一つの値で埋める
	//https://qiita.com/oyas/items/70b7e29d0388392ef2ce
	template<typename A, size_t N, typename T>
	void Fill(A(&array)[N], const T& val)
	{
		std::fill((T*)array, (T*)(array + N), val);
	}

	 
}