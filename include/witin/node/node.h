/*
 * @Author: your name
 * @Date: 2020-08-22 11:04:29
 * @LastEditTime: 2020-08-22 16:39:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings EditO
 * @FilePath: /tvm/home/afly/work/witin/include/fy/node.h
 */

#pragma once
#ifndef _NODE_H
#define _NODE_H

#include <witin/global.h>
#include <witin/node/baseNode.h>
#include <witin/tensor/tensor.h>

namespace witin{
namespace base{

class Tensor;
using namespace witin::node;

struct ir_op{
	int op_type;
	int op_version;
	bool same_shape;
	int param_size;
};

//int set_opnode_input_tensor(class OpNode op, int input_idx, Tensor tensor){
//	//if(tensor->getConsumersNum() >= MAX_CONSUMERS_SIZE)
//	//{
//	//	std::cout<<"error in set opnode input tensor"<<std::endl;	
//	//	return -1;
//	//}
//	
//	op.input_tensors[input_idx] = tensor;
//
//
//}

class OpNode : public node{
    public:
        OpNode(){}
        OpNode(int id, std::string name)
        : node(id, name)
        {
            this->id = id;
            this->name = name;
        }
		std::string getName(){
            return name;
        }

		//int set_input_tensors(vector<Tensor> ts){}
		//int get_input_tensors(vector<Tensor> & ts){}
		//int set_output_tensors(vector<Tensor> ts){}
		//int get_output_tensors(vector<Tensor> & ts){}
		//
		//virtual std::vector<int> infer_shape(){};

	private:
        int id;
		std::string name;
		//vector<witin::tensor::Tensor> input_tensors;
		//vector<Tensor> output_tensors;
		struct ir_op op;
};

} // namespace node
} // namespace witin

#endif //NODE_H



