#pragma once
#include"Object.h"
#include"Delegate.h"
#include<atomic>
namespace DirectX12PioneerEngine
{
	namespace Core{
		template<typename ParameterType>
		 class Action
		{

			 using Callback= void (*)(ParameterType) ;
			typedef struct ActionMember
			{
				std::shared_ptr<Object> target;
				Callback callback;
			}ActionMember;

			  Delegate<ActionMember> *Container
		public:
			Action()
			{
				Container = new Delegate<ActionMember>();
			}
			void Invoke()
			{
				
			}
			
			void Add(Object *obj,Callback callback)
			{
				ActionMember data;
				data.target = obj->shared_from_this();
				data.callback = callback;

			}
			void Remove(Object* obj, Callback callback)
			{

			}
			~Action()
			{

			}
		private:

		};
	}
}
