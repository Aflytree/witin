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

class ATTRS{

	public:

};

class OpNode : public node{
    public:
        OpNode(){}
        OpNode(int id, std::string name)
        : node(id, name)
        {
            this->id = id;
            this->name = name;
        }
		~OpNode()
		{
			if(top_tensors.size() > 0)
			{
				for(Tensor* kv : top_tensors)
					delete kv;
			}
			if(bottom_tensors.size() > 0)
			{
				for(Tensor* kv : bottom_tensors)
					delete kv;
			}
			if(array_input_tensors.size() > 0)
			{
				for(Tensor* kv : array_input_tensors)
					delete kv;
			}
			if(array_output_tensors.size() > 0)
			{
				for(Tensor* kv : array_output_tensors)
					delete kv;
			}
		}

		string getName()
		{
            return name;
        }

		int getID()
		{
			return id;
		}

		int set_opnode_input_tensor(class OpNode *op, int input_idx,
														Tensor* tensor);
		int set_input_tensors(vector<Tensor*> ts)
		{
			for(auto kv : ts)
				input_tensors.push_back(kv);
			//for(int i = 0; i < input_tensors.size();i++)
			//{
			//	DLOG(INFO)<<"    TENSOR_TYPE:"<<input_tensors[i]->tensor_type;
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
			//DLOG(INFO)<<"OpNode get input tensors, size = "<<input_tensors.size();
			for(auto kv : input_tensors)
				ts.push_back(kv);
			return 0;
		}

		virtual vector<vector<int> > infer_shape()
		{
			vector<vector<int> > ret;
			return ret;
		}

		virtual vector<vector<int> > getInputShape()
		{
			vector<vector<int> > ret;
			return ret;
		}
		virtual void forward(Tensor* input_tensor, Tensor* output_tensor,
												   Tensor* array_result) {};

		virtual bool isUseConstTensor(){return false;}
		virtual int getConstTensor(Tensor **t){return 0;}

		vector<Tensor*> input_tensors;
		vector<Tensor*> output_tensors;

		/* **************for op forward***************** */
		/* all tensor type is CONST_TYPE */
		void updateInputTensors(Tensor* ts);
		void updateOutputTensors(Tensor* ts, Tensor* array_result_tensor);

		vector<Tensor*> getBottomTensors(){return bottom_tensors;}
		vector<Tensor*> getTopTensors(){return top_tensors;}

		//input tensors type:Const
		vector<Tensor*> top_tensors;
		//output tensors type:Const
		vector<Tensor*> bottom_tensors;

		//input tensors type:Const
		vector<Tensor*> array_input_tensors;
		//output tensors type:Const
		vector<Tensor*> array_output_tensors;

	private:
        int id;
		std::string name;
		struct ir_op op;
};

//int OpNode::set_opnode_input_tensor(class OpNode *op, int input_idx, Tensor* tensor)
//{
//	//if(tensor->getConsumersNum() >= MAX_CONSUMERS_SIZE)
//	//{
//	//	DLOG(INFO)<<"error in set opnode input tensor"<<;
//	//	return -1;
//	//}
//
//	op->input_tensors[input_idx] = tensor;
//
//}

} // namespace node
} // namespace witin

#endif //NODE_H



