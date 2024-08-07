using namespace std;

int ct_getRealSelectedIndex();
int ct_setRealSelectedIndex(int value);
int ct_redraw();
int ct_redraw(int selectedCell);
int ct_readCommand(int * value);
int ct_writeCommand(int value);
void ct_addMessage(const char* message);
int ct_hexToInt(string st, int *result);
char *ct_intToHex(unsigned Value, unsigned Digits);
int ct_init(bool needRedraw);
bool ct_isNumber(const string s);