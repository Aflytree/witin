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
	weight_params(){
		start=0;
		end=0;
		size=0;
	}
}WEIGHT_PARAMS;

typedef struct bias_params{
	int start;
	int end;
	int size;
	bias_params(){
		start=0;
		end=0;
		size=0;
	}
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

	Array_Grp(){
		w_win_column_s=0;
		w_win_column_e=0;
		w_win_column_len=0;

		w_win_row_s=0;
		w_win_row_e=0;
		w_win_row_len=0;

		store_addr=0;
		store_len=0;
		regfile_addr_start=0;
		regfile_addr_len=0;
	}

} ARRAY_GRP_CONFIG;


inline void dump_arry_grp_cfg(ARRAY_GRP_CONFIG agc)
{
	DLOG(INFO)<<"----------arr_grp_cfg----------";
	DLOG(INFO)<<"agc.w_win_column_s : \t"<< agc.w_win_column_s;
	DLOG(INFO)<<"agc.w_win_column_e : \t"<< agc.w_win_column_e;
	DLOG(INFO)<<"agc.w_win_column_len : \t"<< agc.w_win_column_len;
	DLOG(INFO)<<"agc.w_win_row_s : \t"<< agc.w_win_row_s;
	DLOG(INFO)<<"agc.w_win_row_e : \t"<< agc.w_win_row_e;
	DLOG(INFO)<<"agc.w_win_row_len : \t"<< agc.w_win_row_len;
	DLOG(INFO)<<"agc.store_addr : \t"<< agc.store_addr;
	DLOG(INFO)<<"agc.store_len : \t"<< agc.store_len;
	DLOG(INFO)<<"agc.regfile_addr_start : \t"<< agc.regfile_addr_start;
	DLOG(INFO)<<"agc.regfile_addr_len : \t"<< agc.regfile_addr_len;

	DLOG(INFO)<<"WEIGHT_PARAMS : ";
	DLOG(INFO)<<"     start : \t"<<agc.w_prams.start;
	DLOG(INFO)<<"       end : \t"<<agc.w_prams.end;
	DLOG(INFO)<<"      size : \t"<<agc.w_prams.size;
}

typedef struct Bias_Config{

	int bias_column_s;
	int bias_column_e;
	int bias_column_len;
	int bias_row_s;
	int bias_row_e;
	int bias_row_len;
	Bias_Config(){
		bias_column_s=0;
		bias_column_e=0;
		bias_column_len=0;
		bias_row_s=0;
		bias_row_e=0;
		bias_row_len=0;
	}
	BIAS_PARAMS b_prams;
}BIAS_CONFIG;

inline void dump_bias_config_cfg(BIAS_CONFIG mgc)
{
	DLOG(INFO)<<"----------bias config ----------";

	DLOG(INFO)<<"mgc.bias_win_column_s : \t"<< mgc.bias_column_s;
	DLOG(INFO)<<"mgc.bias_win_column_e : \t"<< mgc.bias_column_e;
	DLOG(INFO)<<"mgc.bias_win_column_len : \t"<< mgc.bias_column_len;
	DLOG(INFO)<<"mgc.bias_win_row_s : \t"<< mgc.bias_row_s;
	DLOG(INFO)<<"mgc.bias_win_row_e : \t"<< mgc.bias_row_e;
	DLOG(INFO)<<"mgc.bias_win_row_len : \t"<< mgc.bias_row_len;

	DLOG(INFO)<<"BIAS_PARAMS : ";
	DLOG(INFO)<<"     start : \t"<<mgc.b_prams.start;
	DLOG(INFO)<<"       end : \t"<<mgc.b_prams.end;
	DLOG(INFO)<<"      size : \t"<<mgc.b_prams.size;
}

//matrix mul
typedef struct Mul_Grp{
	//source
	int m0_addr;
	int m1_addr;
	vector<int> in_addrs;
	//store
	int pstore_addr;
    //length
	int mul_length;
	//???
	int mul_sht;
	int rsv;
	Mul_Grp(){
		m0_addr=0;
		m1_addr=0;
		pstore_addr=0;
		mul_length=0;
		mul_sht=0;
	}
}MUL_GRP_CONFIG;

inline void dump_mul_grp_cfg(MUL_GRP_CONFIG mgc)
{
	DLOG(INFO)<<"----------mul_grp_cfg----------";
	if(mgc.in_addrs.size() != 0)
	{
		DLOG(INFO)<<"mgc.m0_addr : \t"<< mgc.in_addrs[0];
		DLOG(INFO)<<"mgc.m1_addr : \t"<< mgc.in_addrs[1];
	}
	DLOG(INFO)<<"mgc.pstore_addr : \t"<< mgc.pstore_addr;
	DLOG(INFO)<<"mgc.mul_length : \t"<< mgc.mul_length;
	DLOG(INFO)<<"mgc.mul_sht : \t"<< mgc.mul_sht;
}

