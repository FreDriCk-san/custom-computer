#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "MyTerm.h"

using namespace std;

int descriptor = 0;

/// <summary>
/// Очистка терминала
/// </summary>
/// <returns>
/// <para/> 0: Успех
/// <para/> -1: Ошибка
/// </returns>
int mt_clrscr()
{
    return write(descriptor, "\E[H\E[J", 6);
}

/// <summary>
/// Возвращает количество строк и столбцов в терминале.
/// </summary>
/// <param name="rows">Кол-во строк</param>
/// <param name="cols">Кол-во столбцов</param>
/// <returns>
/// <para/> 0: Успех
/// <para/> -1: Ошибка
/// </returns>
int mt_getscreensize(int *rows, int *cols)
{
    struct winsize ws;
    if (!ioctl(descriptor, TIOCGWINSZ, &ws))
    {
        *rows = static_cast<int>(ws.ws_row);
        *cols = static_cast<int>(ws.ws_col);
        return 0;
    }
    else
    {
        return -1;
    }
}


/// <summary>
/// Перемещает каретку на указанную позицию
/// </summary>
/// <param name="rows">Кол-во строк</param>
/// <param name="cols">Кол-во столбцов</param>
/// <returns>
/// <para/> 0: Успех
/// <para/> -1: Ошибка
/// </returns>
int mt_gotoXY (int x, int y)
{
    int maxRows; 
    int maxColumns;
    mt_getscreensize(&maxRows, &maxColumns);
    if(x < 0 || y < 0 || maxColumns <= x || maxRows <= y)
    {
        cout << "bruh" << x << " " << y << "max "<< maxRows << " " << maxColumns;
        return -1;
    }

    char command[100];
    sprintf(command, "\E[%d;%dH", y, x);
    int size = strlen(command);
    return write(descriptor, command, size);
}

/// <summary>
/// Устанавливает цвет теста
/// </summary>
/// <param name="color">Enum Colors</param>
/// <returns>
/// <para/> 0: Успех
/// <para/> -1: Ошибка
/// </returns>
int mt_setfgcolor (Colors color)
{
    int numColor = static_cast<int>(color);

    //command = \E[3%p1%dm
    char command[100];
    sprintf(command, "\E[3%dm", numColor);
    int size = strlen(command);
    return write(descriptor, command, size);
}

/// <summary>
/// Устанавливает цвет фона
/// </summary>
/// <param name="color">Enum Colors</param>
/// <returns>
/// <para/> 0: Успех
/// <para/> -1: Ошибка
/// </returns>
int mt_setbgcolor (Colors color)
{
    int numColor = static_cast<int>(color);

    //command = \E[4%p1%dm
    char command[100];
    sprintf(command, "\E[4%dm", numColor);
    int size = strlen(command);
    return write(descriptor, command, size);
}

/// <summary>
/// Функция инициализации, для установки в каком терминале работать.
/// По-умолчанию запускается в 0 терминале, т.е. в том, котором поднимается приложение
/// </summary>
/// <param name="aDescriptor">Номер текстового терминала</param>
/// <returns>
/// <para/> 0: Успех
/// <para/> -1: Ошибка
/// </returns>
int mt_init(int aDescriptor)
{
    if(aDescriptor < 0)
    {
        cout << "Descriptor is negative" << endl;
        return -1;
    }
    else if(aDescriptor > 6)
    {
        cout << "Descriptor can't be move 6" << endl;
        return -1;
    }
    else if(aDescriptor != 0)
    {
        string path = "/dev/tty" + to_string(aDescriptor);
        aDescriptor = open(path.data(), O_WRONLY);
        if(aDescriptor == -1)
        {
            fprintf (stderr, "Cant open terminal.\n");
            return -1;
        }
    }

    descriptor = aDescriptor;
    return 0;
}

//Возвращаем ссылку на терминал
int mt_getDescriptor()
{
    return descriptor;
}