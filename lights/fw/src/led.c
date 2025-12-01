#include "led.h"

#include "utils.h"

void lights_led_init(LightsLed *led, PwmChannel pwm_channel) {
    led->pwm_channel = pwm_channel;
    led->brightness = LIGHTS_LED_DEFAULT_BRIGHTNESS;
}

void lights_led_set_brightness(LightsLed *led, uint8_t brightness) {
    led->brightness = brightness;
}

void lights_led_update(LightsLed *led) {
    utils_pwm_set_duty_cycle(led->pwm_channel, led->brightness);
}