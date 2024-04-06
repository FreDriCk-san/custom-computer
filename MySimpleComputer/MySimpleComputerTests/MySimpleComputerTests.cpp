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
		/// ���� ��������� �������� ������ ������
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
		/// ���� ������������� ������ ������
		/// </summary>
		TEST_METHOD(TestMemGet)
		{
			sc_memoryInit();

			sc_memorySet(1, 25);

			int value;
			int memGetGood = sc_memoryGet(1, &value);
			Assert::AreEqual(memGetGood, 1);
			Assert::AreEqual(value, 25);

			// ��������� ��������� �������� value
			value = 123;

			int memSetBad = sc_memoryGet(101, &value);
			Assert::AreEqual(memSetBad, -1);
			Assert::AreEqual(value, 123);
		}


		/// <summary>
		/// ���� ���������� ����������� ������ � �������� ����
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
		/// ���� �������� ����������� ������ � �������� ����
		/// </summary>
		TEST_METHOD(TestMemLoad)
		{
			sc_memoryInit();

			sc_memorySet(0, 25);
			sc_memorySet(1, 30);
			sc_memorySet(3, 60);

			int memSaveGood = sc_memorySave("TestMemSave.bin");
			Assert::AreEqual(memSaveGood, 1);

			// ��������� ������ ����������� ������
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
		/// ���� ��������� �������� �������� ������
		/// </summary>
		TEST_METHOD(TestFlagSet)
		{
			sc_regInit();

			int regSetGood = sc_regSet(1, 1);
			Assert::AreEqual(regSetGood, 1);
		}


		/// <summary>
		/// ���� ��������� �������� �������� ������
		/// </summary>
		TEST_METHOD(TestFlagGet)
		{
			int flagInit = sc_regInit();

			sc_regSet(1, 1);
			
			int value;
			int regGetGood = sc_regGet(1, &value);
			Assert::AreEqual(regGetGood, 1);
			Assert::AreEqual(value, 1);
		}
	};
}
