
#ifndef __CC2500_H__
#define __CC2500_H__


#include "contiki.h"
#include "dev/spi.h"
#include "dev/radio.h"
#include "dev/cc2500-const.h"

#define CC2500_MAX_PACKET_LEN      63   // XXX 64?
int cc2500_init(void);
int cc2500_set_channel(int channel);
int cc2500_get_channel(void);
int cc2500_rssi(void);
void cc2500_set_pan_addr(unsigned pan,
                                unsigned addr,
                                const uint8_t *ieee_addr);

//param power Between 1 and 31.
void cc2500_set_txpower(uint8_t power);
int cc2500_get_txpower(void);
#define CC2500_TXPOWER_MAX  31
#define CC2500_TXPOWER_MIN   0
int cc2500_interrupt(void);
int cc2500_on(void);
int cc2500_off(void);
void cc2500_set_cca_threshold(int value);

#if 0
extern signed char cc2500_last_rssi;
extern uint8_t cc2500_last_correlation;
extern const struct radio_driver cc2500_driver;
extern rtimer_clock_t cc2500_time_of_arrival,
  cc2500_time_of_departure;
extern int cc2500_authority_level_of_sender;
#endif

#define CC2500_STROBE(s)                                   \
  do {                                                  \
    CC2500_SPI_ENABLE();                                \
    SPI_WRITE(s);                                       \
    CC2500_SPI_DISABLE();                               \
  } while (0)

/* Write to a register in the CC2500                         */
/* Note: the SPI_WRITE(0) seems to be needed for getting the */
/* write reg working on the Z1 / MSP430X platform            */
#define CC2500_WRITE_REG(adr,data)                              \
  do {                                                       \
    CC2500_SPI_ENABLE();                                     \
    SPI_WRITE_FAST(adr);                                     \
    SPI_WRITE_FAST((uint8_t)((data) >> 8));                  \
    SPI_WRITE_FAST((uint8_t)(data & 0xff));                  \
    SPI_WAITFORTx_ENDED();                                   \
    SPI_WRITE(0);                                            \
    CC2500_SPI_DISABLE();                                    \
  } while(0)

/* Read a register in the CC2500 */
#define CC2500_READ_REG(adr,data)                          \
  do {                                                  \
    CC2500_SPI_ENABLE();                                \
    SPI_WRITE(adr | 0x40);                              \
    data = (uint8_t)SPI_RXBUF;                          \
    SPI_TXBUF = 0;                                      \
    SPI_WAITFOREORx();                                  \
    data = SPI_RXBUF << 8;                              \
    SPI_TXBUF = 0;                                      \
    SPI_WAITFOREORx();                                  \
    data |= SPI_RXBUF;                                  \
    CC2500_SPI_DISABLE();                               \
  } while(0)

#define CC2500_READ_FIFO_BYTE(data)                        \
  do {                                                  \
    CC2500_SPI_ENABLE();                                \
    SPI_WRITE(CC2500_RXFIFO | 0x40);                    \
    (void)SPI_RXBUF;                                    \
    SPI_READ(data);                                     \
    clock_delay(1);                                     \
    CC2500_SPI_DISABLE();                               \
  } while(0)

#define CC2500_READ_FIFO_BUF(buffer,count)                                 \
  do {                                                                  \
    uint8_t i;                                                          \
    CC2500_SPI_ENABLE();                                                \
    SPI_WRITE(CC2500_RXFIFO | 0x40);                                    \
    (void)SPI_RXBUF;                                                    \
    for(i = 0; i < (count); i++) {                                      \
      SPI_READ(((uint8_t *)(buffer))[i]);                               \
    }                                                                   \
    clock_delay(1);                                                     \
    CC2500_SPI_DISABLE();                                               \
  } while(0)

#define CC2500_WRITE_FIFO_BUF(buffer,count)                                \
  do {                                                                  \
    uint8_t i;                                                          \
    CC2500_SPI_ENABLE();                                                \
    SPI_WRITE_FAST(CC2500_TXFIFO);                                           \
    for(i = 0; i < (count); i++) {                                      \
      SPI_WRITE_FAST(((uint8_t *)(buffer))[i]);                              \
    }                                                                   \
    SPI_WAITFORTx_ENDED();                                              \
    CC2500_SPI_DISABLE();                                               \
  } while(0)

/* Write to RAM in the CC2500 */
#define CC2500_WRITE_RAM(buffer,adr,count)                 \
  do {                                                       \
    uint8_t i;                                               \
    CC2500_SPI_ENABLE();                                     \
    SPI_WRITE_FAST(0x80 | ((adr) & 0x7f));                   \
    SPI_WRITE_FAST(((adr) >> 1) & 0xc0);                     \
    for(i = 0; i < (count); i++) {                           \
      SPI_WRITE_FAST(((uint8_t*)(buffer))[i]);               \
    }                                                        \
    SPI_WAITFORTx_ENDED();                                   \
    CC2500_SPI_DISABLE();                                    \
  } while(0)

/* Read from RAM in the CC2500 */
#define CC2500_READ_RAM(buffer,adr,count)                    \
  do {                                                       \
    uint8_t i;                                               \
    CC2500_SPI_ENABLE();                                     \
    SPI_WRITE(0x80 | ((adr) & 0x7f));                        \
    SPI_WRITE((((adr) >> 1) & 0xc0) | 0x20);                 \
    SPI_RXBUF;                                               \
    for(i = 0; i < (count); i++) {                           \
      SPI_READ(((uint8_t*)(buffer))[i]);                     \
    }                                                        \
    CC2500_SPI_DISABLE();                                    \
  } while(0)

/* Read status of the CC2500 */
#define CC2500_GET_STATUS(s)                       \
  do {                                          \
    CC2500_SPI_ENABLE();                        \
    SPI_WRITE(CC2500_SNOP);                     \
    s = SPI_RXBUF;                              \
    CC2500_SPI_DISABLE();                       \
  } while (0)

#endif /* __CC2500_H__ */

