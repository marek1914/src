#include "hal_local.h"
#include "rad_include.h"
#include "stk_master.h"
#include "stk_slave.h"
#include "dbg_log.h"
#include "pub_protocol.h"
#include "pub_param.h"
#include "dev_key.h"

void loop(void)
{
	while(1)
	{
		dev_key_task();
		hal_standby();
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int main(void)
{
	hal_init();
	pub_param_init();
	
	rad_init();
	dbg_init();
#ifdef SLAVE
	stk_slave_init();
#else
	stk_master_init();
#endif
	loop();
	
	return 0;
}
