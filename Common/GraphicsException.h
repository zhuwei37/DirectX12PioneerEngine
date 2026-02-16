#pragma once
#include<string>
#include"Windows.h"
class GraphicsException
{
public:
    GraphicsException() = default;
    GraphicsException(HRESULT hr, const std::string& functionName, const std::string& filename, int lineNumber);

   /* std::string ToString()const;*/

    long ErrorCode = 0;
    std::string Message;
    std::string FunctionName;
    std::string Filename;
    int LineNumber = -1;
};
