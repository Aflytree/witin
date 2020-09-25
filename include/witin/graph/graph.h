/*
 * @Author: your name
 * @Date: 2020-05-04 11:35:46
 * @LastEditTime: 2020-05-06 23:13:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/afly_ai/include/witin/graph/graph.h
 */
#pragma once
#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdlib.h>
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graphviz.hpp>

#include <dmlc/logging.h>
#include <witin/node/node.h>
#include <witin/global.h>

namespace boost {

}

namespace witin{
namespace base{

using namespace boost;

struct EdgeProperty{
	EdgeProperty(int src = 0,
				 int dst = 0) :
				 src(src), dst(dst)
	{}
    int src;
    int dst;
};

typedef std::shared_ptr<witin::base::OpNode> baseOpNodePtr;

// enum vertex_properties_t { vertex_properties };
// enum edge_properties_t { edge_properties };

template < typename NodeDataType, typename EdgeDataType>
class witin_graph
{
    typedef struct edge_t
	{
        edge_t(NodeDataType src = nullptr,
               NodeDataType dst = nullptr) :
               srcNode(src), dstNode(dst)
        {}
        NodeDataType srcNode;
        NodeDataType dstNode;
        EdgeProperty data;

    }UserEdge;

    typedef property<vertex_name_t, NodeDataType>
                                        VertexProperty;
    // typedef property<edge_properties_t, EdgeDataType>
    //                                     EdgeWeightProperty;
    typedef property<edge_weight_t, EdgeDataType>
                                        EdgeWeightProperty2;
    typedef boost::adjacency_list<listS, vecS, bidirectionalS,
	//typedef boost::adjacency_list<listS, vecS, directedS,
                        VertexProperty, EdgeWeightProperty2> graph_type_;
    typedef typename graph_traits<graph_type_>::vertex_iterator
                                                        vertex_iter;
    // 边迭代器类型
    typedef typename graph_traits<graph_type_>::edge_iterator edge_iter;

    /* a bunch of graph-specific typedefs */
    typedef typename graph_traits<graph_type_>::vertex_descriptor Vertex;
    typedef typename graph_traits<graph_type_>::edge_descriptor Edge;
    typedef std::pair<Edge, Edge> EdgePair;

    typedef typename graph_traits<graph_type_>::adjacency_iterator adjacency_iter;
    typedef typename graph_traits<graph_type_>::out_edge_iterator out_edge_iter;
    typedef typename graph_traits<graph_type_>::in_edge_iterator in_iter, in_end;
    typedef typename graph_traits<graph_type_>::in_edge_iterator in_edge_iter, in_edge_end;

    typedef typename graph_traits<graph_type_>::degree_size_type degree_t;

    typedef std::pair<adjacency_iter, adjacency_iter> adjacency_vertex_range_t;
    typedef std::pair<out_edge_iter, out_edge_iter> out_edge_range_t;
    typedef std::pair<vertex_iter, vertex_iter> vertex_range_t;
    typedef std::pair<edge_iter, edge_iter> edge_range_t;

    public:

		/* default constructors etc. */
        witin_graph(){}

		/*deconstructors*/
        virtual ~witin_graph(){}

        /* structure modification methods */
        void Clear()
		{
            inter_graph.clear();
        }

        /*
		 * add node to inter_graph and update global local_map_
		 */
        Vertex addNode(const NodeDataType& node)
		{
            auto r = local_map_.find(node);
            if(r != local_map_.end()){
                LOG(FATAL) << "node is already added!";
            }
            auto v = add_vertex(node, inter_graph);
            local_map_[node] = v;
            return v;
        }

		/*
		 * add edge to inter_graph
		 */
        void addEdge(NodeDataType node1, NodeDataType node2,
                                    EdgeDataType edge)
		{
            auto r1 = local_map_.find(node1);
            auto r2 = local_map_.find(node2);
            if(r1 == local_map_.end() && r2 == local_map_.end())
			{
                LOG(FATAL) << "Should add node first!";
            }
            add_edge(r1->second, r2->second, edge, inter_graph);

        }

