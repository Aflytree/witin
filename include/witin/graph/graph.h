/*
 * @Author: your name
 * @Date: 2020-05-04 11:35:46
 * @LastEditTime: 2020-05-06 23:13:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/afly_ai/include/fy/graph.h
 */
#pragma once
#ifndef _GRAPH_H
#define _GRAPH_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <dmlc/logging.h>
#include "../node/node.h"

namespace boost {

}

namespace witin{
namespace graph{

using namespace boost;
using namespace witin::node;

struct EdgeProperty{
    int src;
    int dst;
};

// enum vertex_properties_t { vertex_properties };
// enum edge_properties_t { edge_properties };

typedef std::shared_ptr<witin::node::OpNode> baseOpNodePtr;

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
    typedef boost::adjacency_list<listS, vecS, directedS,
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
        void Clear(){
            inter_graph.clear();
        }

        //不可用inline函数
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

        void addEdge(NodeDataType node1, NodeDataType node2,
                                    EdgeDataType edge)
		{
            auto r1 = local_map_.find(node1);
            auto r2 = local_map_.find(node2);
            if(r1 == local_map_.end() && r2 == local_map_.end()){
                LOG(FATAL) << "should add node first!";
            }
            add_edge(r1->second, r2->second, edge, inter_graph);

        }

        void in_degree(NodeDataType node);
        void out_degree(NodeDataType node);
        std::vector<NodeDataType> inNodes();
        std::vector<NodeDataType> outNodes();
        //void printAllNodes();
        //void witin_graph<NodeDataType, EdgeDataType>::printAllNodes(){
        void printAllNodes()
        {
            DLOG(INFO)<<"print all nodes:";
            vertex_range_t vrange = vertices(inter_graph);
            auto vertexprop = get(boost::vertex_name, inter_graph);
            for(vertex_iter itr = vrange.first; itr!=vrange.second; ++itr){
                NodeDataType vprop = vertexprop[*itr];
                cout <<vprop->getName() <<"["<<*itr <<"]"<< endl;

                auto tmp = *itr;
            }
        };
        
        std::vector<NodeDataType> getAllNodes()
        {
            DLOG(INFO)<<"get all nodes:";
            std::vector<NodeDataType> v;
            auto vertexprop = get(boost::vertex_name, inter_graph);

            vertex_range_t vrange = vertices(inter_graph);
            for(vertex_iter itr = vrange.first; itr!=vrange.second; ++itr){
                NodeDataType vprop = vertexprop[*itr];
                v.push_back(vprop);
                DLOG(INFO)<<vprop;
            }
            return v;
        };
        
        void printAllEdges()
        {
            DLOG(INFO)<<"all edges:";
            edge_range_t erange = edges(inter_graph);
            if(erange.first == erange.second){
                DLOG(INFO)<<"None";
                return;
            }
            for(edge_iter itr = erange.first; itr != erange.second; ++itr){
                auto vertexprop = get(boost::edge_weight, inter_graph);
                auto s = get(boost::vertex_name, inter_graph, source(*itr, inter_graph));
                auto t = get(boost::vertex_name, inter_graph, target(*itr, inter_graph));
                // baseOpNodePtr vprop = vertexprop(*itr);
                cout <<s->getName() <<"["<<source(*itr, inter_graph)<<"]" << "-->" <<
                    t->getName() <<"["<<target(*itr, inter_graph) <<"]"<< endl;
            }
        };

        //std::string print(){
        void print()
		{
            printAllNodes();
            printAllEdges();
        }
        //void printAllEdges();
        //std::vector<NodeDataType> getAllNodes();
        std::vector<EdgeDataType> getEAlledges();
        vector<EdgeDataType>  getInEdges(const NodeDataType& data);

        size_t getOutDegree(const NodeDataType& data);
        size_t getInDegree(const NodeDataType& data);
        size_t removeNode(const NodeDataType& data);
        int32_t removeEdge(const EdgeDataType & edge);
        int32_t removeEdge(const NodeDataType& node1,const NodeDataType& node2);
        size_t getNumNodes() const{return num_vertices[inter_graph];};
        size_t getNumEdges() const{return num_edges[inter_graph];};
        //boost dfs方法遍历图
        vector<NodeDataType> DFS();
        vector<NodeDataType> BFS();

        //整个图的inputNodes
        vector<NodeDataType>  getInputNodes();
        //整个图的outputNodes
        vector<NodeDataType>  getOutputNodes()
		{
            //vector<NodeDataType> DFSNodes = DFS();
            //outdegree为0
        }

        int32_t writeToPDF(char const *filename)
		{
        }

    private:
        graph_type_ inter_graph;
        /*NodeDataType << -- >> Vertex*/
        std::map<NodeDataType, Vertex> local_map_;
        //std::vector<>
};

typedef witin::graph::witin_graph<baseOpNodePtr, EdgeProperty> WitinGraphType;

}// namespace graph
}// namespace witin

#endif //GRAPH_H

