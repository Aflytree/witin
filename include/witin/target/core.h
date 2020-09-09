/*************************************************************************
	> File Name: core.h
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Aug 26 19:39:47 2020
 ************************************************************************/
#pragma once
#ifndef _ACTIVE_H
#define _ACTIVE_H

#include <witin/global.h>
#include <witin/target/mem.h>

//map<Tensor, bias_mem_record> tensor_bias_mem_map;
//map<Tensor, array_mem_record> tensor_array_mem_map;
//map<Tensor, dacfifo_mem_record> tensor_dacfifo_mem_map;
//map<Tensor, regfile_mem_record> tensor_regfile_mem_map;

namespace witin{
namespace base{

#define NON_TDNN_MAX_ROUND_NUM 64
#define NON_TDNN_MIN_ROUND_NUM 21
#define TDNN_MAX_ROUND_NUM 42
#define TDNN_MIN_ROUND_NUM 18

//bit, not Byte
#define ROUND_CONFIG_MEM_SIZE 128 * 64

typedef struct weight_params{
	int start;
	int end;
	int size;
}WEIGHT_PARAMS;

typedef struct bias_params{
	int start;
	int end;
	int size;
}BIAS_PARAMS;

//array grp
typedef struct Array_Grp{
	int w_win_column_s;
	int w_win_column_e;
	int w_win_column_len;
	
	int w_win_row_s;
	int w_win_row_e;
	int w_win_row_len;
	
	int store_addr;
	int store_len;

	int regfile_addr_start;
	int regfile_addr_len;
	WEIGHT_PARAMS w_prams;
	
} ARRAY_GRP_CONFIG;


inline void dump_arry_grp_cfg(ARRAY_GRP_CONFIG agc)
{
	cout<<"----------arr_grp_cfg----------"<<endl;
	cout<<"agc.w_win_column_s : \t"<< agc.w_win_column_s<<endl;
	cout<<"agc.w_win_column_e : \t"<< agc.w_win_column_e<<endl;
	cout<<"agc.w_win_column_len : \t"<< agc.w_win_column_len<<endl;
	cout<<"agc.w_win_row_s : \t"<< agc.w_win_row_s<<endl;
	cout<<"agc.w_win_row_e : \t"<< agc.w_win_row_e<<endl;
	cout<<"agc.w_win_row_len : \t"<< agc.w_win_row_len<<endl;
	cout<<"agc.store_addr : \t"<< agc.store_addr<<endl;
	cout<<"agc.store_len : \t"<< agc.store_len<<endl;
	cout<<"agc.regfile_addr_start : \t"<< agc.regfile_addr_start<<endl;
	cout<<"agc.regfile_addr_len : \t"<< agc.regfile_addr_len<<endl;

	cout<<"WEIGHT_PARAMS : "<<endl;
	cout<<"     start : \t"<<agc.w_prams.start<<endl;
	cout<<"       end : \t"<<agc.w_prams.end<<endl;
	cout<<"      size : \t"<<agc.w_prams.size<<endl;


}

typedef struct Bias_Config{
	int bias_addr_s;
	int bias_addr_e;
	int bias_addr_len;

	BIAS_PARAMS b_prams;
}BIAS_CONFIG;


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

typedef struct Actv_Config{
	string actv_type;
	int addr;
	int limit;
}ACTV_GRP_CONFIG; 

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
typedef struct Dacfifo_Grp0{
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
typedef struct Dacfifo_Grp1{
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
typedef struct Dacfifo_Grp2{
	//start and end address of DACFIFO for 
	//array calculation
	int ys_round;
	int ye_round;
	int rsv;
}DACFIFO_GRP2_CONFIG;


typedef struct roundControlS{
	int cnt;
	bool weight_en;
	bool bias_en;
	bool shift_en;
	bool add_en;
	bool actv_en;
	bool max_pooling_en;
	bool reactive_en;
	bool mult_en;
	bool readdr_en;
	bool TDNN_en;
	bool fifo_grp0_en;
	bool fifo_grp1_en;
	bool fifo_grp2_en;
	
	//whether paused when the cur round done
	bool round_pause;
	
	//default
	roundControlS(){
		cnt = 0;
		weight_en = true;
		bias_en  = false;
		shift_en = false;
		add_en = false;
		actv_en = false;
		max_pooling_en = false;
		reactive_en = false;
		mult_en = false;
		readdr_en = false;
		TDNN_en = false;
		fifo_grp0_en = false;
		fifo_grp1_en = false;
		fifo_grp2_en = false;
		round_pause = false;
	}
}RD_CONTROL_ENABLE;

inline void dump_rd_ctrl_enable(RD_CONTROL_ENABLE rce)
{
	cout<<"----------RD_CONTROL_ENABLE----------"<<endl;
	cout<<"rce.cnt : \t"<< rce.cnt<<endl;
	cout<<"rce.weight_en : \t"<< rce.weight_en<<endl;
	cout<<"rce.bias_en : \t "<< rce.bias_en<<endl;
	cout<<"rce.shift_en : \t"<< rce.shift_en<<endl;
	cout<<"rce.add_en : \t"<< rce.add_en<<endl;
	cout<<"rce.actv_en : \t"<< rce.actv_en<<endl;
	cout<<"rce.mult_en : \t"<< rce.mult_en<<endl;
	cout<<"rce.readdr_en : \t"<< rce.readdr_en<<endl;
	cout<<"rce.TDNN_en : \t"<< rce.TDNN_en<<endl;
	cout<<"rce.fifo_grp0_en : \t"<< rce.fifo_grp0_en<<endl;
	cout<<"rce.fifo_grp1_en : \t"<< rce.fifo_grp1_en<<endl;
	cout<<"rce.fifo_grp2_en : \t"<< rce.fifo_grp2_en<<endl;
	cout<<endl;
}

typedef struct RoundControl{

	RD_CONTROL_ENABLE	 rd_control_enable;
	ARRAY_GRP_CONFIG	 array_grp_config;
	MUL_GRP_CONFIG		 mul_grp_config;
	ACTV_GRP_CONFIG		 actv_grp_config;
	READTV_GRP_CONFIG	 readtv_grp_config;
	MAX_POOLING_CONFIG	 max_pooling_config;
	READDER_CONFIG		 readder_config;
	DACFIFO_GRP0_CONFIG	 dacfifo_grp0_config;
	DACFIFO_GRP1_CONFIG	 dacfifo_grp1_config;
	DACFIFO_GRP2_CONFIG  dacfifo_grp2_config;

}ROUND_CONFIG;

///*
// * control of a ROUND, including top control
// * and submodule control
// */
//class RoundConfig{
//	public:
//		RoundConfig(){}	
//		RoundConfig(ROUND_CONTROL rc)
//		{
//			roundControl = rc;
//		}
//		
//		int getRoundControl(ROUND_CONTROL rc);
//		int dump(){
//			//dump all round config in this function		
//		}
//	
//	private:
//		ROUND_CONTROL roundControl;
//	
//};

/*
 * single core 
 *
 */

class Core{
	public:
		Core(){}
		Core(std::vector<ROUND_CONFIG> rc,
			int roundTotal = 0)
		{
			rounds = rc;
			RoundTotal = roundTotal;
		}
		
		int setRoundTotal(int t)
		{
			RoundTotal = t;
		}	
		int getRoundTotal(int &t){
			
			t = RoundTotal;
			return 0;
		}	
		
		int getRoundConfig(ROUND_CONFIG rc);
		
		int dump(){
			int t;
			getRoundTotal(t);
			cout<<"RoundTotal:"<<t<<endl;
		}
		
	private:
		//ROUND_CONFIG
		std::vector<ROUND_CONFIG> rounds;
		int RoundTotal = 0;
		
		//Mem resource
		
};

/*
 * four core 
 *
 */

class multiCore{
	public:
		multiCore(){}
		multiCore(std::vector<Core> c)
		{
			core = c;
		}
	private:
		std::vector<Core> core;
};

} //namespace core
} //namespace witin

#endif //_ACTIVE_H

