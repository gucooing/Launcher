#ifndef __LIBTOOLS_INJECTORLIB_DLLINJECTOR_DLLINJECTOR_H__
#define __LIBTOOLS_INJECTORLIB_DLLINJECTOR_DLLINJECTOR_H__

#include <Windows.h>
#include <string>

namespace libTools
{
	class CDllInjector
	{
	public:
		CDllInjector() = default;
		~CDllInjector() = default;

		// Injector DLl to Already Exist Process ...
		static BOOL RemoteThreadInjectorDLL(_In_ DWORD dwPid, _In_ CONST std::wstring& wsDllPath);

		// Create Process and Injector DLL to this process ... (wsDllPath could be empty)  use RemoteThread to Injector DLL ...
		static BOOL CreateProcess_And_InjectorDLL(_In_ CONST std::wstring& wsProcessCommand, _In_ CONST std::wstring& wsDllPath, _Out_opt_ PROCESS_INFORMATION* pProcInfo = nullptr);

#ifndef _WIN64
		// Create Process and Injector DLL to this Process .. (wsDllPath couldn't be empty) use shellcode to Injector DLL ....
		static BOOL CreateProcess_And_ShellCodeInjectorDLL(_In_ CONST std::wstring& wsProcessCommand, _In_ CONST std::wstring& wsDllPath, _Out_opt_ PROCESS_INFORMATION* pProcInfo = nullptr);
#endif
		// Debug权限
		static BOOL PromotionDebugPrivlige();

		//
		static BOOL IsRunAsAdministrator();

		// 
		static BOOL RaisePrivilige(_In_ LPCWSTR pwszPrivilegeName);

		// win7的backup 和 restore权限
		static BOOL PromotionBackupPrivlige();

		// 修改win7系统文件的权限
		static BOOL AddModifyWin7SystemFilePrivlige(_In_ CONST std::wstring& wsFilePath);
	};
}

#endif // !__LIBTOOLS_INJECTORLIB_DLLINJECTOR_DLLINJECTOR_H__
