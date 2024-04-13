#include <stdio.h>

#define va_start(v, l)	__builtin_va_start(v, l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v, T)	__builtin_va_arg(v, T)


void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, char* str, int base, int capital)
{
    int i = 0;
    bool isNegative = false;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if(num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    } else if (num < 0) {
        num = -num; // Won't have minus sign, because convention or whatever
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + (capital ? 'A' : 'a') : rem + '0';
        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    reverse(str, i);
    str[i] = '\0';
    return str;
}

int strnlen(const char * s, int count)
{
    const char *sc;

    for (sc = s; count-- && *sc != '\0'; ++sc)
        /* nothing */;
    return sc - s;
}

int _vsprintf(char* buf, const char* fmt, va_list args) {
    char *str;
    char *s;
    char numbuf[16];
    int len;

    for(str = buf; *fmt; ++fmt) {
        if(*fmt != '%') {
            *str++ = *fmt;
            continue;
        }

        ++fmt;
        switch(*fmt) {
            case 's':
                s = va_arg(args, char *);
                len = strnlen(s, 1024); // Max length: 1024

                for (int i = 0; i < len; ++i)
                    *str++ = *s++;
                continue;
            case 'c':
                *str++ = va_arg(args, int); // Can't be char for some reason, va_args seems to be funky
                continue;
            case 'd':
                itoa(va_arg(args, int), numbuf, 10, 0);
                for(int i = 0; numbuf[i] != '\0'; i++){
                    *str++ = numbuf[i];
                }
                continue;
            case 'x':
                itoa(va_arg(args, int), numbuf, 16, 0);
                *str++ = '0';
                *str++ = 'x';
                for(int i = 0; numbuf[i] != '\0'; i++){
                    *str++ = numbuf[i];
                }
                continue;
            case 'X':
                itoa(va_arg(args, int), numbuf, 16, 1);
                *str++ = '0';
                *str++ = 'X';
                for(int i = 0; numbuf[i] != '\0'; i++){
                    *str++ = numbuf[i];
                }
                continue;
        }
    }

    *str = '\0';
}

int _printf(const char* fmt, ...) {
    char printf_buf[1024];
    va_list args;
    int printed;

    va_start(args, fmt);
    printed = _vsprintf(printf_buf, fmt, args);
    va_end(args);

    puts(printf_buf);

    return printed;
}

int _sprintf(char* buf, const char* fmt, ...) {
    va_list args;
    int printed;

    va_start(args, fmt);
    printed = _vsprintf(buf, fmt, args);
    va_end(args);
    return printed;
}

int main() {
    _printf("Hello World, %s, %c", "Wow!!!", 'E');
    _printf("123 + 321 = %d", 123 + 321);
    _printf("123 - 321 = %d", 123 - 321);
    _printf("0x293 + 0xe19 = %x", 0x293 + 0xe19);
    _printf("0X293 + 0XE19 = %X", 0x293 + 0xe19);

    char fstring[1024];
    _sprintf(fstring,"Hello World, %s, %c", "Wow!!!", 'E');

    _printf("My formatted string: %s", fstring);

    return 0;
}
