/*************************************************************************
	> File Name: session.cpp
	> Author: afly
	> Mail: aifei.zhang@witintech.com
	> Created Time: Wed Aug 26 19:31:55 2020
 ************************************************************************/
#include <fstream>
#include <ctime>
#include <witin/session/session.h>
#include <witin/graph/graph.h>
#include <witin/global.h>
#include <witin/target/core.h>
#include <witin/target/mem.h>
#include <witin/op/nn.h>
//#include <witin/op/active.h>
#include "../../include/witin/op/active.h"
#include <witin/op/math.h>
#include <witin/utils/debug.h>
#include <witin/target/util.h>

using namespace witin::base;

//class tmpNode no;
typedef std::shared_ptr<witin::base::OpNode> baseOpNodePtr;
typedef std::shared_ptr<witin::base::mvOpNode> mvOpNodePtr;
typedef std::shared_ptr<witin::base::LogOpNode> logNodePtr;
typedef std::shared_ptr<witin::base::TanhOpNode> tanhOpNodePtr;
typedef std::shared_ptr<witin::base::ActOpNode> actOpNodePtr;

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
		root["params_path"] = "./params.dat";
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
			round_cfg["gain"] =  512;

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

			Json::Value bias;
			bias["bias_column_s"] = rc[i].bias_config.bias_column_s;
			bias["bias_column_e"] = rc[i].bias_config.bias_column_e;
			bias["bias_column_len"] = rc[i].bias_config.bias_column_len;
			bias["bias_row_s"] = rc[i].bias_config.bias_row_s;
			bias["bias_row_e"] = rc[i].bias_config.bias_row_e;
			bias["bias_row_len"] = rc[i].bias_config.bias_row_len;
			Json::Value bias_params;
			bias_params["start"] = rc[i].bias_config.b_prams.start;
			bias_params["end"] = rc[i].bias_config.b_prams.end;
			bias_params["size"] = rc[i].bias_config.b_prams.size;
			bias["bias_params"] = Json::Value(bias_params);

			if(rc[i].rd_control_enable.weight_en)
			{
				round_cfg["weight"] = Json::Value(weight);
			}

			if(rc[i].rd_control_enable.actv_en)
			{
				round_cfg["actv"] = Json::Value(actv);
			}

			if(rc[i].rd_control_enable.bias_en)
			{
				round_cfg["bias"] = Json::Value(bias);
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
	/*
	 * init mem from a file
	 * used to multi net init
	 */
	int init_mem(void)
	{
		int generalUsedSize = 0;
		int arrayColumnUsedSize = 0;
		int arrayRowUsedSize = 0;

		FILE*data = fopen("./build/memInitFile.data", "r");
		if(!data)
		{
			LOG(FATAL)<<"Open memInitFile.data error!";
		}

		fscanf(data, "%d", &generalUsedSize);
		fscanf(data, "%d", &arrayColumnUsedSize);
		fscanf(data, "%d", &arrayRowUsedSize);

		regFileMem.setGeneralUsedSize(generalUsedSize);
		caculateArryMem.setArrayColumnUsedSize(arrayColumnUsedSize);
		caculateArryMem.setArrayRowUsedSize(arrayRowUsedSize);

		fclose(data);
		return 0;
	}

	/*
	 * generate  array map
	 */

	// int generateArrayMap(const char* json_path, const char* params_path)
	// {
	// 	PyObject *pName, *pModule, *pDict, *pFunc;
	// 	PyObject *pArgs, *pValue;

	// 	Py_Initialize();
	// 	DLOG(INFO)<<"debug point";
	// 	DLOG(INFO)<<"debug point";
	// 	PyRun_SimpleString("print('hello world')");
	// 	PyRun_SimpleString("import sys");
	// 	DLOG(INFO)<<"debug point";

	// 	// PyRun_SimpleString("sys.path.append('./')");
	// 	char const* pyScriptName = "get_array_map";
	// 	char const* arrayJson = "./array.json";
	// 	char const* mapTxt = "./map.txt";
	// 	char const* logFile = "./log.txt";
	// 	DLOG(INFO)<<"debug point";

	// 	pName = PyUnicode_DecodeFSDefault(pyScriptName);

	// 	pModule = PyImport_Import(pName);
	// 	Py_DECREF(pName);
	// 	DLOG(INFO)<<"debug point";

	// 	if(pModule != NULL)
    //     {
    //         pFunc = PyObject_GetAttrString(pModule, "get_array_map");
    //         /* pFunc is a new reference */

    //         if (pFunc && PyCallable_Check(pFunc))
    //         {
    //             pArgs = PyTuple_New(4);
	// 			DLOG(INFO)<<"debug point";

	// 			PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", arrayJson));
	// 			PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", json_path));
	// 			PyTuple_SetItem(pArgs, 2, Py_BuildValue("s", mapTxt));
	// 			PyTuple_SetItem(pArgs, 3, Py_BuildValue("s", logFile));
    //             pValue = PyObject_CallObject(pFunc, pArgs);
    //             Py_DECREF(pArgs);
    //             if (pValue != NULL)
    //             {
	// 				DLOG(INFO)<<"Result of call: "<<PyLong_AsLong(pValue);
    //                 Py_DECREF(pValue);
    //             }
    //             else
    //             {
    //                 Py_DECREF(pFunc);
    //                 Py_DECREF(pModule);
    //                 PyErr_Print();
	// 				LOG(FATAL)<<"Call failed";
    //                 return 1;
    //             }
    //         }
    //         else
    //         {
    //             if (PyErr_Occurred())
    //                 PyErr_Print();
	// 			LOG(FATAL)<<"Cannot find function";
    //         }
    //         Py_XDECREF(pFunc);
    //         Py_DECREF(pModule);
    //     }
    //     else
    //     {
    //         PyErr_Print();
	// 		LOG(FATAL)<<"Failed to load ";
    //         return 1;
    //     }
	// 	return 0;
	// }

	// /*
	//  * generate chip register config
	//  */
	// int generateRegConfig(const char* json_path, const char* params_path)
	// {
	// 	// PyObject *pName, *pModule, *pDict, *pFunc;
	// 	// PyObject *pArgs, *pValue;


	// 	// Py_Initialize();

	// 	// PyRun_SimpleString("import sys");
	// 	// PyRun_SimpleString("sys.path.append('./')");

	// 	// // pName = PyUnicode_DecodeFSDefault("get_array_map");
	// 	// pName = PyUnicode_FromString("get_array_map");

	// 	// pModule = PyImport_Import(pName);
	// 	return 0;
	// }


	int32_t Session::build(WitinGraphType &InGraph, vector<vector<int> > shapes)
	{
		Json::Value root;
		string params_path = "./build/params.dat";
		FILE*stream = fopen("./build/params.dat", "w");
		//weight and bias params offset
		int file_offset = 0;

		DLOG(INFO)<<"**************Session build************** ";
		auto in_nodes = InGraph.inNodes();
		auto out_nodes = InGraph.outNodes();
		Core core;
		DLOG(INFO)<<"graph in_nodes.size(): "<<in_nodes.size();
		vector<baseOpNodePtr> op_list = InGraph.graph_topological_sort();

		int roundTotal = (int)op_list.size();
		core.setRoundTotal(roundTotal);
		std::vector<ROUND_CONFIG> rounds;
		map<Tensor*, struct mem_record> tensor_mem_record_map;

		int multi_net  = 0;
		if(1 == multi_net)
		{
			init_mem();
		}

		/*
		 * create all the tensors
		 *
		 */
		DLOG(INFO)<<"=============Generate Tensors================";
		DLOG(INFO)<<"Create input and output tensors ...";
		for(size_t i = 0; i < op_list.size(); i++)
		{
			//tensor
			if(isInputNode(op_list[i], in_nodes))
			{
				DLOG(INFO)<<"This is input node: "<<op_list[i]->getName();
				baseOpNodePtr node = op_list[i];
				vector<vector<int> > input_shape = node->getInputShape();
				vector<Tensor*> input_tensors;

				if(input_shape[0].size() == 0)
				{
					LOG(FATAL)<<"input_shape size should not be zero !";
				}

				for(int p = 0; p <input_shape.size(); p++)
				{
					//delete input_tensor_ptr in deconstructor of opNode
					Tensor* input_tensor_ptr = new Tensor(input_shape[p], PLACEHOLDER_TYPE);
					input_tensors.push_back(input_tensor_ptr);
				}

				if(node->isUseConstTensor())
				{
					Tensor* const_tensor;
					node->getConstTensor(&const_tensor);
					input_tensors.push_back(const_tensor);
				}

				//set node input tensor, include const tensor
				node->set_input_tensors(input_tensors);

				//infer_shape: get node output shape
				vector<vector<int> > out_shape = node->infer_shape();
				Tensor* output_tensor_ptr = new Tensor(out_shape[0], PLACEHOLDER_TYPE);
				vector<Tensor*> output_tensors;
				output_tensors.push_back(output_tensor_ptr);

				//set node output tensor
				node->set_output_tensors(output_tensors);

#ifdef WITIN_DEBUG
				vector<Tensor*>tmp_tensors;
				node->get_output_tensors(tmp_tensors);

				for(auto kk : tmp_tensors)
				{
					DLOG(INFO)<<": "<<node->getName();
					DLOG(INFO)<<"output_tensor :";
					kk->print();
				}
#endif
			}
			else /* normal node */
			{
				DLOG(INFO)<<"This is normal node : "<<op_list[i]->getName();
				vector<Tensor*> input_tensors;
				baseOpNodePtr node = op_list[i];

				vector<baseOpNodePtr> innodes = InGraph.innodes_of_node(op_list[i]);
				DLOG(INFO)<<"Input node total:"<<innodes.size();
				//innodes size now is 1
				//TODO:tensor_index of the front node
				for(size_t j = 0; j < innodes.size(); j++)
				{
					//innodes -> baseNodePtr -> get_output_tensors;
					//
					//in this cases, this node input_tensors just pointer to
					//the output tensors of the input node, so we can allocate
					//the same memory for the memory of output tensors and the
					//memory of input tensors.
					vector<Tensor*> previous_output_tensors;
					innodes[j]->get_output_tensors(previous_output_tensors);

					DLOG(INFO)<<"inputnode :"<<j<<" "<<innodes[j]->getName();
					for(auto kv : previous_output_tensors)
					{
						//DLOG(INFO)<<" input_tensor";
						//kv->print();
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
				//set node input tensor
				node->set_input_tensors(input_tensors);

				//infer_shape: get node output shape
				vector<vector<int> > out_shape = node->infer_shape();
				vector<Tensor*> output_tensors;

				Tensor* output_tensor_ptr = new Tensor(out_shape[0], PLACEHOLDER_TYPE);
				output_tensors.push_back(output_tensor_ptr);
				//set node output tensor
				node->set_output_tensors(output_tensors);
			}
		}

		//const Tensor: array
		//input / output : regfile

		DLOG(INFO)<<"=====================alloc=====================";
		DLOG(INFO)<<"Alloc memory and generate config for every round ...";
		for(size_t i = 0; i < op_list.size(); i++)
		{
			if(isInputNode(op_list[i], in_nodes))
			{
				DLOG(INFO)<<"[INPUT] node:"<<op_list[i]->getName();

				RD_CONTROL_ENABLE rce;
				rce.cnt = i;
				ROUND_CONFIG round_cfg_in;
				ARRAY_GRP_CONFIG arry_grp_cfg;
				WEIGHT_PARAMS weight_params;
				BIAS_CONFIG bias_cfg;
				ACTV_GRP_CONFIG actv_grp_cfg;
				READDER_CONFIG readder_cfg;
				REACTV_GRP_CONFIG reactv_grp_cfg;
				MUL_GRP_CONFIG mul_grp_cfg;
				BIAS_PARAMS bias_params;

				baseOpNodePtr node = op_list[i];
				vector<Tensor*> tensors;

				int node_id = node->getID();
				if(node_id == MV_OPNODE_ID)
				{
					rce.weight_en = true;
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

					if(mv_ptr->getBiasEn())
						rce.bias_en = true;
					else
						rce.bias_en = false;

				}
				else if(node_id == POOL_OPNODE_ID){}
				else if(node_id == TANH_OPNODE_ID){}
				else if(node_id == SIGMOID_OPNODE_ID){}
				else if(node_id == RELU_OPNODE_ID){}
				else if(node_id == LOG_OPNODE_ID){}
				else if(node_id == ABS_OPNODE_ID){}
				else if(node_id == MAX_OPNODE_ID){}
				else if(node_id == MIN_OPNODE_ID){}
				else if(node_id == INVERT_OPNODE_ID){}
				else if(node_id == SHIFT_OPNODE_ID){}
				else if(node_id == ADD_OPNODE_ID)
				{
					rce.readdr_en=true;
				}
				else if(node->getID() == ACT_OPNODE_ID)
				{
					rce.reactive_en=true;
					actOpNodePtr act_ptr = std::dynamic_pointer_cast<ActOpNode>(node);
					reactv_grp_cfg.reactv_sel = act_ptr->act_type;
				}
				else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
				{
					rce.mult_en = true;
				}
				else
				{
					LOG(FATAL)<<"Do not support opnode ID :"<<node_id;
				}
				DLOG(INFO)<<"weight_en:"<<rce.weight_en;
				//getShape
				vector<vector<int> > input_shape = node->getInputShape();
				vector<Tensor*> input_tensors;
				node->get_input_tensors(input_tensors);

				DLOG(INFO)<<"[INPUTNODE] input_tensors total : "<<input_tensors.size();
				for(int j = 0; j < input_tensors.size();j++)
				{
					if(input_tensors[j]->tensor_type == PLACEHOLDER_TYPE)
					{
						DLOG(INFO)<<"		No."<<j<<" PLACEHOLDER_TYPE";
						//1.allocate mem for input if not none : regfile mem
						int availeMem = regFileMem.getAvailableMem();
						if(availeMem > 0)
						{
							int start_alloc_addr = regFileMem.getGeneralUsedSize();
							DLOG(INFO)<<"regfileMem: start_alloc_addr "<<start_alloc_addr;
							DLOG(INFO)<<"regfileMem:        availeMem "<<availeMem;

							if(availeMem > input_shape[0][1])
							{
								regFileMem.allocMemAddr(start_alloc_addr, input_shape[0][1]);
							}
							else
							{
								LOG(FATAL)<<"regFileMem is not enough for input shape !";
							}

							struct mem_record mr;
							mr.mem_type = REGFILE_MEM_TYPE;
							mr.start = start_alloc_addr;
							mr.len = input_shape[0][1];

							if(node->getID() == MV_OPNODE_ID)
							{
								arry_grp_cfg.regfile_addr_start = start_alloc_addr;
								arry_grp_cfg.regfile_addr_len = input_shape[0][1];
							}
							else if(node->getID() == ADD_OPNODE_ID)
							{
								readder_cfg.add_addr.push_back(mr.start);
								readder_cfg.readder_length = mr.len;
							}
							else if(node->getID() == ABS_OPNODE_ID){}
							else if(node->getID() == ACT_OPNODE_ID)
							{
								reactv_grp_cfg.reactv_fetch_addr = mr.start;
								reactv_grp_cfg.reactv_store_length = mr.len;
							}
							else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
							{
								mul_grp_cfg.in_addrs.push_back(mr.start);
								mul_grp_cfg.mul_length = mr.len;
							}
							else{
								DLOG(INFO)<<"Do not support opnode ID";
							}

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
						DLOG(INFO)<<"	No."<<j<<" CONST_TYPE";
						int	column_size =  input_tensors[j]->getShape()[1];
						int	start_alloc_column_addr = caculateArryMem.getArryColumnUsedSize();
						int retColumnAddr = caculateArryMem.allocColumnMem(start_alloc_column_addr, column_size);
						if(retColumnAddr)
							start_alloc_column_addr = retColumnAddr;

						int row_size =  input_tensors[j]->getShape()[0];
						int start_alloc_row_addr = 0;
						int retAddr = caculateArryMem.allocRowMem(start_alloc_row_addr, row_size);
						if(retAddr)
							start_alloc_row_addr = retAddr;

						struct mem_record mr;

						mr.mem_type = ARRAY_MEM_TYPE;
						mr.column_start = start_alloc_column_addr;
						mr.column_len = column_size;
						mr.row_start = start_alloc_row_addr;
						mr.row_len = row_size;
						tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
														(input_tensors[j], mr));
						arry_grp_cfg.w_win_column_s = start_alloc_column_addr;
						arry_grp_cfg.w_win_column_e = start_alloc_column_addr + column_size - 1;
						DLOG(INFO)<<"	w_win_column_e:"<<start_alloc_column_addr + column_size - 1;

						arry_grp_cfg.w_win_column_len = column_size;
						arry_grp_cfg.w_win_row_s = start_alloc_row_addr;
						arry_grp_cfg.w_win_row_e = start_alloc_row_addr + row_size - 1;
						arry_grp_cfg.w_win_row_len = row_size;

						//manager file params.dat
						DLOG(INFO)<<": print weight to params.dat ";
						DLOG(INFO)<<"	   row_size:"<<row_size;
						DLOG(INFO)<<"	column_size:"<<column_size;
						clock_t startTime, endTime;
						startTime = clock();
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
						endTime = clock();
						mvOpNodePtr node_ptr = std::dynamic_pointer_cast<mvOpNode>(node);

						if(node_ptr->getBiasEn())
						{
							vector<int16_t> bias_data = node_ptr->getBiasData();
							//需要为bias分配空间
							size_t bias_size = bias_data.size();
							vector<int16_t> data1 ;
							vector<int16_t> data2 ;
							for(size_t bias_index = 0; bias_index < bias_data.size();bias_index++)
							{
								int16_t up = (bias_data[bias_index] / 128) / 2;
								int16_t down = (bias_data[bias_index] / 128) -  (bias_data[bias_index] / 128) / 2;
								data1.push_back(up);
								data2.push_back(down);
							}
							for(size_t data1_idx = 0; data1_idx < data1.size();data1_idx++)
								fprintf(stream, "%c", data1[data1_idx]);
							for(size_t data2_idx = 0; data2_idx < data2.size();data2_idx++)
								fprintf(stream, "%c", data2[data2_idx]);
							bias_params.start = file_offset;
							bias_params.end = file_offset + 2 * bias_size - 1;
							bias_params.size = 2 * bias_size;
							file_offset += 2 * bias_size;

							int bias_start_row_addr = biasRegionMem.allocBiasRowRegionMem(2);
							bias_cfg.bias_row_len = 2;
							bias_cfg.bias_row_s = bias_start_row_addr;
							bias_cfg.bias_row_e = bias_start_row_addr + 2 - 1;

							bias_cfg.bias_column_len = column_size;
							bias_cfg.bias_column_s = start_alloc_column_addr;
							bias_cfg.bias_column_e = start_alloc_column_addr + column_size - 1;
							bias_cfg.b_prams = bias_params;
						}
						else
						{
							for(size_t data1_idx = 0; data1_idx < column_size;data1_idx++)
								fprintf(stream, "%c", 0);
							for(size_t data2_idx = 0; data2_idx < column_size;data2_idx++)
								fprintf(stream, "%c", 0);

							int bias_start_row_addr = biasRegionMem.allocBiasRowRegionMem(2);
							bias_cfg.bias_row_len = 2;
							bias_cfg.bias_row_s = bias_start_row_addr;
							bias_cfg.bias_row_e = bias_start_row_addr + 2 - 1;

							bias_cfg.bias_column_len = column_size;
							bias_cfg.bias_column_s = start_alloc_column_addr;
							bias_cfg.bias_column_e = start_alloc_column_addr + column_size - 1;
							bias_cfg.b_prams = bias_params;
						}

#ifdef TIME_PROF
						std::cout<<"The copy time is "<<(double)(endTime - startTime) / CLOCKS_PER_SEC <<"s"<<std::endl;
#endif
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

				DLOG(INFO)<<"[INPUTNODE] out_tensors total : "<<output_tensors.size();
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

							if(node->getID() == MV_OPNODE_ID)
							{
								arry_grp_cfg.store_addr = start_alloc_addr;
								arry_grp_cfg.store_len = tensor_shape[1];
							}
							else if(node->getID() == ADD_OPNODE_ID)
							{
								readder_cfg.readder_store = start_alloc_addr;
							}
							else if(node->getID() == ABS_OPNODE_ID){}
							else if(node->getID() == EXP_OPNODE_ID){}
							else if(node->getID() == ACT_OPNODE_ID)
							{
								reactv_grp_cfg.reactv_store_addr=start_alloc_addr;
							}
							else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
							{
								mul_grp_cfg.pstore_addr=start_alloc_addr;
							}
							else
							{
								LOG(FATAL)<<"Unsupport opnode id";
							}

							tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
																	(output_tensors[k], mr));
						}
						else
						{
							LOG(FATAL)<<"No regFileMem is available !";
						}
					}
				}
				round_cfg_in.rd_control_enable = rce;
				round_cfg_in.array_grp_config = arry_grp_cfg;
				round_cfg_in.readder_config = readder_cfg;
				round_cfg_in.actv_grp_config = actv_grp_cfg;
				round_cfg_in.bias_config = bias_cfg;
				round_cfg_in.mul_grp_config = mul_grp_cfg;
#ifdef WITIN_DEBUG
				dump_round_info(round_cfg_in);
#endif
				rounds.push_back(round_cfg_in);
			}
			else  /*normal node*/
			{
				DLOG(INFO)<<"[NORMAL] node:"<<op_list[i]->getName();
				RD_CONTROL_ENABLE rce;
				rce.cnt = i;

				ROUND_CONFIG round_cfg_norm;
				ARRAY_GRP_CONFIG arry_grp_cfg;
				WEIGHT_PARAMS weight_params;
				BIAS_PARAMS bias_params;
				BIAS_CONFIG bias_cfg;
				ACTV_GRP_CONFIG actv_grp_cfg;
				READDER_CONFIG readder_cfg;
				REACTV_GRP_CONFIG reactv_grp_cfg;
				MUL_GRP_CONFIG mul_grp_cfg;

				baseOpNodePtr node = op_list[i];

				if(node->getID() == MV_OPNODE_ID)
				{
					rce.weight_en = true;
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

					if(mv_ptr->getBiasEn())
						rce.bias_en = true;
					else
						rce.bias_en = false;

				}
				else if(node->getID() == ADD_OPNODE_ID)
				{
					rce.readdr_en=true;
				}
				else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
				{
					rce.mult_en=true;
				}
				else if(node->getID() == ACT_OPNODE_ID)
				{
					rce.reactive_en=true;
					actOpNodePtr act_ptr = std::dynamic_pointer_cast<ActOpNode>(node);
					reactv_grp_cfg.reactv_sel = act_ptr->act_type;
				}
				else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
				{
					rce.mult_en = true;
				}
				else
				{
					LOG(FATAL)<<"Do not support opnode ID";
				}

				vector<Tensor*> input_tensors;
				node->get_input_tensors(input_tensors);
				//1.inputnode share same memory

				//tensor_mem_record_map.insert(pair<Tensor*, struct mem_record>
				//											(input_tensors[j], mr));

				DLOG(INFO)<<"[NORMAL] input_tensors total : "<<input_tensors.size();
				for(size_t n = 0; n < input_tensors.size();n++ )
				{
					auto iter = tensor_mem_record_map.find(input_tensors[n]);

					//input tensor and not const_type
					//if found then update input addr of this round : regfile mem
					if(iter != tensor_mem_record_map.end())
					{
						DLOG(INFO)<<"find tensor in tensor_mem_record_map";
						if(node->getID() == MV_OPNODE_ID)
						{
							struct mem_record mr = iter->second;
							arry_grp_cfg.regfile_addr_start = mr.start;
							arry_grp_cfg.regfile_addr_len = mr.len;
						}
						else if(node->getID() == ADD_OPNODE_ID)
						{
							struct mem_record mr = iter->second;
							readder_cfg.add_addr.push_back(mr.start);
							readder_cfg.readder_length = mr.len;
						}
						else if(node->getID() == ABS_OPNODE_ID){}
						else if(node->getID() == ACT_OPNODE_ID)
						{
							struct mem_record mr = iter->second;
							reactv_grp_cfg.reactv_fetch_addr = mr.start;
							reactv_grp_cfg.reactv_store_length = mr.len;
						}
						else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
						{
							struct mem_record mr = iter->second;
							mul_grp_cfg.in_addrs.push_back(mr.start);
							mul_grp_cfg.mul_length = mr.len;
						}
						else{
							LOG(FATAL)<<"Do not support opnode ID";
						}
					}
					else
					{
						DLOG(WARNING)<<"could not find tensor in tensor_mem_record_map, maybe const tensor";
					}

					//  allocate mem for const Tensor if not none, : array mem
					//  then print to file
					if(input_tensors[n]->tensor_type == CONST_TYPE)
					{
						int column_size =  input_tensors[n]->getShape()[1];
						int start_alloc_column_addr = caculateArryMem.getArryColumnUsedSize();
						int retColumnAddr = caculateArryMem.allocColumnMem(start_alloc_column_addr, column_size);
						if(retColumnAddr)
							start_alloc_column_addr = retColumnAddr;

						int row_size =  input_tensors[n]->getShape()[0];
						int start_alloc_row_addr = 0;
						int retAddr = caculateArryMem.allocRowMem(start_alloc_row_addr, row_size);
						if(retAddr)
							start_alloc_row_addr = retAddr;
						struct mem_record mr;

						mr.mem_type = ARRAY_MEM_TYPE;
						mr.column_start = start_alloc_column_addr;
						mr.column_len = column_size;
						mr.row_start = start_alloc_row_addr;
						mr.row_len = row_size;

						arry_grp_cfg.w_win_column_s = start_alloc_column_addr;
						arry_grp_cfg.w_win_column_e = start_alloc_column_addr + column_size - 1;
						DLOG(INFO)<<"	w_win_column_e:"<<start_alloc_column_addr + column_size - 1;

						arry_grp_cfg.w_win_column_len = column_size;
						arry_grp_cfg.w_win_row_s = start_alloc_row_addr;
						arry_grp_cfg.w_win_row_e = start_alloc_row_addr + row_size - 1;
						arry_grp_cfg.w_win_row_len = row_size;
						clock_t startTime, endTime;
						startTime = clock();
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
						endTime = clock();

						mvOpNodePtr node_ptr = std::dynamic_pointer_cast<mvOpNode>(node);
						if(node_ptr->getBiasEn())
						{
							vector<int16_t> bias_data = node_ptr->getBiasData();
							//需要为bias分配空间
							size_t bias_size = bias_data.size();
							vector<char> data1 ;
							vector<char> data2 ;
							for(size_t bias_index = 0; bias_index < bias_data.size();bias_index++)
							{
								char up = (bias_data[bias_index] / 128) / 2;
								char down = (bias_data[bias_index] / 128) - (bias_data[bias_index] / 128) / 2;
								data1.push_back(up);
								data2.push_back(down);
							}
							for(size_t data1_idx = 0; data1_idx < data1.size();data1_idx++)
								fprintf(stream, "%c", data1[data1_idx]);
							for(size_t data2_idx = 0; data2_idx < data2.size();data2_idx++)
								fprintf(stream, "%c", data2[data2_idx]);
							bias_params.start = file_offset;
							bias_params.end = file_offset + 2 * bias_size - 1;
							bias_params.size = 2 * bias_size;
							file_offset += 2 * bias_size;

							int bias_start_row_addr = biasRegionMem.allocBiasRowRegionMem(2);
							bias_cfg.bias_row_len = 2;
							bias_cfg.bias_row_s = bias_start_row_addr;
							bias_cfg.bias_row_e = bias_start_row_addr + 2 - 1;

							bias_cfg.bias_column_len = column_size;
							bias_cfg.bias_column_s = start_alloc_column_addr;
							bias_cfg.bias_column_e = start_alloc_column_addr + column_size - 1;
							bias_cfg.b_prams = bias_params;
						}
						else
						{
							for(size_t data1_idx = 0; data1_idx < column_size;data1_idx++)
								fprintf(stream, "%c", 0);
							for(size_t data2_idx = 0; data2_idx < column_size;data2_idx++)
								fprintf(stream, "%c", 0);

							int bias_start_row_addr = biasRegionMem.allocBiasRowRegionMem(2);
							bias_cfg.bias_row_len = 2;
							bias_cfg.bias_row_s = bias_start_row_addr;
							bias_cfg.bias_row_e = bias_start_row_addr + 2 - 1;

							bias_cfg.bias_column_len = column_size;
							bias_cfg.bias_column_s = start_alloc_column_addr;
							bias_cfg.bias_column_e = start_alloc_column_addr + column_size - 1;
							bias_cfg.b_prams = bias_params;
						}
#ifdef TIME_PROF
						DLOG(INFO)<<"The copy time is "<<(double)(endTime - startTime) / CLOCKS_PER_SEC <<"s";
#endif
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

				DLOG(INFO)<<"[NORMAL] output_tensors total : "<<norm_output_tensors.size();
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
							if(node->getID() == MV_OPNODE_ID)
							{
								arry_grp_cfg.store_addr = start_alloc_addr;
								arry_grp_cfg.store_len = tensor_shape[1];
							}
							else if(node->getID() == ADD_OPNODE_ID)
							{
								readder_cfg.readder_store = start_alloc_addr;
							}
							else if(node->getID() == ABS_OPNODE_ID){}
							else if(node->getID() == EXP_OPNODE_ID){}
							else if(node->getID() == ACT_OPNODE_ID)
							{
								reactv_grp_cfg.reactv_store_addr=start_alloc_addr;
							}
							else if(node->getID() == ELEMWISE_MUL_OPNODE_ID)
							{
								mul_grp_cfg.pstore_addr=start_alloc_addr;
							}
							else
							{
								LOG(FATAL)<<"Unsupport opnode id";
							}

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

				round_cfg_norm.rd_control_enable = rce;
				round_cfg_norm.array_grp_config = arry_grp_cfg;
				round_cfg_norm.readder_config = readder_cfg;
				round_cfg_norm.reactv_grp_config = reactv_grp_cfg;
				round_cfg_norm.bias_config = bias_cfg;
				round_cfg_norm.mul_grp_config = mul_grp_cfg;
#ifdef WITIN_DEBUG
				dump_round_info(round_cfg_norm);
#endif
				rounds.push_back(round_cfg_norm);
			}
		}

		fclose(stream);
		string boardConfigJsonPath = "./build/BoardConfig.json";
		writeToJson(rounds, params_path, boardConfigJsonPath);
		// generateRegConfig(boardConfigJsonPath.c_str(), params_path.c_str());
		// generateArrayMap(boardConfigJsonPath.c_str(), params_path.c_str());

		//write mem useage to a file
		FILE*memSt;
		memSt = fopen("./build/memInitFile.data", "w");
		DLOG(INFO)<<"Write mem usage into memInitFile.data!";
		fprintf(memSt, "%d\n", regFileMem.getGeneralUsedSize());
		fprintf(memSt, "%d\n", caculateArryMem.getArryColumnUsedSize());
		fprintf(memSt, "%d\n", caculateArryMem.getArryRowUsedSize());
		fclose(memSt);
		DLOG(INFO)<<"Generate BoardConfig end";

	}

	int32_t Session::genCalibrationConfig(WitinGraphType &InGraph,
											std::vector<Tensor*> input_tensors)
	{
		DLOG(INFO)<<"========Generate Array Calibration Config============";
		FILE*stream_in = fopen("./build/output/map/expected_in.bin", "wb");
		FILE*stream_out = fopen("./build/output/map/expected_out.bin", "wb");
		if(!stream_in)
		{
			LOG(FATAL)<<"Open expected_in.bin error!";
		}
		if(!stream_out)
		{
			LOG(FATAL)<<"Open expected_out.bin error!";
		}
		auto in_nodes = InGraph.inNodes();
		auto out_nodes = InGraph.outNodes();
		vector<int> shape_input = input_tensors[0]->getShape();
		DLOG(INFO)<<"calibration input shape:"<<shape_input;
		// input_tensors[0]->print();
		// input_tensors[1]->print();

		vector<baseOpNodePtr> op_list = InGraph.graph_topological_sort();
		vector<Tensor*> output_tensors;

		vector<vector<vector<int16_t>>> data_3dim_output;
		vector<vector<vector<int16_t>>> data_3dim_input;
		data_3dim_output.resize(op_list.size());
		data_3dim_input.resize(op_list.size());

		for(auto r = 0; r < op_list.size(); r++)
		{
			data_3dim_output.resize(input_tensors.size());
			data_3dim_input.resize(input_tensors.size());
		}

		for(size_t idx = 0; idx < input_tensors.size(); idx++)
		// for(size_t idx = 0; idx < 2; idx++)
		{
			for(size_t i = 0; i < op_list.size(); i++)
			{
				if(isInputNode(op_list[i], in_nodes))
				{
					DLOG(INFO)<<"[Input Node]";
					baseOpNodePtr node = op_list[i];
					vector<vector<int> > int_shape = node->infer_shape();
					Tensor* out = new Tensor(int_shape[0], CONST_TYPE);
					Tensor* array_result_tensor = new Tensor(int_shape[0], CONST_TYPE);
					node->forward(input_tensors[idx], out, array_result_tensor);
					node->updateInputTensors(input_tensors[idx]);
					node->updateOutputTensors(out, array_result_tensor);
					DLOG(INFO)<<"[forward]: output Tensor:";
					array_result_tensor->print();
					vector<int16_t>data_output;
					vector<int16_t>data_input;

					//out --> vector<char> data_output
					for(int k = 0; k < int_shape[0][1]; k ++)
						data_output.push_back((int16_t)((char*)array_result_tensor->getData())[k]);
					data_3dim_output[i].push_back(data_output);

					for(int k = 0; k < (input_tensors[idx]->getShape())[1]; k ++)
						data_input.push_back((int16_t)((char*)input_tensors[idx]->getData())[k]);
					data_3dim_input[i].push_back(data_input);

					delete out;
					delete array_result_tensor;
				}
				else if(isOutputNode(op_list[i], out_nodes))
				{
					DLOG(INFO)<<"[Output Node]";
					baseOpNodePtr node = op_list[i];
					vector<baseOpNodePtr> innodes = InGraph.innodes_of_node(op_list[i]);
					vector<Tensor*> in_node_tensors = innodes[0]->getBottomTensors();
					vector<vector<int> > int_shape = node->infer_shape();
					Tensor* out = new Tensor(int_shape[0], CONST_TYPE);
					Tensor* array_result_tensor = new Tensor(int_shape[0], CONST_TYPE);
					node->forward(in_node_tensors[idx], out, array_result_tensor);
					node->updateInputTensors(input_tensors[idx]);
					node->updateOutputTensors(out, array_result_tensor);
					DLOG(INFO)<<"[forward]: *****final output Tensor*****:";
					out->print();
					output_tensors.push_back(out);
					//out --> vector<char> data_output
					vector<int16_t>data_output;
					vector<int16_t>data_input;

					for(int k = 0; k < int_shape[0][1]; k ++)
						data_output.push_back((int16_t)((char*)array_result_tensor->getData())[k]);
					data_3dim_output[i].push_back(data_output);

					for(int k = 0; k < (in_node_tensors[idx]->getShape())[1]; k ++)
						data_input.push_back((int16_t)((char*)in_node_tensors[idx]->getData())[k]);
					data_3dim_input[i].push_back(data_input);

					delete out;
					delete array_result_tensor;
				}
				else
				{
					DLOG(INFO)<<"[Normal Node]";
					baseOpNodePtr node = op_list[i];
					vector<baseOpNodePtr> innodes = InGraph.innodes_of_node(op_list[i]);
					vector<Tensor*> in_node_tensors = innodes[0]->getBottomTensors();
					vector<vector<int> > int_shape = node->infer_shape();
					Tensor* out = new Tensor(int_shape[0], CONST_TYPE);
					Tensor* array_result_tensor = new Tensor(int_shape[0], CONST_TYPE);
					node->forward(in_node_tensors[idx], out, array_result_tensor);
					node->updateInputTensors(input_tensors[idx]);
					node->updateOutputTensors(out, array_result_tensor);
					DLOG(INFO)<<"[forward]: *****final outout Tensor*****:";
					//out --> vector<char> data_output
					vector<int16_t>data_output;
					vector<int16_t>data_input;
					for(int k = 0; k < int_shape[0][1]; k ++)
						data_output.push_back((int16_t)((char*)array_result_tensor->getData())[k]);
					data_3dim_output[i].push_back(data_output);

					for(int k = 0; k < (in_node_tensors[idx]->getShape())[1]; k ++)
						data_input.push_back((int16_t)((char*)in_node_tensors[idx]->getData())[k]);
					data_3dim_input[i].push_back(data_input);

					delete out;
					delete array_result_tensor;
				}
			}
		}
		int layers = op_list.size();
		fwrite(&layers, sizeof(u_int16_t), 1, stream_in);
		for(size_t m = 0; m < op_list.size(); m++)
		// for(size_t m = 0; m < 1; m++)
		{
			int n = 0;
			int z = 0;
			//input
			for (auto kv : data_3dim_input[m])
			{
				fwrite(kv.data(), sizeof(int16_t), kv.size(), stream_in);
				// DLOG(INFO)<<"[in] m = "<<m<<" n = "<<ln<< " size = "<<kv.size()<<" data:"<<kv;
				n++;
			}
			//output
			for (auto kv : data_3dim_output[m])
			{
				for(auto kk : kv)
				// DLOG(INFO)<<"data:"<<(int)kk;
				fwrite(kv.data(), sizeof(int16_t), (int)(kv.size()), stream_out);
				// DLOG(INFO)<<"[out] m = "<<m<<" z = "<<z<< " size = "<<kv.size()<<" data:"<<kv;
				z++;
			}
		}
		fclose(stream_in);
		fclose(stream_out);
		DLOG(INFO)<<"Calibration total input size:"<<output_tensors.size();
		DLOG(INFO)<<"Generate Array Calibration Config end !";
	}

	int32_t Session::run(WitinGraphType &InGraph, std::vector<Tensor*> input_tensors)
	{

	}

} //namespace base
} //namespace witin
