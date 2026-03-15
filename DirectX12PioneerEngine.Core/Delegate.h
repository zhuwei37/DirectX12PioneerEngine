#pragma once
#include<vector>

namespace DirectX12PioneerEngine
{
	namespace Core
	{
		template<typename T>
		class Delegate
		{   
			using DelegateContainer = std::vector<T>*;
			DelegateContainer mData;
		public:
			Delegate()
			{
				mData = new std::vector<T>(1);
			}
			DelegateContainer Combine(T data)
			{

			}
			DelegateContainer Remove(T data)
			{

			}
			~Delegate()
			{
				delete mData;
			}
			
		};
	}
}