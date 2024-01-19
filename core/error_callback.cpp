#include "error_callback.h"
#include <stdio.h>

void errorCallback(int error, const char* description)
{
    fputs(description, stderr);
}