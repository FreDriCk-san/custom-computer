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
			sc_reset();

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
			sc_reset();

			int encodedValue;
			sc_memorySetAndEncode(1, -10, &encodedValue);

			int value;
			int memGetGood = sc_memoryGetAndDecode(1, &value);
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
			sc_reset();

			int encodedValue;
			sc_memorySetAndEncode(0, 25, &encodedValue);
			sc_memorySetAndEncode(1, 30, &encodedValue);
			sc_memorySetAndEncode(3, 60, &encodedValue);

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
			sc_reset();

			int encodedValue;
			sc_memorySetAndEncode(0, 25, &encodedValue);
			sc_memorySetAndEncode(1, 30, &encodedValue);
			sc_memorySetAndEncode(3, 60, &encodedValue);

			int memSaveGood = sc_memorySave("TestMemSave.bin");
			Assert::AreEqual(memSaveGood, 1);

			// Симуляция сброса
			sc_reset();

			int memLoadGood = sc_memoryLoad("TestMemSave.bin");
			Assert::AreEqual(memLoadGood, 1);

			int value;
			int memGetGood = sc_memoryGetAndDecode(1, &value);
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
			sc_reset();

			int regSetGood = sc_regSet(1, 1);
			Assert::AreEqual(regSetGood, 1);
		}


		/// <summary>
		/// Тест получения значения регистра флагов
		/// </summary>
		TEST_METHOD(TestFlagGet)
		{
			sc_reset();

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
			sc_reset();

			// 10 = 1010
			// 20 = 10100
			// result: 0_0001010_0010100 = 1300

			int commandPtr;
			int goodEncode = sc_commandSetAndEncode(10, 20, &commandPtr);
			Assert::AreEqual(goodEncode, 1);
			Assert::AreEqual(commandPtr, 1300);


			// 10 = 1010
			// -20 = 1010100
			// result: 0_0001010_1010100 = 1364

			sc_commandSetAndEncode(10, -20, &commandPtr);
			Assert::AreEqual(commandPtr, 1364);
		}


		/// <summary>
		/// Тест чтения метода из ЦП
		/// </summary>
		TEST_METHOD(TestMethodDecode)
		{
			sc_reset();

			// 10 = 1010
			// 20 = 10100
			// result: 0_0001010_0010100 = 1300

			int commandPtr;
			sc_commandSetAndEncode(10, 20, &commandPtr);
			
			int commandId;
			int operand;
			int goodDecode = sc_commandDecode(commandPtr, &commandId, &operand);
			Assert::AreEqual(goodDecode, 1);
			Assert::AreEqual(commandId, 10);
			Assert::AreEqual(operand, 20);


			// 76 = 1001100
			// -20 = 1010100
			// result: 0_1001100_1010100 = 9812

			sc_commandSetAndEncode(76, -20, &commandPtr);

			sc_commandDecode(commandPtr, &commandId, &operand);
			Assert::AreEqual(commandId, 76);
			Assert::AreEqual(operand, -20);
		}


		/// <summary>
		/// Проверка работы некоторых команд процессора (пример из методички)
		/// </summary>
		TEST_METHOD(TestExecution)
		{
			sc_reset();

			// 00: READ A 09		;(Ввод А)
			int readACommand;
			sc_commandSetAndEncode(READ, 9, &readACommand);

			// 01: READ B 10		;(Ввод В)
			int readBCommand;
			sc_commandSetAndEncode(READ, 10, &readBCommand);

			// 02: LOAD 09			;(Загрузка А в аккумулятор)
			int loadCommand;
			sc_commandSetAndEncode(LOAD, 9, &loadCommand);

			// 03: SUB 10			;(Отнять В)
			int subCommand;
			sc_commandSetAndEncode(SUB, 10, &subCommand);

			// 04: JNEG 07			;(Переход на 07, если отрицательное)
			int jnegCommand;
			sc_commandSetAndEncode(JNEG, 7, &jnegCommand);

			// 05: WRITE 09			;(Вывод А)
			int writeACommand;
			sc_commandSetAndEncode(WRITE, 9, &writeACommand);

			// 06: HALT 00			;(Останов)
			int firstHaltCommand;
			sc_commandSetAndEncode(HALT, 0, &firstHaltCommand);

			// 07: WRITE 10			;(Вывод В)
			int writeBCommand;
			sc_commandSetAndEncode(WRITE, 10, &writeBCommand);

			// 08: HALT 00			;(Останов)
			int secondHaltCommand;
			sc_commandSetAndEncode(HALT, 0, &secondHaltCommand);

			int memSaveGood = sc_memorySave("TestExecution.bin");
			Assert::AreEqual(memSaveGood, 1);

			sc_run();

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
	};
}
