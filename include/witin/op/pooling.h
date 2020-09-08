/*================================================================
*   
*   @file       ：pooling.h
*   @author     ：afly
*   @date       ：2020.08.23
*   @description：
*
*================================================================*/
#ifndef _POOLING_H
#define _POOLING_H

#include <witin/node/node.h>
#include <witin/global.h>

namespace witin{
namespace base{


class poolOpNode : public OpNode{
    public:
		constNode(){}
		constNode(int id = POOL_OPNODE_ID, const std::string name = "")
        : OpNode{id, name}
        {
            this->id = id;
            this->name = name;
        }
    private :
        int id;
        std::string name;
};

} //namespace base
} //namespace witin

#endif //POOLING_H




