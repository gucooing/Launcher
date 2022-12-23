#include "NoTraceModuleInjector.h"
#include <include/FileLib/File.h>
#include <include/HookLib/InlineHook/InlineHook.h>

#ifndef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"FileLib_Debug.lib")
#pragma comment(lib,"HookLib_Debug.lib") 
#else
#pragma comment(lib,"FileLib.lib")
#pragma comment(lib,"HookLib.lib")
#endif // _DEBUG


libTools::CNoTraceModuleInjector::RtlInitUnicodeStringPtr		libTools::CNoTraceModuleInjector::_RtlInitUnicodeStringPtr	= nullptr;
libTools::CNoTraceModuleInjector::NtUnmapViewOfSectionPtr		libTools::CNoTraceModuleInjector::_NtUnmapViewOfSectionPtr	= nullptr;
libTools::CNoTraceModuleInjector::NtOpenFilePtr					libTools::CNoTraceModuleInjector::_NtOpenFilePtr			= nullptr;
libTools::CNoTraceModuleInjector::NtOpenDirectoryObjectPtr		libTools::CNoTraceModuleInjector::_NtOpenDirectoryObjectPtr = nullptr;
libTools::CNoTraceModuleInjector::NtOpenSectionPtr				libTools::CNoTraceModuleInjector::_NtOpenSectionPtr			= nullptr;
libTools::CNoTraceModuleInjector::NtMapViewOfSectionPtr			libTools::CNoTraceModuleInjector::_NtMapViewOfSectionPtr	= nullptr;
libTools::CNoTraceModuleInjector::RtlNtStatusToDosErrorPtr		libTools::CNoTraceModuleInjector::_RtlNtStatusToDosErrorPtr	= nullptr;
libTools::CNoTraceModuleInjector::NtClosePtr					libTools::CNoTraceModuleInjector::_NtClosePtr				= nullptr;
libTools::CNoTraceModuleInjector::NtCreateFilePtr				libTools::CNoTraceModuleInjector::_NtCreateFilePtr			= nullptr;
libTools::CNoTraceModuleInjector::NtCreateSectionPtr			libTools::CNoTraceModuleInjector::_NtCreateSectionPtr		= nullptr;
libTools::CNoTraceModuleInjector::NtQuerySectionPtr				libTools::CNoTraceModuleInjector::_NtQuerySectionPtr		= nullptr;
libTools::CNoTraceModuleInjector::LdrLoadDllPtr					libTools::CNoTraceModuleInjector::_LdrLoadDllPtr			= nullptr;
libTools::CNoTraceModuleInjector::RtlCompareUnicodeStringPtr	libTools::CNoTraceModuleInjector::_RtlCompareUnicodeStringPtr= nullptr;
libTools::CNoTraceModuleInjector::RtlPushFramePtr				libTools::CNoTraceModuleInjector::_RtlPushFramePtr			= nullptr;
libTools::CNoTraceModuleInjector::RtlGetFramePtr				libTools::CNoTraceModuleInjector::_RtlGetFramePtr			= nullptr;
libTools::CNoTraceModuleInjector::RtlPopFramePtr				libTools::CNoTraceModuleInjector::_RtlPopFramePtr			= nullptr;
libTools::CNoTraceModuleInjector::NtQueryAttributesFilePtr		libTools::CNoTraceModuleInjector::_NtQueryAttributesFilePtr	= nullptr;
libTools::CNoTraceModuleInjector::RtlDosPathNameToNtPathName_UPtr	libTools::CNoTraceModuleInjector::_RtlDosPathNameToNtPathName_UPtr= nullptr;
libTools::CNoTraceModuleInjector::NtOpenFilePtr					libTools::CNoTraceModuleInjector::_NtOpenFilePtr_Old		= nullptr;
libTools::CNoTraceModuleInjector::NtCreateSectionPtr			libTools::CNoTraceModuleInjector::_NtCreateSectionPtr_Old	= nullptr;
libTools::CNoTraceModuleInjector::NtQuerySectionPtr				libTools::CNoTraceModuleInjector::_NtQuerySectionPtr_Old	= nullptr;
libTools::CNoTraceModuleInjector::NtMapViewOfSectionPtr			libTools::CNoTraceModuleInjector::_NtMapViewOfSectionPtr_Old = nullptr;
libTools::CNoTraceModuleInjector::NtClosePtr					libTools::CNoTraceModuleInjector::_NtClosePtr_Old			= nullptr;
libTools::CNoTraceModuleInjector::NtQueryAttributesFilePtr		libTools::CNoTraceModuleInjector::_NtQueryAttributesFilePtr_Old = nullptr;



