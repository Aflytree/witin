#pragma once
#include <witin/graph/graph.h>
#include <witin/node/node.h>
#include <witin/tensor/tensor.h>
#include <witin/global.h>


using namespace witin::tensor;
using namespace witin::graph;

namespace witin{
namespace session{
    typedef std::shared_ptr<witin::node::OpNode> baseOpNodePtr;

	class Session{
	public:	
    	Session()
		{}
		int32_t build(WitinGraphType &InGraph){
		
		}
		
		int32_t run(WitinGraphType &InGraph, std::vector<Tensor> inputs){
		
		}
    
	private:
		WitinGraphType inGraph;

    };
	
} //namespace session

} //namespace witin


