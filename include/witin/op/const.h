/*================================================================
*   @file       :const.h
*   @author     :afly
*   @date       :2020.08.25
*   @description:
*
*================================================================*/
#ifndef _CONST_OP_H
#define _CONST_OP_H

#include <witin/node/node.h>
#include <witin/global.h>

namespace witin{
namespace base{

//using namespace witin::node;

class constNode : public OpNode{
    public:
		constNode(){}
        constNode(int id = CONST_NODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

} //namespace cnst
} //namespace witin


#endif //_CONST_OP_H



