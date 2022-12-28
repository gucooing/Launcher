#ifndef __LIBTOOLS_PROCESSLIB_PROCESS_PROCESSMODULE_H__
#define __LIBTOOLS_PROCESSLIB_PROCESS_PROCESSMODULE_H__

#include <Windows.h>
#include <vector>
#include <functional>
#include <string>

namespace libTools
{
	class CProcessModule
	{
	public:
		struct ProcessModuleContent
		{
			std::wstring wsModuleName;
			std::wstring wsModulePath;
			DWORD		 dwImageBase;
			DWORD		 dwImageSize;
		};
	public:
		CProcessModule() = default;
		~CProcessModule() = default;

		static BOOL GetProcessModule(_In_ HANDLE hProcess, _Out_ std::vector<ProcessModuleContent>& Vec);

		static BOOL FindProcessModule(_In_ HANDLE hProcess, _In_ CONST std::wstring& wsModuleName, _Out_ ProcessModuleContent& Content);
	private:
		static BOOL QueryProcessModule(_In_ HANDLE hProcess, _In_ std::function<BOOL(CONST ProcessModuleContent&)> ActionPtr);
	};
}




#endif // !__LIBTOOLS_PROCESSLIB_PROCESS_PROCESSMODULE_H__
