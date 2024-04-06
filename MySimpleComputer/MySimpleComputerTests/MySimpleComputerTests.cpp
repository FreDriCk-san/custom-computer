#include "pch.h"
#include "CppUnitTest.h"
#include "../MySimpleComputer/MySimpleComputer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MySimpleComputerTests
{
	TEST_CLASS(MySimpleComputerTests)
	{
	public:
		
		/// <summary>
		/// Тест изменения значения ячейки памяти
		/// </summary>
		TEST_METHOD(TestMemSet)
		{
			sc_memoryInit();

			int memSetGood = sc_memorySet(0, 25);
			Assert::AreEqual(memSetGood, 1);
			
			int memSetBad = sc_memorySet(101, 54);
			Assert::AreEqual(memSetBad, -1);
		}


		/// <summary>
		/// Тест инициализации ячейки памяти
		/// </summary>
		TEST_METHOD(TestMemGet)
		{
			sc_memoryInit();

			sc_memorySet(1, -10);

			int value;
			int memGetGood = sc_memoryGet(1, &value);
			Assert::AreEqual(memGetGood, 1);
			Assert::AreEqual(value, -10);

			// Симуляция изменения значения value
			value = 123;

			int memSetBad = sc_memoryGet(101, &value);
			Assert::AreEqual(memSetBad, -1);
			Assert::AreEqual(value, 123);
		}


		/// <summary>
		/// Тест сохранения содержимого памяти в бинарный файл
		/// </summary>
		TEST_METHOD(TestMemSave)
		{
			sc_memoryInit();

			sc_memorySet(0, 25);
			sc_memorySet(1, 30);
			sc_memorySet(3, 60);

			int memSaveGood = sc_memorySave("TestMemSave.bin");
			Assert::AreEqual(memSaveGood, 1);

			int memSaveBad = sc_memorySave("");
			Assert::AreEqual(memSaveBad, -1);
		}


		/// <summary>
		/// Тест загрузки содержимого памяти в бинарный файл
		/// </summary>
		TEST_METHOD(TestMemLoad)
		{
			sc_memoryInit();

			sc_memorySet(0, 25);
			sc_memorySet(1, 30);
			sc_memorySet(3, 60);

			int memSaveGood = sc_memorySave("TestMemSave.bin");
			Assert::AreEqual(memSaveGood, 1);

			// Симуляция сброса оперативной памяти
			sc_memoryInit();

			int memLoadGood = sc_memoryLoad("TestMemSave.bin");
			Assert::AreEqual(memLoadGood, 1);

			int value;
			int memGetGood = sc_memoryGet(1, &value);
			Assert::AreEqual(memGetGood, 1);
			Assert::AreEqual(value, 30);

			int memLoadBad = sc_memoryLoad("");
			Assert::AreEqual(memLoadBad, -1);
		}


		/// <summary>
		/// Тест изменения значения регистра флагов
		/// </summary>
		TEST_METHOD(TestFlagSet)
		{
			sc_regInit();

			int regSetGood = sc_regSet(1, 1);
			Assert::AreEqual(regSetGood, 1);
		}


		/// <summary>
		/// Тест получения значения регистра флагов
		/// </summary>
		TEST_METHOD(TestFlagGet)
		{
			sc_regInit();

			sc_regSet(1, 1);
			
			int value;
			int regGetGood = sc_regGet(1, &value);
			Assert::AreEqual(regGetGood, 1);
			Assert::AreEqual(value, 1);
		}


		/// <summary>
		/// Тест записи метода в ЦП
		/// </summary>
		TEST_METHOD(TestMethodEncode)
		{
			sc_memoryInit();

			// 10 = 1010
			// 20 = 10100
			// result: 0_0001010_0010100 = 1300

			int commandPtr;
			int goodEncode = sc_commandEncode(10, 20, &commandPtr);
			Assert::AreEqual(goodEncode, 1);
			Assert::AreEqual(commandPtr, 1300);


			// 10 = 1010
			// -20 = 1010100
			// result: 0_0001010_1010100 = 1364

			sc_commandEncode(10, -20, &commandPtr);
			Assert::AreEqual(commandPtr, 1364);
		}


		/// <summary>
		/// Тест чтения метода из ЦП
		/// </summary>
		TEST_METHOD(TestMethodDecode)
		{
			sc_memoryInit();

			// 10 = 1010
			// 20 = 10100
			// result: 0_0001010_0010100 = 1300

			int commandPtr;
			sc_commandEncode(10, 20, &commandPtr);
			
			int commandId;
			int operand;
			int goodDecode = sc_commandDecode(commandPtr, &commandId, &operand);
			Assert::AreEqual(goodDecode, 1);
			Assert::AreEqual(commandId, 10);
			Assert::AreEqual(operand, 20);


			// 76 = 1001100
			// -20 = 1010100
			// result: 0_1001100_1010100 = 9812

			sc_commandEncode(76, -20, &commandPtr);

			sc_commandDecode(commandPtr, &commandId, &operand);
			Assert::AreEqual(commandId, 76);
			Assert::AreEqual(operand, -20);
		}


		/// <summary>
		/// Проверка работы некоторых команд процессора (пример из методички)
		/// </summary>
		TEST_METHOD(TestExecution)
		{
			sc_regInit();
			sc_memoryInit();

			// 00: READ A 09		;(Ввод А)
			sc_memorySet(9, 3);

			// 01: READ B 10		;(Ввод В)
			sc_memorySet(10, 8190);

			// 02: LOAD 09			;(Загрузка А в аккумулятор)
			int loadCommand;
			sc_commandEncode(LOAD, 9, &loadCommand);

			// 03: SUB 10			;(Отнять В)
			int subCommand;
			sc_commandEncode(SUB, 10, &subCommand);

			// 04: JNEG 07			;(Переход на 07, если отрицательное)
			int jnegCommand;
			sc_commandEncode(JNEG, 7, &jnegCommand);

			// 05: WRITE 09			;(Вывод А)
			int writeACommand;
			sc_commandEncode(WRITE, 9, &writeACommand);

			// 06: HALT 00			;(Останов)
			int firstHaltCommand;
			sc_commandEncode(HALT, 0, &firstHaltCommand);

			// 07: WRITE 10			;(Вывод В)
			int writeBCommand;
			sc_commandEncode(WRITE, 10, &writeBCommand);

			// 08: HALT 00			;(Останов)
			int secondHaltCommand;
			sc_commandEncode(HALT, 0, &secondHaltCommand);



			int command, operand;
			sc_commandDecode(loadCommand, &command, &operand);

			// Проверка аккумулятора
			int accum = sc_accumGet();
			Assert::AreEqual(accum, 3);
			
			sc_commandDecode(subCommand, &command, &operand);

			// 3 - 8190 = -8187
			accum = sc_accumGet();
			Assert::AreEqual(accum, -8187);

			sc_commandDecode(jnegCommand, &command, &operand);

			sc_commandDecode(writeACommand, &command, &operand);

			sc_commandDecode(writeBCommand, &command, &operand);

			sc_commandDecode(secondHaltCommand, &command, &operand);
		}
	};
}
