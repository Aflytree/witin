/*************************************************************************
	> File Name: core.h
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Aug 26 19:39:47 2020
 ************************************************************************/
#ifndef _ACTIVE_H
#define _ACTIVE_H

#include <witin/global.h>
#include <witin/target/mem.h>

using namespace std;
using namespace witin::mem;

namespace witin{
namespace core{

#define NON_TDNN_MAX_ROUND_NUM 64
#define NON_TDNN_MIN_ROUND_NUM 21
#define TDNN_MAX_ROUND_NUM 42
#define TDNN_MIN_ROUND_NUM 18

//bit, not Byte
#define ROUND_CONFIG_MEM_SIZE 128 * 64

//array grp
typedef struct Array_Grp{
	int rsv;
	int winy_size;
	//bias
	int array_bias_start;
	int array_bias_end;
	//precison
	int precision_trim;
	//the start and end 
	//address of adc dataout in column
	int xs;
	int xe
	//store_addr
	int store_addr;
	int infer_data_format;
	int format_con_en;
	
	//whethr paused when the cur round done
	bool round_pause;
} ARRAY_GRP_CONFIG;

//matrix mul
typedef struct Mul_Grp{
	//source
	int m0_addr;
	int m1_addr;
	//store
	int pstore_addr;
    //length
	int mul_length;
	//???
	int mul_sht;
	int rsv;
}MUL_GRP_CONFIG;


//read tv
typedef struct Readtv_Grp{
	//source addr
	int reactv_fetch_addr;
	//target addr
	int reactv_store_addr;
	//length
	int reactv_store_length;
	//000 bypass 001 sigmoid 010 tanh 011 relu
	int reactv_sel;
	int rsv;

}READTV_GRP_CONFIG;

//max pooling 
typedef struct Max_Pooling{
	//the read start address of regfile
	int rsource ;	
	//the write start address of regfile for max matrix
	//when TDNN mode, it is the start addrss of the TDNN block
	int rdes;

	//mxtrix width and height
	int Rwidth;
	int Rheight;

	//strides in x and y direction
	int stridex;
	int stridey;
    
	//dont know
	int winx;
	//reserved
	int rsv;
}MAX_POOLING_CONFIG;


//??? what?
typedef struct Read_Der{
	//left and right start of regfile
	int readdrl_addr;
	int readdrr_addr;

	int readder_length;

	//store start address
	int readder_store;
	int rsv;
}READDER_CONFIG;

//spi to dacfifo
typedef Dacfifo_Grp0{
	//start addrss of array-row for spi to dacfifo
	int ys00;
	//end
	int ye00;
	
	//cmmon address pointer registers
	int rs_cmn_addr_pt;
	int rs_length;
	//the stride of rs01 read st
	int rs_stride;
	int rsv;
}DACFIFO_GRP0_CONFIG;

// regfile to dacfifo
typedef Dacfifo_Grp1{
	//start and end address of array-row for 
	//the first layer regfile to dacfifo
	int ys01;
	int ye01;
	//start and end , the meaning is differenct 
	//depend on tdnn_en an y01_sw_en
	int rs;
	int re;
	int rsv;
}DACFIFO_GRP1_CONFIG;


//dacfifo  start and end address for arry calculation
typedef Dacfifo_Grp2{
	//start and end address of DACFIFO for 
	//array calculation
	int ys_round;
	int ye_round;
	int rsv;
}DACFIFO_GRP2_CONFIG;


typedef struct roundControlS{
	//round configuration packet len per-round
	int packet_len = 0;
	//spi to dacfifo enable
	bool y00_en = 0;
	//regfile to dacfifo enable
	bool reactv_en = 0;
	
	bool y00_sw_en;
	bool y00_sw_en;
	
	//tdnn enable
	bool tdnn_en = 0;
	
	//adc amplitude selection
	int amp_2_adc;
	
	//??
	int dacff_oper;
	int dacff_format;
	bool dacff_invt_sel
	//??
	bool s2_sel = 0;
	bool invt_sel = 0;
	bool add_sel = 0;
	bool a0s_sel = 0;
	bool actv_sel = 0;//activation op
	
	int imt_value;
	bool sft_sel = 0;

    //left or right shift
	bool sft_lr;
	//1 2 4 4 5 6 7 8
	int sft_num;
	//max_en
	bool max_pool_en = 0;

	bool am_sel = 0; //re-addition
	
	bool mul_en = 0;
	bool reactv_en = 0;
	//???
	int fetch_addr;
	
}RD_CONTROL_INTER;


typedef struct RoundControl{

	struct RD_CONTROL_INTER rd_control_inter;
	struct ARRAY_GRP_CONFIG		array_grp_config;
	struct MUL_GRP_CONFIG		mul_grp_config;
	struct READTV_GRP_CONFIG	readtv_grp_config;
	struct MAX_POOLING_CONFIG	max_pooling_config;
	struct READDER_CONFIG		readder_config;
	struct DACFIFO_GRP0_CONFIG	dacfifo_grp0_config;
	struct DACFIFO_GRP1_CONFIG	dacfifo_grp1_config;
	struct DACFIFO_GRP2_CONFIG  dacfifo_grp2_config;

}ROUND_CONTROL;

/*
 * control of a ROUND, including top control
 * and submodule control
 */
class RoundConfig{
	public:
	
		RoundConfig(ROUND_CONTROL rc)
		{
			roundControl = rc;
		}
		
		int getRoundControl(ROUND_CONTROL rc);
	
	private:
		struct ROUND_CONTROL roundControl;
	
};

/*
 * single core 
 *
 */

class Core{
	public:
		Core(std::vector<RoundConfig> rc,
			int roundTotal = 0)
		{
			rounds = rc;
			RoundTotal = roundTotal;
		}
		
		int setRoundTotal(int t)
		{
			RoundTotal = t;
		}	
		int getRoundTotal(int &t);	
		
		int getRoundConfig(RoundConfig rc)
	private:
		//RoundConfig
		std::vector<RoundConfig> rounds;
		int RoundTotal = 0;
		
		//Mem resource
		class RegFileMem		regFileMem;
		class BiasRegionMem     biasRegionMem;
		class CaculateArryMem   caculateArryMem; 
		class DACFifoMem		dacFiflMem;
};

/*
 * four core 
 *
 */

class multiCore{
	public:
		multiCore(std:;vector<Core> c)
		{
			core = c;
		}
	private:
		std::vector<Core> core(4);
};

} //namespace core
} //namespace witin

#endif //_ACTIVE_H

