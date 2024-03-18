/*
 * Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __HISI_RTG_H
#define __HISI_RTG_H

static inline struct related_thread_group* lookup_related_thread_group(unsigned int group_id) { return NULL; }
static inline int sched_set_group_id(pid_t pid, unsigned int group_id) { return 0; }
static inline int sched_set_preferred_cluster(unsigned int grp_id, int sched_cluster_id) { return 0; }
static inline int sched_set_group_normalized_util(unsigned int grp_id, unsigned long util, unsigned int flag) { return 0; }
static inline int sched_set_group_window_size(unsigned int grp_id, unsigned int rate) { return 0; }
static inline int sched_set_group_window_rollover(unsigned int grp_id) { return 0; }
static inline int sched_set_group_freq(unsigned int grp_id, unsigned int freq) { return 0; }
static inline int sched_set_group_freq_update_interval(unsigned int grp_id, unsigned int interval) { return 0; }
static inline int sched_set_group_util_invalid_interval(unsigned int grp_id, unsigned int interval) { return 0; }
static inline unsigned int sched_get_group_id(struct task_struct *p) { return 0; }

#endif
