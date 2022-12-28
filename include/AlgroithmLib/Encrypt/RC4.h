#ifndef __LIBTOOLS_ALGORITHMLIB_ENCRYP_RC4_H__
#define __LIBTOOLS_ALGORITHMLIB_ENCRYP_RC4_H__

#include <Windows.h>
#include <vector>
#include <string>
#include <memory>

namespace libTools
{
	/*
	使用方法：
	RC4 r("Key")
	r.GetEncrypText("明文",密文)

	解密(密文和明文的长度是一毛一样的)
	RC4 r("key");
	r.GetKeyStream(“密文|明文的长度”,密钥流)
	RC4_Decryption::DecrypText(密文,密钥流,返回的明文)

	加密方式:
	大概就是调用keyStream(Len) 生成一个跟明文长度一样的密钥流
	然后用这个密钥流 ^ 明文 = 密文

	解密方法
	也要调用keyStream(Len) 用密钥生成一个密钥流(明文和密文的长度是一样的)
	密钥流 ^ 密文 = 明文
	*/
	class CRC4 
	{
	public:
		CRC4(_In_ CONST CHAR* pszKeyText, _In_ UINT uKeyLength);

		// 获取密钥流(明文长度,加密流), 这是调用解密函数之前调用的
		std::shared_ptr<CHAR> GetKeyStream(_In_ UINT uEnctypTextLength);

		// 获取加密文本(明文,明文长度)
		std::shared_ptr<CHAR> GetEncryptText(_In_ CONST CHAR* pszPlanText, _In_ UINT uLength);
	private:
		// 初始化状态向量S和临时向量T，供keyStream方法调用
		void Initial();

		// 初始排列状态向量S，供keyStream方法调用
		void rangeS();
		
		// 生成密钥流 len:明文为len个字节
		void keyStream(_In_ UINT uLen);
	private:
		// 状态向量，共256字节
		UCHAR S[256]; 
		// 临时向量，共256字节
		UCHAR T[256]; 
		// 密钥长度，keylen个字节，取值范围为1-256
		UINT _uKeyLen;
		// 可变长度密钥
		std::vector<CHAR> K;
		// 密钥流
		std::vector<CHAR> k;
	};


	class CRC4_Decryption 
	{
	public:
		CRC4_Decryption() = default;
		~CRC4_Decryption() = default;

		// 加密文本,加密的密钥流, 加密长度(加密文本和密钥流的长度是一样的), 返回解密的文本
		static std::shared_ptr<CHAR> DecryptText(_In_ CONST CHAR* pszEnctryText, _In_ UINT uEncryptLength, _In_ CONST CHAR* pszKeyStream);
	};
}




#endif // !__LIBTOOLS_ALGORITHMLIB_ENCRYP_RC4_H__
