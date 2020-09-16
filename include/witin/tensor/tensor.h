/*
 * @Author: your name
 * @Date: 2020-08-22 10:19:32
 * @LastEditTime: 2020-08-22 22:38:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/include/witin/tensor/tensor.h
 */
#pragma once
#ifndef _TENSOR_H
#define _TENSOR_H

#include <witin/global.h>
#include <witin/node/node.h>
#include <witin/target/mem.h>

#define MAX_CONSUMERS_SIZE 8

namespace witin{
namespace base{

class TensorNode
{
};

class operation
{
};

class OpNode;

enum TENSOR_TYPE
{
	CONST_TYPE = 0,
	PLACEHOLDER_TYPE,
	NORMAL_TYPE
};

enum DATA_TYPE
{
	INT8_TYPE = 0,
	INT16_TYPE,
	FP16_TYPE,
	FP32_TYPE
};


//template <typename T>
class Tensor{
	//const TensorNode* operator ->();
    //typedef T T_
    public: 
		//default
		Tensor();
		
		Tensor(vector<int> shape = {}, enum TENSOR_TYPE tensor_type = PLACEHOLDER_TYPE, 
								  int data_type = INT8_TYPE)
		{
			this->shape = shape;
			this->data_type = data_type;
			this->tensor_type = tensor_type;
		}
		
		bool operator ==(const Tensor & other) const;
    	bool operator !=(const Tensor & other) const;
    	
		int ndim(int &ndim) const{
			ndim = shape.size();
		}
    	void setShape(std::vector<int> s);
    	vector<int> getShape() const;
		void print() const;
    	
		void *getData() const;
    	int getSize(int & size) const;
    	int setData(void * data_in);
		
		
		int setConsumers(vector<OpNode> ops){}
	
		int getConsumers(vector<OpNode> &ops){}
		int getConsumersNum()
		{
			return 0;
			//return (int)consumers.size();
		}

		int setProducer(OpNode *op){}
		int getProducer(OpNode *op){}

		int getDataType(){ return data_type; }
        operation* op;
	
		//type: CONST, PLACEHOLDER
		enum TENSOR_TYPE tensor_type;
		
		int fillTensor(Tensor &t, vector<int> shape, void *data,
						bool random);

    private:
		//Tensor shape 
		std::vector<int> shape;
		//data type  0:int8,  1:float16
		int data_type;
		//the operation produce this Tensor
		OpNode* producer;
		//the output index  from source operation
		int value_index{0};
		//ops who use this tensor 
		vector<OpNode> consumers;	
		// tensor <<===>> mem_record
		struct mem_record * mr; 
		void* __data;
};

}// namespace base
}// namespace witin

#endif //TENSOR_H

