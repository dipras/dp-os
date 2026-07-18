#include "irq.h"
#include "pic.h"
#include "../../drivers/keyboard.h"

void irq_handler(interrupt_frame_t* frame) {
    switch (frame->irq) {
        case 0:
            // Timer
            break;

        case 1:
            keyboard_on_irq();
            break;
    }

    pic_send_eoi(frame->irq);
}