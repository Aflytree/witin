#include <witin/target/core.h>
#include <witin/target/util.h>

namespace witin{
namespace base{

void dump_round_info(ROUND_CONFIG rc)
{
    dump_rd_ctrl_enable(rc.rd_control_enable);
    if(rc.rd_control_enable.weight_en)
    {
		  dump_arry_grp_cfg(rc.array_grp_config);
    }
    if(rc.rd_control_enable.readdr_en)
    {
		  dump_readder_grp_cfg(rc.readder_config);
    }
    if(rc.rd_control_enable.reactive_en)
    {
		  dump_reactv_grp_cfg(rc.reactv_grp_config);
    }

    if(rc.rd_control_enable.mult_en)
    {
		  dump_mul_grp_cfg(rc.mul_grp_config);
    }

    if(rc.rd_control_enable.bias_en)
    {
		  dump_bias_config_cfg(rc.bias_config);
    }

}

} //namespace base
} //namespace witin
