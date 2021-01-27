#include "string.h"
#include "../drivers/screen.h"

void int_to_ascii(int n, char str[]) {
	int i, sign;
	if((sign = n) < 0) {
		n = -n;
	}
	i = 0;
	do {
		str[i++] = n % 10 + '0';
	} while((n /= 10) > 0);

	if (sign < 0) {
		str[i++] = '-';
	}
	str[i] = '\0';
	reverse(str);
}

void reverse(char head[])
{
  if (!head) return;
  char *tail = head;
  while(*tail) ++tail;    // find the 0 terminator, like head+strlen
  --tail;               // tail points to the last real char
                        // head still points to the first
  for( ; head < tail; ++head, --tail) {
      // walk pointers inwards until they meet or cross in the middle
      char h = *head, t = *tail;
      *head = t;           // swapping as we go
      *tail = h;
  }
}

int strlen(char s[]) {
	int length = 0;
	while(s[length] != '\0') {
		++length;
	}
	return length;
}

void backspace(char s[]) {
	int len = strlen(s);
	s[len-1] = '\0';
}

void append(char s[], char n) {
	int len = strlen(s);
	s[len] = n;
	s[len+1] = '\0';
}

int strcmp(char s1[], char s2[]) {
	int i;
	for(i = 0; s1[i] == s2[i]; i++) {
		if (s1[i] == '\0') return 0;
	}
	return s1[i] - s2[i];
}
