#include "pch.h"
#include <iostream>
#include <fstream>
#include "MySimpleComputer.h"
#include <vector>

using namespace std;

bool commandExists(int command);
int executeCommand(int command, int operand);

// Оперативная память
const int _memorySize = 100;
static int _memory[_memorySize];


// Регистр флагов содержит 5 разрядов:
// 1) Переполнение при выполнении операции;		[П]			[CF] - Carry flag
// 2) Ошибка деления на 0;						[О]			[ZF] - Zero flag
// 3) Ошибка выхода за границы памяти;			[М]			[OF] - Overflow flag
// 4) Игнорирование тактовых импульсов;			[Т]			[IF] - Interrupt enable flag
// 5) Указана неверная команда.					[Е]			[MF] - Method failure flag
int _regFlag;


/// <summary>
/// Аккумулятор, используется для временного хранения данных и результатов операции
/// </summary>
int _accumulator;


/// <summary>
/// Счётчик комманд, указывающий на адрес ячейки памяти, в которой хранится текущая выполняемая операция
/// </summary>
int _instructionCounter = -1;






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
		sc_regSet(OF, 1);
		return -1;
	}

	_memory[address] = value;

	return 1;
}


/// <summary>
/// Возвращает закодированное представление числа в памяти
/// </summary>
/// <param name="value">Число для кодирования</param>
/// <param name="number">Результат кодирования</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Значение вышло за границы памяти (более 14 разрядов);
/// </returns>
int sc_memoryEncode(int value, int* number) {

	// В память можно записать значение не превышающее 14 разрядов
	if (std::abs(value) >= (1 << 13)) {
		//_regFlag = CF;
		return -1;
	}

	// Если число отрицательное, в старший разряд (14 разряд) записывается '1'
	if (value < 0) {
		int absValue = (~value) + 1;
		value = absValue | (1 << 13);
	}

	// Для отличия команды от значения необходимо записать в 15 разряд числа значение 1
	int newValue = value | (1 << 14);

	*number = newValue;

	return 1;
}


/// <summary>
/// Запись закодированного значения в указанную ячейку памяти
/// </summary>
/// <param name="address">Адрес ячейки памяти</param>
/// <param name="value">Значение</param>
/// <param name="number">Результат кодирования</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Значение вышло за границы памяти (более 14 разрядов);
/// <para/> -2: Ошибка, выход за допустимые границы;
/// </returns>
int sc_memorySetAndEncode(int address, int value, int* number) {

	int encodedValue;
	if (sc_memoryEncode(value, &encodedValue) != 1) {
		return -1;
	}

	if (sc_memorySet(address, encodedValue) != 1) {
		return -2;
	}

	*number = encodedValue;

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
		sc_regSet(OF, 1);
		return -1;
	}

	*value = _memory[address];

	return 1;
}


/// <summary>
/// Возвращает значение числа в указанной ячейке памяти
/// </summary>
/// <param name="value">Содержимое ячейки памяти</param>
/// <param name="number">Число</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Попытка достать команду;
/// </returns>
int sc_numberDecode(int value, int* number) {

	// Проверка 15 разряда, вдруг пытаются достучаться до команды
	bool isNumber = (bool((1 << 14) & value));
	if (!isNumber) {
		return -1;
	}

	// Записываем '0' в 15 разряд
	int clearValue = value & ~(1 << 14);

	// Если 14 разряд равен 1, то число является отрицательным
	// В таком случае зануляем 14 разряд и умножаем на -1
	bool isNegative = (bool((1 << 13) & clearValue));
	if (isNegative) {
		clearValue = (clearValue & ~(1 << 13)) * -1;
	}

	*number = clearValue;

	return 1;
}


