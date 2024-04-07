#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <map>

using namespace std;

#include "MyBigChars.h"

map<char, int*> symbols;

//выводит строку символов с использованием дополнительной кодировочной таблицы
int bc_printA (char * str)
{
    int descriptor = mt_getDescriptor();

    int count = strlen(str);
    char* command = new char[count];
    //Не работает в терминале vsc
    sprintf(command, "\E(0%s\E(B", str);
    int size = strlen(command);
    
    return write(descriptor, command, size);
}

char* bc_convertStringToCharArr(string line)
{
    char *realLine = new char[line.size()];
    strcpy(realLine, line.c_str());
    return realLine;
}

int bc_printA(string line)
{
    char *realLine = bc_convertStringToCharArr(line);
    int result = bc_printA(realLine);
    delete [] realLine;
    return result;
}

//выводит на экран псевдографическую рамку, 
//в которой левый верхний угол располагается в строке x1 и
//столбце y1, а её ширина и высота равна y2 столбцов и x2 строк
int bc_box(int x1, int y1, int width, int height)
{
    if(width <= 0 || height <= 0)
    {
        return -1;
    }

    int nowYPosition = y1;
    for(int i = 0; i < height; i++)
    {
        if(mt_gotoXY(x1, nowYPosition) == -1)
        {
            return -1;
        }

        char start;
        char middle;
        char end;
        string line = "";
        //Top
        if(i == 0)
        {
            start = 'l';
            middle = 'q';
            end = 'k';
        }
        //Bottom
        else if(i == height - 1)
        {
            start = 'm';
            middle = 'q';
            end = 'j';
        }
        else
        {
            start = end = 'x';
            middle = ' ';
        }

        line += start;
        for(int j = 1; j < width - 1; j++)
        {
            line+= middle;
        }
        line +=end;

        nowYPosition++;
        if(bc_printA(line) == -1)
        {
            return -1;
        }
    }

    return 0;
}

//Отрисовка больших символов. 
//Символы рисуются по values. В первой строке выводится 8
//младших бит первого числа, во второй следующие 8, в третьей и  следующие. 
//В 5 строке выводятся 8 младших бит второго числа и т.д.
//Значения: 0 - проблел, 1 - ACS_CKBOARD
//Точка 0, 0 является 7 бит
int bc_printbigchar(int values[2], int x, int y, Colors foreground, Colors background)
{
    if(mt_setfgcolor(foreground) == -1 || mt_setbgcolor(background) == -1)
    {
        return -1;
    }

    int nowYPosition = y;
    for(int i = 0; i < 2; i++)
    {
        int value = values[i];
        for(int j = 0; j < 4; j++)
        {
            if(mt_gotoXY(x, nowYPosition) == -1)
            {
                return -1;
            }

            string line = "";
            for(int k = 7; k >= 0; k--)
            {
                int index = (j * 8) + k;
                int flag = (value >> (index)) & 0x1;
                if(flag == 1)
                {
                    line += 'a';
                }
                else
                {
                    line +=  ' ';
                }
            }

            nowYPosition++;
            if(bc_printA(line) == -1)
            {
                return -1;
            }
        }
    }

    return 0;
}

//Установить значение в определенном бите в big по заданным x,y
int bc_setbigcharpos(int * big, int x, int y, int value)
{
    //len(big) != 2
    if(x < 0 || y < 0)
    {
        return -1;
    }

    int reg = y * 8 + (7 - x);
    int item;
    int index;
    if(reg < 32)
    {
        item = big[0];
        index = 0;
    }
    else if(reg < 64)
    {
        item = big[1];
        reg -= 32;
        index = 1;
    }
    else
    {
        return -1;
    }

    if(value == 0)
    {
        item = item & (~(1 << (reg)));
    }
    else
    {
        item = item | (1 << (reg));
    }

    big[index] = item;
    return 0;
}

//Получить значение в определенном бите в big по заданным x,y
int bc_getbigcharpos(int * big, int x, int y, int *value)
{
    //len(big) != 2
    if(x < 0 || y < 0)
    {
        return -1;
    }

    int reg = y * 8 + (7 - x);
    int item;
    if(reg < 32)
    {
        item = big[0];
    }
    else if(reg < 64)
    {
        item = big[1];
        reg -= 32;
    }
    else
    {
        return -1;
    }

    *value = (item >> reg) & 0x1;
    return 0;
}

int bc_bigcharwrite(int fd, int * big, int count)
{
    return 0;
}

int bc_bigcharread (int fd, int * big, int need_count, int * count)
{
    return 0;
}

//Загрузить шрифты из файла.
//На данный момент следующие условния:
//Сначало идет ключ, потом идет матрица из 0 и 1 размером 8х8.
//Пример числа 8:
//8
//00000000
//01111110
//01000010
//01000010
//01111110
//01000010
//01000010
//01111110
int bc_loadSymbols(const char * filename)
{
    symbols.clear();
    
    string filePath(filename);
    fstream myfile;
	myfile.open(filePath, ios::in);

	if (!myfile) 
    {
		return -1;
	}

    string line;
    bool isheader = true;
    int indexLine;
    char header;
    int *code;
    while (getline(myfile, line))
    {
        if(isheader)
        {
            header =  line[0];  
            code = new int[2];
            code[0] = 0;
            code[1] = 0;

            isheader = false;
            indexLine = 0;
        }
        else
        {
            if(line.length() != 8)
            {
                return -1;
            }

            for(int i = 0; i < 8; i++)
            {
                int value = line[i] - '0';
                bc_setbigcharpos(code, i, indexLine, value);
            }

            indexLine++;

            if(indexLine == 8)
            {
                symbols[header] = code;
                isheader = true;
            }
        }
        cout << endl;
    }

    myfile.close();
    return 0;
}

//Получить значения для определенного ключа
//Если ключа нет, то result = (0,0) 
int bc_getbigchar(char key, int* result)
{
    result[0] = 0;
    result[1] = 0;
    if(symbols.find(key) == symbols.end())
    {
        return -1;
    }

    int* values = symbols.at(key);
    result[0] = values[0];
    result[1] = values[1];
    return 0;
}

//Отрисовка больших символов по заданному char
int bc_printbigchar(char key, int x, int y, Colors foreground, Colors background)
{
    int values[2];
    if(bc_getbigchar(key, values) == -1)
    {
        return -1;
    }

    return bc_printbigchar(values, x, y ,foreground, background);
}

//Инициализация шрифтов
int bc_initMyBigChars()
{
    bc_loadSymbols("BigSymbolsStore.txt");
}