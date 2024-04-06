#include "pch.h"
#include <iostream>
#include <fstream>
#include "MySimpleComputer.h"
#include <vector>

using namespace std;

// ����������� ������
const int _memorySize = 100;
static int _memory[_memorySize];


// ������� ������ �������� 5 ��������:
// 1) ������������ ��� ���������� ��������;		[CF] - Carry flag
// 2) ������ ������� �� 0;						[ZF] - Zero flag
// 3) ������ ������ �� ������� ������;			[OF] - Overflow flag
// 4) ������������� �������� ���������;			[IF] - Interrupt enable flag
// 5) ������� �������� �������.					[MF] - Method failure flag
int _regFlag;
#define CF 1
#define ZF 2
#define OF 3
#define IF 4
#define MF 5




/// <summary>
/// �������������� ����������� ������ Simple Computer, ������� ���� � ������� ������� ��������.
/// � �������� ������������ ������ ������������ ������ ����� �����, ������������ ���������� � ������ ����������. 
/// ������ ������� ����� 100 ���������.
/// </summary>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������;
/// </returns>
int sc_memoryInit() {

	for (int i = 0; i < _memorySize; ++i) {
		_memory[i] = 0;
	}

	return 1;
}


/// <summary>
/// ������ �������� ��������� ������ ������ ��� value.
/// ���� ����� ������� �� ���������� �������, �� ��������������� ���� ������ �� ������� ������ � ������ ������� ������������ � �������;
/// </summary>
/// <param name="address">����� ������ ������</param>
/// <param name="value">��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������, ����� �� ���������� �������;
/// </returns>
int sc_memorySet(int address, int value) {

	if (address > _memorySize || address < 0) {
		_regFlag = OF;
		return -1;
	}

	_memory[address] = value;

	return 1;
}


/// <summary>
/// ���������� �������� ��������� ������ ������ � value. 
/// ���� ����� ������� �� ���������� �������, �� ��������������� ���� ������ �� ������� ������ � ������ ������� ������������ � �������. 
/// �������� value � ���� ������ �� ����������
/// </summary>
/// <param name="address">����� ������ ������</param>
/// <param name="value">��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������, ����� �� ���������� �������;
/// </returns>
int sc_memoryGet(int address, int* value) {

	if (address > _memorySize || address < 0) {
		_regFlag = OF;
		return -1;
	}

	*value = _memory[address];

	return 1;
}


/// <summary>
/// ��������� ���������� ������ � ���� � �������� ����.
/// </summary>
/// <param name="filename">���� ��� ����������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������ �������� �����;
/// </returns>
int sc_memorySave(char* filename) {

	std::string filePath(filename);

	std::ofstream myfile;
	myfile.open(filePath, std::ios::out | std::ios::binary);

	if (!myfile) {
		return -1;
	}

	for (int i = 0; i < _memorySize; ++i) {
		int value = _memory[i];
		myfile.write((char*)&value, sizeof(value));
	}

	myfile.close();
	
	return 1;
}


/// <summary>
/// ��������� �� ���������� ����� ���������� ����������� ������.
/// </summary>
/// <param name="filename">���� ��� ��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������ ���������� �����;
/// </returns>
int sc_memoryLoad(char* filename) {

	std::string filePath(filename);

	std::ifstream myfile;
	myfile.open(filePath, std::ios::in | std::ios::binary);

	if (!myfile) {
		return -1;
	}

	myfile.seekg(0, myfile.end);
	size_t length = myfile.tellg();
	myfile.seekg(0, myfile.beg);

	if (length == 0 || length <= _memorySize) {
		myfile.close();
		return -1;
	}
	
	for (int i = 0; i < _memorySize; ++i)
		myfile.read((char*)&_memory[i], sizeof(int));

	myfile.close();

	return 1;
}


/// <summary>
/// �������������� ������� ������ ������� ���������
/// </summary>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������;
/// </returns>
int sc_regInit(void) {

	_regFlag = 0;
	return 1;
}


/// <summary>
/// ������������� �������� ���������� �������� ������. 
/// ��� ������� ��������� ������ ������ �������������� �����, ���������� ��������� (#define). 
/// ���� ������ ������������ ����� �������� ��� ������������ ��������, �� ������� ����������� � �������.
/// </summary>
/// <param name="reg">�������</param>
/// <param name="value">��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������������ ����� ��������;
/// <para/> -2: ������������ �������� �����;
/// </returns>
int sc_regSet(int reg, int value) {

	int index = reg - 1;
	if (index < 0 || index > 5)
		return -1;

	int offset = 1 << index;
	switch (value)
	{
		// ����������� �������� ������������ � ����� ��������� ������ � ������.
		// ��� ���� �������� offset �����������, ���� �������������� �����, 
		// ����������� ������� � ��� �������, � ������� ��������� 0
		case 0:
			_regFlag = _regFlag & (~offset);
			break;

		// ����������� �������� ������������ ��� ����� ��������� ������ � ������
		case 1:
			_regFlag = _regFlag | offset;
			break;

		default:
			return -2;
	}

	return 1;
}


/// <summary>
/// ���������� �������� ���������� �����.
/// ���� ������ ������������ ����� ��������, �� ������� ����������� � �������
/// </summary>
/// <param name="reg">�������</param>
/// <param name="value">��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������������ ����� ��������;
/// </returns>
int sc_regGet(int reg, int* value) {

	int index = reg - 1;
	if (index < 0 || index > 5)
		return -1;

	// ���������� _regFlag ����� �������� ������ ����� �������,
	// ��� ��������� ���� �������� � ������ �������.
	int offset = _regFlag >> index;

	// ����� ���� ����� ����������� �������� ����������� ��������� �� 1
	// �.�. ��� ������� �����, ����� �������, ����� �������� �� 0.
	*value = offset & 0x1;

	return 1;
}


/// <summary>
/// �������� ������� � ��������� ������� � ��������� � �������� ��������� � value. 
/// ���� ������� ������������ �������� ��� ������� ��� ��������, �� ������� ����������� � �������.
/// � ���� ������ �������� value �� ����������.
/// </summary>
/// <param name="command">����� �������</param>
/// <param name="operand">�������</param>
/// <param name="value">��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������;
/// </returns>
int sc_commandEncode(int command, int operand, int* value) {


	return -1;
}


/// <summary>
/// ���������� �������� ��� ������� Simple Computer. 
/// ���� ������������� ����������, �� ��������������� ���� ���������� ������� � ������� ����������� � �������
/// </summary>
/// <param name="value">��������</param>
/// <param name="command">����� �������</param>
/// <param name="operand">�������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������;
/// </returns>
int sc_commandDecode(int value, int* command, int* operand) {


	return -1;
}