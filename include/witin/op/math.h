/*================================================================
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
#include <witin/global.h>

namespace witin{
namespace math{

//using namespace witin::node;

class LogOpNode : public OpNode{
    public:
        LogOpNode(){}
        LogOpNode(int id = LOG_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

class AbsOpNode : public OpNode{
    public:
        AbsOpNode(){}
        AbsOpNode(int id = ABS_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

} //namespace math
} //namespace witin


#endif //_MATH_OP_H



