#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

#include "MyReadKey.h"
#include "../../MyTerm/MyTerm/MyTerm.h"

//Проверка нажатых клавиш в терминале
int rk_readkey(Keys * key)
{
    int read_chars;
    char buf[200];
    int descriptor = mt_getDescriptor();

    *key = Keys::None;
    if(read_chars = read(descriptor, buf, 200) <= 0)
    {
        return -1;
    }

    string value = "";
    value.append(buf, 5);
    //F5
    if(value == "\E[15~")
    {
        *key = Keys::Accumulator;
    }
    //F6
    else if(value == "\E[17~")
    {
        *key = Keys::InstructionCounter;
    }
    //F7
    else if(value == "\E[18~")
    {
        *key = Keys::ChangeSelectedValue;
    }
    else
    {
        value = "";
        value.append(buf, 3);
        //Up
        if(value == "\E[A")
        {
            *key = Keys::Up;
        }
        //Right
        else if(value == "\E[C")
        {
            *key = Keys::Right;
        }
        //Down
        else if(value == "\E[B")
        {
            *key = Keys::Down;
        }
        //Left
        else if(value == "\E[D")
        {
            *key = Keys::Left;
        }
        else
        {
            value = "";
            value.append(buf, 1);
            //Load
            if(value == "l")
            {
                *key = Keys::Load;
            }
            //Save
            else if(value == "s")
            {
                *key = Keys::Save;
            }
            //run
            else if(value == "r")
            {
                *key = Keys::Run;
            }
            //step
            else if(value == "t")
            {
                *key = Keys::Step;
            }
            //reset
            else if(value == "i")
            {
                *key = Keys::Reset;
            }
        }
    }

    return 0;
}

struct termios storedSettings;
//Сохранение настроек терминала
int rk_mytermsave()
{
    int descriptor = mt_getDescriptor();
    return tcgetattr(descriptor, &storedSettings);
}

//Восстанавление настроек терминала
int rk_mytermrestore()
{
    int descriptor = mt_getDescriptor();
    return tcsetattr(descriptor, TCSANOW, &storedSettings);
}

//Установка настроек терминала 
//TODO:зачем sigint???
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    int descriptor = mt_getDescriptor();

    struct termios oldSettings;
    if(tcgetattr(descriptor, &oldSettings) == -1)
    {
        return -1;
    }

    struct termios newSettings = oldSettings;
    if(regime == 1)
    {
        newSettings.c_lflag &= (~ICANON);
    }
    else
    {
        newSettings.c_lflag |= (ICANON);
    }

    if(regime == 1)
    {
        newSettings.c_lflag &= (~ECHO);
    }
    else
    {
        newSettings.c_lflag |= (ECHO);
    }

	newSettings.c_cc[VTIME] = vtime;
	newSettings.c_cc[VMIN] = vmin;

    if(tcsetattr(descriptor, TCSANOW, &newSettings))
    {
        return -1;
    }

    return 0;
}

//Переход в канонический вид
int rk_toCanonical()
{
    if(rk_mytermsave() == -1)
    {
        return -1;
    } 

    if(rk_mytermregime(0, 0, 0, 0, 0) == -1)
    {
        rk_mytermrestore();
        return -1;
    }

    return 0;
}

//Переход в неканонический вид
int rk_toNoncanonical()
{
    if(rk_mytermsave() == -1)
    {
        return -1;
    } 

    if(rk_mytermregime(1, 0, 1, 1, 0) == -1)
    {
        rk_mytermrestore();
        return -1;
    }

    return 0;
}