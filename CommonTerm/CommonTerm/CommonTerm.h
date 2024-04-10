using namespace std;

int ct_getRealSelectedInedx();
int ct_setRealSelectedInedx(int value);
int ct_redraw();
int ct_redraw(int selectedCell);
int ct_readCommand(int * value);
int ct_writeCommand(int value);
void ct_addMessage(const char* message);
int ct_hexToInt(string st, int *result);
char *ct_intToHex(unsigned Value, unsigned Digits);
int ct_init(bool needRedraw);