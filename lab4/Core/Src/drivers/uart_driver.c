#include <drivers/uart_driver.h>

extern UART_HandleTypeDef huart6;

static uint8_t rx_buf[UART_RX_BUF_SIZE];
static uint8_t tx_buf[UART_TX_BUF_SIZE];

static volatile uint16_t rx_head = 0, rx_tail = 0;
static volatile uint16_t tx_head = 0, tx_tail = 0;

static uint8_t rx_byte;
static uint8_t interrupts_enabled = 0;

static char s_line_buf[UART_MAX_LINE];
static uint16_t s_line_len = 0;

static inline uint32_t uart_critical_enter(void) {
    uint32_t pmask = __get_PRIMASK();
    __disable_irq();
    return pmask;
}

static inline void uart_critical_exit(uint32_t pmask) {
    __set_PRIMASK(pmask);
}

void uart_set_interrupts(uint8_t enabled)
{
    interrupts_enabled = enabled;
    if (enabled)
    {
        rx_head = rx_tail = 0;
        tx_head = tx_tail = 0;
		HAL_NVIC_EnableIRQ(USART6_IRQn);
        HAL_UART_Receive_IT(&huart6, &rx_byte, 1);
    }
    else
    {
        HAL_UART_AbortReceive_IT(&huart6);
        HAL_UART_AbortTransmit_IT(&huart6);
        HAL_NVIC_DisableIRQ(USART6_IRQn);
    }
}

// Передача символа из МК на ПК
void uart_transmit_char(char ch)
{
    if (!interrupts_enabled) {
        HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, UART_TIMEOUT);
        return;
    }

    uint32_t pmask = uart_critical_enter();

    tx_buf[tx_head] = (uint8_t)ch;
    tx_head = (tx_head + 1) % UART_TX_BUF_SIZE;

	if (tx_head == tx_tail)
		tx_tail = (tx_tail + 1) % UART_TX_BUF_SIZE;

    uint8_t need_start = (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_TXE) != 0) && (tx_head != tx_tail);

    uart_critical_exit(pmask);

    if (need_start) {
        HAL_UART_Transmit_IT(&huart6, &tx_buf[tx_tail], 1);
    }
}

void uart_transmit_string(char *str) {
	while (*str) uart_transmit_char(*str++);
}

void uart_transmitln_string(char *str)
{
	uart_transmit_string(str);
	uart_transmit_string("\r\n");
}

// Прием с ПК на МК
int uart_receive_char(char *ch)
{
    if (!interrupts_enabled) {
        return (HAL_UART_Receive(&huart6, (uint8_t *)ch, 1, 0) == HAL_OK) ? 1 : 0;
    }
    else
    {
        int is_byte_available = 0;
        uint32_t pmask = uart_critical_enter();

        if (rx_head != rx_tail) {
            *ch = rx_buf[rx_tail];
            rx_tail = (rx_tail + 1) % UART_RX_BUF_SIZE;
            is_byte_available = 1;
        }

        uart_critical_exit(pmask);
        return is_byte_available;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART6) return;

    rx_buf[rx_head] = rx_byte;
    rx_head = (rx_head + 1) % UART_RX_BUF_SIZE;

    if (rx_head == rx_tail)
        rx_tail = (rx_tail + 1) % UART_RX_BUF_SIZE;

    HAL_UART_Receive_IT(&huart6, &rx_byte, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART6) return;
    if (!(tx_head == tx_tail))
    {
        tx_tail = (tx_tail + 1) % UART_TX_BUF_SIZE;
        if (!(tx_head == tx_tail))
            HAL_UART_Transmit_IT(&huart6, &tx_buf[tx_tail], 1);
    }
}

void uart_receive_line_task(void) {
    char c;
    while (uart_receive_char(&c)) {
    	uart_transmit_char(c);
        switch (c) {
        case '\r':
        case '\n':
            if (s_line_len > 0) {
                s_line_buf[s_line_len] = '\0';
                uart_transmitln_string("");
                process_command_line(s_line_buf);
                s_line_len = 0;
            }
            break;
        case '\b':
        case 0x7F:
            if (s_line_len > 0) s_line_len--;
            break;
        default:
            if (isprint((unsigned)c) && s_line_len < UART_MAX_LINE - 1)
                s_line_buf[s_line_len++] = c;
        }
    }
}
