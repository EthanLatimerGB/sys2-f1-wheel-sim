#include "communication.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int dma_channel;

uint8_t uart_rx_buffer[256];

/*
 * ES	-> Engine Speed
 * CS	-> Car Speed
 * CG	-> Current Gear
 * T	-> Throttle %
 * B	-> Brake Pressure %
*/

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

/*
* PARSING TEXT FUNCTIONS
*/

Command parseCommand(const char *command_str, const char *value_str)
{
	Command command = {.cmd = EXPR_INVALID,.value = 0 };
	if (strcmp(command_str, "ES") == 0) {
		command.cmd = EXPR_ES;
	} else if (strcmp(command_str, "CS") == 0) {
		command.cmd = EXPR_CS;
	} else if (strcmp(command_str, "CG") == 0) {
		command.cmd = EXPR_CG;
	} else if (strcmp(command_str, "T") == 0) {
		command.cmd = EXPR_T;
	} else if (strcmp(command_str, "B") == 0) {
		command.cmd = EXPR_B;
	}

	char *end;
	int par_val = strtol(value_str, &end, 10);

	// Invalid strings will mark command invalid
	if (*end != '\0') {
		command.cmd = EXPR_INVALID;
		return command;
	}

	par_val < 0 ? par_val = 0 : par_val;
	par_val > 65535 ? par_val = 65535 : par_val;
	command.value = (uint16_t) par_val;

	return command;
}

int parseInput(const char *input, Command *commands, int max_commands)
{
	// Used for context in `strtok_r`.
	int command_count = 0;
	char *saveptr = NULL;	// For splitting semi-colons (commands)
	char buffer[256];

	// Copy contents into buffer, cut off if the input is larger than buffer. 
	int input_size = MIN(sizeof(buffer) - 1, strlen(input));
	strncpy(buffer, input, input_size);

	// Discard an input if it does not terminate with ';'
	if (buffer[input_size - 1] != ';') {
		printf
		    ("The last character over UART is: %c. This is invalid and this entire command will be ignored. \n",
		     buffer[input_size - 1]);
		return -1;
	}
	// Piece stores the current value of the split.
	char *piece = strtok_r(buffer, ";", &saveptr);

	while (piece != NULL && command_count < MAX_COMMANDS) {
		// Discard commands of incompatible length 
		if (strlen(piece) < 3 || strlen(piece) > 8) {
			piece = strtok_r(NULL, ";", &saveptr);
			continue;
		}
		// Similarly, we use this context for splitting commands via ':'
		char *saveptr2 = NULL;	// For splitting colons (key:val pairs)
		char *command_str = strtok_r(piece, ":", &saveptr2);
		char *value_str = strtok_r(NULL, ":", &saveptr2);

		// Invalid splits will skip the command
		if (!command_str || !value_str) {
			piece = strtok_r(NULL, ";", &saveptr);
			continue;
		}

		Command parsed_cmd = parseCommand(command_str, value_str);
		if (parsed_cmd.cmd == EXPR_INVALID)
			continue;

		commands[command_count] = parsed_cmd;

		command_count++;
		piece = strtok_r(NULL, ";", &saveptr);
	}

	// Set all others to invalid so they are not processed
	for (int i = command_count; i < MAX_COMMANDS; i++) {
		commands[i].cmd = EXPR_INVALID;
	}

}
