#ifndef UTIL_H
#define UTIL_H

void debug(char text[]);
void info(char text[]);
void warn(char text[]);
void err(char text[]);

void strrev(char *str);
unsigned long nth_byte(int n, unsigned long bytes);

#endif
