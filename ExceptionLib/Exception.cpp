#include "Exception.h"

namespace libTools
{
	std::function<VOID(CONST std::wstring&)> g_EchoExceptionMsgPtr = nullptr;
	VOID CException::InvokeExceptionPtr(_In_ LPCWSTR pwszFormat, ...)
	{
		if (g_EchoExceptionMsgPtr != nullptr)
		{
			va_list		args;
			WCHAR		szBuffer[1024];
			va_start(args, pwszFormat);
			_vsnwprintf_s(szBuffer, _countof(szBuffer) - 1, _TRUNCATE, pwszFormat, args);
			va_end(args);

			g_EchoExceptionMsgPtr(szBuffer);
		}
	}

	BOOL CException::PrintExceptionCode(_In_ LPEXCEPTION_POINTERS ExceptionPtr)
	{
		//LOG_CF_E(L"ExceptionCode=%X, EIP=%X, Addr=%X", ExceptionPtr->ExceptionRecord->ExceptionCode, ExceptionPtr->ContextRecord->Eip, ExceptionPtr->ExceptionRecord->ExceptionAddress);
		switch (ExceptionPtr->ExceptionRecord->ExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			InvokeExceptionPtr(L" 内存地址非法访问异常!");
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			InvokeExceptionPtr(L" 数据类型未对齐异常 ");
			break;
		case EXCEPTION_BREAKPOINT:
			InvokeExceptionPtr(L" 中断异常!");
			break;
		case EXCEPTION_SINGLE_STEP:
			InvokeExceptionPtr(L" 单步中断异常 ");
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			InvokeExceptionPtr(L" 数组越界 ");
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO: case EXCEPTION_INT_DIVIDE_BY_ZERO:
			InvokeExceptionPtr(L" 除以 0 异常!");
			break;
		case EXCEPTION_INT_OVERFLOW: case EXCEPTION_FLT_OVERFLOW:
			InvokeExceptionPtr(L" 数据溢出异常 ");
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			InvokeExceptionPtr(L" 浮点数计算异常 ");
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			InvokeExceptionPtr(L" 页错误异常 ");
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			InvokeExceptionPtr(L" 非法指令异常 ");
			break;
		case EXCEPTION_STACK_OVERFLOW:
			InvokeExceptionPtr(L" 堆栈溢出!");
			break;
		case EXCEPTION_INVALID_HANDLE:
			InvokeExceptionPtr(L" 无效句柄异常!");
			break;
		default:
			break;
		}
		return TRUE; // return TRUE 表示异常被处理, 否则继续往上一层抛!
	}

}