#ifndef _HEADER_STRING_H_
#define _HEADER_STRING_H_

struct _string;
typedef struct _string string;

string* string_create();

void string_destroy(string* str);

#endif // !_HEADER_STRING_H_
