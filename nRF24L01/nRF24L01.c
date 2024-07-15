/*
 * nRF21L01.c
 *
 *  Created on: Jun 22, 2024
 *      Author: Nguyen Tran Dang Khoa
 */

#include "nRF24L01.h"

void CS_select() 	{ HAL_GPIO_WritePin(NRF24L01_SPI_CS_PIN_PORT, NRF24L01_SPI_CS_PIN_NUMBER, 0); }
void CS_unselect()	{ HAL_GPIO_WritePin(NRF24L01_SPI_CS_PIN_PORT, NRF24L01_SPI_CS_PIN_NUMBER, 1); }
void CE_enable()	{ HAL_GPIO_WritePin(NRF24L01_CE_PIN_PORT, NRF24L01_CE_PIN_NUMBER, 1); }
void CE_disable()	{ HAL_GPIO_WritePin(NRF24L01_CE_PIN_PORT, NRF24L01_CE_PIN_NUMBER, 0); }


uint8_t nrf24l01_write_reg(uint8_t address, uint8_t data)
{
	uint8_t cmd = W_REGISTER | address;
	uint8_t status;
	uint8_t write_data = data;

	CS_select();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	HAL_SPI_Transmit(nrf24l01_SPI, &write_data, 1, 1000);
	CS_unselect();

	return write_data;
}
uint8_t nrf24l01_read_reg(uint8_t address)
{
	uint8_t cmd = R_REGISTER | address;
	uint8_t status;
	uint8_t read_data;

	CS_select();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	HAL_SPI_Receive(nrf24l01_SPI, &read_data, 1, 100);
	CS_unselect();

	return read_data;
}

void nrf24l01_reset()
{
	CE_disable();

	nrf24l01_write_reg(CONFIG, 0x08);
	nrf24l01_write_reg(EN_AA, 0x3F);
	nrf24l01_write_reg(EN_RXADDR, 0x03);
	nrf24l01_write_reg(SETUP_AW, 0x03);
	nrf24l01_write_reg(SETUP_RETR, 0x03);
	nrf24l01_write_reg(RF_CH, 0x02);
	nrf24l01_write_reg(RF_SETUP, 0x07);
	nrf24l01_write_reg(STATUS, 0x0E);
	nrf24l01_write_reg(OBSERVE_TX, 0x00);
	nrf24l01_write_reg(RX_ADDR_P0, 0x00);
	nrf24l01_write_reg(RX_ADDR_P1, 0x00);
	nrf24l01_write_reg(RX_ADDR_P2, 0x00);
	nrf24l01_write_reg(RX_ADDR_P3, 0x00);
	nrf24l01_write_reg(RX_ADDR_P4, 0x00);
	nrf24l01_write_reg(RX_ADDR_P5, 0x00);
	nrf24l01_write_reg(FIFO_STATUS, 0x11);
	nrf24l01_write_reg(DYNPD, 0x00);
	nrf24l01_write_reg(FEATURE, 0x00);

	nrf24l01_flush_rx_fifo();
	nrf24l01_flush_tx_fifo();

	CE_enable();
}

void nrf24l01_rx_init(channel MHz, air_data_rate bps)
{
    nrf24l01_reset();

    CE_disable();

    nrf24l01_prx_mode();
    nrf24l01_power_up();

    nrf24l01_rx_set_payload_widths(NRF24L01_PAYLOAD_LENGTH);

    nrf24l01_set_rf_channel(MHz);
    nrf24l01_set_rf_air_data_rate(bps);
    nrf24l01_set_rf_tx_output_power(_0dBm);

    nrf24l01_set_crc_length(_1byte);
    nrf24l01_set_address_widths(5);

    nrf24l01_auto_retransmit_count(3);
    nrf24l01_auto_retransmit_delay(250);

    CE_enable();
}

void nrf24l01_tx_init(channel MHz, air_data_rate bps)
{
    nrf24l01_reset();

    CE_disable();

    nrf24l01_ptx_mode();
    nrf24l01_power_up();

    nrf24l01_set_rf_channel(MHz);
    nrf24l01_set_rf_air_data_rate(bps);
    nrf24l01_set_rf_tx_output_power(_0dBm);

    nrf24l01_set_crc_length(_1byte);
    nrf24l01_set_address_widths(5);

    nrf24l01_auto_retransmit_count(3);
    nrf24l01_auto_retransmit_delay(250);

    CE_enable();
}

