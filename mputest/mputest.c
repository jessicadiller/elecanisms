#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "spi.h"

#define TOGGLE_LED1         1
#define TOGGLE_LED2         2
#define READ_SW1            3
#define MPU_WRITE_REG       4
#define MPU_READ_REG        5
#define MPU_WRITE_REGS      6
#define MPU_READ_REGS       7
#define TOGGLE_LED3         8 
#define READ_SW2            9
#define READ_SW3            10

_PIN FOO_SCK, FOO_MISO, FOO_MOSI;
_PIN MPU9250_CSN, MPU9250_INT;
_PIN NRF8001_ACT, NRF8001_RDYN, NRF8001_REQN, NRF8001_RESET;

void mpu_writeReg(uint8_t address, uint8_t value) {
    if (address<=0x7E) {
        pin_clear(&MPU9250_CSN);
        spi_transfer(&spi1, address);
        spi_transfer(&spi1, value);
        pin_set(&MPU9250_CSN);
    }
}

uint8_t mpu_readReg(uint8_t address) {
    uint8_t value;

    if (address<=0x7E) {
        pin_clear(&MPU9250_CSN);
        spi_transfer(&spi1, 0x80|address);
        value = spi_transfer(&spi1, 0);
        pin_set(&MPU9250_CSN);
        return value;
    } else
        return 0xFF;
}

void mpu_writeRegs(uint8_t address, uint8_t *buffer, uint8_t n) {
    uint8_t i;

    if (address+n<=0x7E) {
        pin_clear(&MPU9250_CSN);
        spi_transfer(&spi1, address);
        for (i = 0; i<n; i++)
            spi_transfer(&spi1, buffer[i]);
        pin_set(&MPU9250_CSN);
    }
}

void mpu_readRegs(uint8_t address, uint8_t *buffer, uint8_t n) {
    uint8_t i;

    if (address+n<=0x7E) {
        pin_clear(&MPU9250_CSN);
        spi_transfer(&spi1, 0x80|address);
        for (i = 0; i<n; i++)
            buffer[i] = spi_transfer(&spi1, 0);
        pin_set(&MPU9250_CSN);
    } else {
        for (i = 0; i<n; i++)
            buffer[i] = 0xFF;
    }
}

//void ClassRequests(void) {
//    switch (USB_setup.bRequest) {
//        default:
//            USB_error_flags |= 0x01;                    // set Request Error Flag
//    }
//}

void VendorRequests(void) {
    WORD32 address;

    switch (USB_setup.bRequest) {
        case TOGGLE_LED1:
            led_toggle(&led1);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case TOGGLE_LED2:
            led_toggle(&led2);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case READ_SW1:
            BD[EP0IN].address[0] = (uint8_t)sw_read(&sw1);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case MPU_WRITE_REG:
            mpu_writeReg(USB_setup.wValue.b[0], USB_setup.wIndex.b[0]);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case MPU_READ_REG:
            BD[EP0IN].address[0] = mpu_readReg(USB_setup.wValue.b[0]);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case MPU_WRITE_REGS:
            USB_request.setup.bmRequestType = USB_setup.bmRequestType;
            USB_request.setup.bRequest = USB_setup.bRequest;
            USB_request.setup.wValue.w = USB_setup.wValue.w;
            USB_request.setup.wIndex.w = USB_setup.wIndex.w;
            USB_request.setup.wLength.w = USB_setup.wLength.w;
            break;
        case MPU_READ_REGS:
            mpu_readRegs(USB_setup.wValue.b[0], BD[EP0IN].address, USB_setup.wLength.b[0]);
            BD[EP0IN].bytecount = USB_setup.wLength.b[0];
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case TOGGLE_LED3:
            led_toggle(&led3);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case READ_SW2:
            BD[EP0IN].address[0] = (uint8_t)sw_read(&sw2);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case READ_SW3:
            BD[EP0IN].address[0] = (uint8_t)sw_read(&sw3);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        default:
            USB_error_flags |= 0x01;    // set Request Error Flag
    }
}

void VendorRequestsIn(void) {
    switch (USB_request.setup.bRequest) {
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
}

void VendorRequestsOut(void) {
    WORD32 address;

    switch (USB_request.setup.bRequest) {
        case MPU_WRITE_REGS:
            mpu_writeRegs(USB_request.setup.wValue.b[0], BD[EP0OUT].address, USB_request.setup.wLength.b[0]);
            break;
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_spi();

    pin_init(&FOO_SCK, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 9, 9, 8, 9, (uint16_t *)&RPOR4);
    pin_init(&FOO_MISO, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 14, 14, 0, 14, (uint16_t *)&RPOR7);
    pin_init(&FOO_MOSI, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 8, 8, 0, 8, (uint16_t *)&RPOR4);

    pin_init(&MPU9250_CSN, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)NULL, 13, -1, 0, -1, (uint16_t *)NULL);
    pin_init(&MPU9250_INT, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)NULL, 11, -1, 0, -1, (uint16_t *)NULL);

    pin_init(&NRF8001_ACT, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 10, -1, 0, -1, (uint16_t *)NULL);
    pin_init(&NRF8001_RDYN, (uint16_t *)&PORTE, (uint16_t *)&TRISE, 
             (uint16_t *)NULL, 4, -1, 0, -1, (uint16_t *)NULL);
    pin_init(&NRF8001_REQN, (uint16_t *)&PORTE, (uint16_t *)&TRISE, 
             (uint16_t *)NULL, 2, -1, 0, -1, (uint16_t *)NULL);
    pin_init(&NRF8001_RESET, (uint16_t *)&PORTE, (uint16_t *)&TRISE, 
             (uint16_t *)NULL, 3, -1, 0, -1, (uint16_t *)NULL);

    pin_digitalOut(&MPU9250_CSN);
    pin_set(&MPU9250_CSN);

    pin_digitalOut(&NRF8001_REQN);
    pin_set(&NRF8001_REQN);

    pin_digitalOut(&NRF8001_RESET);
    pin_set(&NRF8001_RESET);

    spi_open(&spi1, &FOO_MISO, &FOO_MOSI, &FOO_SCK, 1e6);

    InitUSB();                              // initialize the USB registers and serial interface engine
    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }
    while (1) {
        ServiceUSB();                       // service any pending USB requests
    }
}
