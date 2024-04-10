#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#include "../../MySimpleComputer/MySimpleComputer/MySimpleComputer.h"
#include "../../MyBigChars/MyBigChars/MyBigChars.h"
#include "../../MyReadKey/MyReadKey/MyReadKey.h"
#include "../../CommonTerm/CommonTerm/CommonTerm.h"

using namespace std;

int loadMemoryfromFile()
{
    sc_reset();

    int descriptor = mt_getDescriptor();
    const char* header = "Enter file name for load memory:\n";
    write(descriptor, header, strlen(header));

    rk_toCanonical();

    int read_chars;
    char buf[200];
    read_chars = read(descriptor, buf, 200);
    string path = "";
    if(read_chars <= 0)
    {
        path = "Can't load memory form file " + path + '\n';
        ct_addMessage(bc_convertStringToCharArr(path));
        rk_toNoncanonical();
        return -1;
    }

    path.append(buf, read_chars - 1);
    if(sc_memoryLoad(bc_convertStringToCharArr(path)) == -1)
    {
        path = "Can't load memory form file " + path + '\n';
        ct_addMessage(bc_convertStringToCharArr(path));
        rk_toNoncanonical();
        return -1;
    }

    rk_toNoncanonical();

    path = "Loaded memory form file " + path + '\n';
    ct_addMessage(bc_convertStringToCharArr(path));

    return 0;
}

int saveMemoryfromFile()
{
    int descriptor = mt_getDescriptor();
    const char* header = "Enter new file name for save memory:\n";
    write(descriptor, header, strlen(header));

    rk_toCanonical();

    int read_chars;
    char buf[200];
    read_chars = read(descriptor, buf, 200);
    string path = "";
    if(read_chars <= 0)
    {
        path = "Can't save memory to file " + path + '\n';
        ct_addMessage(bc_convertStringToCharArr(path));
        rk_toNoncanonical();
        return -1;
    }

    path.append(buf, read_chars - 1);
    if(sc_memorySave(bc_convertStringToCharArr(path)) == -1)
    {
        path = "Can't save memory to file " + path + '\n';
        ct_addMessage(bc_convertStringToCharArr(path));
        rk_toNoncanonical();
        return -1;
    }

    rk_toNoncanonical();

    path = "Saved memory to file " + path + '\n';
    ct_addMessage(bc_convertStringToCharArr(path));

    return 0;
}

int changeSelectedCell(int selectedCell)
{
    int descriptor = mt_getDescriptor();
    int read_chars;
    char buf[200];

    rk_toCanonical();

    const char * msg = "Enter new value for seleceted cell:";
    write(descriptor, msg, strlen(msg));
    read_chars = read(descriptor, buf, 200);
    if(read_chars <= 0)
    {
        rk_toNoncanonical();
        return -1;
    }

    rk_toNoncanonical();

    int resultFlag = 0;
    int parseResult;
    int start = 0;
    int sign = 0;
    string message = "";
    if(buf[0] == '+')
    {
        parseResult = 0;
        message.append(buf, 1, 2);

        int command = atoi(message.data());

        message = "";
        message.append(buf, 3, 2);
        if(ct_hexToInt(message, &parseResult) == -1)
        {
            parseResult = 0;
            message = "Incorrect value entered. Set value" + to_string(parseResult) + '\n';
        }
        else
        {
            message = "Changed selected cell to command " + to_string(command) + " operand " + to_string(parseResult) + '\n';
        }

        int resEncode;
        if(sc_commandSetAndEncode(selectedCell, command, parseResult, &resEncode) < 0) 
        {
            message = "Can't change selected cell to command " + to_string(command) + " operand " + to_string(parseResult) + '\n';
            resultFlag = -1;
        }

        ct_addMessage(bc_convertStringToCharArr(message));
    }
    else
    {
        if(buf[0] == '-')
        {
            sign = 1;
            start = 1;
        }
    
        message.append(buf, start, read_chars - start - 1);
        int flag = ct_hexToInt(message, &parseResult);
        if(sign == 1)
        {
            parseResult*=-1;
        }

        if(flag == -1)
        {
            parseResult = 0;
            message = "Incorrect value entered. Set value" + to_string(parseResult) + '\n';
        }
        else
        {
            message = "Changed selected cell to " + to_string(parseResult) + '\n';
        }

        int resEncode;
        if(sc_memorySetAndEncode(selectedCell, parseResult, &resEncode) < 0)
        {
            message = "Can't change selected cell to " + to_string(parseResult) + '\n';
            resultFlag = -1;
        }
        ct_addMessage(bc_convertStringToCharArr(message));
    }

    return resultFlag;
}