/// <summary>
/// Возвращает декодированное число из адреса ячейки памяти
/// </summary>
/// <param name="address">Адрес ячейки памяти</param>
/// <param name="number">Число</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка, выход за допустимые границы;
/// <para/> -2: Попытка достать команду;
/// </returns>
int sc_memoryGetAndDecode(int address, int* number) {

	int value;
	if (sc_memoryGet(address, &value) != 1) {
		return -1;
	}

	int clearNumber;
	if (sc_numberDecode(value, &clearNumber) != 1) {
		return -2;
	}

	*number = clearNumber;

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
/// Установка команды в оперативную память
/// </summary>
/// <param name="command">Закодированная команда</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Команда не поддерживается ЦП;
/// <para/> -2: Попытка записать число как команду;
/// <para/> -3: В оперативной памяти не осталось свободного места;
/// </returns>
int sc_commandSet(int command) {

	if (!commandExists(command)) {
		sc_regSet(MF, 1);
		return -1;
	}

	// Проверка 15 разряда, вдруг пытаются записать число
	bool isNumber = (bool((1 << 14) & command));
	if (isNumber) {
		return -2;
	}

	// Поиск места в оперативной памяти, куда можно записать команду
	int commandIndex = -1;
	for (int i = 0; i < _memorySize; ++i) {
		if (_memory[i] == 0) {
			commandIndex = i;
			break;
		}
	}

	if (commandIndex == -1) {
		return -2;
	}

	_memory[commandIndex] = command;

	return 1;
}


/// <summary>
/// Кодирует команду с указанным номером и операндом и помещает результат в value. 
/// Если указаны неправильные значения для команды или операнда, то функция завершается с ошибкой.
/// В этом случае значение value не изменяется.
/// </summary>
/// <param name="command">Номер команды</param>
/// <param name="operand">Операнд</param>
/// <param name="value">Результат в памяти</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Команда не поддерживается ЦП;
/// <para/> -2: Операнд вышел за границы памяти (максимум 7 разрядов);
/// </returns>
int sc_commandEncode(int command, int operand, int* value) {

	if (!commandExists(command)) {
		return -1;
	}

	// Для операнда предназначены 7 разрядов
	// Проверка на вместимость значения в память
	if (std::abs(operand) >= (1 << 6)) {
		//_regFlag = CF;
		return -2;
	}

	// Если число отрицательное, в старший разряд (7 разряд) записывается '1'
	if (operand < 0) {
		int absValue = (~operand) + 1;
		operand = absValue | (1 << 6);
	}

	// Для кода операции выделены 7 разрядов в середине (14 - 7)
	int subCommand = command << 7;

	int result = subCommand | operand;

	*value = result;

	return 1;
}


/// <summary>
/// Установка команды в оперативную память с последующим её кодированием
/// </summary>
/// <param name="command">Номер команды</param>
/// <param name="operand">Операнд</param>
/// <param name="value">Результат в памяти</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка кодирования команды
/// <para/> -2: Ошибка записи команды
/// </returns>
int sc_commandSetAndEncode(int command, int operand, int* value) {

	int encodedValue;
	if (sc_commandEncode(command, operand, &encodedValue) != 1) {
		return -1;
	}

	if (sc_commandSet(encodedValue) != 1) {
		return -2;
	}

	*value = encodedValue;

	return 1;
}


/// <summary>
/// Возвращает значение команды по адресу
/// </summary>
/// <param name="address">Адрес ячейки памяти</param>
/// <param name="value">Закодированная команда</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка чтения команды;
/// </returns>
int sc_commandGet(int address, int* value) {
	int memValue;
	if (sc_memoryGet(address, &memValue) != 1) {
		return -1;
	}

	*value = memValue;

	return 1;
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
/// <para/> -1: Не удалось найти команду;
/// <para/> -2: Попытка декодировать число;
/// </returns>
int sc_commandDecode(int value, int* command, int* operand) {

	// Поиск индекса в массиве, где расположена команда
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

	// Проверка 15 разряда, вдруг пытаются достучаться до числа
	bool isNumber = (bool((1 << 14) & recCommand));
	if (isNumber) {
		return -2;
	}

	// Инициализация индекса команды
	int subCommand = recCommand >> 7;

	// Зануление индексов команды для определения операнда
	int subOperand = recCommand;
	for (int i = 13; i > 6; --i) {
		subOperand = subOperand & ~(1 << i);
	}

	// Если 7 разряд равен 1, то число является отрицательным
	// В таком случае зануляем 7 разряд и умножаем на -1
	bool isNegative = (bool((1 << 6) & subOperand));
	if (isNegative) {
		subOperand = (subOperand & ~(1 << 6)) * -1;
	}

	*command = subCommand;
	*operand = subOperand;

	return 1;
}


/// <summary>
/// Возвращает декодированную команду из ячейки памяти
/// </summary>
/// <param name="address">Адрес ячейки памяти</param>
/// <param name="command">Номер команды</param>
/// <param name="operand">Операнд</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Ошибка чтения из памяти;
/// <para/> -2: Ошибка декодирования команды;
/// </returns>
int sc_commandGetAndDecode(int address, int* command, int* operand) {

	int memValue;
	if (sc_commandGet(address, &memValue) != 1) {
		return -1;
	}

	int clearCommand;
	int clearOperand;
	if (sc_commandDecode(memValue, &clearCommand, &clearOperand) != 1) {
		return -2;
	}

	*command = clearCommand;
	*operand = clearOperand;

	return 1;
}


/// <summary>
/// Получить значение счётчика команд
/// </summary>
/// <returns></returns>
int sc_instructGet() {
	return _instructionCounter;
}


/// <summary>
/// Установить значение счётчика команд
/// </summary>
/// <param name="value">Новое значение</param>
void sc_instructSet(int value) {
	_instructionCounter = value;
}


/// <summary>
/// Получить значение аккумулятора
/// </summary>
/// <returns></returns>
int sc_accumGet() {
	return _accumulator;
}


/// <summary>
/// Установить значение аккумулятора
/// </summary>
/// <param name="value"></param>
void sc_accumSet(int value) {
	_accumulator = value;
}


/// <summary>
/// Сброс компьютера
/// </summary>
void sc_reset() {
	sc_regInit();
	sc_memoryInit();
	_accumulator = 0;
	_instructionCounter = -1;
}


/// <summary>
/// Проверка адреса ячейки памяти на наличие числа
/// </summary>
/// <param name="address"></param>
/// <returns>
/// 
/// 
/// </returns>
bool sc_isNumber(int address) {

	if (address > _memorySize || address < 0) {
		return false;
	}

	int recCommand = _memory[address];

	// Проверка 15 разряда, вдруг пытаются достучаться до числа
	bool isNumber = (bool((1 << 14) & recCommand));

	return isNumber;
}


/// <summary>
/// Последовательный запуск всех команд
/// </summary>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Не удалось найти команды в оперативной памяти;
/// </returns>
int sc_run() {

	int result = -1;

	int start = _instructionCounter;
	if (start < 0)
		start = 0;

	for (int i = start; i < _memorySize; ) {

		if (sc_isNumber(i)) {
			i++;
			continue;
		}

		int recCommand = _memory[i];

		// Инициализация индекса команды
		int subCommand = recCommand >> 7;

		// Зануление индексов команды для определения операнда
		int subOperand = recCommand;
		for (int i = 13; i > 6; --i) {
			subOperand = subOperand & ~(1 << i);
		}

		// Если 7 разряд равен 1, то число является отрицательным
		// В таком случае зануляем 7 разряд и умножаем на -1
		bool isNegative = (bool((1 << 6) & subOperand));
		if (isNegative) {
			subOperand = (subOperand & ~(1 << 6)) * -1;
		}

		_instructionCounter = i;

		executeCommand(subCommand, subOperand);

		if (_instructionCounter == -1)
			return result;

		if (i == _instructionCounter)
			i++;
		else
			i = _instructionCounter;

		result = 1;
	}

	return result;
}


/// <summary>
/// Пошаговое выполнение команд процессора
/// </summary>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Не удалось найти команды в оперативной памяти;
/// </returns>
int sc_runByStep() {

	int start = _instructionCounter;
	if (start < 0)
		start = 0;

	for (int i = start; i < _memorySize; ) {

		if (sc_isNumber(i)) {
			i++;
			continue;
		}

		int recCommand = _memory[i];

		// Инициализация индекса команды
		int subCommand = recCommand >> 7;

		// Зануление индексов команды для определения операнда
		int subOperand = recCommand;
		for (int i = 13; i > 6; --i) {
			subOperand = subOperand & ~(1 << i);
		}

		// Если 7 разряд равен 1, то число является отрицательным
		// В таком случае зануляем 7 разряд и умножаем на -1
		bool isNegative = (bool((1 << 6) & subOperand));
		if (isNegative) {
			subOperand = (subOperand & ~(1 << 6)) * -1;
		}

		_instructionCounter = i;

		executeCommand(subCommand, subOperand);

		if (_instructionCounter == -1)
			return 1;

		_instructionCounter = i + 1;

		return 1;
	}

	return -1;
}


/// <summary>
/// Проверка на наличие команды процессора
/// </summary>
/// <param name="command">Номер команды</param>
/// <returns>
/// <para/> true: Успех;
/// <para/> false: Ошибка;
/// </returns>
bool commandExists(int command) {

	// Операции ввода\вывода
	if (command == 10 || command == 11)
		return true;

	// Операции загрузки\выгрузки в аккумулятор
	if (command == 20 || command == 21)
		return true;

	// Арифметические операции
	if (command >= 30 || command <= 33)
		return true;

	// Операции передачи управления
	if (command >= 40 || command <= 43)
		return true;

	// Пользовательские функции
	if (command >= 51 || command <= 76)
		return true;

	return false;
}


/// <summary>
/// Выполнить команду ЦП
/// </summary>
/// <param name="command">Номер команды</param>
/// <param name="operand">Операнд</param>
/// <returns>
/// <para/> 1: Успех;
/// <para/> -1: Команда не реализована;
/// <para/> -2: Ошибка выполнения команды;
/// </returns>
int executeCommand(int command, int operand) {

	int clearValue, idCom, idOper, tmp;
	switch (command)
	{
	case READ:
		int num;

		// TODO: Заглушка, надо как-то вызвать здесь терминал на чтение переменной!
		if (operand == 9)
			num = 3;
		else if (operand == 10)
			num = 8190;

		sc_memorySetAndEncode(operand, num, &tmp);
		return 1;

	case WRITE:
		sc_memoryGetAndDecode(operand, &clearValue);
		std::cout << clearValue;
		return 1;

	case LOAD:
		sc_memoryGetAndDecode(operand, &clearValue);
		_accumulator = clearValue;
		return 1;

	case STORE:
		sc_memorySetAndEncode(operand, _accumulator, &tmp);
		return 1;

	case ADD:
		sc_memoryGetAndDecode(operand, &clearValue);
		_accumulator += clearValue;
		return 1;

	case DIVIDE:
		sc_memoryGetAndDecode(operand, &clearValue);
		if (clearValue == 0) {
			sc_regSet(ZF, 1);
			return -2;
		}

		_accumulator /= clearValue;
		return 1;

	case MUL:
		sc_memoryGetAndDecode(operand, &clearValue);
		_accumulator *= clearValue;
		return 1;

	case SUB:
		sc_memoryGetAndDecode(operand, &clearValue);
		_accumulator -= clearValue;
		return 1;

	case JUMP:
		_instructionCounter = operand;
		return 1;

	case JNEG:
		if (_accumulator < 0) {
			_instructionCounter = operand;
		}
		return 1;

	case JZ:
		if (_accumulator == 0) {
			_instructionCounter = operand;
		}
		return 1;

	case HALT:
		sc_reset();
		return 1;

	case NOT:
		// TODO: Инверсия числа?
		return 1;

	case AND:
		sc_memoryGetAndDecode(operand, &clearValue);

		return 1;
	}

	return -1;
}