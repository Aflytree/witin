/*================================================================
*   Copyright (C) 2020 Lynxi Ltd. All rights reserved.
*   
*   @file       ：math.h
*   @author     ：afly
*   @date       ：2020.08.23
*   @description：
*
*================================================================*/
#ifndef _MATH_OP_H
#define _MATH_OP_H

#include <witin/node/node.h>

namespace witin{
namespace math{

using namespace witin::node;

class LogNode : public OpNode{
    public:
        LogNode(){}
        LogNode(int id, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

class AbsNode : public OpNode{
    public:
        AbsNode(){}
        AbsNode(int id, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

} //namespace node
} //namespace witin


#endif //MATH_H



