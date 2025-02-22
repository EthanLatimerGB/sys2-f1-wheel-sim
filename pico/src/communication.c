#include "communication.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void setupUART()
{
	uart_init(UART_ID, BAUD_RATE);

	// Set the TX and RX pins by using the function select on the GPIO
	gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
	gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

	// uart_puts(UART_ID, " Hello, UART!\n");
}

void setupDMAforUART()
{
	// Claim a DMA
	dma_channel = dma_claim_unused_channel(true);
	dma_channel_config dma_conf =
	    dma_channel_get_default_config(dma_channel);

	channel_config_set_transfer_data_size(&dma_conf, DMA_SIZE_8);
	channel_config_set_read_increment(&dma_conf, false);
	channel_config_set_write_increment(&dma_conf, true);
	channel_config_set_dreq(&dma_conf, DREQ_UART0_RX);

	dma_channel_configure(dma_channel, &dma_conf, uart_rx_buffer,
			      &uart_get_hw(UART_ID)->dr, 256, false);
}

void setupDMAInterrupts()
{
	irq_set_exclusive_handler(DMA_IRQ_0, dmaIRQhandler);
	irq_set_enabled(DMA_IRQ_0, true);
	dma_channel_set_irq0_enabled(dma_channel, true);
	dma_channel_start(dma_channel);
}

void dmaIRQhandler()
{

}
