/*************************************************************************
	> File Name: ../src/utils/debug.cpp
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Sep  2 16:26:04 2020
 ************************************************************************/

#include <witin/global.h>
#include <witin/utils/debug.h>



namespace witin{
namespace debug{


void PROGRAM_EXIT(int reason, const char*fmt, ...)
{
	char tmp_buf[1024] = {0};
	va_list args;
	va_start(args, fmt);
	vsprintf(tmp_buf, fmt, args);

	printf("ERROR:%s",tmp_buf);

	switch(reason){
		case 1:
			printf("please check\n");
			break;
		default:
			break;

		printf("Program Exit!\n");
		exit(1);
		
	}

}

} // namespace debug
} // namespace witin

