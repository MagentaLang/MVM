#ifndef UTIL_H
#define UTIL_H

void debug(char text[]);
void info(char text[]);
void warn(char text[]);
void err(char text[]);

void strrev(char *str);
unsigned long nth_byte(int n, unsigned long bytes);
unsigned long maxul(unsigned long a, unsigned long b);

#endif
