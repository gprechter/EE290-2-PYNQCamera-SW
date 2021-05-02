#include "uart.h"

static inline void _kputs(const char *s)
{
        char c;
        for (; (c = *s) != '\0'; s++)
                kputc(c);
}

void kputs(const char *s, bool newline)
{
        _kputs(s);
	if (newline) {
        	kputc('\r');
        	kputc('\n');
	}
}

