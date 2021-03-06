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
#define BIAS_MEM_SIZE 32*1024
#define BIAS_ROW_SIZE 32
#define BIAS_COLUMN_SIZE 1024

//ARRAY
#define ARRAY_MEM_SIZE		2*1024 * 1*1024
#define ARRAY_ROW_UP_SIZE	    1*1024
#define ARRAY_ROW_DOWN_SIZE	    1*1024
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
				LOG(FATAL)<< "addr or size is less than 0  when allocate RegFileMem!";
			}
			DLOG(INFO)<<"REGFILE addr "<<addr<<" size "<<size;
			if((generalUsedSize + size) > REGFILE_MEM_SIZE)
			{
				LOG(FATAL)<< "allocMemAddr is overflow when allocate RegFileMem!";
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
		BiasRegionMem(){
			biasRowUsedSize = 0;
			biasRowUsedStart = 0;
			biasRowUsedEnd = 0;

			biasColumnUsedSize = 0;
			biasColumnUsedStart = 0;
			biasColumnUsedEnd = 0;
		}

		int useBiadMemFlag = 0;

		//alloc
		int allocBiasRowRegionMem(int size)
		{
			DLOG(INFO)<<"biasRowUsedSize1 = "<<biasRowUsedSize;

			int biasStart = biasRowUsedSize;
			biasRowUsedSize += size;
			DLOG(INFO)<<"size = "<<size;
			DLOG(INFO)<<"biasRowUsedSize = "<<biasRowUsedSize;
			DLOG(INFO)<<"biasStart = "<<biasStart;
			if(biasRowUsedSize > BIAS_ROW_SIZE)
				LOG(FATAL)<<"biasRowUsedSize is greater than BIAS_ROW_SIZE "
						  <<biasRowUsedSize<<"vs"<<BIAS_ROW_SIZE;
			return biasStart;
		}
		//free
		int freeBiasRegionMem(int addr, int size){return 0;}

		//bias size
		int biasRowSize = BIAS_ROW_SIZE;
		int biasColumnSize = BIAS_COLUMN_SIZE;

		//row manager
		int biasRowUsedSize = 0;
		int biasRowUsedStart = 0;
		int biasRowUsedEnd = 0;

		//column manager
		int biasColumnUsedSize = 0;
		int biasColumnUsedStart = 0;
		int biasColumnUsedEnd = 0;
};

//static class BiasRegionMem biasRegionMem;

//array mem
//row
//column
class CaculateArryMem : public Mem{
	public:
		CaculateArryMem(){}

		//used mem init
		void init(){}

		//get available mem
		int getRowAvailableMem()
		{
			if(ARRAY_ROW_SIZE - arrayRowUsedSize < 0)
			{
				LOG(FATAL)<<"No caculateColumnArryMem is available !";
			}

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

		//先分配column，再分配row
		//先调用allocColumnMem()， 再调用allocRowMem()
		//alloc Mem
		int allocRowMem(int rowAddr, int size)
		{
			DLOG(INFO)<<" :ARRAY rowaddr "<<rowAddr<<" size "<<size;
			if(!rowNotStartFromZeroFlag)
			{
				//int newRowAddr = 0;
				if(rowAddr < 0 || size < 0)
				{
					LOG(FATAL)<< "addr or size is less than 0  when allocate row CaculateArryMem!";
				}

				if((arrayRowUsedSize + size) > ARRAY_ROW_SIZE)
				{
					LOG(FATAL)<< "allocMemAddr is overflow when allocate row CaculateArryMem!";
				}

				if(size > ARRAY_ROW_UP_SIZE)
				{
					LOG(FATAL)<< "rowAddr size should less than ARRAY_ROW_UP_SIZE or ARRAY_ROW_DOWN_SIZE!";
				}
				if(array_row_down_flag)
					return ARRAY_ROW_UP_SIZE;
			}
			else
			{
				vector<int>::iterator max_value = max_element(arrayDownSpaceMax.begin(), arrayDownSpaceMax.end());
				return *max_value + 1024;
			}

			return 0;
		}

		int allocColumnMem(int &columnAddr, int size)
		{
			DLOG(INFO)<<" :ARRAY columnAddr "<<columnAddr<<" size "<<size;
			DLOG(INFO)<<" :ARRAY arrayColumnUsedSize "<<arrayColumnUsedSize;
			if(columnAddr < 0 || size < 0)
			{
				LOG(FATAL)<<"addr or size is less than 0  when allocate column CaculateArryMem!";
			}

			if((array_row_down_flag == 1) && ((arrayColumnUsedSize + size) > ARRAY_COLUMN_SIZE))
			{
				//LOG(WARNING)<<" array_row_down_flag = 1: Array Column addr is not enough to alloc !";
				rowNotStartFromZeroFlag = 1;
				arrayColumnUsedSize = 0;
				return  16;
			}

			if((arrayColumnUsedSize + size) > ARRAY_COLUMN_SIZE)
			{
				array_row_down_flag = true;
				arrayColumnUsedSize = 0;
				columnAddr = 0;
				arrayDownSpaceMax.push_back(size);
				//LOG(WARNING)<< "Use row down mem space!";
			}

			if(1 == array_row_down_flag && arrayColumnUsedSize > ARRAY_COLUMN_SIZE)
			{
				LOG(FATAL)<<"Column addr is not enough to alloc !";
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

		int rowNotStartFromZeroFlag = 0;
		//array size
		int arraySize = ARRAY_MEM_SIZE;
		int arrayRowSize = ARRAY_ROW_SIZE;
		int arrayColumnSize = ARRAY_COLUMN_SIZE;

		//remained size
		int arrayrowRemainSize = ARRAY_ROW_SIZE;
		int arrayColumnRemainSize = ARRAY_COLUMN_SIZE;
		int arrayTotalRemainSize = ARRAY_MEM_SIZE;

		bool array_row_down_flag = 0;
		//row manager
		int arrayRowUsedSize = 0;
		int arrayRowUsedStart = 0;
		int arrayRowUsedEnd = 0;

		//column manager
		int arrayColumnUsedSize = 0;
		int arrayColumnUsedStart = 0;
		int arrayColumnUsedEnd = 0;

		int arrayUpSpaceMax[10]={0};
		vector<int> arrayDownSpaceMax;
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
