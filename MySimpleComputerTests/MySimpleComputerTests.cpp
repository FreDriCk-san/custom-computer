#include "pch.h"
#include "CppUnitTest.h"
#include "../MySimpleComputer/MySimpleComputer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MySimpleComputerTests
{
	TEST_CLASS(MySimpleComputerTests)
	{
	public:
		
		TEST_METHOD(TestDll)
		{
			int res = show_string();
			Assert::AreEqual(res, 1);
		}
	};
}
