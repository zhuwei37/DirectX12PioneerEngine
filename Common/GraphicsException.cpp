#include"pch.h"
#include <comdef.h>
GraphicsException::GraphicsException(HRESULT hr, const std::string& functionName, const std::string& filename,
	int lineNumber)
	:ErrorCode(hr),
	FunctionName(functionName),
	Filename(filename),
	LineNumber(lineNumber)
{
    char* pMsgBuf = nullptr;

    // 使用FormatMessage分配缓冲区来保存错误消息
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 默认语言
        (LPSTR)&pMsgBuf,
        0,
        nullptr
    );

    std::string str;

    if (nMsgLen > 0)
    {
        // 成功获取到错误消息
        Message = std::string(pMsgBuf, nMsgLen);
    }
}
