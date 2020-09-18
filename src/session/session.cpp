/*************************************************************************
	> File Name: session.cpp
	> Author: afly
	> Mail: aifei.zhang@witintech.com
	> Created Time: Wed Aug 26 19:31:55 2020
 ************************************************************************/
#include <fstream>
#include <witin/session/session.h>
#include <witin/graph/graph.h>
#include <witin/global.h>
#include <witin/target/core.h>
#include <witin/target/mem.h>
#include <witin/op/nn.h>
#include <witin/utils/debug.h>

using namespace witin::base;

typedef std::shared_ptr<witin::base::OpNode> baseOpNodePtr;
typedef std::shared_ptr<witin::base::mvOpNode> mvOpNodePtr;


class CaculateArryMem caculateArryMem;
class DACFifoMem dacFifoMem;
class BiasRegionMem biasRegionMem;
class RegFileMem regFileMem;

namespace witin{
namespace base{

	/*
	 * write board config to json
	 */
	int writeToJson(vector<ROUND_CONFIG> rc, string params_path
										   , string json_path)
	{
		DLOG(INFO)<<"JSON CPP VERSION:"<<JSONCPP_VERSION_STRING;
		Json::Value root;
		root["params_path"] = params_path;
		root["round_total"] = (int)rc.size();

		DLOG(INFO)<<"round total:"<<rc.size();
		for(auto i = 0; i < rc.size() ;i++)
		{
			Json::Value round_cfg;
			round_cfg["cnt"] = rc[i].rd_control_enable.cnt;
			round_cfg["weight_en"] = rc[i].rd_control_enable.weight_en;
			round_cfg["bias_en"] = rc[i].rd_control_enable.bias_en;
			round_cfg["shift_en"] = rc[i].rd_control_enable.shift_en;
			round_cfg["add_en"] = rc[i].rd_control_enable.add_en;
			round_cfg["actv_en"] = rc[i].rd_control_enable.actv_en;
			round_cfg["max_pooling_en"] = rc[i].rd_control_enable.max_pooling_en;
			round_cfg["reactive_en"] = rc[i].rd_control_enable.reactive_en;
			round_cfg["mult_en"] = rc[i].rd_control_enable.mult_en;
			round_cfg["readdr_en"] = rc[i].rd_control_enable.readdr_en;
			round_cfg["TDNN_en"] = rc[i].rd_control_enable.TDNN_en;
			round_cfg["fifo_grp0_en"] = rc[i].rd_control_enable.fifo_grp0_en;
			round_cfg["fifo_grp1_en"] = rc[i].rd_control_enable.fifo_grp1_en;
			round_cfg["fifo_grp2_en"] = rc[i].rd_control_enable.fifo_grp2_en;
			round_cfg["round_pause"] =  rc[i].rd_control_enable.round_pause;

			Json::Value weight;
			weight["w_win_column_s"] = rc[i].array_grp_config.w_win_column_s;
			weight["w_win_column_e"] = rc[i].array_grp_config.w_win_column_e;
			weight["w_win_column_len"] = rc[i].array_grp_config.w_win_column_len;
			weight["w_win_row_s"] = rc[i].array_grp_config.w_win_row_s;
			weight["w_win_row_e"] = rc[i].array_grp_config.w_win_row_e;
			weight["w_win_row_len"] = rc[i].array_grp_config.w_win_row_len;
			weight["reg_in_s"] = rc[i].array_grp_config.regfile_addr_start;
			weight["reg_in_e"] = rc[i].array_grp_config.regfile_addr_start  +
										rc[i].array_grp_config.regfile_addr_len;
			weight["reg_out_s"] = rc[i].array_grp_config.store_addr;
			weight["reg_out_e"] = rc[i].array_grp_config.store_addr +
										rc[i].array_grp_config.store_len;

			Json::Value weight_params;
			weight_params["start"] = rc[i].array_grp_config.w_prams.start;
			weight_params["end"] = rc[i].array_grp_config.w_prams.end;
			weight_params["size"] = rc[i].array_grp_config.w_prams.size;
			weight["weight_params"] = Json::Value(weight_params);

			Json::Value actv;
			actv["type"] = rc[i].actv_grp_config.actv_type;
			actv["limit"] = rc[i].actv_grp_config.limit;

			if(rc[i].rd_control_enable.weight_en)
			{
				round_cfg["weight"] = Json::Value(weight);
			}

			if(rc[i].rd_control_enable.actv_en)
			{
				round_cfg["actv"] = Json::Value(actv);
			}

			root["roundConfig"].append(Json::Value(round_cfg));
		}

		DLOG(INFO)<<"FastWriter:";
		Json::FastWriter fw;
		//Json::StreamWriterBuilder sw;
		//DLOG(INFO)<<fw.write(root);

		DLOG(INFO)<<"StyledWriter:";
		Json::StyledWriter sw;
		//Json::StreamWriterBuilder sw;
		//DLOG(INFO)<<sw.write(root);
		DLOG(INFO)<<"json path:"<<json_path;
		//write to file
		ofstream os;
		os.open(json_path);
		if(!os){
			DLOG(INFO)<<"could not open json_path";
		}
		os << sw.write(root);
		os.close();
		//DLOG(INFO)<<root.toStyledString();
		return 0;
	}

