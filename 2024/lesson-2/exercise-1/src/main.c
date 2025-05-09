/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define SLEEP_TIME_MS 100

/* Get the node identifier for button through its alias sw0. */
#define SW0_NODE DT_ALIAS(sw0)

/* Get the device pointer, pin number, and pin's configuration flags
 * through gpio_dt_spec. */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

/* LED0_NODE is the devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void) {
    /* Verify that the device LED is ready for use. */
    if (!device_is_ready(led.port)) {
        return -1;
    }
    /* Verify that the device button is ready for use. */
    if (!device_is_ready(button.port)) {
        return -1;
    }

    int result;

    result = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (result < 0) {
        return -1;
    }

    /* Configure the pin connected to the button to be an input. */
    result = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (result < 0) {
        return -1;
    }

    while (1) {
        /* Read the status of the button and store it. */
        bool val = gpio_pin_get_dt(&button);

        /* Update the LED according to the status of the button. */
        gpio_pin_set_dt(&led, val);

        /* Put the main thread to sleep for power optimization. */
        k_msleep(SLEEP_TIME_MS);
    }
}