libTools::CNoTraceModuleInjector::CNoTraceModuleInjector() : _pvFileContent(nullptr), _uFileSize(0), _IsAllocMemory(FALSE)
{

}

libTools::CNoTraceModuleInjector::~CNoTraceModuleInjector()
{
	if (_IsAllocMemory && _pvFileContent != nullptr)
	{
		::VirtualFree(_pvFileContent, 0, MEM_RELEASE);
		_pvFileContent = nullptr;
	}
}

BOOL libTools::CNoTraceModuleInjector::SetModuleContent(_In_ CONST std::wstring& wsDllPath)
{
	if (!CFile::ReadFileContent(wsDllPath, _pvFileContent, _uFileSize))
	{
		return FALSE;
	}


	_IsAllocMemory = TRUE;
	return TRUE;
}

VOID libTools::CNoTraceModuleInjector::SetModuleContent(_In_ LPVOID pvFileContent, _In_ SIZE_T uSize)
{
	_pvFileContent = pvFileContent;
	_uFileSize = uSize;
	_IsAllocMemory = FALSE;
}


BOOL libTools::CNoTraceModuleInjector::LoadModule()
{
	if (_pvFileContent == nullptr)
	{
		return FALSE;
	}


	if (!InitLdr())
	{
		return FALSE;
	}


	UNICODE_STRING usDLLName;
	WCHAR szSysDll[MAX_PATH] = { 0 };
	GetSystemDirectory(szSysDll, MAX_PATH);
	lstrcatW(szSysDll, L"\\asycfilt.dll");
	_RtlInitUnicodeStringPtr(&usDLLName, szSysDll);
	if (!NT_SUCCESS(LoadDllFromMemory(&usDLLName, 0)))
	{
		return FALSE;
	}


	return UnHook();
}

