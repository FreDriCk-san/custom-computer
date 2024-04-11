#include <cassert>
#include <iostream>

#include "../../MySimpleComputer/MySimpleComputer/MySimpleComputer.h"
#include "../../MyReadKey/MyReadKey/MyReadKey.h"
#include "../../MyBigChars/MyBigChars/MyBigChars.h"
#include "../../CommonTerm/CommonTerm/CommonTerm.h"

using namespace std;


void TestMemSet();
void TestMemGet();
void TestMemSave();
void TestMemLoad();
void TestFlagSet();
void TestFlagGet();
void TestMethodEncode();
void TestMethodDecode();
void TestExecution();

int main(){
	ct_init(false);
	rk_init(true);

    TestMemSet();
    TestMemGet();
    
    TestMemSave();
    TestMemLoad();

    TestFlagSet();
    TestFlagGet();

    TestMethodEncode();
    TestMethodDecode();

    TestExecution();

	cout << "Test complete!" << endl;
}

// Тест изменения значения ячейки памяти
void TestMemSet(){
    sc_reset();

	int memSetGood = sc_memorySet(0, 25);
	assert(memSetGood == 1);
	
	int memSetBad = sc_memorySet(101, 54);
	assert(memSetBad == -1);
}


// Тест инициализации ячейки памяти
void TestMemGet(){
    sc_reset();

	int encodedValue;
	sc_memorySetAndEncode(1, -10, &encodedValue);

	int value;
	int memGetGood = sc_memoryGetAndDecode(1, &value);
	assert(memGetGood == 1);
	assert(value == -10);

	// Симуляция изменения значения value
	value = 123;

	int memSetBad = sc_memoryGet(101, &value);
	assert(memSetBad == -1);
	assert(value == 123);
}


// Тест сохранения содержимого памяти в бинарный файл
void TestMemSave(){
    sc_reset();

	int encodedValue;
	sc_memorySetAndEncode(0, 25, &encodedValue);
	sc_memorySetAndEncode(1, 30, &encodedValue);
	sc_memorySetAndEncode(3, 60, &encodedValue);

	std::string path = "TestMemSave.bin";
	int memSaveGood = sc_memorySave(bc_convertStringToCharArr(path));
	assert(memSaveGood == 1);

	path = "";
	int memSaveBad = sc_memorySave(bc_convertStringToCharArr(path));
	assert(memSaveBad == -1);
}


// Тест загрузки содержимого памяти в бинарный файл
void TestMemLoad(){
    sc_reset();

	int encodedValue;
	sc_memorySetAndEncode(0, 25, &encodedValue);
	sc_memorySetAndEncode(1, 30, &encodedValue);
	sc_memorySetAndEncode(3, 60, &encodedValue);

	std::string path = "TestMemSave.bin";
	int memSaveGood = sc_memorySave(bc_convertStringToCharArr(path));
	assert(memSaveGood == 1);

	// Симуляция сброса
	sc_reset();

	int memLoadGood = sc_memoryLoad(bc_convertStringToCharArr(path));
	assert(memLoadGood == 1);
	
	int value;
	int memGetGood = sc_memoryGetAndDecode(1, &value);
	assert(memGetGood == 1);
	assert(value == 30);

	path = "";
	int memLoadBad = sc_memoryLoad(bc_convertStringToCharArr(path));
	assert(memLoadBad == -1);
}


// Тест изменения значения регистра флагов
void TestFlagSet(){
    sc_reset();

	int regSetGood = sc_regSet(1, 1);
	assert(regSetGood == 1);
}


// Тест получения значения регистра флагов
void TestFlagGet(){
    sc_reset();

	sc_regSet(1, 1);
			
	int value;
	int regGetGood = sc_regGet(1, &value);

	assert(regGetGood == 1);
	assert(value == 1);
}


// Тест записи метода в ЦП
void TestMethodEncode(){
    sc_reset();

	// 10 = 1010
	// 20 = 10100
	// result: 0_0001010_0010100 = 1300

	int commandPtr;
	int goodEncode = sc_commandSetAndEncode(0, 10, 20, &commandPtr);
	assert(goodEncode == 1);
	assert(commandPtr == 1300);


	// 10 = 1010
	// 98 = ??
	// result: ?? = ??

	int isSuccess = sc_commandSetAndEncode(1, 10, 98, &commandPtr);
	assert(isSuccess == 1);

	int commandId;
	int operand;
	int goodDecode = sc_commandDecode(commandPtr, &commandId, &operand);
	assert(commandId == 10);
	assert(operand == 98);
}


// Тест чтения метода из ЦП
void TestMethodDecode(){

    sc_reset();

	// 10 = 1010
	// 20 = 10100
	// result: 0_0001010_0010100 = 1300

	int commandPtr;
	sc_commandSetAndEncode(0, 10, 20, &commandPtr);
			
	int commandId;
	int operand;
	int goodDecode = sc_commandDecode(commandPtr, &commandId, &operand);
	assert(goodDecode == 1);
	assert(commandId == 10);
	assert(operand == 20);


	// 76 = 1001100
	// -20 = 1010100
	// result: 0_1001100_1010100 = 9812

	sc_commandSetAndEncode(1, 76, 20, &commandPtr);

	sc_commandDecode(commandPtr, &commandId, &operand);
	assert(commandId == 76);
	assert(operand == 20);
}


// Проверка работы некоторых команд процессора (пример из методички)
void TestExecution(){
    sc_reset();

	// 00: READ A 09		;(Ввод А)
	int readACommand;
	sc_commandSetAndEncode(0, READ, 9, &readACommand);

	// 01: READ B 10		;(Ввод В)
	int readBCommand;
	sc_commandSetAndEncode(1, READ, 10, &readBCommand);

	// 02: LOAD 09			;(Загрузка А в аккумулятор)
	int loadCommand;
	sc_commandSetAndEncode(2, LOAD, 9, &loadCommand);

	// 03: SUB 10			;(Отнять В)
	int subCommand;
	sc_commandSetAndEncode(3, SUB, 10, &subCommand);

	// 04: JNEG 07			;(Переход на 07, если отрицательное)
	int jnegCommand;
	sc_commandSetAndEncode(4, JNEG, 7, &jnegCommand);

	// 05: WRITE 09			;(Вывод А)
	int writeACommand;
	sc_commandSetAndEncode(5, WRITE, 9, &writeACommand);

	// 06: HALT 00			;(Останов)
	int firstHaltCommand;
	sc_commandSetAndEncode(6, HALT, 0, &firstHaltCommand);

	// 07: WRITE 10			;(Вывод В)
	int writeBCommand;
	sc_commandSetAndEncode(7, WRITE, 10, &writeBCommand);

	// 08: HALT 00			;(Останов)
	int secondHaltCommand;
	sc_commandSetAndEncode(8, HALT, 0, &secondHaltCommand);

	std::string path = "TestExecution.bin";
	int memSaveGood = sc_memorySave(bc_convertStringToCharArr(path));
	assert(memSaveGood == 1);

	sc_countrolUnit();
	sc_run();

	int accum = sc_accumGet();
	cout << endl << accum << endl;
	
	/*sc_runByStep();
	sc_runByStep();
	sc_runByStep();
	sc_runByStep();
	sc_runByStep();
	sc_runByStep();
	sc_runByStep();
	sc_runByStep();
	sc_runByStep();*/
}