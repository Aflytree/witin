/*
 * @Author: your name
 * @Date: 2020-08-22 11:43:08
 * @LastEditTime: 2020-08-22 22:58:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tvm/home/afly/work/witin/src/graph.cpp
 */
#include <iostream>
#include <witin/graph/graph.h>
#include <dmlc/logging.h>


namespace witin {
namespace graph {

    using namespace witin::node;
    using namespace std;

    //未定义的引用
    // template <typename NodeDataType, typename EdgeDataType>
    // void inter_graph<NodeDataType, EdgeDataType>::addEdge(NodeDataType node1,
    //                                                      NodeDataType node2,
    //                                                      EdgeDataType edge)
    // {
    //     auto r1 = local_map_.find(node1);
    //     auto r2 = local_map_.find(node2);
    //     if(r1 == local_map_.end() && r2 == local_map_.end()){
    //         LOG(FATAL) << "should add node first!";
    //     }
    //     add_edge(r1->second, r2->second, edge, inter_graph);
    // }

    template <typename NodeDataType, typename EdgeDataType>
    void afly_graph<NodeDataType, EdgeDataType>::in_degree(NodeDataType node){
    };

    template <typename NodeDataType, typename EdgeDataType>
    void afly_graph<NodeDataType, EdgeDataType>::out_degree(NodeDataType node){
    };

    template <typename NodeDataType, typename EdgeDataType>
    std::vector<NodeDataType> afly_graph<NodeDataType, EdgeDataType>::inNodes(){
    };

    template <typename NodeDataType, typename EdgeDataType>
    std::vector<NodeDataType> afly_graph<NodeDataType, EdgeDataType>::outNodes(){
    };

    
    
    //template <typename NodeDataType, typename EdgeDataType>
    //std::vector<NodeDataType> afly_graph<NodeDataType, EdgeDataType>::getAllNodes(){
    //    DLOG(INFO)<<"get all nodes:";
    //    std::vector<NodeDataType> v;
    //    auto vertexprop = get(boost::vertex_name, inter_graph);

    //    vertex_range_t vrange = vertices(inter_graph);
    //    for(vertex_iter itr = vrange.first; itr!=vrange.second; ++itr){
    //        NodeDataType vprop = vertexprop[*itr];
    //        v.push_back(vprop);
    //        DLOG(INFO)<<vprop;
    //    }
    //    return v;
    //};

    //template <typename NodeDataType, typename EdgeDataType>
    //void afly_graph<NodeDataType, EdgeDataType>::printAllNodes(){
    //    DLOG(INFO)<<"print all nodes:";
    //    vertex_range_t vrange = vertices(inter_graph);
    //    auto vertexprop = get(boost::vertex_name, inter_graph);
    //    for(vertex_iter itr = vrange.first; itr!=vrange.second; ++itr){
    //        NodeDataType vprop = vertexprop[*itr];
    //        cout <<vprop->getName() <<"["<<*itr <<"]"<< endl;

    //        auto tmp = *itr;
    //    }
    //};

    //template <typename NodeDataType, typename EdgeDataType>
    //void afly_graph<NodeDataType, EdgeDataType>::printAllEdges(){
    //    DLOG(INFO)<<"print all edges:";
    //    edge_range_t erange = edges(inter_graph);
    //    if(erange.first == erange.second){
    //        DLOG(INFO)<<"None";
    //        return;
    //    }
    //    for(edge_iter itr = erange.first; itr != erange.second; ++itr){
    //        auto vertexprop = get(boost::edge_weight, inter_graph);
    //        auto s = get(boost::vertex_name, inter_graph, source(*itr, inter_graph));
    //        auto t = get(boost::vertex_name, inter_graph, target(*itr, inter_graph));
    //        // baseOpNodePtr vprop = vertexprop(*itr);
    //        cout <<s->getName() <<"["<<source(*itr, inter_graph)<<"]" << "-->" <<
    //                            t->getName() <<"["<<target(*itr, inter_graph) <<"]"<< endl;


    //    }
    //}


}//namespace graph

} //namespace witin