		/*
		 * get in degree of a node
		 */
		int in_degree_node(NodeDataType node)
		{
			//vertex_range_t vrange = vertices(inter_graph);
			//auto vertexprop = get(boost::vertex_name, inter_graph);
			//
			//boost::graph_traits<graph_type_>::vertex_iterator i, end;
			//boost::graph_traits<graph_type_>::in_edge_iterator ei, edge_end;

			//for(boost::tie(i,end) = vertices(inter_graph); i != end; ++i)
			//{
			//	DLOG(INFO) << *i << " <-- ";
			//	for (boost::tie(ei,edge_end) = in_edges(*i, inter_graph); ei != edge_end; ++ei)
			//		DLOG(INFO) << source(*ei, inter_graph) << "  ";
			//}
			int idegree = 0;
			for(auto kv : local_map_)
			{
				if(node == kv.first)
				{
					idegree = in_degree(kv.second, inter_graph);
				}
			}
			return idegree;
		}

		/*
		 * get in nodes of a node
		 */
		std::vector<NodeDataType> innodes_of_node(NodeDataType node)
		{
			std::vector<NodeDataType> node_list;
			vertex_range_t vrange = vertices(inter_graph);
			auto vertexprop = get(boost::vertex_name, inter_graph);

            edge_range_t erange = edges(inter_graph);

			if(erange.first == erange.second)
			{
				DLOG(INFO)<<"Empty Edges In Inter_graph!";
            }

            for(edge_iter itr = erange.first; itr != erange.second; ++itr)
			{
                auto vertexprop = get(boost::edge_weight, inter_graph);
                auto s = get(boost::vertex_name, inter_graph, source(*itr, inter_graph));
                auto t = get(boost::vertex_name, inter_graph, target(*itr, inter_graph));

				if(node == t)
				{
					node_list.push_back(s);
					DLOG(INFO) <<s->getName() <<"["<<source(*itr, inter_graph)<<"]" << "-->" <<
					    t->getName() <<"["<<target(*itr, inter_graph) <<"]";
				}
            }
			return node_list;
		}

		/*
		 * get out nodes of a node
		 */
		std::vector<NodeDataType> outnodes_of_node(NodeDataType node)
		{



		}


		/*
		 * get out degree of a node
		 */
		int out_degree_node(NodeDataType node)
		{
			int degree = 0;
			for(auto kv : local_map_)
			{
				if(node == kv.first)
				{
					auto edges = out_edges(kv.second, inter_graph);
					degree = out_degree(kv.second, inter_graph);
				}
			}
			return degree;
		}

		/*
		 * get all input nodes of the inter_graph
		 */
		std::vector<NodeDataType> inNodes()
		{
			std::vector<NodeDataType> nodes;
			std::vector<NodeDataType > allNodes = getAllNodes();
			for(auto node : allNodes)
			{
				if(0 == in_degree_node(node))
				{
					nodes.push_back(node);
				}
			}
			return nodes;
		}

		/*
		 * get all output nodes of inter_graph
		 */
		std::vector<NodeDataType> outNodes()
		{
			std::vector<NodeDataType> nodes;
			std::vector<NodeDataType> allNodes = getAllNodes();
			for(auto node : allNodes)
			{
				if(0 == out_degree_node(node))
				{
					nodes.push_back(node);
				}
			}
			return nodes;
		}

		/*
		 * print all nodes of inter_graph
		 */
		void printAllNodes()
        {
            //DLOG(INFO)<<"Dump all nodes:";
            vertex_range_t vrange = vertices(inter_graph);
            auto vertexprop = get(boost::vertex_name, inter_graph);
            for(vertex_iter itr = vrange.first; itr!=vrange.second; ++itr){
                NodeDataType vprop = vertexprop[*itr];
                DLOG(INFO) <<vprop->getName() <<"["<<*itr <<"]";
            }
        };

		/*
		 *  store all nodes of inter_graph into a vector
		 */
        std::vector<NodeDataType> getAllNodes()
        {
            //DLOG(INFO)<<"Get all nodes:";
            std::vector<NodeDataType> v;
            auto vertexprop = get(boost::vertex_name, inter_graph);

            vertex_range_t vrange = vertices(inter_graph);
            for(vertex_iter itr = vrange.first; itr!=vrange.second; ++itr){
                NodeDataType vprop = vertexprop[*itr];
                v.push_back(vprop);
                //DLOG(INFO)<<vprop;
            }
            return v;
        };

