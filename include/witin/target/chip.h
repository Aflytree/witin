/*************************************************************************
	> File Name: chip.h
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Aug 26 19:39:47 2020
 ************************************************************************/
#ifndef _ACTIVE_H
#define _ACTIVE_H

#include<witin/global.h>
#include<witin/target/core.h>

using namespace witin::mem;

namespace witin{
namespace chip{

/*
 * single chip
 */
class Chip{
	public:
		chip(Core c){core = c;}
		int getCore(Core c);
	private:
		class Core core;

};


} //namespace chip
} //namespace witin

#endif //_ACTIVE_H

