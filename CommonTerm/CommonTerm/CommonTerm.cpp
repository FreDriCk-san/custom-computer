#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#include "../../MySimpleComputer/MySimpleComputer/MySimpleComputer.h"
#include "../../MyBigChars/MyBigChars/MyBigChars.h"
#include "../../MyReadKey/MyReadKey/MyReadKey.h"

using namespace std;

bool _needRedraw = true;
int _realSelectedCell = 0;
//Конвертирование значения в HEX
char *ct_intToHex(unsigned Value, unsigned Digits)
{
    char *Hex = new char[Digits];
    Hex[Digits]='\0';
    int r;
    while (Value>=16 && Digits!=0)
    {
        r=Value%16;
        if (r>=10) 
        {
            Hex[--Digits]= r+55;
        }
        else
        {
            Hex[--Digits]= r+48;
        }
        Value/=16;
    }
 
    if (Digits >0)
    {
        if (Value>=10) 
        {
            Hex[--Digits]= Value+55;
        }
        else 
        {
            Hex[--Digits]= Value+48;
        }
 
        for (int i = Digits-1; i >= 0; i--)
        {
            Hex[i]=48;
        }
    }

  return Hex;
}

//Конвертирование из HEX
int ct_hexToInt(string st, int *result)
{
    int i, k;
    int s = 0;
    for (i = 0; st[i] != '\0'; i++)
    {
        int c;
        switch (c = toupper(st[i]))
        {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            k = c - 'A' + 10;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            k = c - '0';
            break;
        default:
            cout << c << endl;
            return -1;
        }
        s = (s << 4) + k;
    }
    *result = s;
    return 0;
}

//Проверка строки на число
bool ct_isNumber(const std::string s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && (isdigit(*it) || *it == '-'))
    {
        ++it;
    }
    return !s.empty() && it == s.end();
}

//Получение текста по заданному индексу
string getCellText(int indexCell)
{
    if(indexCell < 0 || indexCell > 100)
    {
        return "";
    }

    int res;
    string line = "";
    //Если в ячейке памяти хранится число
    if(sc_isNumber(indexCell))
    {
        line += " "; 
        int number;
        res = sc_memoryGetAndDecode(indexCell, &number);
        if(res == -1)
        {
            return "";
        }

        line += ct_intToHex(number, 4);
    }
    //Если в ячейке памяти хранится команда
    else
    {
        line += "+"; 
        int command;
        int opperand;
        res = sc_commandGetAndDecode(indexCell, &command, &opperand);
        if(res == -1)
        {
            return "";
        }

        line += to_string(command); 
        line += ct_intToHex(opperand, 2);
    }

    return line;
}

//Отрисовка области памяти
int ct_drawMemory(int selectedCell)
{
    int descriptor = mt_getDescriptor();

    //Хардкод
    int startBoxX = 1;
    int startBoxY = 1;
    int width = 62;
    int height = 12;
    //Рисуем границы
    int res = bc_box(startBoxX, startBoxY, width, height);
    if(res == -1)
    {
        return -1;
    }

    //Рисуем заголовок
    const char* header = "Memory";
    int headerSize = strlen(header);
    res = mt_gotoXY((width / 2) - (headerSize / 2), startBoxY);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, header, headerSize);

    int indexCell;

    //Рисуем ячейки памяти
    int nowXPosition = startBoxX + 1;
    int nowYPosition = startBoxY + 1;
    for(int i = 0; i < 10; i++)
    {
        int tmpX = nowXPosition;
        for(int j = 0; j < 10; j++)
        {
            indexCell = i * 10 + j;
            int cellValue;

            res = mt_gotoXY(tmpX, nowYPosition);
            if(res == -1)
            {
                return -1;
            }
            
            string line = getCellText(indexCell);
            char* realLine =  bc_convertStringToCharArr(line);
            //Если это выделенная ячейка, то закрашиваем ее
            if(indexCell == selectedCell)
            {
                mt_setbgcolor(Colors::White);
                mt_setfgcolor(Colors::Black);
                write(descriptor, realLine, 5);
                mt_setbgcolor(Colors::Black);
                mt_setfgcolor(Colors::White);
            }
            else
            {
                 write(descriptor, realLine, 5);
            }

            delete []realLine;
            tmpX+= 6;
        }

        nowYPosition++;
    }
    return 0;
}

