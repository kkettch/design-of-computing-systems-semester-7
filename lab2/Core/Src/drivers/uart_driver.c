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
    if (enabled) // включаем работу с прерываниями
    {
        rx_head = rx_tail = 0; // обнуление указателя на буфер, хранящий данные пришедшие с SDK (rx_buf)
        tx_head = tx_tail = 0; // обнуление указателя на буфер, хранящий данные для отправки на SDK (tx_buf)
        HAL_UART_Receive_IT(&huart6, &rx_byte, 1); // "подписка" на прием 1 байта - сейчас ничего не прочитается, только когда байт будет готов и вызовется HAL_UART_RxCpltCallback
    }
    else // выключаем работу с прерываниями
    {
        HAL_UART_AbortReceive_IT(&huart6); // обрываем операцию на ожидание прерывания для получение байта с SDK
        HAL_UART_AbortTransmit_IT(&huart6); // обрываем операция на получение прерывания для отправки байта на SDK
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
    if (!interrupts_enabled) // прерывания выключены
    {
        HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, UART_TIMEOUT); // отправляем один символ за максимум UART_TIMEOUT мс
        return;
    }
    uint16_t next = (tx_head + 1) % UART_TX_BUF_SIZE; // обновляем ссылку на следующую ячейку в tx_buf
    if (!rb_full(tx_head, tx_tail, UART_RX_BUF_SIZE)) // если в tx_buf еще есть место
    {
        tx_buf[tx_head] = (uint8_t)ch;	// кладем 1 байт в tx-очередь
        tx_head = next;	// меняем указатель на следующий (пока пустой) байт в tx-очереди
        if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_TXE)) // UART свободен (Transmit data register empty == true)
            HAL_UART_Transmit_IT(&huart6, &tx_buf[tx_tail], 1); // запускается передача первого байта
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
    if (!interrupts_enabled) // прерывания выключены
    {
        if (HAL_UART_Receive(&huart6, (uint8_t *)ch, 1, 0) == HAL_OK) // принимаем 1 байт с таймаутом 0 (неблокирующе) - если байта возврат 0 и идем дальше
            return 1;
        else
            return 0;
    }
    else // с прерываниями
    {
    	if (rb_empty(rx_head, rx_tail)) return 0; // в буфере на прием rx_buf ничего нет, очередь пустая
        *ch = rx_buf[rx_tail]; // забираем из очереди принятый байт
        rx_tail = (rx_tail + 1) % UART_RX_BUF_SIZE; // смещаем указатель на следующий в очереди байт
        return 1;
    }
}

/*
 * Коллбэк на пришедший байт - срабатывает каждый раз, когда реально пришел новый байт
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART6) return;
    uint16_t next = (rx_head + 1) % UART_RX_BUF_SIZE; // указатель на адрес следующего элемента в rx_buf
    if (!rb_full(rx_head, rx_tail, UART_RX_BUF_SIZE)) // проверяем что в rx_buf есть место
    {
        rx_buf[rx_head] = rx_byte;
        rx_head = next;
    }
    HAL_UART_Receive_IT(&huart6, &rx_byte, 1); // сразу после приемя ждем следующий байт (запускаем непрерывный поток)
}

/*
 * Коллбэк на уже отправленный байт - срабатывает каждый раз, когда реально отправила один байт из очереди tx_buf
 * Срабатывает после отправки текущего байта - продвигается хвост очереди
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART6) return;
    if (!rb_empty(tx_head, tx_tail)) // если очередь не пустая
    {
        tx_tail = (tx_tail + 1) % UART_TX_BUF_SIZE; // этот байт уже ушёл, идем к следующему
        if (!rb_empty(tx_head, tx_tail))
            HAL_UART_Transmit_IT(&huart6, &tx_buf[tx_tail], 1); // шлем следующий байт
    }
}


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
