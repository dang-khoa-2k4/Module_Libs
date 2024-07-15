/*
 * nRF21L01.h
 *
 *  Created on: Jun 22, 2024
 *      Author: Nguyen Tran Dang Khoa
 */

#ifndef SRC_NRF21L01_H_
#define SRC_NRF21L01_H_

#include "main.h"
#include "string.h"
#include <stdbool.h>

/* User Configurations */
extern SPI_HandleTypeDef 				          hspi1;
#define nrf24l01_SPI                     (&hspi1)

#define NRF24L01_SPI_CS_PIN_PORT         GPIOB
#define NRF24L01_SPI_CS_PIN_NUMBER       GPIO_PIN_13

#define NRF24L01_CE_PIN_PORT             GPIOB
#define NRF24L01_CE_PIN_NUMBER           GPIO_PIN_12

#define NRF24L01_IRQ_PIN_PORT            GPIOA
#define NRF24L01_IRQ_PIN_NUMBER          GPIO_PIN_8

#define NRF24L01_PAYLOAD_LENGTH          8     // 1 - 32bytes
/* End User Configurations */

/* nRF24L01+ typedefs */
#define MIN_FREQUENCY 2400
typedef uint8_t count;
typedef uint8_t widths;
typedef uint16_t delay;
typedef uint16_t channel;

typedef enum
{
    _250kbps = 2,
    _1Mbps   = 0,
    _2Mbps   = 1
} air_data_rate;

typedef enum
{
    _0dBm  = 3,
    _6dBm  = 2,
    _12dBm = 1,
    _18dBm = 0
} output_power;

typedef enum
{
	_1byte = 0,
	_2byte = 1
} crc_length;
/* FUNCTION PART */

/* Main Functions */

/**
  * @brief  Init Rx Mode for nRF24lL01
  * @param  MHz is RF channel (2.4GHz - 2.525GHz)
  * @param  bps is air data rate (0.25, 1, 2 Mbps)
*/
void nrf24l01_rx_init(channel MHz, air_data_rate bps);

/**
  * @brief  Init Tx Mode for nRF24lL01
  * @param  MHz is frequency for RF channel (2.4GHz - 2.525GHz)
  * @param  bps is air data rate (0.25, 1, 2 Mbps)
*/
void nrf24l01_tx_init(channel MHz, air_data_rate bps);

/**
  * @brief  Receive payload
  * @param  rx_payload is the received data
  *
*/
void nrf24l01_rx_receive(uint8_t* rx_payload);

/**
  * @brief  Receive payload
  * @param  tx_payload is the transmitted data
  *
*/
uint8_t nrf24l01_tx_transmit(uint8_t* tx_payload);

/**
  * @brief  Check tx_ds or max_rt (used for retransmit)
  * @param  Null
*/
void nrf24l01_tx_irq();


/* Sub Functions */
/**
  * @brief  reset all register to default
  * @param  Null

*/
void nrf24l01_reset();

/**
  * @brief  Active PRX mode
  * @param  Null
*/
void nrf24l01_prx_mode();

/**
  * @brief  Active PTX mode
  * @param  Null

*/
void nrf24l01_ptx_mode();

/**
  * @brief  Power up the device
  * @param  Null
*/
void nrf24l01_power_up();

/**
  * @brief  Power down the device
  * @param  Null
*/
void nrf24l01_power_down();

/**
  * @brief  Get current status register
  * @param  Null
  * @return Status register
*/
uint8_t nrf24l01_get_status();

/**
  * @brief  Get current fifo status register
  * @param  Null
  * @return FIFO status register
*/
uint8_t nrf24l01_get_fifo_status();

// Static payload lengths

/**
  * @brief  Set payload width
  * @param  bytes is set width
  * @param  Channel
  *
*/
void nrf24l01_rx_set_payload_widths(widths bytes);

/**
  * @brief  Read payload from in fifo
  * @param  rx_payload is set data
  * @return status
*/
uint8_t nrf24l01_read_rx_fifo(uint8_t* rx_payload);

/**
  * @brief  Write payload into fifo
  * @param  tx_payload is written data
  * @return status
*/
uint8_t nrf24l01_write_tx_fifo(uint8_t* tx_payload);

/**
  * @brief  Flush Rx FIFO
  * @param  Null
*/
void nrf24l01_flush_rx_fifo();

/**
  * @brief  Flush Tx FIFO
  * @param  Null
*/
void nrf24l01_flush_tx_fifo();


