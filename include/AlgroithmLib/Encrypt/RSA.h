#ifndef __LIBTOOLS_ALGORITHMLIB_ENCRYP_RSA_H__
#define __LIBTOOLS_ALGORITHMLIB_ENCRYP_RSA_H__

#include <Windows.h>
#include <vector>
#include <string>

namespace libTools
{
	class CRSA
	{
	public:
		struct PublicKeyClass
		{
			LONGLONG modulus;
			LONGLONG exponent;
		};

		struct PrivateKeyClass
		{
			LONGLONG modulus;
			LONGLONG exponent;
		};
	public:
		CRSA() = default;
		~CRSA() = default;

		void GenerateKey(_Out_ PublicKeyClass* PubKey, _Out_ PrivateKeyClass* PriKey);

		// pszOutText.size must == pszEncryText.size
		VOID Encrypt(_In_ CONST PublicKeyClass& PubKey, _In_ CONST std::vector<CHAR>& VecText, _Out_ std::vector<DWORD>& VecBinary) CONST;

		VOID Encrypt(_In_ CONST PublicKeyClass& PubKey, _In_ CONST std::wstring& wsText, _Out_ std::vector<DWORD>& VecBinary) CONST;

		VOID Decrypt(_In_ CONST PrivateKeyClass& PrivKey, _In_ CONST std::vector<DWORD>& VecEncrypText, _Out_ std::vector<CHAR>& VecBinary) CONST;

		VOID Decrypt(_In_ CONST PrivateKeyClass& PrivKey, _In_ CONST std::vector<DWORD>& VecEncrypText, _Out_ std::wstring& wsText) CONST;
	private:

		// 最大公约数
		LONGLONG Gcd(_In_ LONGLONG a, _In_ LONGLONG b) CONST;

		LONGLONG ExtEuclid(_In_ LONGLONG a, _In_ LONGLONG b) CONST;

		LONGLONG Rsa_ModExp(_In_ LONGLONG b, _In_ LONGLONG e, _In_ LONGLONG m) CONST;

		// 生成随机素数
		LONGLONG GenerateRandPrime();
	};
}




#endif // !__LIBTOOLS_ALGORITHMLIB_ENCRYP_RSA_H__
