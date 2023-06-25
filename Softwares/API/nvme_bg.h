// SPDX-License-Identifier: GPL-3.0
/*
 * nvme_bg.h -- nvme_bg export API to user
 *
 * Copyright 2021, Yu Wang
 */

#ifndef _NVME_BG_H
#define _NVME_BG_H

#include <sys/syscall.h>
#include <unistd.h>

#define __NR_nvme_bg_setup	(436)
#define BG_DESC_LEN		(64)
#define WRITEBACK_ID		(1)
#define FLUSH_ID		(2)
#define COMPACTION_ID		(3)
#define FIO_ID			(4)
#define DEF_DEADLINE		(30e3)
#define FLAGS_SHIFT		(16)

enum NVME_BG_OP {
	OP_REGISTER		= 0,
	OP_DEREGISTER		= 1,
	OP_CHANGE_QOS		= 2,
	OP_SET_ID		= 3
};

enum NVME_BG_FLAG {
	FLAG_QoS_BW		= 0,
	FLAG_QoS_DDL		= 1,
	FLAG_DEREGISTER		= 2
};

// ###############
// # Private API #
// ###############

static inline long bg_set(unsigned int bg_id)
{
	return syscall(__NR_nvme_bg_setup, OP_SET_ID, bg_id, NULL, NULL);
}

static inline long bg_unset(void)
{
	return bg_set(0);
}

// ##############
// # Public API #
// ##############

static inline long bg_register(unsigned int flags, unsigned int qos, char *desc,
			       unsigned int *bg_id) {
	unsigned int opcode_flags = OP_REGISTER | (flags << FLAGS_SHIFT);
	return syscall(__NR_nvme_bg_setup, opcode_flags, qos, desc, bg_id);
}

static inline long bg_deregister(void)
{
	return syscall(__NR_nvme_bg_setup, OP_DEREGISTER, 0, NULL, NULL);
}

static inline long bg_change_qos(unsigned int flags, unsigned int qos)
{
	unsigned int opcode_flags = OP_CHANGE_QOS | (flags << FLAGS_SHIFT);
	return syscall(__NR_nvme_bg_setup, opcode_flags, qos, NULL, NULL);
}

#endif /* _NVME_BG_H */
