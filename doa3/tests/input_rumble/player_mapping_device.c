#include "pad_mapping.h"
SHORT player_test_key(int key);
#define GetAsyncKeyState player_test_key
#include "../../src/input/pad_mapping.c"