int changeAccumulatorValue()
{
    int descriptor = mt_getDescriptor();
    int read_chars;
    char buf[200];

    rk_toCanonical();

    const char * msg = "Enter new value for accumulator:";
    write(descriptor, msg, strlen(msg));
    read_chars = read(descriptor, buf, 200);
    if(read_chars <= 0)
    {
        rk_toNoncanonical();
        return -1;
    }

    rk_toNoncanonical();

    int result;
    int flag;
    int start = 0;
    int sign = 0;
    string message = "";
    if(buf[0] == '+')
    {
        message = "Can't set command in accumulator. Set value -> 0 \n";
        ct_addMessage(bc_convertStringToCharArr(message));
        return 0;
    }
    else
    {
        if(buf[0] == '-')
        {
            sign = 1;
            start = 1;
        }
    
        message.append(buf, start, read_chars - start - 1);
        flag = ct_hexToInt(message, &result);
        if(sign == 1)
        {
            result*=-1;
        }

        if(flag == -1)
        {
            result = 0;
            message = "Incorrect value entered. Set value" + to_string(result) + '\n';
        }
        else
        {
            message = "Changed accumulator to " + to_string(result) + '\n';
        }

        sc_accumSet(result);
        ct_addMessage(bc_convertStringToCharArr(message));
    }
    

    return 0;
}

int changeInstructionCounter()
{
    int descriptor = mt_getDescriptor();
    int read_chars;
    char buf[200];

    rk_toCanonical();

    const char * msg = "Enter new InstructionCounter:";
    write(descriptor, msg, strlen(msg));
    read_chars = read(descriptor, buf, 200);
    if(read_chars <= 0)
    {
        rk_toNoncanonical();
        return -1;
    }

    rk_toNoncanonical();

    int value = atoi(buf);
    sc_instructSet(value);

    string path = "Changed InstructionCounter to " + to_string(value) + '\n';
    ct_addMessage(bc_convertStringToCharArr(path));
    return 0;
}

int mainFunc()
{
    int selectedCell;
    while(true)
    {
        selectedCell = ct_getRealSelectedInedx();
        ct_redraw(selectedCell);
        Keys key;
        if(rk_readkey(&key) == -1)
        {
            continue;
        }

        int row;
        int mod;
        switch(key)
        {
            case Keys::Load:
                loadMemoryfromFile();
                break;

            case Keys::Save:
                saveMemoryfromFile();
                break;

            case Keys::Run:
                sc_run();
                break;

            case Keys::Step:
                sc_runByStep();
                break;

            case Keys::Reset:
                sc_reset();
                break;

            case Keys::Accumulator:
                changeAccumulatorValue();
                break;

            case Keys::InstructionCounter:
                changeInstructionCounter();
                break;

            case Keys::ChangeSelectedValue:
                changeSelectedCell(selectedCell);
                break;

            case Keys::Left:
                row = selectedCell / 10;
                mod = selectedCell % 10;
                if(mod == 0)
                {
                    selectedCell = (row * 10) + 9;
                }
                else
                {
                    selectedCell--;
                }
                ct_setRealSelectedInedx(selectedCell);
                break;
            case Keys::Up:
                row = selectedCell / 10;
                mod = selectedCell % 10;
                if(row == 0)
                {
                    row = 9;
                }
                else
                {
                    row--;
                }
                selectedCell = (row * 10) + mod;
                ct_setRealSelectedInedx(selectedCell);
                break;

            case Keys::Right:
                row = selectedCell / 10;
                mod = selectedCell % 10;
                if(mod == 9)
                {
                    selectedCell = row * 10;
                }
                else
                {
                    selectedCell++;
                }
                ct_setRealSelectedInedx(selectedCell);
                break;

            case Keys::Down:
                row = selectedCell / 10;
                mod = selectedCell % 10;
                if(row == 9)
                {
                    row = 0;
                }
                else
                {
                    row++;  
                }
                selectedCell = (row * 10) + mod;
                ct_setRealSelectedInedx(selectedCell);
                break;

            case Keys::None:
            default:
                break;
        }
    } 
}

int initAll(int descriptor)
{
    int res = mt_init(descriptor);
    if(res == -1)
    {
        return -1;
    }

    sc_reset();
    res = ct_init(true);
    if(res == -1)
    {
        return -1;
    }

    res = rk_toNoncanonical();
    if(res == -1)
    {
        return -1;
    }

    return bc_initMyBigChars();
}

int main(int argc, char *argv[])
{
    int descriptor = 0;
    /*if(argc > 2)
    {
        cout << "Too many arguments" << endl;
        return -1;
    }
    else if(argc == 2)
    {
        string str = argv[1];
        if(!ct_isNumber(str))
        {
            cout << "Descriptor is not a number" << endl;
            return -1;
        }

        descriptor = atoi(argv[1]);
    }*/

    if(initAll(descriptor) == -1)
    {
        return -1;
    }

    mainFunc();

    return 0;
}