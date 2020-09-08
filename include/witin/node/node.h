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

struct ir_op{
	int op_type;
	int op_version;
	bool same_shape;
	int param_size;
};

class Tensor;


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

		int set_opnode_input_tensor(class OpNode *op, int input_idx, 
														Tensor* tensor);
		int set_input_tensors(vector<Tensor*> ts)
		{
			for(auto kv : ts)
				input_tensors.push_back(kv);
			//for(int i = 0; i < input_tensors.size();i++)
			//{
			//	cout<<"    TENSOR_TYPE:"<<input_tensors[i]->tensor_type<<endl;
			//}
			return 0;
		}

		int set_output_tensors(vector<Tensor*> ts)
		{
			for(auto kv : ts)
				output_tensors.push_back(kv);
			return 0;
		}

		int get_output_tensors(vector<Tensor*>  &ts)
		{
			for(auto kv : output_tensors)
			{
				//kv->print();
				ts.push_back(kv);
			}
			return 0;
		}
		
		int get_input_tensors(vector<Tensor*> &ts)
		{
			cout<<"OpNode get input tensors, size = "<<input_tensors.size()<<endl;
			for(auto kv : input_tensors)
				ts.push_back(kv);
			return 0;
		}

		virtual vector<int> infer_shape(){}
		virtual vector<int> getInputShape(){}
		virtual bool isUseConstTensor(){}
		virtual int getConstTensor(Tensor **t){}
		
		vector<Tensor*> input_tensors;
		vector<Tensor*> output_tensors;
	private:
        int id;
		std::string name;
		struct ir_op op;
};

//int OpNode::set_opnode_input_tensor(class OpNode *op, int input_idx, Tensor* tensor)
//{
//	//if(tensor->getConsumersNum() >= MAX_CONSUMERS_SIZE)
//	//{
//	//	std::cout<<"error in set opnode input tensor"<<std::endl;	
//	//	return -1;
//	//}
//	
//	op->input_tensors[input_idx] = tensor;
//	
//}

} // namespace node
} // namespace witin

#endif //NODE_H



