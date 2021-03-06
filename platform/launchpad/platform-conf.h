/*
 * Copyright (c) 2013, Marcus Linderoth, http://forfunandprof.it
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         Platform configuration header file
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */
#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__
/*--------------------------------------------------------------------------*/
/*
 * The following are some defines that can be changed in order to change what
 * is included at compile-time, what CPU speed is used etc. You should copy
 * this file to your local project folder and change it there, not in the
 * platform/launchpad folder!
 */

/* CPU speed in Hz; these correspond to the factory calibrated settings */
#ifndef F_CPU
// #define F_CPU                   18000000uL
#define F_CPU                   16000000uL
// #define F_CPU                   8000000uL
//#define F_CPU                   4000000uL
//#define F_CPU                   12000000uL    // don't use the 12 MHz, it is not ready (UART etc)
#endif /* F_CPU */

/* is it an 2452 or 2553 mcu? Only these two can be defined for now.
XXX I will fix (later) so that the Makefile automatically choses the right mcu define
but until then you also have to change that in the Makefile.launchpad. You'll
see, if you get lots of "undefined reference to UCSB0..." it's because the wrong
mcu is defined there. This define just removes lots of code that, should you have
a 2452, it will compile anyway. */
#ifndef _MCU_
#define _MCU_                   2553
//#define _MCU_                   2452
#endif /* _MCU_ */
/*
Here is a short summary of the Launchpad mcu's
                 2211     2231    2452      2553
    ROM             2        2       8        16
    RAM           128      128     256       512
    UART                                       1
    SPI/I2C                  1       1         1
    GPIO           10       10      16        24
    touch-IO                         y         y
    Timers-16       1        1       1         2
    WDT             y        y       y         y
    ADC10                    8       8         8
    ADC12
    PDIP-x         14       14      20        20
    tempsensor               y       y         y
        1.8 V to 3.6 V
        16 MHz
        Wake-Up Less Than 1 µs
          Active Mode: 230 µA at 1 MHz, 2.2 V
          Standby Mode: 0.5 µA
          Off Mode (RAM Retention): 0.1 µA
  For Contiki,
    timer 1 (two CCR's) is used for the two internal clocks.
    Timer 2 for PWM.
    USCI 1 for UART serial/printfs.
    USCI 2 for SPI to radio (blocking so won't go to LPM while communicating).
  Thus we see that 2452 has an SPI USCI but not enough RAM for radio+Rime, nor
    will it accomodate the PWM or serial modules as they use the second timer.
*/

/*---------------------------------------------------------------------------*/
/* use UART serial port? (printfs), note, edit contiki-main if higher speed than 9600 is wanted. */
#ifndef USE_SERIAL
#define USE_SERIAL              1
#endif /* USE_SERIAL */

/*
 * The Launchpad is limited to 9600 by the msp430-usb bridge; higher speeds can
 * be used with a separate uartserial->usb cable connected to the rxtx pins on
 * the header, but this Contiki port is kept at a maximum simplicity now, hence
 * 9600. If you do get one such cable, you can increase this to eg 115200.
 */
#ifndef UART_SPEED
#define UART_SPEED              9600
#endif /* UART_SPEED */
/*---------------------------------------------------------------------------*/
/*
 * the LEDs on pins P1.6 and P1.0 can interfere with other peripherals, therefore
 * one can disable them with this config. Notably, the green LED P1.6 interferes
 * with i2c.
 */
#ifndef USE_LEDS
#define USE_LEDS              1
#endif /* USE_LEDS */
/*---------------------------------------------------------------------------*/
/* use the Rime networking stack and a radio driver? */
#ifndef USE_RADIO
#define USE_RADIO               0
#endif /* USE_RADIO */

/*
 * board revision: older boards had a pull-up resistor connected to the button
 * which was removed to conserve power and save money. If the resistor is there,
 * the button needs some extra care behind the curtains, which will happen if this is
 * set to 1. To check if you need to set this to 1, check for a small (ca 1 by
 * 2 mm) rectangular thing in the spot where it says R34, close to the top of
 * the left pin headers (when the USB connector is facing up). If it is there,
 * set this to 1.
 *
 * Note: setting this to 1 will save power on old boards but button will not work
 * on >=rev1.5 boards; if set to 0 the button will work on rev1.3, 1.4 and 1.5 but
 * draw more power on the 1.3 and 1.4 revisions.
 */