	/*
	 * write data to file
	 */
	int writeDataToFile(char* file_path, int start, int len, char *data)
	{
		FILE*stream = fopen(file_path, "w");
		fseek(stream, start, 0);
		fwrite(data, len, 1, stream);
		fclose(stream);
		return 0;
	}

	/*
	 * read data from file
	 */
	int readDataFromFile(char* file_path, int start, int len, char *data)
	{
		FILE*stream = fopen(file_path, "r");
		fseek(stream, start, 0);
		fread(data, len, 1, stream);
		fclose(stream);
		return 0;
	}

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
		Json::Value root;
		string data = "./params.dat";
		FILE*stream = fopen("./params.dat", "w");
		int file_offset = 0;

		DLOG(INFO)<<"*******************Session build******************* ";
		auto in_nodes = InGraph.inNodes();
		auto out_nodes = InGraph.outNodes();
		Core core;
		DLOG(INFO)<<"in_nodes.size(): "<<in_nodes.size();
		vector<baseOpNodePtr> op_list = InGraph.graph_topological_sort();

		int roundTotal = (int)op_list.size();
		core.setRoundTotal(roundTotal);
		//core.dump();
		std::vector<ROUND_CONFIG> rounds;

		map<Tensor*, struct mem_record> tensor_mem_record_map;

		/*
		 * create all the tensors
		 *
		 */
		DLOG(INFO)<<"===================================";
		DLOG(INFO)<<"Create input and output tensors ...";
		for(size_t i = 0; i < op_list.size(); i++)
		{
			//tensor
			if(isInputNode(op_list[i], in_nodes))
			{
				DLOG(INFO)<<"This is input node:";
				baseOpNodePtr node = op_list[i];
				vector<int> input_shape = node->getInputShape();
				vector<Tensor*> input_tensors;

				if(input_shape.size() == 0)
				{
					LOG(FATAL)<<"input_shape size is zero !";
				}

				//Tensor in(input_shape, PLACEHOLDER_TYPE);
				Tensor* input_tensor_ptr = new Tensor(input_shape, PLACEHOLDER_TYPE);
				//input_tensor_ptr = &in;
				input_tensors.push_back(input_tensor_ptr);

				DLOG(INFO)<<": tensor_type = "<<input_tensor_ptr->tensor_type;
				if(node->isUseConstTensor())
				{
					Tensor* const_tensor;
					node->getConstTensor(&const_tensor);
					input_tensors.push_back(const_tensor);
				}

				DLOG(INFO)<<": input_tensors size = "<<input_tensors.size();
				//set node input tensor
				node->set_input_tensors(input_tensors);

				//infer_shape: get node output shape
				vector<int> out_shape = node->infer_shape();

				//Tensor out(out_shape, PLACEHOLDER_TYPE);
				//Tensor* output_tensor_ptr = (Tensor *)malloc(sizeof(Tensor));
				Tensor* output_tensor_ptr = new Tensor(out_shape, PLACEHOLDER_TYPE);
				//output_tensor_ptr->setShape(out_shape);
				//output_tensor_ptr->tensor_type = PLACEHOLDER_TYPE;


				vector<Tensor*> output_tensors;
				output_tensors.push_back(output_tensor_ptr);

				//set node output tensor
				node->set_output_tensors(output_tensors);
				vector<Tensor*>tmp_tensors;
				node->get_output_tensors(tmp_tensors);

				for(auto kk : tmp_tensors)
				{
					DLOG(INFO)<<": "<<node->getName();
					DLOG(INFO)<<": output_tensor";
					kk->print();
				}
			}
			else /* normal node */
			{
				DLOG(INFO)<<"This is normal node";
				vector<Tensor*> input_tensors;
				baseOpNodePtr node = op_list[i];

				vector<baseOpNodePtr> innodes = InGraph.innodes_of_node(op_list[i]);
				//innodes size now is 1
				//TODO:tensor_index of the front node
				for(size_t j = 0; j < innodes.size(); j++)
				{
					//innodes -> baseNodePtr -> get_output_tensors;
					//
					//in this cases, this node input_tensors just pointer to
					//the output tensors of the input node, so we can allocate
					//the same memory for the memory of output tensors and the
					//memory of input tensor.
					vector<Tensor*> previous_output_tensors;
					innodes[j]->get_output_tensors(previous_output_tensors);

					DLOG(INFO)<<innodes[j]->getName();
					for(auto kv : previous_output_tensors)
					{
						DLOG(INFO)<<" : input_tensor";
						kv->print();
						input_tensors.push_back(kv);
					}
				}

				//const tensor
				if(node->isUseConstTensor())
				{
					Tensor* const_tensor;
					node->getConstTensor(&const_tensor);
					input_tensors.push_back(const_tensor);
				}
				DLOG(INFO)<<"input_tensors.size : "<<input_tensors.size();
				//set node input tensor
				node->set_input_tensors(input_tensors);

				//infer_shape: get node output shape
				vector<int> out_shape = node->infer_shape();
				vector<Tensor*> output_tensors;

				//Tensor out(out_shape, PLACEHOLDER_TYPE);
				Tensor* output_tensor_ptr = new Tensor(out_shape, PLACEHOLDER_TYPE);
				output_tensors.push_back(output_tensor_ptr);
				DLOG(INFO)<<"out_tensors.size : "<<output_tensors.size();
				//set node output tensor
				node->set_output_tensors(output_tensors);
			}
		}

