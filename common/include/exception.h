#ifndef _EXCEPTION_
#define _EXCEPTION_

#define EXCEPTION_OUT_OF_MEMORY 137

void exception_throw(char *name, char *msg, int code);

void exception_throw_OutOfMemory(char *msg);

#endif