#ifndef BOARD_OLD_REVISION
#define BOARD_OLD_REVISION      0
#endif /* BOARD_OLD_REVISION */

/*
 * LEDs; only change if you are not using the Launchpad board and have LEDs
 * on other pins. You will likely have to change in cpu/msp430/g2xxx/leds-arch.c
 * as well as core/dev/leds.{c, h} if you want to add more LEDs or change pins.
 */
#define LEDS_PORT(type)         P1##type
#define LEDS_CONF_RED           (1<<0)
#define LEDS_CONF_GREEN         (1<<6)
#define LEDS_CONF_YELLOW        (1<<6)    /* only here to remove compilation errors */
#define LEDS_CONF_ALL           (LEDS_CONF_GREEN | LEDS_CONF_RED)


/* PWM freq; can be higher and lower; setting to lower will increase accuracy of
  the duty cycle (rounding errors) calculated in pwm_on(). */
#define PWM_CONF_FREQ           128

#ifdef PWM_CONF_FREQ
#define PWM_FREQ                PWM_CONF_FREQ
#else
#define PWM_FREQ                128
#endif

/* P1.3 is the switch 2 on the Launchpad PCB, but other pins can be used. */
//XXX now defined in platform/launchpad/button.h
//#define BUTTON_CONF_PORT        P1
//#define BUTTON_CONF_PIN         (1<<3)

/* use irq/dma with UART to save system resources, otherwise synchrous (blocking) */
// XXX not working yet, is only blocking so leave as 0 now.
#define UART0_CONF_TX_WITH_INTERRUPT  0

/* this is where in memory the node id is stored (must first be written by burn),
  these memory locations are non-volatile and will not be erased by a 'full erase'
  unless explicitly told to, hence we can save some data here that will survive
  reboots and periods without power. Infomem A has some factory calibration data.
  Store node id in infomem C as it is normally empty (A is occupied and D seems
  not empty). 64 B each. Node id is prepended with the magic bytes 0xBEEF. */
#define INFOMEM_D                     ((uint8_t*)0x00001000)
#define INFOMEM_C                     ((uint8_t*)0x00001040)
#define INFOMEM_B                     ((uint8_t*)0x00001080)
#define INFOMEM_A                     ((uint8_t*)0x000010c0)
#define NODEID_INFOMEM_LOCATION       INFOMEM_C
/*--------------------------------------------------------------------------*/
/*
 * The CC2500 definitions - pins, ports, interrupt vector etc - are defined in
 * the following files:
 *    spi.c, spi.h
 *    cc2500.c, cc2500.h, cc2500-arch.c, cc2500-const.h
 */
/*--------------------------------------------------------------------------*/
/* some helping defines that normally should not be changed */
/* Clock resolutions */
#define CLOCK_CONF_SECOND               (128UL)
#define RTIMER_CONF_SECOND              32768

/* Types for clocks and uip_stats */
typedef unsigned short uip_stats_t;
typedef unsigned long clock_time_t;
typedef unsigned long off_t;
/* errors ------------------------------------*/
#if _MCU_ != 2452 && _MCU_ != 2553
#error "Wrong mcu type defined; check your platform-conf.h and/or project-conf.h!"
#endif

#if F_CPU != 16000000uL && F_CPU != 8000000uL && F_CPU != 4000000uL
#error "Unsupported CPU speed; check your platform-conf.h and/or project-conf.h!"
#endif

#if _MCU_ == 2452 && USE_SERIAL
#error "2452 has no hardware UART, either set 2553 or set USE_SERIAL to 0 in platform-conf.h"
#endif
/* other various things ----------------------------*/
#define BAUD2UBR(baud) ((F_CPU/baud))
#define CCIF
#define CLIF

#define HAVE_STDINT_H
#define MSP430_MEMCPY_WORKAROUND 1
#if defined(__MSP430__) && defined(__GNUC__) && MSP430_MEMCPY_WORKAROUND
#else /* __GNUC__ &&  __MSP430__ && MSP430_MEMCPY_WORKAROUND */
#define w_memcpy memcpy
#endif /* __GNUC__ &&  __MSP430__ && MSP430_MEMCPY_WORKAROUND */
#include "msp430def.h"
/*--------------------------------------------------------------------------*/
#endif /* __PLATFORM_CONF_H__ */
