#include "stdio.h"
#include "string.h"
#include "tty.h"

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
	int len = 1;
	size_t base_size = strlen(base);
	if (n < 0) {
		tty_putchar('-');
		n = -n;
		++len;
	}
	if (n / base_size)
		len += putnbr_base(n / base_size, base);
	tty_putchar(base[n % base_size]);
	return len;
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
		unsigned int p = va_arg(*ap, unsigned int);
		len = !p ? putstr_count("(nil)")
				 : putstr_count("0x") + putnbr_base_unsigned(p, X_BASE_LO);
	}
	return len;
}

int printf(const char* restrict format, ...) {
	va_list ap;
	va_start(ap, format);
	int start = 0;
	int len = 0;
	for (size_t i = 0; format[i]; i++) {
		if (!format[i + 1]) {
			tty_write(&format[start], i + 1 - start);
		} else if (format[i] == '%') {
			tty_write(&format[start], i - start);
			len += read_flag(format[i + 1], &ap);
			start = i + 2;
			++i;
		} else {
			len++;
		}
	}
	va_end(ap);
	return len;
}
