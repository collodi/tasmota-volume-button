//#ifdef USE_VOLUME_BUTTON

#warning **** volume button is included. ****
#define XDRV_101 101

// const char clk_commands[] PROGMEM = "|"  // No Prefix
//   "clock|"
//   "comptimer|"
//   "numbers";

// void (* const clk_command[])(void) PROGMEM = {
//   &cmd_clock, &cmd_timer, &cmd_numbers };

bool Xdrv101(uint32_t function) {
    bool result = false;

    switch (function) {
        case FUNC_INIT:
            break;
        case FUNC_EVERY_250_MSECOND:
            break;
        case FUNC_COMMAND:
            //result = DecodeCommand(clk_commands, clk_command);
            break;
    }

    return result;
}

//#endif