/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void)
{
	int ret = 0;
	const struct device *sensor = DEVICE_DT_GET(DT_ALIAS(ambient_temp0));
	struct sensor_value last_val = { 0 };
	struct sensor_value val = { 0 };

	printk("Zephyr Example Application %s\n", APP_VERSION_STRING);
	printk("Reached and executing main...\n");

	if (!device_is_ready(sensor)) {
		LOG_ERR("Sensor not ready");
		return 0;
	}
	else {
		printk("Sensor successfully initialized...\n");
	}

	while (1) {
		ret = sensor_sample_fetch(sensor);
		if (ret < 0) {
			LOG_ERR("Could not fetch sample (%d)", ret);
			return 0;
		}
		else {
			LOG_DBG("Sample received\n");
		}

		ret = sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &val);
		if (ret < 0) {
			LOG_ERR("Could not get sample (%d)", ret);
			return 0;
		}
		else {
			LOG_DBG("Sample decoded\n");
		}

		printk("Temperature: int: %d frac:%d\n", val.val1, val.val2);
		last_val = val;

		k_sleep(K_MSEC(1000));
	}

	return 0;
}

