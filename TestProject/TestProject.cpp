// TestProject.cpp : 定义应用程序的入口点。
//

#include "TestProject.h"
#include<iostream>
#include<map>
#include"EngineManager.h"
#define MAX_LOADSTRING 100
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//enum ShaderValueType
//{
//    Float,
//    Float2,
//    Float3,
//    Float3x3,
//    Float4x4,
//    Int,
//    Bool,
//    Texture2D,
//};
//
//class IShaderValueTempalte
//{
//public:
//    virtual ShaderValueType GetValueType() = 0;
//    
//};
//
//template<typename T>
//class IShaderValue:public IShaderValueTempalte
//{
//public:
//    virtual ShaderValueType GetValueType()override
//    {
//        return mType;
//    }
//    virtual T Get() = 0;
//    virtual void Set(T value) = 0;
//protected:
//    ShaderValueType mType;
//};
//class ShaderIntValue :public IShaderValue<int>
//{
//public:
//    ShaderIntValue()
//    {
//        mValue = 100;
//        mType = ShaderValueType::Int;
//    }
//    virtual int Get() override
//    {
//        return mValue;
//    }
//    virtual void Set(int value)override
//    {
//        mValue = value;
//    }
//private:
//    int mValue;
//
//};
//class ShaderFloatValue :public IShaderValue<float>
//{
//public:
//    ShaderFloatValue()
//    {
//        mType = ShaderValueType::Float;
//        mValue = 2.0f;
//    }
//    virtual float Get() override
//    {
//        return mValue;
//    }
//    virtual void Set(float value)override
//    {
//        mValue = value;
//    }
//private:
//    float mValue;
//};
//class TemplateBaseClass
//{
//public:
//    virtual int Get() = 0;
//    virtual ~TemplateBaseClass() {};
//};
//template<typename T>
//class TemplateClass :public TemplateBaseClass
//{
//public:
//    int Get()override
//    {
//
//        return 1;
//    }
//    void Set(T v)
//    {
//       
//        b = v;
//    }
//    T b = 0;
//    ~TemplateClass()
//    {
//        b = 1;
//    }
//};

void Test1(int type,void* p)
{
    auto p=EngineManager::Get();
    p = nullptr;
 /*   std::vector<std::shared_ptr< TemplateBaseClass>> svs;

    TemplateClass<int>* p1=new  TemplateClass<int>;
    TemplateBaseClass* base =p1;
    TemplateClass<int>* p2 = dynamic_cast<TemplateClass<int>*>(base);
    delete base;
    svs.push_back( std::shared_ptr<TemplateBaseClass>( base));
    return;
    std::shared_ptr<TemplateClass<int>> p3 = std::make_shared<TemplateClass<int>>();
    std::shared_ptr< TemplateBaseClass>  tbcptr;
    tbcptr = (p3);
    auto p4=std::dynamic_pointer_cast<TemplateClass<int>>(tbcptr);
    p4->Set(4124);
    p2->Set(10);
    ShaderFloatValue floatv;
    floatv.Set(1.0f);
    IShaderValueTempalte* shader =dynamic_cast<IShaderValueTempalte*>( &floatv);
    ShaderFloatValue* v =static_cast<ShaderFloatValue*>( shader);
    IShaderValue<float>*  shadervalue = dynamic_cast<IShaderValue<float>*>(shader);
    std::map<int,std::shared_ptr< IShaderValueTempalte>> valueMap;
   
   valueMap[1] = std::make_shared<ShaderFloatValue>(floatv);
    valueMap[2] = std::make_shared<ShaderFloatValue>();
    valueMap[3] = std::make_shared<ShaderIntValue>();
    for (auto value : valueMap)
    {
        switch (value.second->GetValueType())
        {
        case ShaderValueType::Float:
        {
            auto floatvalue = dynamic_cast<ShaderFloatValue*>(value.second.get());
            float f = floatvalue->Get();
            OutputDebugStringA(std::to_string(f).c_str());
            break;
        }
        case ShaderValueType::Int:
        {
            auto intvalue = dynamic_cast<ShaderIntValue*>(value.second.get());
            int f = intvalue->Get();
         
            break;
        }
        default:break;
        }
    }
    switch (type)
    {
    case 1:
    {
        auto floatptr=reinterpret_cast<float*> (p);
        std::cout << *floatptr << std::endl;
        OutputDebugStringA(std::to_string(*floatptr).c_str());
        break;
    }
    case 2:
    {
        auto floatptr = reinterpret_cast<TestStruct*> (p);
        std::cout << floatptr->ID << std::endl;
        break;
    }
    default:break;
    }*/
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
  
    Test1(1, nullptr);
  // TestStruct ts = { 23,3.0 };
    return 0;
}



