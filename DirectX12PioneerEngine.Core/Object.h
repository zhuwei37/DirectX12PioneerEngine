#pragma once
#include<memory>

namespace DirectX12PioneerEngine
{
	namespace Core
	{

		class Object :public std::enable_shared_from_this<Object>
		{
		public:
			Object()
			{
				//this->shared_from_this();
			}
		};
	}
}