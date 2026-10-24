/* A coroutine with live locals, flag operands and branch changes across yield. */
void fixture_original(void)
{
    uint32_t _fs0 = 0, _fs1 = 0;
    uint32_t ebp;
    int _flags = 0;
    ebp = g_seh_ebp;
loc_00001000: ;
    ebp += 13;
loc_00001010: ;
    _fs0 = ebp ^ fixture_input;
    ebp = ebp * 33u + fixture_input;
    PUSH32(esp, 1);
    PUSH32(esp, 0); sub_001C65D8(); /* call 0x001C65D8 */
loc_00001020: ;
    esp += 4;
    _fs1 += ebp;
    _flags ^= (int)(fixture_input & 1);
    if (_flags) fixture_value += _fs0 ^ _fs1;
    else fixture_value = fixture_value * 17u + ebp;
    goto loc_00001010;
}