// Clear IRQ pin. Change LOW to HIGH

/**
  * @brief  Clear RX_DR
  * @param  Null
*/
void nrf24l01_clear_rx_dr();

/**
  * @brief  Clear TX_DS
  * @param  Null
*/
void nrf24l01_clear_tx_ds();

/**
  * @brief  Clear MAX_RT (max number of retransmit interrupt)
  * @param  Null
*/
void nrf24l01_clear_max_rt();

/**
  * @brief  Set RF channel
  * @param  MHz is frequency for RF channel (2.4GHz - 2.525GHz)
*/
void nrf24l01_set_rf_channel(channel MHz);

/**
  * @brief  Set Tx Output power
  * @param  dBm is set output power
*/
void nrf24l01_set_rf_tx_output_power(output_power dBm);

/**
  * @brief  Set air data rate for RF module
  * @param  bps is set air data rate
*/
void nrf24l01_set_rf_air_data_rate(air_data_rate bps);

/**
  * @brief  Set crc length for Auto Acknowledgement (CRC giúp kiểm tra tính toàn vẹn của dữ liệu khi truyền không dây bằng nRF24L01.)
  * @param  bytes is length of crc (bytes is 1 or 2 bytes)(CRCO register: 0 -> 1 bytes; 1 -> 2 bytes)
  *
*/
void nrf24l01_set_crc_length(crc_length bytes);

/**
  * @brief  Set address width
  * @param  bytes is set address width
*/
void nrf24l01_set_address_widths(widths bytes);

/**
  * @brief  Set number of time it is allowed to retransmit (0 - 15)
  * @param  cnt is number of time retransmit
*/
void nrf24l01_auto_retransmit_count(count cnt);

/**
  * @brief  Set retransmit delay (250 - 4000 us) (250us per step)
  * @param  us is delay time for retransmit
*/
void nrf24l01_auto_retransmit_delay(delay us);

/**
  * @brief  write into register
  * @param  address is address of register
  * @param 	data is written data
*/
uint8_t nrf24l01_write_reg(uint8_t address, uint8_t data);

/**
  * @brief  read data from register
  * @param  address is address of register
*/
uint8_t nrf24l01_read_reg(uint8_t address);

/**
  * @brief  Set low for CS pin
  * @param  Null
*/
void CS_select();

/**
  * @brief  Set high for CS pin
  * @param  Null
*/
void CS_unselect();

/**
  * @brief  Set high for CE pin
  * @param  Null
*/
void CE_enable();

/**
  * @brief  Set low for CE pin
  * @param  Null
*/
void CE_disable();

/* END FUNCTION PART */


/* nRF24L01+ Commands */
#define R_REGISTER    					0x00
#define W_REGISTER    					0x20
#define REGISTER_MASK 					0x1F
#define ACTIVATE      					0x50
#define R_RX_PL_WID   					0x60
#define R_RX_PAYLOAD  					0x61
#define W_TX_PAYLOAD  					0xA0
#define W_ACK_PAYLOAD 					0xA8
#define FLUSH_TX      					0xE1
#define FLUSH_RX      					0xE2
#define REUSE_TX_PL   					0xE3
#define NOP           					0xFF
/* End nRF24L01+ Commands */

/* nRF24L01+ Registers */
#define CONFIG      					0x00
#define EN_AA       					0x01
#define EN_RXADDR   					0x02
#define SETUP_AW    					0x03
#define SETUP_RETR  					0x04
#define RF_CH       					0x05
#define RF_SETUP    					0x06
#define STATUS      					0x07
#define OBSERVE_TX  					0x08
#define CD          					0x09
#define RX_ADDR_P0  					0x0A
#define RX_ADDR_P1  					0x0B
#define RX_ADDR_P2  					0x0C
#define RX_ADDR_P3  					0x0D
#define RX_ADDR_P4  					0x0E
#define RX_ADDR_P5  					0x0F
#define TX_ADDR     					0x10
#define RX_PW_P0    					0x11
#define RX_PW_P1    					0x12
#define RX_PW_P2    					0x13
#define RX_PW_P3    					0x14
#define RX_PW_P4    					0x15
#define RX_PW_P5    					0x16
#define FIFO_STATUS 					0x17
#define DYNPD	    					0x1C
#define FEATURE	    					0x1D
/* End nRF24L01+ Registers */

#endif /* SRC_NRF21L01_H_ */
