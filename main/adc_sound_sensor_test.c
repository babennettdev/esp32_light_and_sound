/* Blake Bennett 10/30/2019
 * Testing sound sensor for analog response
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#define GPIO_OUTPUT_IO_0    13
#define GPIO_OUTPUT_IO_1    12
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
volatile int cnt = 0;

static void gpio_light_test(void* arg)
{
    while(1){
        
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
        cnt++;
        gpio_set_level(GPIO_OUTPUT_IO_1, cnt % 2);
        //printf("count:  %d\n", (cnt%2));
        vTaskDelay(1000/ portTICK_RATE_MS);
    }
}

void app_main(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 1;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_2_5);
    
    xTaskCreate(gpio_light_test, "gpio_light_test", 2048, NULL, 10, NULL);
    while(1){
        int val = adc1_get_raw(ADC1_CHANNEL_0);
        printf("ADC value: %d\n", val);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}
