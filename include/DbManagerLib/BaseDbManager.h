#ifndef __LIBTOOLS_DBMANAGERLIB_BASEDBMANAGER_H__
#define __LIBTOOLS_DBMANAGERLIB_BASEDBMANAGER_H__

#include <Windows.h>
#include <vector>
#include <sqltypes.h>	// Type SQLHENV & SQLHDBC & SQLHSTMT
#include <sqlext.h>		// macro SQL_NULL_HENV
#include <mutex>
#include <thread>
#include <queue>
#include <ProcessLib/Lock/Lock.h>

namespace libTools
{
	class CBaseDbManager : public libTools::CThreadLock
	{
	private:
		struct SQLEnvParam
		{
			SQLHENV hEnv = SQL_NULL_HENV;
			SQLHDBC hDbc = SQL_NULL_HDBC;
			SQLHSTMT hStmt = SQL_NULL_HSTMT;
		};
	public:
		typedef std::vector<std::wstring> Table;
	public:
		CBaseDbManager() = default;
		~CBaseDbManager();

		// Warning, Declare local Variable Max Size = 128
		BOOL ExcuteSQL(_In_ CONST std::wstring wsSQL, _In_ UINT uResultCount, _Out_ std::vector<Table>& VecResult) CONST;

		// 
		BOOL ExcuteSQL(_In_ CONST std::wstring wsSQL, _Out_ std::vector<Table>& VecResult) CONST;

		// 
		BOOL ExcuteSQL_SingleResult(_In_ CONST std::wstring& wsSQL, _Out_ std::wstring& wsResultText) CONST;

		//
		BOOL ExcuteSQL_NoneResult(_In_ CONST std::wstring& wsSQL) CONST;

		//
		VOID AsyncExcuteSQL(_In_ CONST std::wstring& wsSQL);

		//
		VOID RunAsyncThread();

		//
		VOID StopAsyncThread();
	private:
		// 
		BOOL InitializeSQLEnv(_Out_ SQLEnvParam& Env) CONST;

		// 
		VOID PrintSQLErrText(_In_ SQLSMALLINT fHandleType, _In_ SQLHANDLE handle) CONST;

		//
		VOID FreeMem(_In_ SQLEnvParam& Env) CONST;

		// 
		virtual VOID Initialize() = NULL;

	private:
		VOID _AsyncThread();
	protected:
		std::wstring	_wsDns;
		std::wstring	_wsDbUser;
		std::wstring	_wsDbPass;
		BOOL			_bRun = FALSE;
		std::thread		_hAsyncThread;
		mutable std::mutex _Mtx;
		
		std::queue<std::wstring> _QueAsyncSQL;
	};
}

#endif // !__LIBTOOLS_DBMANAGERLIB_BASEDBMANAGER_H__