		//const Tensor: array
		//input / output : regfile

		DLOG(INFO)<<"===============================alloc===============================";
		DLOG(INFO)<<"Alloc memory and generate config for every round ...";
		for(size_t i = 0; i < op_list.size(); i++)
		{
			if(isInputNode(op_list[i], in_nodes))
			{
				DLOG(INFO)<<"input node:";
				ROUND_CONFIG round_cfg_in;
				RD_CONTROL_ENABLE rce;
				rce.cnt = i;
				rce.weight_en = true;
				ARRAY_GRP_CONFIG arry_grp_cfg;
				ACTV_GRP_CONFIG actv_grp_cfg;
				WEIGHT_PARAMS weight_params;

				baseOpNodePtr node = op_list[i];
				vector<Tensor*> tensors;

				if(node->getID() == MV_OPNODE_ID)
				{
					DLOG(INFO)<<"This dense node!!";
					mvOpNodePtr mv_ptr = std::dynamic_pointer_cast<mvOpNode>(node);
					if(mv_ptr == NULL)
					{
						LOG(FATAL)<<"dynamic_pointer_cast is error baseOpNodePtr ==>> mvOpNodePtr";
					}

					if(mv_ptr->getActEn())
					{
						rce.actv_en = true;
						actv_grp_cfg.actv_type = mv_ptr->getActType();
						actv_grp_cfg.limit= 127;
					}
					else
					{
						rce.actv_en = false;
					}
				}

				//getShape
				vector<int> input_shape = node->getInputShape();
				if(input_shape.size() != 2)
				{
					LOG(FATAL)<<"input_shape size should be 2";
				}

				vector<Tensor*> input_tensors;
				node->get_input_tensors(input_tensors);

				DLOG(INFO)<<": INPUTNODE input_tensors size = "<<input_tensors.size();
				for(int j = 0; j < input_tensors.size();j++)
				{
					if(input_tensors[j]->tensor_type == PLACEHOLDER_TYPE)
					{
						//1.allocate mem for input if not none : regfile mem
						int availeMem = regFileMem.getAvailableMem();
						if(availeMem > 0)
						{
							int start_alloc_addr = regFileMem.getGeneralUsedSize();
							DLOG(INFO)<<": start_alloc_addr "<<start_alloc_addr;
							DLOG(INFO)<<": availeMem "<<availeMem;

							if(availeMem > input_shape[1])
							{
								regFileMem.allocMemAddr(start_alloc_addr, input_shape[1]);
							}
							else
							{
								LOG(FATAL)<<"regFileMem is not enough for input shape !";
							}

							struct mem_record mr;
							mr.mem_type = REGFILE_MEM_TYPE;
							mr.start = start_alloc_addr;
							mr.len = input_shape[1];

							arry_grp_cfg.regfile_addr_start = start_alloc_addr;
							arry_grp_cfg.regfile_addr_len = input_shape[1];

							tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
															(input_tensors[j], mr));

						}
						else
						{
							LOG(FATAL)<<"No regFileMem is available !";
						}
					}
					//2.allocate mem for const Tensor if not none, : array mem
					//  then print to file
					if(input_tensors[j]->tensor_type == CONST_TYPE)
					{
						int availeColumnMem = caculateArryMem.getColumnAvailableMem();
						int column_size = 0;
						int start_alloc_column_addr = 0;
						if(availeColumnMem > 0)
						{
							column_size =  input_tensors[j]->getShape()[1];
							start_alloc_column_addr = caculateArryMem.getArryColumnUsedSize();

							if(availeColumnMem > column_size)
							{
								caculateArryMem.allocColumnMem(start_alloc_column_addr, column_size);
							}
							else
							{
								LOG(FATAL)<<"caculateArryMem is not enough for COLUMN allocate!";
							}
						}
						else
						{
							LOG(FATAL)<<"No caculateColumnArryMem is available !";
						}

						int availeRowMem = caculateArryMem.getRowAvailableMem();
						int row_size =  input_tensors[j]->getShape()[0];
						int start_alloc_row_addr = caculateArryMem.getArryRowUsedSize();
						if(availeRowMem > 0)
						{

							if(availeRowMem > row_size)
							{
								caculateArryMem.allocRowMem(start_alloc_row_addr, row_size);
							}
							else
							{
								LOG(FATAL)<<"caculateArryMem is not enough for ROW allocate!";
							}
						}
						else
						{
							LOG(FATAL)<<"No caculateRowArryMem is available !";
						}

						struct mem_record mr;

						mr.mem_type = ARRAY_MEM_TYPE;
						mr.column_start = start_alloc_column_addr;
						mr.column_len = column_size;
						mr.row_start = start_alloc_row_addr;
						mr.row_len = row_size;
						tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
														(input_tensors[j], mr));
						arry_grp_cfg.w_win_column_s = start_alloc_column_addr;
						arry_grp_cfg.w_win_column_e = start_alloc_column_addr + column_size;
						arry_grp_cfg.w_win_column_len = column_size;
						arry_grp_cfg.w_win_row_s = start_alloc_row_addr;
						arry_grp_cfg.w_win_row_e = start_alloc_row_addr + row_size;
						arry_grp_cfg.w_win_row_len = row_size;

						//manager file params.dat
						DLOG(INFO)<<": print weight to params.dat ";
						DLOG(INFO)<<"	row_size:"<<row_size;
						DLOG(INFO)<<"	column_size:"<<column_size;

						for(int fp = 0; fp < row_size ;fp++)
						{
							for(int fm = 0; fm < column_size ;fm++)
							{
								char fdata = (char)((char*)input_tensors[j]->getData())[fm + fp * column_size];
								//DLOG(INFO)<<" data : "<<fm+fp*column_size<<" f:"<<fdata<<"fend  int:"<<(int)fdata;
								//fwrite(&fdata, sizeof(char), 1, stream);
								fprintf(stream, "%c", fdata);
							}
						}

						weight_params.start = file_offset;
						weight_params.end = file_offset + column_size * row_size - 1;
						weight_params.size = column_size * row_size;

						//file ptr ahead column_size * row_size
						file_offset += column_size * row_size;

						arry_grp_cfg.w_prams = weight_params;
					}
				}

