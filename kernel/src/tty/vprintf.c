#include "tty.h"
#include <stdio.h>
#include <string.h>

#define D_BASE "0123456789"
#define X_BASE_LO "0123456789abcdef"
#define X_BASE_UP "0123456789ABCDEF"

int putnbr_base_unsigned(unsigned int n, const char* base) {
	if (!base)
		return 0;
	int len = 1;
	size_t base_size = strlen(base);
	if (n / base_size)
		len += putnbr_base_unsigned(n / base_size, base);
	tty_putchar(base[n % base_size]);
	return len;
}

int putnbr_base(long n, const char* base) {
	if (!base)
		return 0;
	int len = 0;
	if (n < 0) {
		tty_putchar('-');
		n = -n;
		++len;
	}
	return len + putnbr_base_unsigned((unsigned int)n, base);
}

int putstr_count(char* str) {
	if (!str)
		return 0;
	tty_putstr(str);
	return (int)strlen(str);
}

int read_flag(char flag, va_list* ap) {
	int len = 1;
	if (flag == 'c')
		tty_putchar(va_arg(*ap, int));
	else if (flag == '%')
		tty_putchar('%');
	else if (flag == 'd')
		len = putnbr_base(va_arg(*ap, int), D_BASE);
	else if (flag == 'u')
		len = putnbr_base_unsigned(va_arg(*ap, unsigned int), D_BASE);
	else if (flag == 'x')
		len = putnbr_base_unsigned(va_arg(*ap, unsigned int), X_BASE_LO);
	else if (flag == 'X')
		len = putnbr_base_unsigned(va_arg(*ap, unsigned int), X_BASE_UP);
	else if (flag == 's') {
		char* s = va_arg(*ap, char*);
		len = !s ? putstr_count("(null)") : putstr_count(s);
	} else if (flag == 'p') {
		void* p = va_arg(*ap, void*);
		len = !p ? putstr_count("(nil)")
				 : putstr_count("0x") +
					   putnbr_base_unsigned((unsigned long)p, X_BASE_LO);
	} else {
		len = 0;
	}
	return len;
}

int vprintf(const char* restrict format, va_list ap) {
	int start = 0;
	int len = 0;
	for (size_t i = 0; format[i]; i++) {
		if (format[i] == '%' && format[i + 1]) {
			tty_write(&format[start], i - start);
			len += i - start + read_flag(format[i + 1], &ap);
			start = i + 2;
			++i;
		} else if (!format[i + 1]) {
			tty_write(&format[start], i + 1 - start);
			len += i + 1 - start;
		}
	}
	return len;
}
