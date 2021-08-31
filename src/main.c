#include "MDR32Fx.h"

int main(void) {
    MDR_RST_CLK->PER_CLOCK |= (1 << 23);
    MDR_PORTC->PWR |= PORT_PWR2_Msk;
    MDR_PORTC->ANALOG |= 0x4;
    MDR_PORTC->OE |= 0x4;

    volatile uint16_t wait = 0;

    while (1) {
        wait++;

        if (wait == 65535) {
            MDR_PORTC->RXTX ^= 0x4;
        }
    }
}