		/*
		 *  store all nodes of inter_graph into a vector
		 */
        void printAllEdges()
        {
            //DLOG(INFO)<<"Dump all edges:";
            edge_range_t erange = edges(inter_graph);

			if(erange.first == erange.second)
			{
				DLOG(INFO)<<"Empty Edges In Inter_graph!";
                return;
            }
            for(edge_iter itr = erange.first; itr != erange.second; ++itr)
			{
				typename boost::property_map<graph_type_, edge_weight_t>::type edgeprop =
															get(boost::edge_weight, inter_graph);
				//auto edgeprop = get(boost::edge_weight, inter_graph);
                auto s = get(boost::vertex_name, inter_graph, source(*itr, inter_graph));
                auto t = get(boost::vertex_name, inter_graph, target(*itr, inter_graph));
                DLOG(INFO) <<s->getName() <<"["<<source(*itr, inter_graph)<<"]["<<edgeprop[*itr].src<<"]" << "-->" <<
                    t->getName() <<"["<<target(*itr, inter_graph) <<"]["<<edgeprop[*itr].dst<<"]";
            }
        }

		/*
		 *  dump total graph to console
		 */
        void print()
		{
            DLOG(INFO)<<"Dump Graph:";
            DLOG(INFO)<<"==========================";
            DLOG(INFO)<<"graph nodes:";
			printAllNodes();
            DLOG(INFO)<<"graph edges:";
            printAllEdges();
            DLOG(INFO)<<"==========================";
        }

		/*
		 *  total nodes of inter_graph
		 */
		size_t getNumNodes() const{return num_vertices[inter_graph];};

		/*
		 *  total edges of inter_graph
		 */
		size_t getNumEdges() const{return num_edges[inter_graph];};

		std::vector<EdgeDataType> getEAlledges();
        vector<EdgeDataType>  getInEdges(const NodeDataType& data);

        size_t getOutDegree(const NodeDataType& data);
        size_t getInDegree(const NodeDataType& data);
        size_t removeNode(const NodeDataType& data);
        int32_t removeEdge(const EdgeDataType & edge);
        int32_t removeEdge(const NodeDataType& node1,const NodeDataType& node2);

		/*
		 *  graph ===> linear vector
		 *
		 */
		vector<NodeDataType> graph_topological_sort()
		{
			std::vector<NodeDataType> ret;
			deque<int> top_order;
			topological_sort(inter_graph, front_inserter(top_order));

            auto vertexprop = get(boost::vertex_name, inter_graph);
			for(auto k : top_order)
			{
				NodeDataType node = vertexprop[k];
				ret.push_back(node);
				DLOG(INFO)<<node->getName();
			}
			return ret;
		}

		vector<NodeDataType> DFS();
		vector<NodeDataType> BFS();

        int32_t writeToPDF(std::string filename)
		{
			std::ofstream ofs(filename);
			string tmp1 = filename;
			if(ofs.fail())
			{
				LOG(FATAL)<<"Open file  "<<filename<<" error!";
			}
			//write_graphviz(ofs, inter_graph, make_label_writer());
			write_graphviz(ofs, inter_graph);
			char* file = strtok(const_cast<char*>(filename.c_str()), ".");
			string const& cc = string(file) + string(".pdf");
			char const* c = cc.c_str();
			//.dot ===> pdf
			string all = "dot -Tpdf " + tmp1 + " -o "+  cc;
			const char* all_cmd = all.c_str();
			DLOG(INFO)<<"dot == > pdf cmd:"all_cmd;
			system(all_cmd);
		}

    private:
        graph_type_ inter_graph;
        /*NodeDataType << -- >> Vertex*/
        std::map<NodeDataType, Vertex> local_map_;
};

typedef witin::base::witin_graph<baseOpNodePtr, EdgeProperty> WitinGraphType;

}// namespace graph
}// namespace witin

#endif //GRAPH_H