void nrf24l01_rx_receive(uint8_t* rx_payload)
{
    nrf24l01_read_rx_fifo(rx_payload);
    nrf24l01_clear_rx_dr();
    nrf24l01_flush_rx_fifo();
    // for testing
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

uint8_t nrf24l01_read_rx_fifo(uint8_t* rx_payload)
{
	uint8_t cmd = R_RX_PAYLOAD;
	uint8_t status;

	CS_unselect();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	HAL_SPI_Receive(nrf24l01_SPI, rx_payload, NRF24L01_PAYLOAD_LENGTH, 1000);
	CS_select();

	return status;
}

uint8_t nrf24l01_tx_transmit(uint8_t* tx_payload)
{
    nrf24l01_write_tx_fifo(tx_payload);

    uint8_t fifo_status = nrf24l01_read_reg(FIFO_STATUS);

    // check TX fifo is empty ?
    if ((fifo_status & (1 << 4)) && !(fifo_status & (1 << 3)))
    {
    	nrf24l01_flush_tx_fifo();
    	return 1;
    }
	return 0;
}

uint8_t nrf24l01_write_tx_fifo(uint8_t* tx_payload)
{
	uint8_t cmd = W_TX_PAYLOAD;
	uint8_t status;

	CS_unselect();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	HAL_SPI_Transmit(nrf24l01_SPI, tx_payload, NRF24L01_PAYLOAD_LENGTH, 1000);
	CS_select();

	return status;
}

void nrf24l01_flush_rx_fifo()
{
	uint8_t cmd = FLUSH_RX;
	uint8_t status;

	CS_unselect();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	CS_select();
}

void nrf24l01_flush_tx_fifo()
{
	uint8_t cmd = FLUSH_TX;
	uint8_t status;

	CS_unselect();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	CS_select();
}

void nrf24l01_clear_rx_dr()
{
	uint8_t new_status = nrf24l01_get_status();

	new_status |= 0x40; //set RX_DR to 1 for clear bit
	nrf24l01_write_reg(STATUS, new_status);
}

void nrf24l01_clear_tx_ds()
{
	uint8_t new_status = nrf24l01_get_status();

	new_status |= 0x20; //set TX_DS to 1 for clear bit
	nrf24l01_write_reg(STATUS, new_status);
}

void nrf24l01_clear_max_rt()
{
	uint8_t new_status = nrf24l01_get_status();

	new_status |= 0x10; //set MAX_RT to 1 for clear bit
	nrf24l01_write_reg(STATUS, new_status);
}

void nrf24l01_ptx_mode()
{
	uint8_t new_config = nrf24l01_read_reg(CONFIG);
	new_config &= 0xFE; 	// set last bit is 0
	nrf24l01_write_reg(CONFIG, new_config);
}

void nrf24l01_prx_mode()
{
	uint8_t new_config = nrf24l01_read_reg(CONFIG);
	new_config |= 1; 	// set last bit is 1
	nrf24l01_write_reg(CONFIG, new_config);
}

void nrf24l01_power_up()
{
	uint8_t new_config = nrf24l01_read_reg(CONFIG);
	new_config |= (1 << 1); 	// set second bit is 1
	nrf24l01_write_reg(CONFIG, new_config);
}

void nrf24l01_power_down()
{
	uint8_t new_config = nrf24l01_read_reg(CONFIG);
	new_config &= 0xFD; 	// set second bit is 0
	nrf24l01_write_reg(CONFIG, new_config);
}

void nrf24l01_set_rf_channel(channel MHz)
{
	nrf24l01_write_reg(RF_CH, MHz - MIN_FREQUENCY);
}

void nrf24l01_set_rf_air_data_rate(air_data_rate bps)
{
	uint8_t new_rf_air_rate = nrf24l01_read_reg(RF_SETUP) & 0xD7; // & 8b1101_0111 (set RF_DR_LOW and RF_DR_HIGH to 0)

	switch (bps)
	{
	case _250kbps:
		new_rf_air_rate |= (1 << 3);
		break;
	case _1Mbps:
		new_rf_air_rate |= 0;
		break;
	case _2Mbps:
		new_rf_air_rate |= (1 << 5);
		break;
	}

	nrf24l01_write_reg(RF_SETUP, new_rf_air_rate);
}

void nrf24l01_set_rf_tx_output_power(output_power dBm)
{
	uint8_t new_rf_output_power = nrf24l01_read_reg(RF_SETUP) & 0xF9; // & 8b1101_0111 (set PWR to 00)
	new_rf_output_power |= (1 << dBm);
	nrf24l01_write_reg(RF_SETUP, new_rf_output_power);
}

void nrf24l01_set_crc_length(crc_length bytes)
{
	uint8_t new_crc_length = nrf24l01_read_reg(CONFIG);

	switch (bytes)
	{
	case _1byte:
		new_crc_length &= 0xFB;
		break;
	case _2byte:
		new_crc_length |= 0x04;
		break;
	}
}

void nrf24l01_set_address_widths(widths bytes)
{
	nrf24l01_write_reg(SETUP_AW, bytes - 2);
}

uint8_t nrf24l01_get_status()
{
	uint8_t cmd = nrf24l01_read_reg(NOP);

	uint8_t status;

	CS_select();
	HAL_SPI_TransmitReceive(nrf24l01_SPI, &cmd, &status, 1, 1000);
	CS_unselect();

	return status;
}

uint8_t nrf24l01_get_fifo_status()	{	return nrf24l01_read_reg(FIFO_STATUS);	}

void nrf24l01_rx_set_payload_widths(widths bytes)
{
	// pipe 0 and 1 are selected by default
	nrf24l01_write_reg(RX_ADDR_P0, bytes);
}

void nrf24l01_auto_retransmit_count(count cnt)
{
	uint8_t new_config = nrf24l01_read_reg(SETUP_RETR);

	new_config &= 0xF0; // set ARC to 0000
	new_config |= cnt; // set ARC

	nrf24l01_write_reg(SETUP_RETR, new_config);
}

void nrf24l01_auto_retransmit_delay(delay us)
{
	uint8_t new_config = nrf24l01_read_reg(SETUP_RETR);

	new_config &= 0x0F; // set ARD to 0000
	new_config |= ((us / 250 - 1) << 4); // set ARD

	nrf24l01_write_reg(SETUP_RETR, new_config);
}