//read tv
typedef struct Reactv_Grp{
	//source addr
	int reactv_fetch_addr;
	//target addr
	int reactv_store_addr;
	//length
	int reactv_store_length;
	//000 bypass 001 sigmoid 010 tanh 011 relu
	int reactv_sel;
	int rsv;
	Reactv_Grp(){
		reactv_fetch_addr=0;
		reactv_store_addr=0;
		reactv_store_length=0;
		reactv_sel=0;
	}

}REACTV_GRP_CONFIG;

inline void dump_reactv_grp_cfg(REACTV_GRP_CONFIG rgc)
{
	DLOG(INFO)<<"----------react_grp_cfg----------";
	DLOG(INFO)<<"rgc.reactv_fetch_addr : \t"<< rgc.reactv_fetch_addr;
	DLOG(INFO)<<"rgc.reactv_store_addr : \t"<< rgc.reactv_store_addr;
	DLOG(INFO)<<"rgc.reactv_store_length : \t"<< rgc.reactv_store_length;
	DLOG(INFO)<<"rgc.reactv_sel : \t"<< rgc.reactv_sel;
}


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
	Actv_Config(){
		actv_type="relu";
		addr=0;
		limit=127;
	}
}ACTV_GRP_CONFIG;

//add --> radd
typedef struct Read_Der{
	//left and right start of regfile
	int readdrl_addr;
	int readdrr_addr;
	vector<int> add_addr;

	int readder_length;

	//store start address
	int readder_store;
	int rsv;
	Read_Der(){
		readdrl_addr=0;
		readdrr_addr=0;
		//TODO:add_addr
		readder_length=0;
		readder_store=0;
	}
}READDER_CONFIG;


inline void dump_readder_grp_cfg(READDER_CONFIG rc)
{
	DLOG(INFO)<<"----------readdr_grp_cfg----------";
	DLOG(INFO)<<"rc.add_addr size:"<<rc.add_addr.size();
	if(0 != rc.add_addr.size())
	{
		DLOG(INFO)<<"rc.readdrl_addr : \t"<< rc.add_addr[0];
		DLOG(INFO)<<"rc.readdrr_addr : \t"<< rc.add_addr[1];
	}
	DLOG(INFO)<<"rc.readder_length : \t"<< rc.readder_length;
	DLOG(INFO)<<"rc.readder_store : \t"<< rc.readder_store;
}

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

	roundControlS(){
		cnt = 0;
		weight_en = false;
		bias_en  = false;
		shift_en = false;
		add_en = false;
		actv_en = false;
		max_pooling_en = false;
		reactive_en = false;
		mult_en = false;
		readdr_en = false;
		TDNN_en = false;
		fifo_grp0_en = true;
		fifo_grp1_en = true;
		fifo_grp2_en = true;
		round_pause = false;
	}
}RD_CONTROL_ENABLE;

inline void dump_rd_ctrl_enable(RD_CONTROL_ENABLE rce)
{
	DLOG(INFO)<<"----------RD_CONTROL_ENABLE----------";
	DLOG(INFO)<<"rce.cnt : \t"<< rce.cnt;
	DLOG(INFO)<<"rce.weight_en : \t"<< rce.weight_en;
	DLOG(INFO)<<"rce.bias_en : \t "<< rce.bias_en;
	DLOG(INFO)<<"rce.shift_en : \t"<< rce.shift_en;
	DLOG(INFO)<<"rce.add_en : \t"<< rce.add_en;
	DLOG(INFO)<<"rce.actv_en : \t"<< rce.actv_en;
	DLOG(INFO)<<"rce.max_pooling_en : \t"<< rce.max_pooling_en;
	DLOG(INFO)<<"rce.reactive_en : \t"<< rce.reactive_en;
	DLOG(INFO)<<"rce.mult_en : \t"<< rce.mult_en;
	DLOG(INFO)<<"rce.readdr_en : \t"<< rce.readdr_en;
	DLOG(INFO)<<"rce.TDNN_en : \t"<< rce.TDNN_en;
	DLOG(INFO)<<"rce.fifo_grp0_en : \t"<< rce.fifo_grp0_en;
	DLOG(INFO)<<"rce.fifo_grp1_en : \t"<< rce.fifo_grp1_en;
	DLOG(INFO)<<"rce.fifo_grp2_en : \t"<< rce.fifo_grp2_en;
}

typedef struct RoundControl{

	RD_CONTROL_ENABLE	 rd_control_enable;
	ARRAY_GRP_CONFIG	 array_grp_config;
	BIAS_CONFIG          bias_config;
	MUL_GRP_CONFIG		 mul_grp_config;
	ACTV_GRP_CONFIG		 actv_grp_config;
	REACTV_GRP_CONFIG	 reactv_grp_config;
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
			DLOG(INFO)<<"RoundTotal:"<<t;
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

