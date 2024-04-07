#include <iostream>
#include <fstream>
#include "MySimpleComputer.h"
#include <vector>

using namespace std;

int _regFlag;

// Оперативная память
const int _memorySize = 100;
static int _memory[_memorySize];


/// <summary>
/// Инициализирует оперативную память Simple Computer, задавая всем её ячейкам нулевые значения.
/// В качестве «оперативной памяти» используется массив целых чисел, определенный статически в рамках библиотеки. 
/// Размер массива равен 100 элементам.
/// </summary>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка;
/// </returns>
int sc_memoryInit() {

	for (int i = 0; i < _memorySize; ++i) {
		_memory[i] = 0;
	}

	return 1;
}


/// <summary>
/// Задает значение указанной ячейки памяти как value.
/// Если адрес выходит за допустимые границы, то устанавливается флаг «выход за границы памяти» и работа функции прекращается с ошибкой;
/// </summary>
/// <param name="address">Адрес ячейки памяти</param>
/// <param name="value">Значение</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка, выход за допустимые границы;
/// </returns>
int sc_memorySet(int address, int value) {

	if (address > _memorySize || address < 0) {
		_regFlag = OF;
		return -1;
	}

	_memory[address] = value;

	return 1;
}


/// <summary>
/// Возвращает значение указанной ячейки памяти в value. 
/// Если адрес выходит за допустимые границы, то устанавливается флаг «выход за границы памяти» и работа функции прекращается с ошибкой. 
/// Значение value в этом случае не изменяется
/// </summary>
/// <param name="address">Адрес ячейки памяти</param>
/// <param name="value">Значение</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка, выход за допустимые границы;
/// </returns>
int sc_memoryGet(int address, int* value) {

	if (address > _memorySize || address < 0) {
		_regFlag = OF;
		return -1;
	}

	*value = _memory[address];

	return 1;
}


/// <summary>
/// Сохраняет содержимое памяти в файл в бинарном виде.
/// </summary>
/// <param name="filename">Файл для сохранения</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка создания файла;
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
/// Загружает из указанного файла содержимое оперативной памяти.
/// </summary>
/// <param name="filename">Файл для загрузки</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка считывания файла;
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
/// Инициализирует регистр флагов нулевым значением
/// </summary>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка;
/// </returns>
int sc_regInit(void) {

	_regFlag = 0;
	return 1;
}


/// <summary>
/// Устанавливает значение указанного регистра флагов. 
/// Для номеров регистров флагов должны использоваться маски, задаваемые макросами (#define). 
/// Если указан недопустимый номер регистра или некорректное значение, то функция завершается с ошибкой.
/// </summary>
/// <param name="reg">Регистр</param>
/// <param name="value">Значение</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Недопустимый номер регистра;
/// <para/> -2: Недопустимое значение флага;
/// </returns>
int sc_regSet(int reg, int value) {

	int index = reg - 1;
	if (index < 0 || index > 5)
		return -1;

	int offset = 1 << index;
	switch (value)
	{
		// Выполняется операция поразрядного И между регистром флагов и числом.
		// При этом значение offset маскируется, путём инвертирования числа, 
		// содержащего единицу в том разряде, в котором необходим 0
		case 0:
			_regFlag = _regFlag & (~offset);
			break;

		// Выполняется операция поразрядного ИЛИ между регистром флагов и числом
		case 1:
			_regFlag = _regFlag | offset;
			break;

		default:
			return -2;
	}

	return 1;
}


/// <summary>
/// Возвращает значение указанного флага.
/// Если указан недопустимый номер регистра, то функция завершается с ошибкой
/// </summary>
/// <param name="reg">Регистр</param>
/// <param name="value">Значение</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Недопустимый номер регистра;
/// </returns>
int sc_regGet(int reg, int* value) {

	int index = reg - 1;
	if (index < 0 || index > 5)
		return -1;

	// Переменная _regFlag будет сдвинута вправо таким образом,
	// что требуемый флаг окажется в первом разряде.
	int offset = _regFlag >> index;

	// После чего будет произведена опреация логического умножения на 1
	// Т.е. все разряды числа, кроме первого, будут умножены на 0.
	*value = offset & 0x1;

	return 1;
}


/// <summary>
/// Кодирует команду с указанным номером и операндом и помещает результат в value. 
/// Если указаны неправильные значения для команды или операнда, то функция завершается с ошибкой.
/// В этом случае значение value не изменяется.
/// </summary>
/// <param name="command">Номер команды</param>
/// <param name="operand">Операнд</param>
/// <param name="value">Значение</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка;
/// </returns>
int sc_commandEncode(int command, int operand, int* value) {


	return -1;
}


/// <summary>
/// Декодирует значение как команду Simple Computer. 
/// Если декодирование невозможно, то устанавливается флаг «ошибочная команда» и функция завершается с ошибкой
/// </summary>
/// <param name="value">Значение</param>
/// <param name="command">Номер команды</param>
/// <param name="operand">Операнд</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка;
/// </returns>
int sc_commandDecode(int value, int* command, int* operand) {


	return -1;
}