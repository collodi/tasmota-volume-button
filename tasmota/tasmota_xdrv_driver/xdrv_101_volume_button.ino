#ifdef USE_VOLUME_BUTTON

#warning **** volume button is included. ****
#define XDRV_101 101

#define BTN_PIN 0
#define LED_PIN 2

const unsigned long debounceDuration = 50;
unsigned long last_btn_pressed = 0;
bool last_btn_state = 0;

char dev_id[500];

const char clk_commands[] PROGMEM = "|"  // No Prefix
  "led";

void (* const clk_command[])(void) PROGMEM = {
  &cmd_led };

void volume_button_setup(void) {
    pinMode(BTN_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, 1);

    snprintf(dev_id, sizeof(dev_id), MQTT_TOPIC, system_get_chip_id());
}

int64_t get_epoch_ms()  {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL);
}

void cmd_led(void) {
    if (strcmp(XdrvMailbox.data, "on") == 0)
        digitalWrite(LED_PIN, 0);
    else if (strcmp(XdrvMailbox.data, "off") == 0)
        digitalWrite(LED_PIN, 1);

    ResponseCmndDone();
}

void report_button_push(int64_t epoch) {
    char data[100];
    sprintf(data, "%s,%lld", dev_id, epoch);

    MqttPublishPayload("server/cmnd/buttonpush", data, strlen(data), false);
}

void check_button_push(void) {
    if (millis() - last_btn_pressed <= debounceDuration)
        return;

    bool btn_state = digitalRead(BTN_PIN);
    if (btn_state == last_btn_state)
        return;

    last_btn_pressed = millis();
    last_btn_state = btn_state;

    if (btn_state == 1 && MqttIsConnected()) {
        int64_t push_epoch = get_epoch_ms();
        report_button_push(push_epoch);
    }
}

bool Xdrv101(uint32_t function) {
    bool result = false;

    switch (function) {
        case FUNC_INIT:
            volume_button_setup();
            break;
        case FUNC_EVERY_50_MSECOND:
            check_button_push();
            break;
        case FUNC_COMMAND:
            result = DecodeCommand(clk_commands, clk_command);
            break;
    }

    return result;
}

#endif