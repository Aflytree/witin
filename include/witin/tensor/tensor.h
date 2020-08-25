/*
 * @Author: your name
 * @Date: 2020-08-22 10:19:32
 * @LastEditTime: 2020-08-22 22:38:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/include/fy/tensor.h
 */
#pragma once
#ifndef _TENSOR_H
#define _TENSOR_H


#include <vector>
#include <witin/global.h>
using namespace std;

namespace witin{
namespace tensor{

class TensorNode{

};

class operation{


};


//template <typename T>
class Tensor{
    // const TensorNode* operator ->();
    //typedef T T_
    public: 
		Tensor();
		
		bool operator ==(const Tensor & other) const;
    	bool operator !=(const Tensor & other) const;
    	
		size_t ndim() const;
    	void setShape(std::vector<int> s);
    	std::vector<int> getShape() const;
		void print() const;
    	void *getData() const;
    	int getSize() const;
    	int setData(void * data_in);
		
		string getDataType(){
		
		};
        operation* op;

    private:
        std::vector<int> shape;
        void* __data;
		//T type;
};
}// namespace tensor
}// namespace witin

#endif //TENSOR_H

