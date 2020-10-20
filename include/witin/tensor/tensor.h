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
		~Tensor()
		{
			free(__data);
		};

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
			return ndim;
		}
    	void setShape(std::vector<int> s);
    	vector<int> getShape() const;
		void print() const;

		//[11, 1] --> [1, 11]
		void transpose()
		{
			reverse(shape.begin(), shape.end());
		}

		void *getData() const;
    	int getSize(int & size) const;
    	int setData(void * data_in);

		int setConsumers(vector<OpNode> ops){ return 0;}
		int getConsumers(vector<OpNode> &ops){ return 0;}
		int getConsumersNum()
		{
			return 0;
			//return (int)consumers.size();
		}

		int setProducer(OpNode *op){return 0;}
		int getProducer(OpNode *op){return 0;}

		int getDataType(){ return data_type; }
        operation* op;

		//type: CONST, PLACEHOLDER
		enum TENSOR_TYPE tensor_type;

		int fillTensor(Tensor &t, vector<int> shape, void *data,
						bool random);

		std::vector<int> shape{0};
    private:
		//Tensor shape
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
//440 - 512
inline int fillTensorPtr(Tensor *t, vector<int> shape, void *value)
{
	char * data;
	int size = 1;
	for(size_t i = 0; i < shape.size();i++)
	{
		size *= shape[i];
	}
	//DLOG(INFO)<<"size1 = "<<size;
	data = (char*)malloc(size);

	for(int i = 0; i < shape[0]; i++)
	{
		for(int j = 0; j < shape[1]; j++)
		{
			data[i * shape[1] + j] = ((char*)value)[i * shape[1] + j];
			//DLOG(INFO)<<"data:"<<(int)(char)(i+j);
			//DLOG(INFO)<<"data:"<<(int)data[i*shape[1] + j];
		}
	}
	t->setData((void*)data);
	free(data);
	return 0;
}

inline int fillTensorPtr(Tensor *t, vector<int> shape,  vector<char>value)
{
	char * data;
	int size = 1;
	for(size_t i = 0; i < shape.size();i++)
	{
		size *= shape[i];
	}
	data = (char*)malloc(size);

	for(int i = 0; i < shape[0]; i++)
	{
		for(int j = 0; j < shape[1]; j++)
		{
			data[i * shape[1] + j] = value[i * shape[1] + j];
			//DLOG(INFO)<<"data:"<<(int)(char)(i+j);
			//DLOG(INFO)<<"data:"<<(int)data[i*shape[1] + j];
		}
	}
	t->setData((void*)data);

	free(data);
	return 0;
}

}// namespace base
}// namespace witin

#endif //TENSOR_H

