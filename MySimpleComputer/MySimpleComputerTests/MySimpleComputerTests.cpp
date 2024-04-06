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

			sc_memorySet(1, -10);

			int value;
			int memGetGood = sc_memoryGet(1, &value);
			Assert::AreEqual(memGetGood, 1);
			Assert::AreEqual(value, -10);

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
			sc_regInit();

			sc_regSet(1, 1);
			
			int value;
			int regGetGood = sc_regGet(1, &value);
			Assert::AreEqual(regGetGood, 1);
			Assert::AreEqual(value, 1);
		}


		/// <summary>
		/// ���� ������ ������ � ��
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
		/// ���� ������ ������ �� ��
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
		/// �������� ������ ��������� ������ ���������� (������ �� ���������)
		/// </summary>
		TEST_METHOD(TestExecution)
		{
			sc_regInit();
			sc_memoryInit();

			// 00: READ A 09		;(���� �)
			sc_memorySet(9, 3);

			// 01: READ B 10		;(���� �)
			sc_memorySet(10, 8190);

			// 02: LOAD 09			;(�������� � � �����������)
			int loadCommand;
			sc_commandEncode(LOAD, 9, &loadCommand);

			// 03: SUB 10			;(������ �)
			int subCommand;
			sc_commandEncode(SUB, 10, &subCommand);

			// 04: JNEG 07			;(������� �� 07, ���� �������������)
			int jnegCommand;
			sc_commandEncode(JNEG, 7, &jnegCommand);

			// 05: WRITE 09			;(����� �)
			int writeACommand;
			sc_commandEncode(WRITE, 9, &writeACommand);

			// 06: HALT 00			;(�������)
			int firstHaltCommand;
			sc_commandEncode(HALT, 0, &firstHaltCommand);

			// 07: WRITE 10			;(����� �)
			int writeBCommand;
			sc_commandEncode(WRITE, 10, &writeBCommand);

			// 08: HALT 00			;(�������)
			int secondHaltCommand;
			sc_commandEncode(HALT, 0, &secondHaltCommand);



			int command, operand;
			sc_commandDecode(loadCommand, &command, &operand);

			// �������� ������������
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