				//3.allocate output tensor memory : regfile mem
				vector<Tensor*> output_tensors;
				node->get_output_tensors(output_tensors);

				DLOG(INFO)<<": INPUTNODE out_tensors size = "<<output_tensors.size();
				for(int k = 0; k < output_tensors.size();k++)
				{
					if(output_tensors[k]->tensor_type == PLACEHOLDER_TYPE)
					{
						vector<int> tensor_shape = output_tensors[k]->getShape();

						//allocate mem for output if not none : regfile mem
						//assume  out_tensor: 2-dim  1 x n  ,get n in tensor_shape[1]
						int availeMem = regFileMem.getAvailableMem();
						if( availeMem > 0)
						{
							int start_alloc_addr = regFileMem.getGeneralUsedSize();
							if(availeMem > tensor_shape[1])
							{
								regFileMem.allocMemAddr(start_alloc_addr, tensor_shape[1]);
							}
							else
							{
								LOG(FATAL)<<"regFileMem is not enough for input shape !";
							}

							struct mem_record mr;
							mr.mem_type = REGFILE_MEM_TYPE;
							mr.start = start_alloc_addr;
							mr.len = tensor_shape[1];

							arry_grp_cfg.store_addr = start_alloc_addr;
							arry_grp_cfg.store_len = tensor_shape[1];

							tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
															(output_tensors[k], mr));
						}
						else
						{
							LOG(FATAL)<<"No regFileMem is available !";
						}
					}
				}
				dump_rd_ctrl_enable(rce);
				dump_arry_grp_cfg(arry_grp_cfg);
				round_cfg_in.rd_control_enable = rce;
				round_cfg_in.array_grp_config = arry_grp_cfg;
				round_cfg_in.actv_grp_config = actv_grp_cfg;

