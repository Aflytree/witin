#pragma once
#include <witin/graph/graph.h>
#include <witin/node/node.h>
#include <witin/tensor/tensor.h>
#include <witin/global.h>

namespace witin{
namespace base{

	typedef std::shared_ptr<witin::base::OpNode> baseOpNodePtr;

	class Session{
		public:
    		Session()
			{}
			int32_t build(WitinGraphType &InGraph, vector<vector<int> > shape);

			int32_t genCalibrationConfig(WitinGraphType &InGraph, vector<Tensor*> input_tensors);

			int32_t run(WitinGraphType &InGraph, vector<Tensor*> input_tensors);


		private:
			WitinGraphType inGraph;

    };

} //namespace session

} //namespace witin


