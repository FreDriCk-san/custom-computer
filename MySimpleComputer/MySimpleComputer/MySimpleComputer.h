#pragma once

#ifdef MYSIMPLECOMPUTER_EXPORTS
#define MYSIMPLECOMPUTER_API __declspec(dllexport)
#else
#define MYSIMPLECOMPUTER_API __declspec(dllimport)
#endif


#pragma region ������ ������
// ������������ ��� ���������� ��������
#define CF 1

// ������ ������� �� 0
#define ZF 2

// ������ ������ �� ������� ������
#define OF 3

// ������������� �������� ���������
#define IF 4

// ������� �������� �������
#define MF 5
#pragma endregion


#pragma region ������ ������ ��

#pragma region �������� �����/������
// ���� � ��������� � ��������� ������ ������ � ��������� ������������
#define READ 10

// ����� �� �������� �������� ��������� ������ ������
#define WRITE 11
#pragma endregion


#pragma region �������� ��������/�������� � �����������
// �������� � ����������� �������� �� ���������� ������ ������
#define LOAD 20

// ��������� �������� �� ������������ �� ���������� ������ ������
#define STORE 21
#pragma endregion


#pragma region �������������� ��������
// ��������� �������� ����� � ������������ � ����� �� ��������� ������ ������ (��������� � ������������)
#define ADD 30

// �������� �� ����� � ������������ ����� �� ��������� ������ ������ (��������� � ������������)
#define SUB 31

// ��������� ������� ����� � ������������ �� ����� �� ��������� ������ ������ (��������� � ������������)
#define DIVIDE 32

// ��������� ������������ ����� � ������������ �� ����� �� ��������� ������ ������ (��������� � ������������)
#define MUL 33
#pragma endregion


#pragma region �������� �������� ����������
// ������� � ���������� ������ ������
#define JUMP 40

// ������� � ���������� ������ ������, ���� � ������������ ��������� ������������� �����
#define JNEG 41

// ������� � ���������� ������ ������, ���� � ������������ ��������� ����
#define JZ 42

// �������, ����������� ��� ���������� ������ ���������
#define HALT 43
#pragma endregion


#pragma region ���������������� �������
// �������� �������� ����� � ������������ � ��������� ���������� � ��������� ������ ������
#define NOT 51

// ���������� �������� � ����� ���������� ������������ � ������ �� ���������� ������ (��������� � ������������)
#define AND 52

// ���������� �������� ��� ����� ���������� ������������ � ������ �� ���������� ������ (��������� � ������������)
#define OR 53

// ���������� �������� ����������� ��� ����� ���������� ������������ � ������ �� ���������� ������ (��������� � ������������)
#define XOR 54

// ������� � ���������� ������ ������, ���� � ������������ ��������� ������������� �����
#define JNS 55

// ������� � ���������� ������ ������, ���� ��� �������� ��������� ������������
#define JC 56

// ������� � ���������� ������ ������, ���� ��� �������� �� ��������� ������������
#define JNC 57

// ������� � ���������� ������ ������, ���� ��������� ���������� �������� ������
#define JP 58

// ������� � ���������� ������ ������, ���� ��������� ���������� �������� ��������
#define JNP 59

// ���������� �������� ����� ����������� ��������� ������ ������ ����� (��������� � ������������)
#define CHL 60

// ���������� �������� ����� ����������� ��������� ������ ������ ������ (��������� � ������������)
#define SHR 61

// ����������� �������� ����� ����������� ��������� ������ ������ ����� (��������� � ������������)
#define RCL 62

// ����������� �������� ����� ����������� ��������� ������ ������ ������ (��������� � ������������)
#define RCR 63

// ��������� ��������������� ���� ����������� ��������� ������ ������ (��������� � ������������)
#define NEG 64

// �������� ����������� ��������� ������ ������ � ������� ������, ����� ������� ��������� � ������������ (��������� � ������������)
#define ADDC 65

