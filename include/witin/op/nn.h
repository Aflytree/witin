/*================================================================
*   @file       :nn.h
*   @author     :afly
*   @date       :2020.08.25
*   @description:
*
*================================================================*/
#pragma once
#ifndef _NN_OP_H
#define _NN_OP_H

#include <witin/global.h>
#include <witin/tensor/tensor.h>

namespace witin{
namespace base{


class MV_ATTRS : public ATTRS{
public:
	int act_en;
	string act_type;
};


class mvOpNode : public OpNode{
    public:
		mvOpNode(){}
		virtual ~mvOpNode(){
			delete const_tensor;
		}
        mvOpNode(vector<int> shape,
				 Tensor *c_tensor,
				 int id = MV_OPNODE_ID,
				 const std::string name = "",
				 bool act_en = 0,
				 string act_type = ""
				 )
        : OpNode{id, name}
        {
			this->input1_shape = shape;
			const_tensor = new Tensor(c_tensor->getShape(), CONST_TYPE);
			fillTensorPtr(this->const_tensor,
					c_tensor->getShape(),
					c_tensor->getData());
			//this->const_tensor = c_tensor;
            this->id = id;
            this->name = name;
			this->act_en = act_en;
			this->act_type = act_type;
			this->mv_attrs.act_en = act_en;
			this->mv_attrs.act_type = act_type;
        }

		//int get_input_tensors(vector<Tensor*> & ts)
		//{
		//	DLOG(INFO)<<"mvOpNode get input tensors";
		//}

		vector<int> infer_shape()
		{
			vector<int>	input1_shape = getInputShape();
			Tensor *t;
			getConstTensor(&t);

			vector<int> input2_shape = t->getShape();

			if(input1_shape.size() != 2 || input2_shape.size() != 2)
			{
				DLOG(INFO)<<"[infer_shape] mvOpNode shape size should be 2!";
			}
			vector<int> out_shape;

			out_shape.push_back(input1_shape[0]);
			out_shape.push_back(input2_shape[1]);
			//out_shape.assign((int)input1_shape.size() - 1, input2_shape[1]);
			for(auto kv : out_shape)
			{

				DLOG(INFO)<<"[infer shape] out_shape : "<<kv;
			}
			return out_shape;
		};

		vector<int> getInputShape(){return input1_shape;}

		bool isUseConstTensor(){return true;}

		int getConstTensor(Tensor **t)
		{
			*t = const_tensor;
			return 0;
		}

		bool getActEn()
		{
			return act_en;
		}

		string getActType()
		{
			return act_type;
		}

    private :
        int id;
        string name;
		vector<int> input1_shape;
		class MV_ATTRS mv_attrs;
		bool act_en;
		string act_type;
		Tensor *const_tensor;
};

} //namespace nn
} //namespace witin


#endif //_NN_OP_H



