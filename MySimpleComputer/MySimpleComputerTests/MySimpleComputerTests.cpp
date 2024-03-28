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
			int memInit = sc_memoryInit();

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
			int memInit = sc_memoryInit();

			sc_memorySet(1, 25);

			int value;
			int memGetGood = sc_memoryGet(1, &value);
			Assert::AreEqual(memGetGood, 1);
			Assert::AreEqual(value, 25);

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
			int memInit = sc_memoryInit();

			sc_memorySet(0, 25);
			sc_memorySet(1, 30);
			sc_memorySet(3, 60);

			int memSaveGood = sc_memorySave("TestMemSave.bin");
			Assert::AreEqual(memSaveGood, 1);

			int memSaveBad = sc_memorySave("");
			Assert::AreEqual(memSaveGood, -1);
		}


		TEST_METHOD(TestMemLoad)
		{
			int memInit = sc_memoryInit();

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
		}

	};
}
