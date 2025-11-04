#include <stdint.h>

extern uint32_t _sdata, _edata, _sbss, _ebss, _estack;
int main(void);

void Reset_Handler(void) {
    uint32_t *src = (uint32_t *)((uintptr_t)&_edata - ((uintptr_t)&_edata - (uintptr_t)&_sdata));
    uint32_t *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;

    for (uint32_t *p = &_sbss; p < &_ebss; ++p) *p = 0;

    main();
    while (1) {}
}

__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),
    Reset_Handler,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0
};
