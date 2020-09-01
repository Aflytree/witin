/*************************************************************************
	> File Name: session.cpp
	> Author: afly
	> Mail: aifei.zhang@witintech.com 
	> Created Time: Wed Aug 26 19:31:55 2020
 ************************************************************************/

#include <witin/session/session.h>
#include <witin/graph/graph.h>
#include <witin/global.h>
#include <deque>

using namespace witin::graph;
using namespace witin::session;
//using namespace witin::target;


typedef std::shared_ptr<witin::base::OpNode> baseOpNodePtr;

namespace witin{
namespace session{
	
	/*
	 * isInputNode
	 */

	bool isInputNode(baseOpNodePtr bs, vector<baseOpNodePtr> bs_list)
	{
		for(auto node : bs_list)
		{
			if(node == bs)
				return true;
		}
	}
	
	/*
	 * isOutputNode
	 */

	bool isOutputNode(baseOpNodePtr bs, vector<baseOpNodePtr> bs_list)
	{
		for(auto node : bs_list)
		{
			if(node == bs)
				return true;
		}
	}

	int32_t Session::build(WitinGraphType &InGraph, vector<vector<int> > shapes)
	{
		//std::cout<<"Session build "<<std::endl;
		//auto in_nodes = InGraph.inNodes();
		//auto out_nodes = InGraph.outNodes();
		//Core core;
		//std::cout<<"in_nodes.size(): "<<in_nodes.size()<<std::endl;
		//vector<baseOpNodePtr> topological_list = InGraph.graph_topological_sort();
		//
		//int roundTotal = (int)topological_list.size(); 
		//core.setRoundTotal(roundTotal);
		//
		//std::vector<RoundConfig> rounds;
		//RoundConfig rdc;
		//
		//for(size_t i = 0; i < topological_list.size(); i++)
		//{
		//	if(isInputNode(topological_list[i], in_nodes))
		//	{
		//			
		//	}
		//	else if(isOutputNode(topological_list[i], out_nodes))
		//	{
		//									
		//	}
		//	else
		//	{
		//	
		//	
		//	
		//	}


		//		
		//	
		//
		//}
	
	}
	
	int32_t Session::run(WitinGraphType &InGraph, std::vector<Tensor> inputs)
	{
	
	}



} //namespace witin
} //namespace witin

