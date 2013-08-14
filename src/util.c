#include "globals.h"
#include "util.h"
	
#define DIGITS 4
char *itoa(int i)
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[DIGITS + 2];
  char *p = buf + DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

uint16_t cstringLength(char * cstring) {
	uint16_t size = 0;
	while(*cstring++ != '\0') {
		size++;
	}
	return size;
}

char * concatStr(char * a, char * b) {
	static char buffer[50];
	uint8_t index = 0;
	
	while (*a != '\0') {
		buffer[index++] = *a++;
	}
	while(*b != '\0') {
		buffer[index++] = *b++;
	}
	buffer[index] = '\0';
	return &buffer[0];
}
