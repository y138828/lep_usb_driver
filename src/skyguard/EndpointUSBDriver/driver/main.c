#include "endpoint_define.h"
#include <linux/percpu-defs.h>
#include <linux/kprobes.h>
#include <linux/stacktrace.h>
#include <linux/thread_info.h>
#include <linux/kthread.h>
#include <net/inet_common.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SkyGuard Endpoint Team Kernel Module");
MODULE_AUTHOR("SkyGuard Endpoint Team");
MODULE_VERSION("0.1");


static int fh_init(void)
{
	int err = 0;
	do
	{
		pr_info("module loaded\n");
	} while (false);
	return err;
}

static void fh_exit(void)
{
	pr_info("module unloaded\n");
}

module_init(fh_init);
module_exit(fh_exit);