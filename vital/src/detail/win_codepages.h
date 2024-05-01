#pragma once
#ifndef _VITAL_WIN_CODEPAGES_
#define _VITAL_WIN_CODEPAGES_

#include "../vtl/core.h"

#include <cstring>

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

struct windows_encoding
{
    const char* name;
    unsigned codepage;
    unsigned was_tested;
};

inline bool operator<(const windows_encoding& l, const char* name)
{
    return strcmp(l.name, name) < 0;
}

static windows_encoding all_windows_encodings[] = 
{
    { "asmo708",                708, 0 },
    { "big5",                   950, 0 },
    { "cp1025",                 21025, 0 },
    { "cp1250",                 1250, 0 },
    { "cp1251",                 1251, 0 },
    { "cp1252",                 1252, 0 },
    { "cp1253",                 1253, 0 },
    { "cp1254",                 1254, 0 },
    { "cp1255",                 1255, 0 },
    { "cp1256",                 1256, 0 },
    { "cp1257",                 1257, 0 },
    { "cp866",                  866, 0 },
    { "cp874",                  874, 0 },
    { "cp875",                  875, 0 },
    { "cp932",                  932, 0 },
    { "cp936",                  936, 0 },
    { "csiso2022jp",            50221, 0 },
    { "dos720",                 720, 0 },
    { "dos862",                 862, 0 },
    { "euccn",                  51936, 0 },
    { "eucjp",                  20932, 0 },
    { "euckr",                  51949, 0 },
    { "gb18030",                54936, 0 },
    { "gb2312",                 936, 0 },
    { "gbk",                    936, 0 },
    { "hzgb2312",               52936, 0 },
    { "ibm00858",               858, 0 },
    { "ibm00924",               20924, 0 },
    { "ibm01026",               1026, 0 },
    { "ibm01047",               1047, 0 },
    { "ibm01140",               1140, 0 },
    { "ibm01141",               1141, 0 },
    { "ibm01142",               1142, 0 },
    { "ibm01143",               1143, 0 },
    { "ibm01144",               1144, 0 },
    { "ibm01145",               1145, 0 },
    { "ibm01146",               1146, 0 },
    { "ibm01147",               1147, 0 },
    { "ibm01148",               1148, 0 },
    { "ibm01149",               1149, 0 },
    { "ibm037",                 37, 0 },
    { "ibm273",                 20273, 0 },
    { "ibm277",                 20277, 0 },
    { "ibm278",                 20278, 0 },
    { "ibm280",                 20280, 0 },
    { "ibm284",                 20284, 0 },
    { "ibm285",                 20285, 0 },
    { "ibm290",                 20290, 0 },
    { "ibm297",                 20297, 0 },
    { "ibm420",                 20420, 0 },
    { "ibm423",                 20423, 0 },
    { "ibm424",                 20424, 0 },
    { "ibm437",                 437, 0 },
    { "ibm500",                 500, 0 },
    { "ibm737",                 737, 0 },
    { "ibm775",                 775, 0 },
    { "ibm850",                 850, 0 },
    { "ibm852",                 852, 0 },
    { "ibm855",                 855, 0 },
    { "ibm857",                 857, 0 },
    { "ibm860",                 860, 0 },
    { "ibm861",                 861, 0 },
    { "ibm863",                 863, 0 },
    { "ibm864",                 864, 0 },
    { "ibm865",                 865, 0 },
    { "ibm869",                 869, 0 },
    { "ibm870",                 870, 0 },
    { "ibm871",                 20871, 0 },
    { "ibm880",                 20880, 0 },
    { "ibm905",                 20905, 0 },
    { "ibmthai",                20838, 0 },
    { "iso2022jp",              50220, 0 },
    { "iso2022jp",              50222, 0 },
    { "iso2022kr",              50225, 0 },
    { "iso88591",               28591, 0 },
    { "iso885913",              28603, 0 },
    { "iso885915",              28605, 0 },
    { "iso88592",               28592, 0 },
    { "iso88593",               28593, 0 },
    { "iso88594",               28594, 0 },
    { "iso88595",               28595, 0 },
    { "iso88596",               28596, 0 },
    { "iso88597",               28597, 0 },
    { "iso88598",               28598, 0 },
    { "iso88598i",              38598, 0 },
    { "iso88599",               28599, 0 },
    { "johab",                  1361, 0 },
    { "koi8r",                  20866, 0 },
    { "koi8u",                  21866, 0 },
    { "ksc56011987",            949, 0 },
    { "latin1",                 28591, 0 },
    { "latin1",                 1252, 0 },
    { "macintosh",              10000, 0 },
    { "ms936",                  936, 0 },
    { "shiftjis",               932, 0 },
    { "sjis",                   932, 0 },
    { "unicodefffe",            1201, 0 },
    { "usascii",                20127, 0 },
    { "utf16",                  1200, 0 },
    { "utf32",                  12000, 0 },
    { "utf32be",                12001, 0 },
    { "utf7",                   65000, 0 },
    { "utf8",                   65001, 0 },
    { "windows1250",            1250, 0 },
    { "windows1251",            1251, 0 },
    { "windows1252",            1252, 0 },
    { "windows1253",            1253, 0 },
    { "windows1254",            1254, 0 },
    { "windows1255",            1255, 0 },
    { "windows1256",            1256, 0 },
    { "windows1257",            1257, 0 },
    { "windows1258",            1258, 0 },
    { "windows874",             874, 0 },
    { "windows932",             932, 0 },
    { "windows936",             936, 0 },
    { "xchinesecns",            20000, 0 },
    { "xchineseeten",           20002, 0 },
    { "xcp20001",               20001, 0 },
    { "xcp20003",               20003, 0 },
    { "xcp20004",               20004, 0 },
    { "xcp20005",               20005, 0 },
    { "xcp20261",               20261, 0 },
    { "xcp20269",               20269, 0 },
    { "xcp20936",               20936, 0 },
    { "xcp20949",               20949, 0 },
    { "xcp50227",               50227, 0 },
    { "xebcdickoreanextended",  20833, 0 },
    { "xeuropa",                29001, 0 },
    { "xia5",                   20105, 0 },
    { "xia5german",             20106, 0 },
    { "xia5norwegian",          20108, 0 },
    { "xia5swedish",            20107, 0 },
    { "xisciias",               57006, 0 },
    { "xisciibe",               57003, 0 },
    { "xisciide",               57002, 0 },
    { "xisciigu",               57010, 0 },
    { "xisciika",               57008, 0 },
    { "xisciima",               57009, 0 },
    { "xisciior",               57007, 0 },
    { "xisciipa",               57011, 0 },
    { "xisciita",               57004, 0 },
    { "xisciite",               57005, 0 },
    { "xmacarabic",             10004, 0 },
    { "xmacce",                 10029, 0 },
    { "xmacchinesesimp",        10008, 0 },
    { "xmacchinesetrad",        10002, 0 },
    { "xmaccroatian",           10082, 0 },
    { "xmaccyrillic",           10007, 0 },
    { "xmacgreek",              10006, 0 },
    { "xmachebrew",             10005, 0 },
    { "xmacicelandic",          10079, 0 },
    { "xmacjapanese",           10001, 0 },
    { "xmackorean",             10003, 0 },
    { "xmacromanian",           10010, 0 },
    { "xmacthai",               10021, 0 },
    { "xmacturkish",            10081, 0 },
    { "xmacukrainian",          10017, 0 }
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_WIN_CODEPAGES_