/*
 * Scanner de barramento I2C para Zephyr RTOS v2.7.1
 * Encontra dispositivos no barramento I2C_0 da placa FRDM-KL25Z
 */
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>

// definida pela placa. Para a FRDM-KL25Z, o padrão é "I2C_0".
#define I2C_DEVICE_NAME "I2C_0"

void main(void)
{
    // Obtém o ponteiro para o dispositivo I2C usando seu nome
    const struct device *i2c_dev = device_get_binding(I2C_DEVICE_NAME);

    // 1. Verifica se o controlador I2C foi encontrado
    if (!i2c_dev) {
        printk("Erro: Controlador I2C '%s' não foi encontrado!\n", I2C_DEVICE_NAME);
        return;
    }

    printk("Iniciando scanner I2C no barramento %s...\n", I2C_DEVICE_NAME);
    uint8_t devices_found = 0;

    for (uint8_t addr = 0x01; addr < 0x80; addr++) {
        struct i2c_msg msg;
        uint8_t dummy_buffer; // Um buffer qualquer, não será usado

        // Para sondar um endereço, tentamos uma escrita de 0 bytes.
        // Se o dispositivo responder (ACK), a função i2c_write retorna 0.
        msg.buf = &dummy_buffer; // Precisa de um ponteiro válido
        msg.len = 0;             // Mas com tamanho zero
        msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

        // A função i2c_transfer é a forma principal de interagir com o I2C
        if (i2c_transfer(i2c_dev, &msg, 1, addr) == 0) {
            printk("Dispositivo encontrado no endereco 0x%02X\n", addr);
            devices_found++;
        }
    }

    if (devices_found == 0) {
        printk("Nenhum dispositivo I2C encontrado.\n");
    } else {
        printk("Scan concluido. %d dispositivo(s) encontrado(s).\n", devices_found);
    }
}
