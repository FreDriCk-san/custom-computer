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

int mt_clrscr();
int mt_getscreensize(int *rows, int *cols);
int mt_gotoXY (int x, int y);
int mt_setfgcolor (Colors color);
int mt_setbgcolor (Colors color) ;
int mt_getDescriptor();
int mt_init(int aDescriptor);