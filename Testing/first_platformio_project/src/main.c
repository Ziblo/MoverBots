// standard includes
#include <stdio.h>
#include <stdlib.h>
// for delay
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// for spi
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define IMU_HOST SPI2_HOST
#define PIN_NUM_MISO 36
#define PIN_NUM_MOSI 38
#define PIN_NUM_CLK 37
#define PIN_NUM_CS 35

void app_main(){
    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .max_transfer_sz = 16}; // 16 bit transfer size
    spi_device_interface_config_t devcfg = {
        .command_bits = 1,                 // Default amount of bits in command phase (0-16)
        .address_bits = 7,                 // Default amount of bits in address phase (0-64)
        .dummy_bits = 0,                   // Amount of dummy bits to insert between address and data phase.
        .mode = 1,                         // SPI mode 1. Clock Polarity = 0. Clock Phase = 1.
        .clock_speed_hz = 7 * 1000 * 1000, // Clock out at 7 MHz
        .spics_io_num = PIN_NUM_CS,        // CS pin
        .queue_size = 15,                  // We want to be able to queue up to 15 transactions at a time
    };
    spi_transaction_t IMU_who_am_i = {
        .cmd = 1,     // read = 1. write = 0.
        .addr = 0,    // 7b0000000 = WHO_AM_I
        .length = 16, // total data length in bits
    };
    // spi_transaction_ext_t IMU_who_am_i_ext = {
    //     .base = IMU_who_am_i,
    //     .command_bits = 1,
    //     .address_bits = 7,
    //     .dummy_bits = 8,
    // };
    // Initialize the SPI bus
    ret = spi_bus_initialize(IMU_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret); // check for error
    // Attach the IMU to the SPI bus
    ret = spi_bus_add_device(IMU_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    ret = spi_device_transmit(spi, &IMU_who_am_i);
    ESP_ERROR_CHECK(ret);
    if (ret == ESP_OK){
        // Access the received data from the transaction buffer
        uint16_t received_data = *((uint16_t *)IMU_who_am_i.rx_buffer);

        // Print the received data
        printf("Received data: 0x%x\n", received_data);
    }
    else {
        // Handle error if the transaction failed
        printf("SPI transaction failed with error code: %d\n", ret);
    }
    while (1){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