BOOL libTools::CNoTraceModuleInjector::InitLdr()
{
	HMODULE hNtdll = GetModuleHandle(L"ntdll.dll");
#define TrySetProcAddr(Type) _##Type##Ptr = reinterpret_cast<Type##Ptr>(::GetProcAddress(hNtdll, #Type)); \
		if(_##Type##Ptr == nullptr)	\
		{							\
			return FALSE;			\
		}							\

	TrySetProcAddr(RtlInitUnicodeString);
	TrySetProcAddr(NtUnmapViewOfSection);
	TrySetProcAddr(NtOpenFile);
	TrySetProcAddr(NtOpenDirectoryObject);
	TrySetProcAddr(NtOpenSection);
	TrySetProcAddr(NtMapViewOfSection);
	TrySetProcAddr(RtlNtStatusToDosError);
	TrySetProcAddr(NtClose);
	TrySetProcAddr(NtCreateFile);
	TrySetProcAddr(NtCreateSection);
	TrySetProcAddr(NtQuerySection);
	TrySetProcAddr(LdrLoadDll);
	TrySetProcAddr(RtlCompareUnicodeString);
	TrySetProcAddr(RtlPushFrame);
	TrySetProcAddr(RtlGetFrame);
	TrySetProcAddr(RtlPopFrame);
	TrySetProcAddr(NtQueryAttributesFile);
	TrySetProcAddr(RtlDosPathNameToNtPathName_U);


	return Hook();
}

BOOL libTools::CNoTraceModuleInjector::Hook()
{
	return CInlineHook::Hook(_NtOpenFilePtr, reinterpret_cast<LPVOID>(_NtOpenFile), reinterpret_cast<LPVOID*>(&_NtOpenFilePtr_Old)) &&
		CInlineHook::Hook(_NtCreateSectionPtr, reinterpret_cast<LPVOID>(_NtCreateSection), reinterpret_cast<LPVOID*>(&_NtCreateSectionPtr_Old)) &&
		CInlineHook::Hook(_NtQuerySectionPtr, reinterpret_cast<LPVOID>(_NtQuerySection), reinterpret_cast<LPVOID*>(&_NtQuerySectionPtr_Old)) &&
		CInlineHook::Hook(_NtMapViewOfSectionPtr, reinterpret_cast<LPVOID>(_NtMapViewOfSection), reinterpret_cast<LPVOID*>(&_NtMapViewOfSectionPtr_Old)) &&
		CInlineHook::Hook(_NtClosePtr, reinterpret_cast<LPVOID>(_NtClose), reinterpret_cast<LPVOID*>(&_NtClosePtr_Old)) &&
		CInlineHook::Hook(_NtQueryAttributesFilePtr, reinterpret_cast<LPVOID>(_NtQueryAttributesFile), reinterpret_cast<LPVOID*>(&_NtQueryAttributesFilePtr_Old));
}

BOOL libTools::CNoTraceModuleInjector::UnHook()
{
	return CInlineHook::UnHook(_NtOpenFilePtr, _NtOpenFilePtr_Old) &&
		CInlineHook::UnHook(_NtCreateSectionPtr, _NtCreateSectionPtr_Old) &&
		CInlineHook::UnHook(_NtQuerySectionPtr, _NtQuerySectionPtr_Old) &&
		CInlineHook::UnHook(_NtMapViewOfSectionPtr, _NtMapViewOfSectionPtr_Old) &&
		CInlineHook::UnHook(_NtClosePtr, _NtClosePtr_Old) &&
		CInlineHook::UnHook(_NtQueryAttributesFilePtr, _NtQueryAttributesFilePtr_Old);
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::LoadDllFromMemory(IN PUNICODE_STRING ModuleFileName, IN ULONG Flags)
{
	NTSTATUS            Status = STATUS_UNSUCCESSFUL;
	PVOID               ModuleBase = nullptr;
	LOAD_MEM_DLL_INFO   MemDllInfo;
	PIMAGE_DOS_HEADER   DosHeader = nullptr;
	PIMAGE_NT_HEADERS   NtHeader = nullptr;
	WCHAR               Lz32DosPath[MAX_PATH] = { 0 };


	ZeroMemory(&MemDllInfo, sizeof(MemDllInfo));
	MemDllInfo.Context = LOAD_MEM_DLL_INFO_MAGIC;
	_RtlPushFramePtr(&MemDllInfo);


	if (!_RtlDosPathNameToNtPathName_UPtr(ModuleFileName->Buffer, &MemDllInfo.MemDllFullPath, NULL, NULL))
	{
		_RtlPopFramePtr(&MemDllInfo);
		return STATUS_UNSUCCESSFUL;
	}


	GetSystemDirectoryW(Lz32DosPath, MAX_PATH);
	lstrcatW(Lz32DosPath, L"\\Lz32.dll");


	if (!_RtlDosPathNameToNtPathName_UPtr(Lz32DosPath, &MemDllInfo.Lz32Path, NULL, NULL))
	{
		_RtlPopFramePtr(&MemDllInfo);
		return STATUS_UNSUCCESSFUL;
	}


	MemDllInfo.Flags = Flags;
	MemDllInfo.MemDllBase = _pvFileContent;
	MemDllInfo.DllBufferSize = _uFileSize;
	DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(_pvFileContent);
	NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<ULONG_PTR>(_pvFileContent) + DosHeader->e_lfanew);
	MemDllInfo.ViewSize = NtHeader->OptionalHeader.SizeOfImage;
	Status = _LdrLoadDllPtr(NULL, 0, ModuleFileName, &ModuleBase);
	if (!NT_SUCCESS(Status) && !FLAG_ON(Flags, LMD_MAPPED_DLL))
	{
		_RtlPopFramePtr(&MemDllInfo);
		return STATUS_UNSUCCESSFUL;
	}

	_RtlPopFramePtr(&MemDllInfo);
	return Status;

}

libTools::CNoTraceModuleInjector::PTEB_ACTIVE_FRAME libTools::CNoTraceModuleInjector::FindThreadFrameByContext(_In_ ULONG_PTR Context)
{
	PTEB_ACTIVE_FRAME Frame = _RtlGetFramePtr();
	while (Frame != NULL && Frame->Context != Context)
	{
		Frame = Frame->Previous;
	}


	return Frame;
}

libTools::CNoTraceModuleInjector::LOAD_MEM_DLL_INFO* libTools::CNoTraceModuleInjector::GetMemoryModuleInstance()
{
	return reinterpret_cast<LOAD_MEM_DLL_INFO *>(FindThreadFrameByContext(LOAD_MEM_DLL_INFO_MAGIC));
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::_NtOpenFile(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG ShareAccess, IN ULONG OpenOptions)
{
	LOAD_MEM_DLL_INFO* pMemoryModule = GetMemoryModuleInstance();
	if (pMemoryModule == nullptr || _RtlCompareUnicodeStringPtr(ObjectAttributes->ObjectName, &pMemoryModule->MemDllFullPath, TRUE))
	{
		return _NtOpenFilePtr_Old(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
	}


	ObjectAttributes->ObjectName = &pMemoryModule->Lz32Path;
	NTSTATUS Status = _NtOpenFilePtr_Old(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
	if (NT_SUCCESS(Status))
	{
		pMemoryModule->DllFileHandle = *FileHandle;
	}
	return Status;
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::_NtCreateSection(_Out_ PHANDLE SectionHandle, _In_ ACCESS_MASK DesiredAccess, _In_ POBJECT_ATTRIBUTES ObjectAttributes, _In_ PLARGE_INTEGER MaximumSize, _In_ ULONG SectionPageProtection, _In_ ULONG AllocationAttributes, _In_ HANDLE FileHandle)
{
	if (FileHandle != NULL)
	{
		LOAD_MEM_DLL_INFO* pMemoryModule = GetMemoryModuleInstance();
		if (pMemoryModule != nullptr && pMemoryModule->DllFileHandle == FileHandle)
		{
			LARGE_INTEGER SectionSize;
			MaximumSize = &SectionSize;
			MaximumSize->QuadPart = pMemoryModule->ViewSize;

			NTSTATUS Status = _NtCreateSectionPtr_Old(SectionHandle, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, ObjectAttributes, MaximumSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);
			pMemoryModule->SectionHandle = *SectionHandle;
			return Status;
		}
	}

	// Excute Old ....
	return _NtCreateSectionPtr_Old(SectionHandle, DesiredAccess, ObjectAttributes, MaximumSize, SectionPageProtection, AllocationAttributes, FileHandle);
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::_NtQuerySection(_In_ HANDLE SectionHandle, _In_ SECTION_INFORMATION_CLASS SectionInformationClass, _Out_ PVOID SectionInformation, _In_ ULONG SectionInformationLength, _Out_ PULONG ResultLength)
{
	LOAD_MEM_DLL_INFO* pMemoryModule = GetMemoryModuleInstance();
	if (pMemoryModule == NULL || SectionHandle == NULL || pMemoryModule->SectionHandle != SectionHandle)
	{
		return _NtQuerySectionPtr_Old(SectionHandle, SectionInformationClass, SectionInformation, SectionInformationLength, ResultLength);
	}


	PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pMemoryModule->MemDllBase);
	PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<ULONG_PTR>(pDosHeader) + pDosHeader->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pNtHeaders->OptionalHeader;


	switch (SectionInformationClass)
	{
	case SectionBasicInformation:
		reinterpret_cast<SECTION_BASIC_INFORMATION *>(SectionInformation)->BaseAddress = pMemoryModule->MappedBase;
		reinterpret_cast<SECTION_BASIC_INFORMATION *>(SectionInformation)->Attributes = 0;
		reinterpret_cast<SECTION_BASIC_INFORMATION *>(SectionInformation)->Size.QuadPart = pMemoryModule->ViewSize;
		break;
	case SectionImageInformation:
		if (ResultLength != NULL)
		{
			*ResultLength = sizeof(SECTION_IMAGE_INFORMATION);
		}


		if (SectionInformationLength < sizeof(SECTION_BASIC_INFORMATION))
		{
			return STATUS_BUFFER_TOO_SMALL;
		}


		if (SectionInformation == NULL)
		{
			break;
		}


		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->TransferAddress = reinterpret_cast<PVOID>(reinterpret_cast<ULONG_PTR>(pDosHeader) + pOptionalHeader->AddressOfEntryPoint);
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->ZeroBits = 0;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->MaximumStackSize = pOptionalHeader->SizeOfStackReserve;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->CommittedStackSize = pOptionalHeader->SizeOfStackCommit;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->SubSystemType = pOptionalHeader->Subsystem;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->SubSystemVersionContent.SubSystemMinorVersion = pOptionalHeader->MinorSubsystemVersion;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->SubSystemVersionContent.SubSystemMajorVersion = pOptionalHeader->MajorSubsystemVersion;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->GpValue = 0;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->ImageCharacteristics = pNtHeaders->FileHeader.Characteristics;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->DllCharacteristics = pOptionalHeader->DllCharacteristics;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->Machine = pNtHeaders->FileHeader.Machine;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->ImageContainsCode = 0; // OptionalHeader->SizeOfCode;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->LoaderFlags = pOptionalHeader->LoaderFlags;
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->ImageFileSize = static_cast<ULONG>(pMemoryModule->DllBufferSize);
		reinterpret_cast<SECTION_IMAGE_INFORMATION *>(SectionInformation)->CheckSum = pOptionalHeader->CheckSum;
		break;
	case SectionRelocationInformation:
		if (SectionInformation != NULL)
		{
			*static_cast<ULONG_PTR *>(SectionInformation) = reinterpret_cast<ULONG_PTR>(pMemoryModule->MappedBase) - static_cast<ULONG_PTR>(pOptionalHeader->ImageBase);
		}


		if (ResultLength != NULL)
		{
			*ResultLength = sizeof(ULONG_PTR);
		}
		break;
	default:
		return _NtQuerySectionPtr_Old(SectionHandle, SectionInformationClass, SectionInformation, SectionInformationLength, ResultLength);
	}

	return STATUS_SUCCESS;
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::_NtMapViewOfSection(_In_ HANDLE SectionHandle, _In_ HANDLE ProcessHandle, _In_ _Out_ PVOID *BaseAddress, _In_ ULONG ZeroBits, _In_ ULONG CommitSize, _In_ _Out_ PLARGE_INTEGER SectionOffset, _In_ _Out_ PULONG ViewSize, _In_ SECTION_INHERIT InheritDisposition, _In_ ULONG AllocationType, _In_ ULONG Protect)
{
	if (SectionHandle == NULL)
	{
		return _NtMapViewOfSectionPtr_Old(SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect);
	}


	LOAD_MEM_DLL_INFO* pMemoryModule = GetMemoryModuleInstance();
	if (pMemoryModule == nullptr)
	{
		return _NtMapViewOfSectionPtr_Old(SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect);
	}


	if (SectionHandle != pMemoryModule->SectionHandle)
	{
		return _NtMapViewOfSectionPtr_Old(SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Protect);
	}

	if (SectionOffset != NULL)
	{
		SectionOffset->QuadPart = 0;
	}


	ULONG NewViewSize = 0;
	if (ViewSize == nullptr)
	{
		ViewSize = &NewViewSize;
	}


	*ViewSize = static_cast<ULONG>(pMemoryModule->ViewSize);
	NTSTATUS Status = _NtMapViewOfSectionPtr_Old(SectionHandle, ProcessHandle, BaseAddress, NULL, NULL, NULL, ViewSize, ViewShare, NULL, PAGE_EXECUTE_READWRITE);
	if (!NT_SUCCESS(Status))
	{
		return Status;
	}


	PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pMemoryModule->MemDllBase);
	PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<ULONG_PTR>(pDosHeader) + pDosHeader->e_lfanew);
	pMemoryModule->MappedBase = *BaseAddress;
	if (FLAG_ON(pMemoryModule->Flags, LMD_MAPPED_DLL))
	{
		CopyMemory(*BaseAddress, pMemoryModule->MemDllBase, pMemoryModule->ViewSize);
	}
	else
	{
		PIMAGE_SECTION_HEADER pSectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(reinterpret_cast<ULONG_PTR>(&pNtHeaders->OptionalHeader) + pNtHeaders->FileHeader.SizeOfOptionalHeader);
		for (ULONG NumberOfSections = pNtHeaders->FileHeader.NumberOfSections; NumberOfSections; ++pSectionHeader, --NumberOfSections)
		{
			CopyMemory(reinterpret_cast<BYTE*>(*BaseAddress) + pSectionHeader->VirtualAddress, reinterpret_cast<BYTE*>(pMemoryModule->MemDllBase) + pSectionHeader->PointerToRawData, pSectionHeader->SizeOfRawData);
		}

#if !defined(_M_IA64)
		CopyMemory(*BaseAddress, pMemoryModule->MemDllBase, 4 * 1024);
#else
		CopyMemory(*BaseAddress, pMemoryModule->MemDllBase, 8 * 1024);
#endif
	}

	return reinterpret_cast<ULONG_PTR>(*BaseAddress) != pNtHeaders->OptionalHeader.ImageBase ? STATUS_IMAGE_NOT_AT_BASE : STATUS_SUCCESS;
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::_NtClose(_In_ HANDLE Handle)
{
	LOAD_MEM_DLL_INFO *pMemoryModule = GetMemoryModuleInstance();
	if (pMemoryModule != nullptr && Handle != NULL)
	{
		if (pMemoryModule->DllFileHandle == Handle)
		{
			pMemoryModule->DllFileHandle = NULL;
		}
		else
		{
			pMemoryModule->SectionHandle = NULL;
		}
	}
	return _NtClosePtr_Old(Handle);
}

NTSTATUS WINAPI libTools::CNoTraceModuleInjector::_NtQueryAttributesFile(_In_ POBJECT_ATTRIBUTES ObjectAttributes, _In_ PFILE_BASIC_INFORMATION FileInformation)
{
	LOAD_MEM_DLL_INFO  *pMemoryModule = GetMemoryModuleInstance();
	if (pMemoryModule == nullptr || _RtlCompareUnicodeStringPtr(ObjectAttributes->ObjectName, &pMemoryModule->MemDllFullPath, TRUE))
	{
		return _NtQueryAttributesFilePtr_Old(ObjectAttributes, FileInformation);
	}


	return STATUS_SUCCESS;
}

#endif