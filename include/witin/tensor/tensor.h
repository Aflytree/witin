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
//#include <witin/target/mem.h>

namespace witin{
namespace base{


class TensorNode{
};

class operation{
};

#define MAX_CONSUMERS_SIZE 8

class OpNode;


//template <typename T>
class Tensor{
    // const TensorNode* operator ->();
    //typedef T T_
    public: 
		Tensor();
		
		bool operator ==(const Tensor & other) const;
    	bool operator !=(const Tensor & other) const;
    	
		int ndim(int &ndim) const{
			ndim = shape.size();
		}
    	void setShape(std::vector<int> s);
    	std::vector<int> getShape() const;
		
		void print() const;
    	
		void *getData() const;
    	int getSize(int & size) const;
    	int setData(void * data_in);
		
		//int setConsumers(vector<OpNode> ops){}
		//int getConsumers(vector<OpNode> &ops){}
		int getConsumersNum()
		{
			return 0;
			//return (int)consumers.size();
		}

		//int setProducer(OpNode op){}
		//int getProducer(OpNode &op){}

		//string getDataType();
        operation* op;
		
		//type: CONST, PLACEHOLDER
		//string tensor_type;

    private:
		//Tensor shape 
		std::vector<int> shape;
		
		//the op produce this Tensor
		//OpNode producer;
		//ops who use this tensor 
		//vector<OpNode> consumers;	
		// tensor <<===>> mem_record
		//struct mem_record * mr; 
		void* __data;
};


}// namespace tensor
}// namespace witin

#endif //TENSOR_H

