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
#include <witin/sim/lib.h>

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

		void forward(Tensor *input_tensor, Tensor* output_tensor, Tensor* array_result)
		{
			DLOG(INFO)<<"mvOpNode forward --->";
			vector<int> shape1 = input_tensor->getShape();
			vector<int> shape2 = const_tensor->getShape();
			vector<int> scale_table = {128, 256, 512,  1024, 2048, 4096};
			vector<int> scale_table_index = {7, 8, 9, 10, 11};
			int index = 0;
			for(size_t idx = 0; idx < scale_table.size(); idx++)
			{
				if(scale_table[idx] == scale)
				{
					index = idx;
					break;
				}
			}
			shift_scale = scale_table_index[index];

			DLOG(INFO)<<"shape1:"<< shape1;
			DLOG(INFO)<<"shape2:"<< shape2;

			vector<int16_t> arrayResult = ComputeArrayMac(input_tensor, const_tensor);
			// DLOG(INFO)<<"arrayResult:"<< arrayResult;
			vector<int16_t> biasResult ;
			vector<char> afterArray;
			vector<char> actResult;
			vector<char> final_result;

			//bias compute
			if(getBiasEn())
			{
				biasResult = ComputeBiasAdd(arrayResult, getBiasData());
				// DLOG(INFO)<<"biasResult:"<< biasResult;
				for(auto kv : biasResult)
				{
					int afterShift = kv >> shift_scale;
					if(round(afterShift) > 127)
						afterArray.push_back(127);
					else if(round(afterShift) < -128)
						afterArray.push_back(-128);
					else
						afterArray.push_back(round(afterShift));
				}
			}
			else
			{
				for(auto kv : arrayResult)
				{
					int afterShift = kv >> shift_scale;
					if(round(afterShift) > 127)
						afterArray.push_back(127);
					else if(round(afterShift) < -128)
						afterArray.push_back(127);
					else
						afterArray.push_back(round(afterShift));
				}
			}
			// DLOG(INFO)<<"afterArray:"<< afterArray;

			//act compute
			if(getActEn())
			{
				if(getActType() == "relu")
					actResult = ComputeRelu(afterArray);
				if(getActType() == "tanh")
					actResult = ComputeTanh(afterArray);
				if(getActType() == "sigmoid")
					actResult = ComputeSigmoid(afterArray);
			}

			//final_result
			if(getBiasEn() && getActEn())
				final_result = actResult;
			else if(getBiasEn() && !getActEn())
				final_result = afterArray;
			else if(!getBiasEn() && !getActEn())
				final_result = afterArray;
			else
				final_result = afterArray;

			vector<vector<int> > int_shape = infer_shape();
			fillTensorPtr(output_tensor, int_shape[0], final_result);
			//output simple array result
			fillTensorPtr(array_result, int_shape[0], afterArray);
		}

    private :
        int id;
        string name;
		vector<vector<int> > input1_shape;
		class ACT_ATTRS act_attrs;
		class BIAS_ATTRS bias_attrs;
		Tensor *const_tensor;
		int scale = 512;
		int shift_scale;
};

} //namespace base
} //namespace witin


#endif //_NN_OP_H



