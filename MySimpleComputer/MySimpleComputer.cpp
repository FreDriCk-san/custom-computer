#include "pch.h"
#include <iostream>
#include "MySimpleComputer.h"

using namespace std;

// DLL internal state variables:
static unsigned long long previous_;

/// <summary>
/// Show some inner string
/// </summary>
/// <returns>Method result</returns>
int show_string() {
	cout << "KEKW";
	return 1;
}