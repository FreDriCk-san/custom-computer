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
// 1) ������������ ��� ���������� ��������;		[�]			[CF] - Carry flag
// 2) ������ ������� �� 0;						[�]			[ZF] - Zero flag
// 3) ������ ������ �� ������� ������;			[�]			[OF] - Overflow flag
// 4) ������������� �������� ���������;			[�]			[IF] - Interrupt enable flag
// 5) ������� �������� �������.					[�]			[MF] - Method failure flag
int _regFlag;


/// <summary>
/// �����������, ������������ ��� ���������� �������� ������ � ����������� ��������
/// </summary>
int _accumulator;


/// <summary>
/// ������� �������, ����������� �� ����� ������ ������, � ������� �������� ������� ����������� ��������
/// </summary>
int _instructionCounter;






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
/// <para/> -2: �������� ����� �� ������� ������ (����� 14 ��������);
/// </returns>
int sc_memorySet(int address, int value) {

	if (address > _memorySize || address < 0) {
		_regFlag = OF;
		return -1;
	}

	// � ������ ����� �������� �������� �� ����������� 14 ��������
	if (std::abs(value) >= (1 << 13)) {
		_regFlag = CF;
		return -2;
	}

	// ���� ����� �������������, � ������� ������ (14 ������) ������������ '1'
	if (value < 0) {
		int absValue = (~value) + 1;
		value = absValue | (1 << 13);
	}

	// ��� ������� ������� �� �������� ���������� �������� � 15 ������ ����� �������� 1
	int newValue = value | (1 << 14);

	_memory[address] = newValue;

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
/// <para/> -2: ������� ������� �������;
/// </returns>
int sc_memoryGet(int address, int* value) {

	if (address > _memorySize || address < 0) {
		_regFlag = OF;
		return -1;
	}

	int recValue = _memory[address];

	// �������� 15 �������, ����� �������� ����������� �� �������
	bool isNumber = (bool((1 << 14) & recValue));
	if (!isNumber) {
		return -2;
	}

	// ���������� '0' � 15 ������
	int clearValue = recValue & ~(1 << 14);

	// ���� 14 ������ ����� 1, �� ����� �������� �������������
	// � ����� ������ �������� 14 ������ � �������� �� -1
	bool isNegative = (bool((1 << 13) & clearValue));
	if (isNegative) {
		clearValue = (clearValue & ~(1 << 13)) * -1;
	}

	*value = clearValue;

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
/// <para/> -1: ������� �� �������������� ��;
/// <para/> -2: ������� ����� �� ������� ������ (�������� 7 ��������);
/// <para/> -3: � ����������� ������ �� �������� ���������� �����;
/// </returns>
int sc_commandEncode(int command, int operand, int* value) {

	if (!commandExists(command)) {
		//_regFlag = MF;
		return -1;
	}

	// ��� �������� ������������� 7 ��������
	// �������� �� ����������� �������� � ������
	if (std::abs(operand) >= (1 << 6)) {
		//_regFlag = CF;
		return -2;
	}

	// ���� ����� �������������, � ������� ������ (7 ������) ������������ '1'
	if (operand < 0) {
		int absValue = (~operand) + 1;
		operand = absValue | (1 << 6);
	}

	// ��� ���� �������� �������� 7 �������� � �������� (14 - 7)
	int subCommand = command << 7;

	int result = subCommand | operand;

	// ����� ����� � ����������� ������, ���� ����� �������� �������
	int commandIndex = -1;
	for (int i = 0; i < _memorySize; ++i) {
		if (_memory[i] == 0) {
			commandIndex = i;
			break;
		}
	}

	if (commandIndex == -1) {
		return -3;
	}

	_memory[commandIndex] = result;
	
	*value = result;

	return 1;
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
/// <para/> -1: �� ������� ����� �������;
/// <para/> -2: ������� ������������ �����;
/// </returns>
int sc_commandDecode(int value, int* command, int* operand) {

	// ����� ������� � �������, ��� ����������� �������
	int commandIndex = -1;
	for (int i = 0; i < _memorySize; ++i) {
		if (_memory[i] == value) {
			commandIndex = i;
			break;
		}
	}

	if (commandIndex == -1) {
		return -1;
	}

	int recCommand = _memory[commandIndex];

	// �������� 15 �������, ����� �������� ����������� �� �����
	bool isNumber = (bool((1 << 14) & recCommand));
	if (isNumber) {
		return -2;
	}

	// ������������� ������� �������
	int subCommand = recCommand >> 7;

	// ��������� �������� ������� ��� ����������� ��������
	int subOperand = recCommand;
	for (int i = 13; i > 6; --i) {
		subOperand = subOperand & ~(1 << i);
	}

	// ���� 7 ������ ����� 1, �� ����� �������� �������������
	// � ����� ������ �������� 7 ������ � �������� �� -1
	bool isNegative = (bool((1 << 6) & subOperand));
	if (isNegative) {
		subOperand = (subOperand & ~(1 << 6)) * -1;
	}

	*command = subCommand;
	*operand = subOperand;

	_instructionCounter = commandIndex;

	executeCommand(subCommand, subOperand);

	// TODO: ����� ������� �������????

	return 1;
}


/// <summary>
/// �������� �������� �������� ������
/// </summary>
/// <returns></returns>
int sc_instructGet() {
	return _instructionCounter;
}


/// <summary>
/// ���������� �������� �������� ������
/// </summary>
/// <param name="value">����� ��������</param>
void sc_instructSet(int value) {
	_instructionCounter = value;
}


/// <summary>
/// �������� �������� ������������
/// </summary>
/// <returns></returns>
int sc_accumGet() {
	return _accumulator;
}


/// <summary>
/// ���������� �������� ������������
/// </summary>
/// <param name="value"></param>
void sc_accumSet(int value) {
	_accumulator = value;
}


/// <summary>
/// �������� �� ������� ������� ����������
/// </summary>
/// <param name="command">����� �������</param>
/// <returns>
/// <para/> true: �����;
/// <para/> false: ������;
/// </returns>
bool commandExists(int command) {

	// �������� �����\������
	if (command == 10 || command == 11)
		return true;

	// �������� ��������\�������� � �����������
	if (command == 20 || command == 21)
		return true;

	// �������������� ��������
	if (command >= 30 || command <= 33)
		return true;

	// �������� �������� ����������
	if (command >= 40 || command <= 43)
		return true;

	// ���������������� �������
	if (command >= 51 || command <= 76)
		return true;

	return false;
}


/// <summary>
/// ��������� ������� ��
/// </summary>
/// <param name="command">����� �������</param>
/// <param name="operand">�������</param>
/// <returns>
/// <para/> 1: �����;
/// <para/> -1: ������� �� �����������;
/// </returns>
int executeCommand(int command, int operand) {

	int clearValue, idCom, idOper;
	switch (command)
	{
		case LOAD:
			// ���������� '0' � 15 ������
			clearValue = _memory[operand] & ~(1 << 14);
			_accumulator = clearValue;
			return 1;

		case SUB:
			// ���������� '0' � 15 ������
			clearValue = _memory[operand] & ~(1 << 14);
			_accumulator -= clearValue;
			return 1;

		case JNEG:
			if (_accumulator < 0) {
				// TODO: ����������� � ��������� �� ���������
				// �������� ������� ������������� ������ � _instructionCounter
				int recCommand = _memory[operand];
				sc_commandDecode(recCommand, &idCom, &idOper);
			}
			return 1;

		case WRITE:
			// ���������� '0' � 15 ������
			clearValue = _memory[operand] & ~(1 << 14);
			std::cout << clearValue;
			return 1;

		case HALT:
			// ��� �������� ���������???
			sc_regInit();
			sc_memoryInit();
			return 1;
	}

	return -1;
}