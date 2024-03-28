#pragma once

#ifdef MYSIMPLECOMPUTER_EXPORTS
#define MYSIMPLECOMPUTER_API __declspec(dllexport)
#else
#define MYSIMPLECOMPUTER_API __declspec(dllimport)
#endif

extern "C" MYSIMPLECOMPUTER_API int sc_memoryInit();

extern "C" MYSIMPLECOMPUTER_API int sc_memorySet(int address, int value);

extern "C" MYSIMPLECOMPUTER_API int sc_memoryGet(int address, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_memorySave(char* filename);

extern "C" MYSIMPLECOMPUTER_API int sc_memoryLoad(char* filename);

extern "C" MYSIMPLECOMPUTER_API int sc_regInit(void);

extern "C" MYSIMPLECOMPUTER_API int sc_regSet(int reg, int value);

extern "C" MYSIMPLECOMPUTER_API int sc_regGet(int reg, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_commandEncode(int command, int operand, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_commandDecode(int value, int* command, int* operand);