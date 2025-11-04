/*
 * root@e2e8e047b92c:/work# qemu-system-arm -machine help | grep stm
 * stm32vldiscovery     ST STM32VLDISCOVERY (Cortex-M3) 
 */

/* Using stm32vldiscovery */

#include <stdint.h>

#define PERIPH_BASE     0x40000000UL
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x00020000UL)

#define GPIOA_BASE      (APB2PERIPH_BASE + 0x0800UL)
#define RCC_BASE        (AHBPERIPH_BASE + 0x1000UL)
#define USART1_BASE     (APB2PERIPH_BASE + 0x3800UL)

#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOA_CRH       (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define USART1_SR       (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile uint32_t *)(USART1_BASE + 0x0C))

static void delay(volatile uint32_t n)
{ 
    while(n--) __asm__("nop");
}

static void uart1_init(void)
{
    RCC_APB2ENR |= (1 << 2);   /* Enable GPIOA clock */
    RCC_APB2ENR |= (1 << 14);  /* Enable USART1 clock */

    GPIOA_CRH &= ~((0xF << (1 * 4)) | (0xF << (2 * 4))); /* Clear bits for PA9/PA10 */
    GPIOA_CRH |=  (0xB << (1 * 4));  /* PA9: MODE=11 (50MHz), CNF=10 (AF PP) */
    GPIOA_CRH |=  (0x4 << (2 * 4));  /* PA10: MODE=00, CNF=01 (floating input) */

    USART1_BRR = 0x341;              /* 9600 baud @ ~8MHz */
    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2); /* UE | TE | RE */
}

static void uart1_putc(uint8_t c)
{
    while (!(USART1_SR & (1<<7))); /* Wait for TXE */
    USART1_DR = (uint8_t)c;
}

static void uart1_puts(const uint8_t *s)
{
    while(*s) uart1_putc(*s++);
}

static void uart1_putint(uint32_t v)
{
    uint8_t b[12];
    uint32_t i=0;
    uint32_t neg=0;
    if (v == 0) { uart1_putc('0'); return; }
    if (v < 0) { neg = 1; v = -v; }
    while (v && i < sizeof b) { b[i++] = '0' + (v % 10); v /= 10; }
    if (neg) uart1_putc('-');
    while (i--) uart1_putc(b[i]);
}

uint32_t main(void)
{
    uint32_t count = 0;

    uart1_init();

    while(1)
    {
        uart1_puts("Running QEMU count: ");
        uart1_putint(count++);
        uart1_puts("\r\n");
        delay(1200000000);
    }
}