//Отрисовка блоков с заголовком и тестом
int ct_drawRegisterBlock(int startBoxX, int startBoxY, int width, int height, const char* header, const char* strValue)
{
    int descriptor = mt_getDescriptor();
    //Рисуем границу
    int res = bc_box(startBoxX, startBoxY, width, height);
    if(res == -1)
    {
        return -1;
    }

    //Рисуем заголовок
    int headerSize = strlen(header);
    res = mt_gotoXY(startBoxX + (width / 2) - (headerSize / 2), startBoxY);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, header, headerSize);

    //Рисуем значение
    int valueSize = strlen(strValue);
    res = mt_gotoXY(startBoxX + (width / 2) - (valueSize / 2), startBoxY + 1);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, strValue, valueSize);
    return 0;
}

//Отрисовка блока с фалгами
const char* ct_getFlagText()
{
    string strValue = "";
    int resultFlag;
    //Достаем флаг переполнения
    int res = sc_regGet(CF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'C';
    strValue += ' ';

    //Достаем флаг деления на 0
    res = sc_regGet(ZF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'Z';
    strValue += ' ';

    //Достаем флаг выхода за границы
    res = sc_regGet(OF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'O';
    strValue += ' ';

    //Достаем флаг игнорирования тактовых импульсов
    res = sc_regGet(IF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'I';
    strValue += ' ';

    //Достаем флаг неверная комманда
    res = sc_regGet(MF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'M';
    strValue += ' ';

    return bc_convertStringToCharArr(strValue);
}

//Отрисовка блоков справа от памяти
int ct_drawRegisterBlock()
{
    int startX = 64;
    int startY = 1;
    int blockWidth = 26;
    int blockHeight = 3;

    //Отрисовка блока аккумулятора
    string strValue = ct_intToHex(sc_accumGet(), 4);
    int res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "accumulator", bc_convertStringToCharArr(strValue));
    if(res == -1)
    {
        return -1;
    }
    startY+= blockHeight;

    //Отрисовка блока индекса выполняемой команды
    int insructionIndexCell = sc_instructGet();
    strValue = to_string(insructionIndexCell);
    res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "instructionCounter", bc_convertStringToCharArr(strValue));
    if(res == -1)
    {
        return -1;
    }
    startY+= blockHeight;

    //Отрисовка блока выполняемой команды
    strValue = getCellText(insructionIndexCell);
    res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "Operation", bc_convertStringToCharArr(strValue));
    if(res == -1)
    {
        return -1;
    }
    startY+= blockHeight;

    //Отрисовка блока флагов
    res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "Flags", ct_getFlagText());
    if(res == -1)
    {
        return -1;
    }

    return 0;
}

//Отрисовка блока выделенного значения большими символами
int ct_drawSelectedSize(int selectedCell)
{
    int startBoxX = 1;
    int startBoxY = 13;
    int blockWidth = 43;
    int blockHeight = 11;

    int descriptor = mt_getDescriptor();
    //Рисуем границу
    int res = bc_box(startBoxX, startBoxY, blockWidth, blockHeight);
    if(res == -1)
    {
        return -1;
    }

    string text = getCellText(selectedCell);
    int nowXPosition = startBoxX + 1;
    int nowYPosition = startBoxY + 1;
    //Рисуем большие символы
    for(int i = 0; i < text.length(); i++)
    {
        res = bc_printbigchar(text[i], nowXPosition, nowYPosition, Colors::White, Colors::Black);
        nowXPosition+=8;
    }
    
    return 0;
}

//Отрисовка текста по позиции
int ct_drawTextInPosition(int x, int y, const char* header, int headerSize)
{
    int descriptor = mt_getDescriptor();
    int res = mt_gotoXY(x, y);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, header, headerSize);
    return 0;
}

//Отрисовкуа хоткеев
int ct_drawHotkeys()
{
    int startBoxX = 45;
    int startBoxY = 13;
    int blockWidth = 45;
    int blockHeight = 11;

    int descriptor = mt_getDescriptor();
    //Отрисовка границ
    int res = bc_box(startBoxX, startBoxY, blockWidth, blockHeight);
    if(res == -1)
    {
        return -1;
    }

    //Рисуем заголовок
    const char* header = "Keys:";
    int headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX + (blockWidth / 2) - (headerSize / 2), startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxX+=2;
    startBoxY++;
    header =  "l - load";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "s - save";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "r - run";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "t - step";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "i - reset";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "F5 - accumulator";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "F6 - instructionCounter";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    startBoxY++;
    header =  "F7 - change selected cell";
    headerSize = strlen(header);
    res = ct_drawTextInPosition(startBoxX, startBoxY, header, headerSize);
    if(res == -1)
    {
        return -1;
    }

    return 0;
}

