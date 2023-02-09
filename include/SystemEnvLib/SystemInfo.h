#ifndef __LIBTOOLS_SYSTEMENVLIB_SYSTEMINFO_H__
#define __LIBTOOLS_SYSTEMENVLIB_SYSTEMINFO_H__



class CSystemInfo
{
public:
	enum class em_System_Version
	{
		Unknow,
		WindowsXp,
		WindowsServer2003,
		WindowsVista,
		WindowsServer2008,
		WindowServer2008R2,
		Windows7,
		WindowsServer2012,
		Windows8,
		Windows10
	};
public:
	CSystemInfo() = default;
	~CSystemInfo() = default;

	// Get Current Os Version
	static em_System_Version GetSystemVersion();
private:

};



#endif // !__LIBTOOLS_SYSTEMENVLIB_SYSTEMINFO_H__
