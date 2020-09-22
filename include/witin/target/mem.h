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
#include <witin/utils/debug.h>

namespace witin{
namespace base{

//BIAS
#define BIAS_MEM_SIZE 16*1024

//ARRAY
#define ARRAY_MEM_SIZE		2*1024 * 1*1024
#define ARRAY_ROW_SIZE	    2*1024
#define ARRAY_COLUMN_SIZE   1*1024

//DAC_FIFO
#define DAC_FIFO_SIZE		1*1024

//REGFILE
#define REGFILE_MEM_SIZE	4*1024
#define REGFILE_MODE		0  //not TDNN
#define REGFILE_1K_SIZE		1*1024
#define REGFILE_3K_SIZE		3*1024


enum MEM_TYPE{
	REGFILE_MEM_TYPE = 0,
	ARRAY_MEM_TYPE,
	BIAS_MEM_TYPE,
	DACFIFO_MEM_TYPE
};


struct mem_record{
	class Tensor * tensor;
	//int used;
	//regfile mem
	int start;
	int len;
	//for ArryMem
	int column_start;
	int column_len;
	int row_start;
	int row_len;
	enum MEM_TYPE mem_type;
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

		int allocMemAddr(int addr, int size)
		{
			if(addr < 0 || size < 0)
			{
				PROGRAM_EXIT(1, "addr or size is less than 0  when allocate RegFileMem!");
			}
			DLOG(INFO)<<"REGFILE addr "<<addr<<" size "<<size;
			if((generalUsedSize + size) > REGFILE_MEM_SIZE)
			{
				PROGRAM_EXIT(1, "allocMemAddr is overflow when allocate RegFileMem!");
			}

			generalUsedSize+=size;
			return 0;
		}

		int setGeneralUsedSize(int value)
		{
			generalUsedSize = value;
			return 0;
		}

		int freeMemAddr(int addr, int size);
		int check_addr(int addr);
		int getGeneralUsedSize()
		{
			return generalUsedSize;
		}

		int getAvailableMem()
		{
			return REGFILE_MEM_SIZE - getGeneralUsedSize();
		}

		//whether split into 1 and 3
		//defalut to 0, not split
		int Mode = 0;

		int regfileSize = REGFILE_MEM_SIZE;
		//is intended for dac
		int dacFifoSize = REGFILE_1K_SIZE;
		//4 - 1
		int generalSize = REGFILE_3K_SIZE;
		int usedSize = 0;
		int dacFifoUsedSize = 0;
		int generalUsedSize = 0;
};

//extern class RegFileMem regFileMem;

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

//static class BiasRegionMem biasRegionMem;

//array mem
//row
//column
class CaculateArryMem : public Mem{
	public:
		CaculateArryMem(){}

		//used mem init
		int init(){}

		//get available mem
		int getRowAvailableMem()
		{
			return ARRAY_ROW_SIZE - arrayRowUsedSize;
		}
		int getColumnAvailableMem()
		{
			return 	ARRAY_COLUMN_SIZE - arrayColumnUsedSize;
		}

		int getArryColumnUsedSize()
		{
			return arrayColumnUsedSize;
		}

		int getArryRowUsedSize()
		{
			return arrayRowUsedSize;
		}

		//alloc Mem
		int allocRowMem(int rowAddr, int size)
		{
			DLOG(INFO)<<" :ARRAY rowaddr "<<rowAddr<<" size "<<size;
			if(rowAddr < 0 || size < 0)
			{
				PROGRAM_EXIT(1, "addr or size is less than 0  when allocate row CaculateArryMem!");
			}

			if((arrayRowUsedSize + size) > ARRAY_ROW_SIZE)
			{
				PROGRAM_EXIT(1, "allocMemAddr is overflow when allocate row CaculateArryMem!");
			}

			arrayRowUsedSize+=size;
			return 0;
		}

		int allocColumnMem(int columnAddr, int size)
		{
			DLOG(INFO)<<" :ARRAY columnAddr "<<columnAddr<<" size "<<size;
			if(columnAddr < 0 || size < 0)
			{
				PROGRAM_EXIT(1, "addr or size is less than 0  when allocate column CaculateArryMem!");
			}

			if((arrayColumnUsedSize + size) > ARRAY_ROW_SIZE)
			{
				PROGRAM_EXIT(1, "allocMemAddr is overflow when allocate column CaculateArryMem!");
			}

			arrayColumnUsedSize+=size;
			return 0;

		}

		int setArrayColumnUsedSize(int value)
		{
			arrayColumnUsedSize = value;
			return 0;
		}

		int setArrayRowUsedSize(int value)
		{
			arrayRowUsedSize = value;
			return 0;
		}

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

//static class DACFifoMem dacFifoMem;

}//namespace mem
}//namespace witin

#endif // _MEM_H
