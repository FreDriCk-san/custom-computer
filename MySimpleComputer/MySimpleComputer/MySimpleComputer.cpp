#include "pch.h"
#include <iostream>
#include <fstream>
#include "MySimpleComputer.h"
#include <vector>

using namespace std;

// DLL internal state variables:
const int _memorySize = 100;
static int _memory[_memorySize];

/*
* �������� �������� ����������:
* 
* EAX [32] - �����������;
* EBX [32] - ����;
* ECX [32] - �������;
* EDX [32] - ������� ������
* EBP [32] - ��������� ����
* ESP [32] - ��������� �����
* ESI [32] - ������ ���������
* EDI [32] - ������ ��������
* EFLAGS [32] - ������� ������
* EIP [32] - ��������� ����������
* CS [16] - ���������� �������
* DS [16] - ���������� �������
* ES [16] - ���������� �������
* FS [16] - ���������� �������
* GS [16] - ���������� �������
* SS [16] - ���������� �������
* 
* 
* �������� EAX, EBX, ECX, EDX � ��� �������� ������ ����������. 
* ��� ����� ����������� ���������� (��� �� ��������� �����������), ������ � ��� ����� ������� ����� ����������.
* 
* �������� EBP, ESP, ESI, EDI � ��� ����� �������� ������ ����������.
* ��� ����� ��� ����� ���������� ����������.
* � ��� ����� ����� ������� ���������������� ������, �� ������ ��� ����� ��� ����� ���������, ����� �� �������� ������������ ���������.
* 
*/


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

	if (address > _memorySize || address < 0)
		return -1;

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

	if (address > _memorySize || address < 0)
		return -1;

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


	return -1;
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
/// <para/> -1: ������;
/// </returns>
int sc_regSet(int reg, int value) {


	return -1;
}


/// <summary>
/// ���������� �������� ���������� �����.
/// ���� ������ ������������ ����� ��������, �� ������� ����������� � �������
/// </summary>
/// <param name="reg">�������</param>
/// <param name="value">��������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������;
/// </returns>
int sc_regGet(int reg, int* value) {


	return -1;
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