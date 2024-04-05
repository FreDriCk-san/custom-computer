#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#include "myTerm.h"

using namespace std;

int descriptor = 0;

/// <summary>
/// ������� ���������
/// </summary>
/// <returns>
/// <para/> 0: �����
/// <para/> -1: ������
/// </returns>
int mt_clrscr()
{
    return write(descriptor, "\E[H\E[J", 6);
}

/// <summary>
/// ���������� ���������� ����� � �������� � ���������.
/// </summary>
/// <param name="rows">���-�� �����</param>
/// <param name="cols">���-�� ��������</param>
/// <returns>
/// <para/> 0: �����
/// <para/> -1: ������
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
/// ���������� ���������� ����� � �������� � ���������.
/// </summary>
/// <param name="rows">���-�� �����</param>
/// <param name="cols">���-�� ��������</param>
/// <returns>
/// <para/> 0: �����
/// <para/> -1: ������
/// </returns>
int mt_gotoXY (int x, int y)
{
    int maxRows; 
    int maxColumns;
    mt_getscreensize(&maxRows, &maxColumns);
    if(x < 0 || y < 0 || maxRows <= x || maxColumns <= y)
    {
        return -1;
    }

    //command = \E[%i%p1%d;%p2%dH
    char command[100];
    sprintf(command, "\E[%d;%dH", x, y);
    int size = strlen(command);
    return write(descriptor, command, size);
}

/// <summary>
/// ������������� ���� �����
/// </summary>
/// <param name="color">Enum Colors</param>
/// <returns>
/// <para/> 0: �����
/// <para/> -1: ������
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
/// ������������� ���� ����
/// </summary>
/// <param name="color">Enum Colors</param>
/// <returns>
/// <para/> 0: �����
/// <para/> -1: ������
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
/// ������� �������������, ��� ��������� � ����� ��������� ��������.
/// ��-��������� ����������� � 0 ���������, �.�. � ���, ������� ����������� ����������
/// </summary>
/// <param name="aDescriptor">����� ���������� ���������</param>
/// <returns>
/// <para/> 0: �����
/// <para/> -1: ������
/// </returns>
int init(int aDescriptor)
{
    descriptor = aDescriptor;
}

enum class Colors
{
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Purple = 5,
    DarkGreen = 6,
    White = 7,
};