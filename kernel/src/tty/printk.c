#include "stdio.h"
#include "string.h"
#include "tty.h"

int console_loglevel = LOGLEVEL;

char* level_name[] = {"EMERG",	"ALERT", "CRIT",  "ERR", "WARN",
					  "NOTICE", "INFO",	 "DEBUG", "",	 "CONT"};

static int get_level(char c) {
	if (c >= '0' || c <= '7')
		return c - '0';
	else if (c == 'c')
		return 9;
	else
		return 8;
}

static int print_level(int level) {
	int len = strlen(level_name[level]);
	tty_putstr(level_name[level]);
	if (level != 8) {
		tty_putstr(" - ");
		len += 3;
	}
	return len;
}

int printk(const char* restrict format, ...) {
	if (!format)
		return 0;
	int level = get_level(format[0]);
	if (level > console_loglevel)
		return 0;
	int len = print_level(level);
	if (level != 8)
		format++;
	va_list ap;
	va_start(ap, format);
	len += vprintf(format, ap);
	va_end(ap);
	return len;
}
