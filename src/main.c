#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/sys/printk.h>

#define DS1307_NODE DT_ALIAS(ds1307)

#if !DT_NODE_HAS_STATUS(DS1307_NODE, okay)
#error "DS1307 RTC device not found in device tree"
#endif

const struct device *rtc = DEVICE_DT_GET(DS1307_NODE);

void main(void)
{
    if (!device_is_ready(rtc)) {
        printk("DS1307 not ready\n");
        return;
    }

    struct rtc_time tm = {
        .tm_year = 2025 - 1900,
        .tm_mon = 6 - 1,
        .tm_mday = 27,
        .tm_hour = 7,
        .tm_min = 23,
        .tm_sec = 0,
    };

    if (rtc_set_time(rtc, &tm) != 0) {
        printk("Erro ao configurar o RTC\n");
    }

    while (1) {
        if (rtc_get_time(rtc, &tm) == 0) {
            printk("Hora: %04d-%02d-%02d %02d:%02d:%02d\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec);
        }
        k_sleep(K_SECONDS(5));
    }
}