//Получить индекс выделенной ячейки
int ct_getRealSelectedIndex()
{
    return _realSelectedCell;
}

//Установить индекс выделенной ячейки
int ct_setRealSelectedIndex(int value)
{
    _realSelectedCell = value;
}

//Буффер сообщейний
string messageBuffer[5];

//Функция отрисовка
int ct_redraw(int selectedCell)
{
    if (!_needRedraw)
        return 0;

    mt_clrscr();

    //Отрисовка памяти
    int res = ct_drawMemory(selectedCell);
    if(res == -1)
    {
        return -1;
    }

    //Отрисовка блоков справа от памяти
    res = ct_drawRegisterBlock();
    if(res == -1)
    {
        return -1;
    }

    //Отрисовка блока с выделенной ячейкой
    res = ct_drawSelectedSize(selectedCell);
    if(res == -1)
    {
        return -1;
    }

    //Отрисовка хоткеев
    res = ct_drawHotkeys();
    if(res == -1)
    {
        return -1;
    }

    res = mt_gotoXY(0, 26);
    if(res == -1)
    {
        return -1;
    }
 
    //Отрисовка блока Input/Output
    int descriptor = mt_getDescriptor();
    const char* header = "Input/Output\n";
    write(descriptor, header, strlen(header));

    //Рисуем все что есть в буфере сообщений
    for(int i = 0; i < 5; i++)
    {
        string item = messageBuffer[i];
        char* message = bc_convertStringToCharArr(item);
        write(descriptor, message, strlen(message));
    }

    return 0;
}

//Перерисовка
int ct_redraw()
{
    return ct_redraw(_realSelectedCell);
}

//Добавление сообщения в буфер
void ct_addMessage(const char* message)
{
    int lastIndex = -1;

    //Ищем свободное место под строку
    for(int i = 0; i < 5; i++)
    {
        if(messageBuffer[i].empty())
        {
            lastIndex = i;
            break;
        }
    }

    //Если такой нету, то сдвигаем все сообщения вверх на один индекс
    if(lastIndex == -1)
    {
        for(int i = 1; i < 5; i++)
        {
            messageBuffer[i - 1] = messageBuffer[i];
        }
        lastIndex = 4;
    }
    messageBuffer[lastIndex] = message;
}

//Функция используется из CPU для ввода пользователем при комманде READ
//Значения должны воодиться в HEX
int ct_readCommand(int * value)
{
    int descriptor = mt_getDescriptor();
    int read_chars;
    char buf[200];

    //Переводим в канонический вид
    rk_toCanonical();

    const char * msg = "Enter value:";
    write(descriptor, msg, strlen(msg));
    read_chars = read(descriptor, buf, 200);
    if(read_chars <= 0)
    {
        //Переводим в неканонический вид
        rk_toNoncanonical();
        return -1;
    }

    //Переводим в неканонический вид
    rk_toNoncanonical();

    int result;
    int flag;
    int start = 0;
    int sign = 0;
    string message = "";
    //Комманды не обрабатываются
    if(buf[0] == '+')
    {
        *value = 0;
        result = 0;
        message = "Incorrect value entered. Contain symbol +. Set value-> 0 \n";
        goto skip;
    }
    //Введено отрицательное значение
    else if(buf[0] == '-')
    {
        sign = 1;
        start = 1;
    }
    
    message.append(buf, start, read_chars - start - 1);
    //Конвертируем в dex
    flag= ct_hexToInt(message, &result);
    //Добавляем знак
    if(sign == 1)
    {
        result*=-1;
    }

    if(flag == -1)
    {
        result = 0;
        message = "Incorrect value entered. Set value->" + to_string(result) + '\n';
    }
    else
    {
        message = "read command->" + to_string(result) + '\n';
    }

skip:
    //Отдаем результат
    *value = result;
    //Добавляем сообщения
    ct_addMessage(bc_convertStringToCharArr(message));
    return 0;
}

//Функция используется из CPU для ввода пользователем при комманде WRITE
//Выводится в DEX 
int ct_writeCommand(int value)
{
    string message = "write command -> " + to_string(value) + '\n';
    ct_addMessage(bc_convertStringToCharArr(message));
    return 0;
}

//Функция инициализации
int ct_init(bool needRedraw)
{
    _realSelectedCell = 0;
    _needRedraw = needRedraw;
    return 0;
}
