#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"

#define PROFILE_APP_ID 0

#define SPP_SERVICE_UUID           0xABF0
#define SPP_DATA_RECV_CHAR_UUID    0xABF1

#define SPP_DATA_RECV_VAL_LEN_MAX 20

static uint8_t spp_data_recv_value[SPP_DATA_RECV_VAL_LEN_MAX] = {0};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x20,
    .max_interval = 0x40,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(SPP_SERVICE_UUID),
    .p_service_uuid = (uint8_t *)&SPP_SERVICE_UUID,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            esp_ble_gap_start_advertising(&adv_params);
            break;
        default:
            break;
    }
}

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    switch (event) {
        case ESP_SPP_DATA_IND_EVT:
            printf("Received data: ");
            for (int i = 0; i < param->data_ind.len; i++) {
                printf("%c", param->data_ind.data[i]);
            }
            printf("\n");
            break;
        default:
            break;
    }
}

void spp_server_init(void) {
    esp_err_t ret;

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        printf("Bluetooth controller initialization failed\n");
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (ret) {
        printf("Bluetooth controller enable failed\n");
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        printf("Bluedroid initialization failed\n");
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        printf("Bluedroid enable failed\n");
        return;
    }

    esp_ble_gap_register_callback(esp_gap_cb);

    esp_spp_register_callback(esp_spp_cb);

    esp_spp_init(ESP_SPP_MODE_CB);
}

void app_main(void) {
    spp_server_init();
}
