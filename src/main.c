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
    struct rtc_time tm; // Apenas declare a struct aqui

    if (!device_is_ready(rtc)) {
        printk("DS1307 not ready\n");
        return;
    }

// A diretiva de compilação condicional começa aqui
#ifdef CONFIG_SET_RTC_TIME
    printk("Configurando a hora do RTC...\n");
    // Define a hora inicial
    tm = (struct rtc_time){
        .tm_year = 2025 - 1900,
        .tm_mon = 6 - 1,          // Junho
        .tm_mday = 28,            // Dia 28
        .tm_hour = 8,             // 08h (horário de Brasília)
        .tm_min = 23,
        .tm_sec = 0,
        .tm_isdst = -1
    };

    if (rtc_set_time(rtc, &tm) != 0) {
        printk("Erro ao configurar o RTC\n");
    } else {
        printk("RTC configurado com sucesso!\n");
    }
#endif // Fim da seção condicional

    printk("Iniciando leitura contínua do RTC.\n");
    while (1) {
        if (rtc_get_time(rtc, &tm) == 0) {
            printk("Hora: %04d-%02d-%02d %02d:%02d:%02d\n",
                   tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                   tm.tm_hour, tm.tm_min, tm.tm_sec);
        } else {
            printk("Erro ao ler a hora do RTC\n");
        }
        k_sleep(K_SECONDS(5));
    }
}