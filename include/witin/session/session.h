#pragma once
#include <witin/graph/graph.h>
#include <witin/node/node.h>
#include <witin/tensor/tensor.h>

namespace witin{
namespace session{
    typedef std::shared_ptr<witin::node::OpNode> baseOpNodePtr;

    class Session{
    	Session()
	{}
	int32_t run(afly_graph &InGraph);
    
    };

} //namespace session

} //namespace witin


