/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#ifndef USB_DEVICE_MAIN_MP_H
#define USB_DEVICE_MAIN_MP_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "cdcdf_acm.h"
#include "cdcdf_acm_desc.h"
#include "RingBuf.h"

#if CONF_USBD_HS_SP
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ_HS
#else
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ
#endif



void cdcd_acm_micropython(void);
void cdc_device_acm_init(void);

extern volatile struct RingBuffer rbuf_in;
extern volatile struct RingBuffer rbuf_out;

extern volatile uint8_t read_done;
extern volatile uint8_t send_done;

extern volatile uint8_t usbd_cdc_buffer_out[CDCD_ECHO_BUF_SIZ];
extern volatile uint8_t usbd_cdc_buffer_in[CDCD_ECHO_BUF_SIZ];


/**
 * \berif Initialize USB
 */
void usb_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USB_DEVICE_MAIN_MP_H
