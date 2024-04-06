#include <unistd.h>
#include <string.h>
#include <iostream>

#include "../../MySimpleComputer/MySimpleComputer/MySimpleComputer.h"
#include "../../MyBigChars/MyBigChars/MyBigChars.h"

using namespace std;

int ct_drawMemory(int selectedCell)
{
    int descriptor = mt_getDescriptor();

    int startBoxX = 1;
    int startBoxY = 1;
    int width = 62;
    int height = 12;
    int res = bc_box(startBoxX, startBoxY, width, height);
    if(res == -1)
    {
        return -1;
    }

    const char* header = "Memory";
    int headerSize = strlen(header);
    res = mt_gotoXY((width / 2) - (headerSize / 2), startBoxY);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, header, headerSize);

    int indexCell;

    int nowXPosition = startBoxX + 1;
    int nowYPosition = startBoxY + 1;
    for(int i = 0; i < 10; i++)
    {
        int tmpX = nowXPosition;
        for(int j = 0; j < 10; j++)
        {
            indexCell = i * 10 + j;
            int cellValue;
            res = sc_memoryGet(indexCell, &cellValue);
            if(res == -1)
            {
                return -1;
            }

            /*int cellCommand;
            int possibleValue;
            //Пытаемся достать значение, если флаг MF корректен
            if(mf == 0)
            {
                res = sc_commandDecode(cellValue, &cellCommand, &possibleValue);
                if(res == -1)
                {   
                    //Значние может быть не коммандой, тогда поменяется флаг MF.
                    //Если флаг установлен, то убираем его.
                    if(res == -1 || mf == 0)
                    {
                        return -1;
                    }
                    else
                    {
                        sc_regSet(MF, 0);
                    }
                }
            }
            //Скорее всего, где-то на уровне исполнения комманд что-то пошло не так
            else
            {

            }*/

            //TODO: memory
            string line = "+0000";

            res = mt_gotoXY(tmpX, nowYPosition);
            if(res == -1)
            {
                return -1;
            }

            char* realLine =  bc_convertStringToCharArr(line);
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

int ct_drawRegisterBlock(int startBoxX, int startBoxY, int width, int height, const char* header, const char* strValue)
{
    int descriptor = mt_getDescriptor();
    int res = bc_box(startBoxX, startBoxY, width, height);
    if(res == -1)
    {
        return -1;
    }

    int headerSize = strlen(header);
    res = mt_gotoXY(startBoxX + (width / 2) - (headerSize / 2), startBoxY);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, header, headerSize);

    int valueSize = strlen(strValue);
    res = mt_gotoXY(startBoxX + (width / 2) - (valueSize / 2), startBoxY + 1);
    if(res == -1)
    {
        return -1;
    }

    write(descriptor, strValue, valueSize);
    return 0;
}

const char* ct_getFlagText()
{
    string strValue = "";
    int resultFlag;
    int res = sc_regGet(CF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'C';
    strValue += ' ';

    res = sc_regGet(ZF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'Z';
    strValue += ' ';

    res = sc_regGet(OF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'O';
    strValue += ' ';

    res = sc_regGet(IF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'I';
    strValue += ' ';

    res = sc_regGet(MF, &resultFlag);
    if(res == -1)
    {
        return "";
    }
    strValue +=  (resultFlag == 0) ? ' ' : 'M';
    strValue += ' ';

    return bc_convertStringToCharArr(strValue);
}

int ct_drawRegisterBlock()
{
    int startX = 64;
    int startY = 1;
    int blockWidth = 26;
    int blockHeight = 3;

    //TODO:load values
    const char* strValue = "+0000";
    int res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "accumulator", strValue);
    if(res == -1)
    {
        return -1;
    }
    startY+= blockHeight;

    res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "instructionCounter", strValue);
    if(res == -1)
    {
        return -1;
    }
    startY+= blockHeight;

    res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "Operation", strValue);
    if(res == -1)
    {
        return -1;
    }
    startY+= blockHeight;

    strValue = ct_getFlagText();
    if(res == -1)
    {
        return -1;
    }

    res = ct_drawRegisterBlock(startX, startY, blockWidth, blockHeight, "Flags", strValue);
    if(res == -1)
    {
        return -1;
    }

    return 0;
}

int ct_drawSelectedSize(int selectedCell)
{
    int startBoxX = 1;
    int startBoxY = 13;
    int blockWidth = 43;
    int blockHeight = 11;

    int descriptor = mt_getDescriptor();
    int res = bc_box(startBoxX, startBoxY, blockWidth, blockHeight);
    if(res == -1)
    {
        return -1;
    }

    //TODO: selected cell
    string text = "+0000";
    int nowXPosition = startBoxX + 1;
    int nowYPosition = startBoxY + 1;
    for(int i = 0; i < text.length(); i++)
    {
        res = bc_printbigchar(text[i], nowXPosition, nowYPosition, Colors::White, Colors::Black);
        nowXPosition+=8;
    }
    
    return 0;
}

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

int ct_drawHotkeys()
{
    int startBoxX = 45;
    int startBoxY = 13;
    int blockWidth = 45;
    int blockHeight = 11;

    int descriptor = mt_getDescriptor();
    int res = bc_box(startBoxX, startBoxY, blockWidth, blockHeight);
    if(res == -1)
    {
        return -1;
    }

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
    

    return 0;
}

int ct_redraw()
{
    mt_clrscr();

    //TODO:address selected cell
    int selectedCell = 42;

    int res = ct_drawMemory(selectedCell);
    if(res == -1)
    {
        return -1;
    }

    res = ct_drawRegisterBlock();
    if(res == -1)
    {
        return -1;
    }

    res = ct_drawSelectedSize(selectedCell);
    if(res == -1)
    {
        return -1;
    }

    res = ct_drawHotkeys();
    if(res == -1)
    {
        return -1;
    }

    res = mt_gotoXY(2, 26);
    if(res == -1)
    {
        return -1;
    }

    //TODO: input/output
    //int descriptor = mt_getDescriptor();
    //write(des)
    return 0;
}

int ct_init()
{
    int res = mt_init(0);
    if(res == -1)
    {
        return -1;
    }

    return bc_initMyBigChars();
}

int main()
{
    sc_memoryInit();
    ct_init();
    ct_redraw();
    return 0;
}