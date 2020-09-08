/*************************************************************************
	> File Name: ../src/utils/debug.cpp
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Sep  2 16:26:04 2020
 ************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>
#include <witin/utils/debug.h>

void PROGRAM_EXIT(int reason, const char*fmt, ...)
{
	char tmp_buf[1024] = {0};
	va_list args;
	va_start(args, fmt);
	vsprintf(tmp_buf, fmt, args);

	printf("ERROR:%s\n",tmp_buf);

	switch(reason){
		case 1:
			printf("please check up reason!\n");
			break;
		default:
			break;

		printf("Program Exit!\n");
		exit(1);
		
	}

}

