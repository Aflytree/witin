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
        mvOpNode(vector<vector<int> > shape,
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
            this->id = id;
            this->name = name;
			this->mv_attrs.act_en = act_en;
			this->mv_attrs.act_type = act_type;
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
			return mv_attrs.act_en;
		}

		string getActType()
		{
			return mv_attrs.act_type;
		}

    private :
        int id;
        string name;
		vector<vector<int> > input1_shape;
		class MV_ATTRS mv_attrs;
		Tensor *const_tensor;
};

} //namespace base
} //namespace witin


#endif //_NN_OP_H



