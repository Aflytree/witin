/*************************************************************************
	> File Name: mem.h
	> Description: used for core mem manager
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Aug 26 19:39:57 2020
 ************************************************************************/
#pragma once
#ifndef _MEM_H
#define _MEM_H

#include <witin/global.h>
#include <witin/tensor/tensor.h>

namespace witin{
namespace mem{

//BIAS
#define BIAS_MEM_SIZE 16*1024

//ARRAY
#define ARRAY_MEM_SIZE			2*1024 * 1*1024
#define ARRAY_ROW_SIZE	    2*1024
#define ARRAY_COLUMN_SIZE   1*1024

//DAC_FIFO
#define DAC_FIFO_SIZE		1*1024 

//REGFILE
#define REGFILE_MEM_SIZE		4*1024
#define REGFILE_MODE		0  //not TDNN
#define REGFILE_1K_SIZE		1*1024 
#define REGFILE_3K_SIZE		3*1024 


class RegFileMem;
class BiasRegionMem;
class CaculateArryMem; 
class DACFifoMem;

struct mem_record{
	//class Tensor * tensor;
	int used;
	int start;
	int len;
};

struct bias_mem_record{
	struct mem_record;
};
struct array_mem_record{
	struct mem_record;
};
struct dacfifo_mem_record{
	struct mem_record;
};
struct regfile_mem_record{
	struct mem_record;
};

class Mem{
	public:

	private:

};

//regfile MEM
class RegFileMem : public Mem{
	public:
		RegFileMem(){}
	
		int getTotalSize();
		int getDACFifoSize();
		int getGenealSize();

		int allocMemAddr(int& addr, int size);
		int freeMemAddr(int addr, int size);
		int check_addr(int addr);
		int getAvailableMem();
		
		//whether split into 1 and 3
		//defalut to 0, not split
		int Mode = 0;

	private:
		int regfileSize = REGFILE_MEM_SIZE;
		//is intended for dac 
		int dacFifoSize = REGFILE_1K_SIZE;
		//4 - 1 
		int generalSize = REGFILE_3K_SIZE;
		int usedSize = 0;
		int dacFifoUsedSize = 0;
		int generalUsedSize = 0;
};

//bias mem
class BiasRegionMem : public Mem{
	public:
		BiasRegionMem(){}

		int useBiadMemFlag = 0;
		
		//alloc
		int allocBiasRegionMem(int &addr, int size);
		//free
		int freeBiasRegionMem(int addr, int size);

	private:
		int biasRegionSize = BIAS_MEM_SIZE;
		
		//mem manager
		int biasRegionUsedSize = 0;
		int biasRegionUsedStart = 0;
		int biasRegionUsedEnd = 0;
};

//array mem
//row
//column
class CaculateArryMem : public Mem{
	public:
		CaculateArryMem(){}
		
		//used mem init
		int init(){}

		//get available mem
		int getRowAvailableMem();
		int getColumnAvailableMem();
		
		//alloc Mem
		int allocRowMem(int addr, int size);
		int allocColumnMem(int addr, int size);
			
	private:
		//array size
		int arraySize = ARRAY_MEM_SIZE;
		int arrayRowSize = ARRAY_ROW_SIZE;
		int arrayColumnSize = ARRAY_COLUMN_SIZE;
		
		//remained size
		int arrayrowRemainSize = ARRAY_ROW_SIZE;
		int arrayColumnRemainSize = ARRAY_COLUMN_SIZE;
		int arrayTotalRemainSize = ARRAY_MEM_SIZE;
		
		//row manager
		int arrayRowUsedSize = 0;
		int arrayRowUsedStart = 0;
		int arrayRowUsedEnd = 0;

		//column manager
		int arrayColumnUsedSize = 0;
		int arrayColumnUsedStart = 0;
		int arrayColumnUsedEnd = 0;
	
		//total manager
		int arrayTotalUsedSize = 0;
};

//dacfifo mem
class DACFifoMem : public Mem{
	public:
		DACFifoMem(){}
		
		//get available mem		
		int getRowAvailableMem();
		//alloc
		int allocDACFifoMem(int addr, int size);
		//free
		int freeDACFifoMem(int addr, int size);
		
		int start = 0;
		int end = 0;
	private:
		int dacFifoSize = DAC_FIFO_SIZE;
		
		//used mem manager
		int dacFifoUsedSize = 0;
		int dacFifoUsedStart = 0;
		int dacFifoUsedEnd = 0;
};


}//namespace mem
}//namespace witin

#endif // _MEM_H
