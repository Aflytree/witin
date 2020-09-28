/*================================================================
*
*   @file       ：active.h
*   @author     ：afly
*   @date       ：2020.08.26
*   @description：
*
*================================================================*/
#pragma once
#ifndef _ACTIVE__H
#define _ACTIVE__H

#include <witin/global.h>
#include <witin/node/node.h>

namespace witin{
namespace base{

//using namespace witin::node;

class TmpNode {


};

class TanhOpNode : public OpNode{
    public:
		TanhOpNode(){}
		TanhOpNode(vector<vector<int> >shape,
                    int id = TANH_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->shape = shape;
            this->id = id;
            this->name = name;
        }

        vector<vector<int> > infer_shape()
		{
			return shape;
		}

		vector< vector<int>> getInputShape()
		{
			return shape;
		}

		bool isUseConstTensor()
		{
			return false;
		}

		vector<vector<int> > shape;
    private :
        int id;
        std::string name;
};

class SigmoidOpNode : public OpNode{
    public:
		SigmoidOpNode(){}
		SigmoidOpNode(vector<vector<int> >shape,
				int id = SIGMOID_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->shape = shape;
            this->id = id;
            this->name = name;
        }

        vector<vector<int> > infer_shape()
		{
			return shape;
		}

		vector< vector<int>> getInputShape()
		{
			return shape;
		}

		bool isUseConstTensor()
		{
			return false;
		}

		vector<vector<int> > shape;
    private :
        int id;
        std::string name;
};

class reluOpNode : public OpNode{
    public:
		reluOpNode(){}
		reluOpNode(vector<vector<int> >shape,
				int id = RELU_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }

        vector<vector<int> > infer_shape()
		{
			return shape;
		}

		vector< vector<int>> getInputShape()
		{
			return shape;
		}

		bool isUseConstTensor()
		{
			return false;
		}

		vector<vector<int> > shape;
    private :
        int id;
        std::string name;
};

class ActOpNode : public OpNode{
    public:
		ActOpNode(){}
		ActOpNode(vector<vector<int> >shape, int act_type = 0,
                    int id = ACT_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->shape = shape;
            this->id = id;
            this->name = name;
            this->act_type = act_type;
        }
        vector<vector<int> > infer_shape()
		{
			return shape;
		}

		vector< vector<int>> getInputShape()
		{
			return shape;
		}

		bool isUseConstTensor()
		{
			return false;
		}

		vector<vector<int> > shape;
        int act_type;
    private :
        int id;
        std::string name;
};


} //namespace base
} //namespace witin

#endif //_ACTIVE_H




