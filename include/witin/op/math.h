/*================================================================
*
*   @file       ：math.h
*   @author     ：afly
*   @date       ：2020.08.23
*   @description：
*
*================================================================*/
#pragma once
#ifndef _MATH_OP_H
#define _MATH_OP_H

#include <witin/node/node.h>
#include <witin/global.h>

namespace witin{
namespace base{


class LogOpNode : public OpNode{
    public:
        LogOpNode(){}
        LogOpNode(vector<vector<int> > shape,
					int id = LOG_OPNODE_ID, const std::string name = "")
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

class AbsOpNode : public OpNode{
    public:
        AbsOpNode(){}
        AbsOpNode(vector<vector<int> > shape,
					int id = ABS_OPNODE_ID, const std::string name = "")
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


class AddOpNode : public OpNode{
    public:
        AddOpNode(){}
        AddOpNode(vector<vector<int> >shape,
					int id = ADD_OPNODE_ID,
					const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
			this->shape = shape;
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



} //namespace base
} //namespace witin


#endif //_MATH_OP_H



