#pragma once

#ifdef MYSIMPLECOMPUTER_EXPORTS
#define MYSIMPLECOMPUTER_API __declspec(dllexport)
#else
#define MYSIMPLECOMPUTER_API __declspec(dllimport)
#endif

extern "C" MYSIMPLECOMPUTER_API int show_string();