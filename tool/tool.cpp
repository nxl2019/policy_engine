#include "tool.h"

namespace CommonFun {

    std::string ToUTF8(const std::string &src, std::string &dst) {
        dst = src;
        return dst;
    }

    std::string ToUTF8(const std::u16string &src, std::string &dst) {
#if (defined WIN32) && (_MSC_VER >= 1900)
        auto p = reinterpret_cast<const int16_t *>(src.data());
          dst = std::wstring_convert< std::codecvt_utf8_utf16<int16_t>, int16_t >{}.to_bytes(p, p + src.size());
#else
        dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(src);
#endif
        return dst;
    }

    std::string ToUTF8(const std::u32string &src, std::string &dst) {
#if (defined WIN32) && (_MSC_VER >= 1900)
        auto p = reinterpret_cast<const __int32 *>(src.data());
          dst = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.to_bytes(p, p + src.size());
#else
        dst = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.to_bytes(src);
#endif
        return dst;
    }

    std::string ToUTF8(const std::wstring &src, std::string &dst) {
        dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(src);
        return dst;
    }


    std::string FromUTF8(const std::string &src, std::string &dst) {
        dst = src;
        return dst;
    }

    std::u16string FromUTF8(const std::string &src, std::u16string &dst) {
#if (defined WIN32) && (_MSC_VER >= 1900)
        auto p = std::wstring_convert< std::codecvt_utf8<int16_t>, int16_t >{}.from_bytes(src);
          auto t = reinterpret_cast<const char16_t *>(p.data());
          dst = std::u16string(t, t + p.size());
#else
        dst = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(src);
#endif
        return dst;
    }

    std::u32string FromUTF8(const std::string &src, std::u32string &dst) {
#if (defined WIN32) && (_MSC_VER >= 1900)
        auto p = std::wstring_convert< std::codecvt_utf8<__int32>, __int32 >{}.from_bytes(src);
          auto t = reinterpret_cast<const char32_t *>(p.data());
          dst = std::u32string(t, t + p.size());
#else
        dst = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(src);
#endif
        return dst;
    }

    std::wstring FromUTF8(const std::string &src, std::wstring &dst) {
        dst = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(src);
        return dst;
    }

    std::string UrlEncode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char) c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    int StrCaseCmp(const char *s1, const char *s2) {
#ifndef WIN32
        return strcasecmp(s1, s2);
#else
        return stricmp(s1, s2);
#endif
    }
}