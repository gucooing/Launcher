#ifndef __LIBTOOLS_PROCESSLIB_LOCK_LOCK_H__
#define __LIBTOOLS_PROCESSLIB_LOCK_LOCK_H__

#include <Windows.h>
#include <mutex>
#include <functional>

namespace libTools
{
	class ILock
	{
	public:
		ILock() = default;
		virtual ~ILock() = default;

		virtual void Lock() = 0;
		virtual void UnLock() = 0;


		virtual VOID DoActionInLock(_In_ std::function<VOID(VOID)> Ptr);

		template<typename T>
		T DoActionReturnInLock(_In_ std::function<T(VOID)> Ptr)
		{
			Lock();
			T bResult = Ptr();
			UnLock();
			return bResult;
		}
	private:

	};


	class CMutexLock : public ILock
	{
	public:
		CMutexLock() = default;
		virtual ~CMutexLock() = default;

		virtual void Lock() override;

		virtual void UnLock() override;
	private:
		std::mutex _Lock;
	};


	class CThreadLock : public ILock
	{
	public:
		CThreadLock();
		~CThreadLock();

		virtual void Lock() override;

		virtual void UnLock() override;

		// support std::lock_guard
		void lock();

		void unlock();
	private:
		CRITICAL_SECTION _Lock;
	};
}




#endif // !__LIBTOOLS_PROCESSLIB_LOCK_LOCKBASE_H__
