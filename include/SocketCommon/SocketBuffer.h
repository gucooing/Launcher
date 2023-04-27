#ifndef __LIBTOOLS_SOCKETCOMMONLIB_SOCKETBUFFER_H__
#define __LIBTOOLS_SOCKETCOMMONLIB_SOCKETBUFFER_H__

#include <Windows.h>
#include <string>
#include <memory>
#include <vector>

namespace libTools
{
	// struct support SocketBuffer operator
	template<typename T>
	struct BaseSocketBuffer
	{
		virtual BaseSocketBuffer& operator << (T& SocketBuffer) = NULL;

		virtual CONST BaseSocketBuffer& operator >> (T& SocketBuffer) CONST = NULL;
	};


	class CSocketBuffer
	{
	public:
		enum em_SocketBuffer_Error
		{
			em_SocketBuffer_Error_None,
			em_SocketBuffer_Error_InvalidSize,
			em_SocketBuffer_Error_InvalidCRC,
			em_SocketBuffer_Error_InvalidHead,
		};
	public:
		CSocketBuffer();
		~CSocketBuffer() = default;

		template<typename T>
		CSocketBuffer(T MsgHead)
		{
			InitializeHead(MsgHead);
		}

		

		template<typename T>
		VOID InitializeHead(T MsgHead)
		{
			clear();
			_dwMsgHead = static_cast<DWORD>(MsgHead);
			AddVerData();
		}

		static VOID SetValue_By_Buffer(_Out_ DWORD& dwValue, _In_ CONST BYTE* Buffer);

		CSocketBuffer& operator << (ULONGLONG ulValue);

		CSocketBuffer& operator << (DWORD dwValue);

		CSocketBuffer& operator << (UINT uValue);

		CSocketBuffer& operator << (int nValue);

		CSocketBuffer& operator << (bool bValue);

		CSocketBuffer& operator << (WORD wValue);

		CSocketBuffer& operator << (BYTE bValue);

		CSocketBuffer& operator << (float fVersion);

		CSocketBuffer& operator << (CONST std::wstring& wsText);

		CSocketBuffer& operator << (CONST SYSTEMTIME& Tick);

		CSocketBuffer& operator << (LPCWSTR lpwszText);

		CSocketBuffer& operator >> (DWORD& dwValue);

		CSocketBuffer& operator >> (ULONGLONG& ulValue);

		CSocketBuffer& operator >> (WORD& wValue);

		CSocketBuffer& operator >> (BYTE& bValue);

		CSocketBuffer& operator >> (bool& bValue);

		CSocketBuffer& operator >> (float& fValue);

		CSocketBuffer& operator >> (UINT& uValue);

		CSocketBuffer& operator >> (std::wstring& wsText);

		CSocketBuffer& operator >> (int& nValue);

		CSocketBuffer& operator >> (SYSTEMTIME& Tick);

		// 
		std::shared_ptr<CHAR> GetBuffer(_Out_ UINT& uSize);

		//
		std::shared_ptr<CHAR> GetSizeBuffer(_In_ UINT uSize);

		//
		UINT CopyToBuffer(_In_ CHAR* OutBuffer, _In_ UINT uMaxOutBufferSize);

		//
		DWORD GetCRC() CONST;

		// Clear Data, Send
		std::shared_ptr<CHAR> GetDataPtr(_Out_ UINT& uSize);

		VOID SetDataPtr(_In_ CONST std::shared_ptr<CHAR>& DataPtr, _In_ UINT uSize);

		VOID SetDataPtr(_In_ CONST CHAR* Buffer, _In_ UINT uSize);

		template<typename T>
		em_SocketBuffer_Error VerDataPtr(T& MsgHead)
		{
			DWORD dwValue1, dwValue2, dwSize = 0;

			*this >> dwSize >> dwValue1 >> dwValue2 >> _dwMsgHead;

			if (dwValue1 != 0x434C536F || dwValue2 != 0x636B6574)
				return em_SocketBuffer_Error::em_SocketBuffer_Error_InvalidCRC;
			else if (dwSize != _Data.size() + 16) // 12 = sizeof(dwValue1) + sizeof(dwValue2) + sizeof(dwMsgType)
				return em_SocketBuffer_Error::em_SocketBuffer_Error_InvalidSize;

			MsgHead = GetMsgHead<T>();
			return em_SocketBuffer_Error::em_SocketBuffer_Error_None;
		}

		template<typename T>
		T GetMsgHead()
		{
			return static_cast<T>(_dwMsgHead);
		}

		UINT size() CONST
		{
			return static_cast<UINT>(_Data.size());
		}

		VOID clear();


		template<typename T>
		VOID AddStruct(CONST T& itm)
		{
			itm >> *this;
		}

		template<typename T>
		T GetStruct()
		{
			T Instance;
			Instance << *this;
			return Instance;
		}
	private:

		VOID AddVerData();

		VOID Set(BYTE Data);

		BYTE Get();

	private:
		std::vector<BYTE> _Data;
		DWORD             _dwMsgHead;
	};
}

#endif // !__LIBTOOLS_SOCKETCOMMONLIB_SOCKETBUFFER_H__