// ��������� �� ����������� ��������� ������ ������ ����������� ������ ������, ����� ������� ��������� � ������������ (��������� � ������������)
#define SUBC 66

// ���������� �������� ����� ����������� ���������� ������� ������ ����� �� ���������� �������� ��������� � ������������ (��������� � ������������)
#define LOGLC 67

// ���������� �������� ����� ����������� ���������� ������� ������ ������ �� ���������� �������� ��������� � ������������ (��������� � ������������)
#define LOGRC 68

// ����������� �������� ����� ����������� ���������� ������� ������ ����� �� ���������� �������� ��������� � ������������ (��������� � ������������)
#define RCCL 69

// ����������� �������� ����� ����������� ���������� ������� ������ ������ �� ���������� �������� ��������� � ������������ (��������� � ������������)
#define RCCR 70

// ����������� ����������� ��������� ������ ������ � ������, ����� ������� ������ � ������������
#define MOVA 71

// ����������� ����������� ������ ������, ����� ������� ���������� � ������������ � ��������� ������ ������
#define MOVR 72

// ����������� ����������� ��������� ������ ������ � ������ ������, ����� ������� ��������� � ������ ������, �� ������� ��������� �������� ������������
#define MOVCA 73

// ����������� � ��������� ������� ������ ����������� ������� ������, ����� �������� ��������� � ������� ������ ��������� � ������������
#define MOVCR 74

// �������� ����������� ��������� ������ ������ � ������� ������, ����� ������� ��������� � ������ ������, ��������� � ������������ (��������� � ������������)
#define ADDC 75

// ��������� �� ����������� ��������� ������ ������ ����������� ������ ������, ����� ������� ��������� � ������ ������, ��������� � ������������ (��������� � ������������)
#define SUBC 76
#pragma endregion

#pragma endregion


extern "C" MYSIMPLECOMPUTER_API int sc_memoryInit();

extern "C" MYSIMPLECOMPUTER_API int sc_memorySet(int address, int value);

extern "C" MYSIMPLECOMPUTER_API int sc_memoryEncode(int value, int* number);

extern "C" MYSIMPLECOMPUTER_API int sc_memorySetAndEncode(int address, int value, int* number);

extern "C" MYSIMPLECOMPUTER_API int sc_memoryGet(int address, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_numberDecode(int value, int* number);

extern "C" MYSIMPLECOMPUTER_API int sc_memoryGetAndDecode(int address, int* number);

extern "C" MYSIMPLECOMPUTER_API int sc_memorySave(char* filename);

extern "C" MYSIMPLECOMPUTER_API int sc_memoryLoad(char* filename);

extern "C" MYSIMPLECOMPUTER_API int sc_regInit(void);

extern "C" MYSIMPLECOMPUTER_API int sc_regSet(int reg, int value);

extern "C" MYSIMPLECOMPUTER_API int sc_regGet(int reg, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_commandSet(int command);

extern "C" MYSIMPLECOMPUTER_API int sc_commandEncode(int command, int operand, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_commandSetAndEncode(int command, int operand, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_commandGet(int address, int* value);

extern "C" MYSIMPLECOMPUTER_API int sc_commandDecode(int value, int* command, int* operand);

extern "C" MYSIMPLECOMPUTER_API int sc_commandGetAndDecode(int address, int* command, int* operand);

extern "C" MYSIMPLECOMPUTER_API int sc_instructGet();

extern "C" MYSIMPLECOMPUTER_API void sc_instructSet(int value);

extern "C" MYSIMPLECOMPUTER_API int sc_accumGet();

extern "C" MYSIMPLECOMPUTER_API void sc_accumSet(int value);

extern "C" MYSIMPLECOMPUTER_API void sc_reset();

extern "C" MYSIMPLECOMPUTER_API bool sc_isNumber(int address);

extern "C" MYSIMPLECOMPUTER_API int sc_run();

extern "C" MYSIMPLECOMPUTER_API int sc_runByStep();