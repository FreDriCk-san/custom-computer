enum class Keys
{
    None = 0,
    Load = 1,
    Save = 2,
    Run = 3,
    Step = 4,
    Reset = 5,
    Accumulator = 6,
    InstructionCounter = 7,
    Left = 8,
    Up = 9,
    Right = 10,
    Down = 11,
};

int rk_readkey(Keys * key);
int rk_toCanonical();
int rk_toNoncanonical();