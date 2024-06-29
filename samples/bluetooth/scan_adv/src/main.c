/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#include <dk_buttons_and_leds.h>

static bool parse_cb(struct bt_data *data, void *user_data) {
dk_set_led_on(DK_LED3);
	printk(" parse_cb: type=0x%x len=%d\n", data->type, data->data_len);
	if(data->type==BT_DATA_SVC_DATA16) {
dk_set_led_on(DK_LED4);
		// pvvx Custom format
		int16_t temperature=data->data[8]+data->data[9]*256;
		/*// "temp=*float*":
		float temp=(float)(data->data[8]+data->data[9]*256)/100.0f;
		printk("temp=%.2f\n", temp);*/
		uint16_t humidity=data->data[10]+data->data[11]*256;
		uint8_t battery_level=data->data[14];
		printk("  temperature*100=%d humidity*100=%d battery_level=%d\n", temperature, humidity, battery_level);
dk_set_led_off(DK_LED4);
		/*return false;
	*/}/* else {*/
dk_set_led_off(DK_LED3);
		return true;
	/*}*/
}

static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *buf)
{
dk_set_led_on(DK_LED2);
	char str[80];
	bt_addr_le_to_str(addr,str,sizeof(str));

	bt_addr_le_t addr1;
	bt_addr_le_from_str("A4:C1:38:88:3B:68","public",&addr1);
	bt_addr_le_t addr2;
	bt_addr_le_from_str("A4:C1:38:DA:99:0E","public",&addr2);
	if(bt_addr_le_cmp(addr,&addr1)==0 || bt_addr_le_cmp(addr,&addr2)==0) {		
		printk("scan_cb(%s,%ddB,%d\n",str,rssi,adv_type);
		bt_data_parse(buf,parse_cb,NULL);
	}
dk_set_led_off(DK_LED2);
}

int main(void)
{
	int err;
	err = dk_leds_init();
	if (err) {
		printk("LEDs init failed (err %d)\n", err);
		return 0;
	}
	dk_set_led_on(DK_LED1);
struct bt_le_scan_param scan_param =
{
    .type = BT_LE_SCAN_TYPE_PASSIVE,
    .options = BT_LE_SCAN_OPT_CODED,
    .interval = 0x40, // 40ms
    .window = 0x20, // 20ms
    .timeout = 0,
    .interval_coded = 0,
    .window_coded = 0,
};

	printk("Starting Scanner/Advertiser Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	printk("Bluetooth initialized\n");

	err = bt_le_scan_start(&scan_param, scan_cb);
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return 0;
	}

	do {
		k_sleep(K_MSEC(400));

	} while (1);
dk_set_led_off(DK_LED1);
	return 0;
}
