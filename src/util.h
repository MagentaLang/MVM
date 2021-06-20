#ifndef UTIL_H
#define UTIL_H

void debug(char text[]);
void info(char text[]);
void warn(char text[]);
void err(char text[]);

void strrev(char *str);
unsigned char nth_byte(int n, unsigned long long int bytes);

#endif
