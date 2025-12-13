#include "led.h"

#include "utils.h"

void lights_led_init(LightsLed *led, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t bit, PwmChannel pwm_channel) {
    utils_gpio_set_output(ddr, bit);

    led->ddr = ddr;
    led->port = port;
    led->bit = bit;
    led->pwm_channel = pwm_channel;
    led->brightness = LIGHTS_LED_BRIGHTNESS_DEFAULT;
}

void lights_led_set_brightness(LightsLed *led, uint8_t brightness) {
    led->brightness = brightness;
}

void lights_led_update(LightsLed *led) {
    if (led->pwm_channel != NO_PWM_CH) {
        utils_pwm_set_duty_cycle(led->pwm_channel, led->brightness);
    } else if (led->pwm_channel == NO_PWM_CH) {
        utils_gpio_write( led->ddr, led->port, led->bit, (bool)(led->brightness != 0) );
    }
}