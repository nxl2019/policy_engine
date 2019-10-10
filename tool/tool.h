#ifndef TOOL_H
#define TOOL_H

#include <assert.h>
#include <locale>
#include <codecvt>
#ifdef WIN32
#include <windows.h>
#include <Shlobj.h>
#else
#include <dlfcn.h>
#include <sys/stat.h>
#include <strings.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#define sprintf_s snprintf
#endif
#include <cctype>
#include <iomanip>
#include <sstream>

namespace CommonFun {
    std::string ToUTF8(const std::string& src, std::string& dst);
    std::string ToUTF8(const std::u16string& src, std::string& dst);
    std::string ToUTF8(const std::u32string& src, std::string& dst);
    std::string ToUTF8(const std::wstring& src, std::string& dst);


    std::string FromUTF8(const std::string& src, std::string& dst);
    std::u16string FromUTF8(const std::string& src, std::u16string& dst);
    std::u32string FromUTF8(const std::string& src, std::u32string& dst);
    std::wstring FromUTF8(const std::string& src, std::wstring& dst);

    std::string UrlEncode(const std::string &value);

    int StrCaseCmp(const char *s1, const char *s2);
}


#endif