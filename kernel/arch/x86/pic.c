#include "pic.h"
#include "../../drivers/io.h"

static void io_wait(void) {
    outb(0x80, 0);
}

void pic_remap(uint8_t master_offset, uint8_t slave_offset) {
    /*
     * Simpan interrupt mask lama.
     *
     * Port data PIC berisi mask IRQ:
     * bit 1 = IRQ dimatikan
     * bit 0 = IRQ diaktifkan
     */
    uint8_t master_mask = inb(PIC1_DATA);
    uint8_t slave_mask  = inb(PIC2_DATA);

    /*
     * ICW1:
     * Beri tahu kedua PIC bahwa kita akan melakukan inisialisasi.
     */
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /*
     * ICW2:
     * Tentukan offset vector baru.
     *
     * Master: IRQ0-IRQ7   -> vector master_offset sampai master_offset + 7
     * Slave:  IRQ8-IRQ15  -> vector slave_offset sampai slave_offset + 7
     */
    outb(PIC1_DATA, master_offset);
    io_wait();

    outb(PIC2_DATA, slave_offset);
    io_wait();

    /*
     * ICW3:
     * Beri tahu hubungan master dan slave.
     *
     * Slave terhubung ke IRQ2 pada master.
     *
     * Master menerima bitmask 00000100b = 0x04.
     * Artinya ada slave pada IRQ2.
     *
     * Slave menerima angka 2.
     * Artinya slave terhubung pada jalur IRQ2 milik master.
     */
    outb(PIC1_DATA, 0x04);
    io_wait();

    outb(PIC2_DATA, 0x02);
    io_wait();

    /*
     * ICW4:
     * Gunakan mode 8086/88.
     */
    outb(PIC1_DATA, ICW4_8086);
    io_wait();

    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    /*
     * Kembalikan interrupt mask lama.
     */
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}

void pic_send_eoi(uint8_t irq) {
    /*
     * Jika IRQ berasal dari slave PIC, kirim EOI ke slave dahulu.
     */
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    /*
     * Semua IRQ tetap melewati master PIC.
     */
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

void pic_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}