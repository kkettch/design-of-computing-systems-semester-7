#include <drivers/uart_driver.h>

extern UART_HandleTypeDef huart6;

static uint8_t rx_buf[UART_RX_BUF_SIZE];
static uint8_t tx_buf[UART_TX_BUF_SIZE];
static volatile uint16_t rx_head = 0, rx_tail = 0;
static volatile uint16_t tx_head = 0, tx_tail = 0;

static uint8_t rx_byte;
static uint8_t interrupts_enabled = 0;

static inline int rb_empty(uint16_t head, uint16_t tail) { return head == tail; }
static inline int rb_full(uint16_t head, uint16_t tail, uint16_t size) { return ((head + 1) % size) == tail; }

static char s_line_buf[UART_MAX_LINE];
static uint16_t s_line_len = 0;

/*
 * Установка режимы работы с прерываниями (enabled = 1) или
 * Выключение режима реботы с прерываниями (enabled = 0)
 */
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

/*
 * Получаем текущее значение режима работы с прерываниями. Включен или нет
 */
uint8_t uart_get_interrupts(void) { return interrupts_enabled; }

/*
 * Передача на ПК от SDK 1 символа
 */
void uart_print_char(char ch)
{
    if (!interrupts_enabled)
    {
        HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, UART_TIMEOUT);
        return;
    }
    uint16_t next = (tx_head + 1) % UART_TX_BUF_SIZE;
    if (!rb_full(tx_head, tx_tail, UART_RX_BUF_SIZE))
    {
        tx_buf[tx_head] = (uint8_t)ch;
        tx_head = next;
        if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_TXE))
            HAL_UART_Transmit_IT(&huart6, &tx_buf[tx_tail], 1);
    }
}

/*
 * Получение на ПК целой строки (вызов uart_print_char для каждого символа строки)
 */
void uart_print_string(char *str) { while (*str) uart_print_char(*str++); }

/*
 * Получение на ПК строки с добавление в конце перехода на новую строку
 */
void uart_println_string(char *str)
{
	uart_print_string(str);
    uart_print_string("\r\n");
}

/*
 * Принимаем байт
 */
static int uart_read_char(char *ch)
{
    if (!interrupts_enabled)
    {
        if (HAL_UART_Receive(&huart6, (uint8_t *)ch, 1, 0) == HAL_OK)
        	return 1;
        else
            return 0;
    }
    else
    {
    	if (rb_empty(rx_head, rx_tail)) return 0;
        *ch = rx_buf[rx_tail];
        rx_tail = (rx_tail + 1) % UART_RX_BUF_SIZE;
        return 1;
    }
}

/*
 * Коллбэк на пришедший байт - срабатывает каждый раз, когда реально пришел новый байт
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART6) return;
    uint16_t next = (rx_head + 1) % UART_RX_BUF_SIZE;
    if (!rb_full(rx_head, rx_tail, UART_RX_BUF_SIZE))
    {
        rx_buf[rx_head] = rx_byte;
        rx_head = next;
    }
    HAL_UART_Receive_IT(&huart6, &rx_byte, 1);
}

/*
 * Коллбэк на уже отправленный байт - срабатывает каждый раз, когда реально отправила один байт из очереди tx_buf
 * Срабатывает после отправки текущего байта - продвигается хвост очереди
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART6) return;
    if (!rb_empty(tx_head, tx_tail))
    {
        tx_tail = (tx_tail + 1) % UART_TX_BUF_SIZE;
        if (!rb_empty(tx_head, tx_tail))
            HAL_UART_Transmit_IT(&huart6, &tx_buf[tx_tail], 1);
    }
}

/*
 *
 */
void uart_receive_line_task(void) {
    char c;
    while (uart_read_char(&c)) {
        uart_print_char(c);
        switch (c) {
        case '\r':
        case '\n':
            if (s_line_len > 0) {
                s_line_buf[s_line_len] = '\0';
                uart_println_string("");
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
