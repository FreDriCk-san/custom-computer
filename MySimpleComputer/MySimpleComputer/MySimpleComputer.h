// Регистр флагов содержит 5 разрядов:
// 1) Переполнение при выполнении операции;		[CF] - Carry flag
// 2) Ошибка деления на 0;						[ZF] - Zero flag
// 3) Ошибка выхода за границы памяти;			[OF] - Overflow flag
// 4) Игнорирование тактовых импульсов;			[IF] - Interrupt enable flag
// 5) Указана неверная команда.					[MF] - Method failure flag
#define CF 1
#define ZF 2
#define OF 3
#define IF 4
#define MF 5

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int* value);
int sc_memorySave(char* filename);
int sc_memoryLoad(char* filename);
int sc_regInit(void);
int sc_regSet(int reg, int value);
int sc_regGet(int reg, int* value);
int sc_commandEncode(int command, int operand, int* value);
int sc_commandDecode(int value, int* command, int* operand);