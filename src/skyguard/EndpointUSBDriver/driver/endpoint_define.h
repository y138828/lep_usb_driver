#ifdef __KERNEL__
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/mount.h>
#include <linux/syscalls.h>
#include <linux/sysctl.h>
#include <linux/file.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/netlink.h>
#include <linux/statfs.h>
#include <net/netlink.h>
#include <net/tcp.h>
#include <linux/security.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#ifndef ENDPOINT_DEFINE_HEADER
#define ENDPOINT_DEFINE_HEADER
 

#endif