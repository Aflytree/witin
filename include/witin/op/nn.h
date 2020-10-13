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


class ACT_ATTRS : public ATTRS{
public:
	int act_en;
	string act_type;
};

class BIAS_ATTRS : public ATTRS{
public:
	int bias_en;
	vector<int16_t> data;
};

class mvOpNode : public OpNode{
    public:
		mvOpNode(){}
		virtual ~mvOpNode(){
			delete const_tensor;
		}
        mvOpNode(vector<vector<int> > shape,
				 Tensor *c_tensor,
				 int id = MV_OPNODE_ID,
				 const std::string name = "",
				 bool act_en = 0,
				 string act_type = "",
				 bool bias_add_en = false,
				 vector<int16_t> bias_data = {}
				 )
        : OpNode{id, name}
        {
			this->input1_shape = shape;
			const_tensor = new Tensor(c_tensor->getShape(), CONST_TYPE);
			fillTensorPtr(this->const_tensor,
					c_tensor->getShape(),
					c_tensor->getData());
            this->id = id;
            this->name = name;
			this->act_attrs.act_en = act_en;
			this->act_attrs.act_type = act_type;
			this->bias_attrs.bias_en = bias_add_en;
			this->bias_attrs.data = bias_data;
        }

		/*
		 * infer output shape of the mvOpNode
		 *
		 */
		vector<vector<int> > infer_shape()
		{
			Tensor *t;
			getConstTensor(&t);

			vector<int> input2_shape = t->getShape();

			if(input1_shape[0].size() != 2 || input2_shape.size() != 2)
			{
				LOG(FATAL)<<"[infer_shape] mvOpNode shape size should be 2! "<<
								input1_shape.size() <<" vs "<<input2_shape.size();
			}
			vector<int> out_shape;

			out_shape.push_back(input1_shape[0][0]);
			out_shape.push_back(input2_shape[1]);

			for(auto kv : out_shape)
			{
				DLOG(INFO)<<"[infer shape] out_shape : "<<kv;
			}
			vector<vector<int> > ret;
			ret.push_back(out_shape);
			return ret;
		};

		/*
		 * get input1 shape of the mvOpNode
		 *
		 */
		vector< vector<int>> getInputShape(){return input1_shape;}

		bool isUseConstTensor(){return true;}

		int getConstTensor(Tensor **t)
		{
			*t = const_tensor;
			return 0;
		}

		bool getActEn()
		{
			return act_attrs.act_en;
		}

		string getActType()
		{
			return act_attrs.act_type;
		}

		bool getBiasEn()
		{
			return bias_attrs.bias_en;
		}

		vector<int16_t> getBiasData()
		{
			return bias_attrs.data;
		}


    private :
        int id;
        string name;
		vector<vector<int> > input1_shape;
		class ACT_ATTRS act_attrs;
		class BIAS_ATTRS bias_attrs;
		Tensor *const_tensor;
};

} //namespace base
} //namespace witin


#endif //_NN_OP_H