				rounds.push_back(round_cfg_in);
			}
			//else if(isOutputNode(op_list[i], out_nodes))
			//{
			//	ROUND_CONFIG round_cfg_out;
			//	baseOpNodePtr node = op_list[i];
			//	//getShape
			//	vector<int> input_shape = node->getInputShape();
			//	if(input_shape.size() == 0)
			//	{
			//		DLOG(INFO)<<"input_shape size is zero";
			//		exit(1);
			//	}

			//	//1.inputnode share same memory
			//	//2.getConst alloc mem for const and print to file
			//	//3.allocate output tensor memory
			//	rounds.push_back(round_cfg_out);
			//	DLOG(INFO)<<"out node:";
			//}
			else  /*normal node*/
			{
				RD_CONTROL_ENABLE rce;
				rce.cnt = i;
				rce.weight_en = true;

				ROUND_CONFIG round_cfg_norm;
				ARRAY_GRP_CONFIG arry_grp_cfg;
				WEIGHT_PARAMS weight_params;
				ACTV_GRP_CONFIG actv_grp_cfg;

				baseOpNodePtr node = op_list[i];
				//getShape
				vector<int> input_shape = node->getInputShape();
				if(input_shape.size() == 0)
				{
					LOG(FATAL)<<"input_shape size is zero !";
				}

				if(node->getID() == MV_OPNODE_ID)
				{
					DLOG(INFO)<<"This dense node!!";
					mvOpNodePtr mv_ptr = std::dynamic_pointer_cast<mvOpNode>(node);
					if(mv_ptr == NULL)
					{
						LOG(FATAL)<<"dynamic_pointer_cast is error baseOpNodePtr ==>> mvOpNodePtr";
					}

					if(mv_ptr->getActEn())
					{
						rce.actv_en = true;
						actv_grp_cfg.actv_type = mv_ptr->getActType();
						actv_grp_cfg.limit= 127;
					}
					else
					{
						rce.actv_en = false;
					}
				}

				vector<Tensor*> input_tensors;
				node->get_input_tensors(input_tensors);
				//1.inputnode share same memory

				//tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
				//											(input_tensors[j], mr));

				DLOG(INFO)<<": NORM input_tensors size = "<<input_tensors.size();
				for(size_t n = 0; n < input_tensors.size();n++ )
				{

					auto iter = tensor_mem_record_map.find(input_tensors[n]);

					//input tensor and not const_type
					//if found then update input addr of this round : regfile mem
					if(iter != tensor_mem_record_map.end())
					{
						DLOG(INFO)<<"find tensor in tensor_mem_record_map";

						struct mem_record mr = iter->second;
						arry_grp_cfg.regfile_addr_start = mr.start;
						arry_grp_cfg.regfile_addr_len = mr.len;
					}
					else
					{
						DLOG(INFO)<<"could not find tensor in tensor_mem_record_map, maybe const tensor";
					}

					//  allocate mem for const Tensor if not none, : array mem
					//  then print to file
					if(input_tensors[n]->tensor_type == CONST_TYPE)
					{
						int availeColumnMem = caculateArryMem.getColumnAvailableMem();
						int start_alloc_column_addr = 0;
						int column_size = 0;

						if(availeColumnMem > 0)
						{
							column_size =  input_tensors[n]->getShape()[1];
							start_alloc_column_addr = caculateArryMem.getArryColumnUsedSize();

							if(availeColumnMem > column_size)
							{
								caculateArryMem.allocColumnMem(start_alloc_column_addr, column_size);
							}
							else
							{
								LOG(FATAL)<<"caculateArryMem is not enough for column allocate!";
							}
						}
						else
						{
							LOG(FATAL)<<"No caculateColumnArryMem is available !";
						}

						int row_size =  input_tensors[n]->getShape()[0];
						int start_alloc_row_addr = caculateArryMem.getArryRowUsedSize();

						int availeRowMem = caculateArryMem.getRowAvailableMem();
						if(availeRowMem > row_size)
						{
							caculateArryMem.allocRowMem(start_alloc_row_addr, row_size);
						}
						else
						{
							LOG(FATAL)<<"caculateArryMem is not enough for Row allocate!";
						}

						struct mem_record mr;

						mr.mem_type = ARRAY_MEM_TYPE;
						mr.column_start = start_alloc_column_addr;
						mr.column_len = column_size;
						mr.row_start = start_alloc_row_addr;
						mr.row_len = row_size;

						arry_grp_cfg.w_win_column_s = start_alloc_column_addr;
						arry_grp_cfg.w_win_column_e = start_alloc_column_addr + column_size;
						arry_grp_cfg.w_win_column_len = column_size;
						arry_grp_cfg.w_win_row_s = start_alloc_row_addr;
						arry_grp_cfg.w_win_row_e = start_alloc_row_addr + row_size;
						arry_grp_cfg.w_win_row_len = row_size;

						//manager file params.dat
						for(int fp = 0; fp < row_size ;fp++)
						{
							for(int fm = 0; fm < column_size; fm++)
							{
								char fdata = ((char*)input_tensors[n]->getData())[fm + fp * column_size];
								//DLOG(INFO)<<" norm data : "<<fm+fp*column_size<<" f:"<<fdata<<"fend  int:"<<(int)fdata;
								//fwrite(&fdata, sizeof(char), 1, stream);
								fprintf(stream, "%c", fdata);
							}
						}
						//row
						weight_params.start = file_offset;
						weight_params.end = file_offset + column_size * row_size - 1;
						weight_params.size = column_size * row_size;

						//file ptr ahead column_size * row_size
						file_offset += column_size * row_size;

						arry_grp_cfg.w_prams = weight_params;
					}
				}

				//allocate output tensor memory : regfile mem
				vector<Tensor*> norm_output_tensors;
				node->get_output_tensors(norm_output_tensors);

				DLOG(INFO)<<": NORM output_tensors size = "<<norm_output_tensors.size();
				for(int m = 0; m < norm_output_tensors.size();m++)
				{
					if(norm_output_tensors[m]->tensor_type == PLACEHOLDER_TYPE)
					{
						vector<int> tensor_shape = norm_output_tensors[m]->getShape();

						//allocate mem for output if not none : regfile mem
						//assume  out_tensor: 2-dim  1 x n  ,get n in tensor_shape[1]
						int normAvaileMem = regFileMem.getAvailableMem();
						if(normAvaileMem > 0)
						{
							int start_alloc_addr = regFileMem.getGeneralUsedSize();
							if(normAvaileMem > tensor_shape[1])
							{
								regFileMem.allocMemAddr(start_alloc_addr, tensor_shape[1]);
							}
							else
							{
								LOG(FATAL)<< "norm regFileMem is not enough for input shape !";
							}

							struct mem_record mr;
							mr.mem_type = REGFILE_MEM_TYPE;
							mr.start = start_alloc_addr;
							mr.len = tensor_shape[1];

							arry_grp_cfg.store_addr = start_alloc_addr;
							arry_grp_cfg.store_len = tensor_shape[1];

							tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
															(norm_output_tensors[m], mr));
						}
						else
						{
							LOG(FATAL)<<"No regFileMem is available !";
						}
					}
				}

				if(isOutputNode(op_list[i], out_nodes))
				{
					DLOG(INFO)<<"This is output node !!";
				}

				dump_rd_ctrl_enable(rce);
				dump_arry_grp_cfg(arry_grp_cfg);
				round_cfg_norm.rd_control_enable = rce;
				round_cfg_norm.array_grp_config = arry_grp_cfg;
				round_cfg_norm.actv_grp_config = actv_grp_cfg;

				rounds.push_back(round_cfg_norm);
			}
		}

		fclose(stream);
		writeToJson(rounds, data, "./BoardConfig.json");
		DLOG(INFO)<<"Generate BoardConfig end";
	}

	int32_t Session::run(WitinGraphType &InGraph, std::vector<Tensor*> inputs)
	{

	}

} //namespace base
} //namespace witin

