/* Copyright (C) 2019 Marko Nippula.
   All Rights Reserved.

   Interrupt handler.
 */

#ifndef INCLUDE_GUARD_IRQ_DEFINED
#define INCLUDE_GUARD_IRQ_DEFINED 1

/* IRQ occurs at 100 Hz frequency. */
#define IRQ_FREQUENCY_HZ 100

/* Use default handler (no op). */
#define IRQ_DEFAULT_HANDLER ((irq_handler_func)(uintptr_t)0)

typedef void (*irq_handler_func)(void *arg);
void irq_set_handler(irq_handler_func handler);
irq_handler_func irq_get_handler(void);
void irq_check_time(void);

#endif /* INCLUDE_GUARD_IRQ_DEFINED */
