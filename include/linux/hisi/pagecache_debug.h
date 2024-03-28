#ifndef _LINUX_PAGECACHE_DEBUG_H
#define _LINUX_PAGECACHE_DEBUG_H

#include <linux/bitops.h>
#include <linux/dcache.h>
#include <linux/path.h>

#define BIT_READAHEAD_SYSCALL_DUMP		BIT(0)
#define BIT_MADVISE_SYSCALL_DUMP		BIT(1)
#define BIT_FILEMAP_FAULT_DUMP			BIT(2)
#define BIT_MMAP_SYNC_READ_DUMP			BIT(3)
#define BIT_GENERIC_SYNC_READ_DUMP		BIT(4)
#define BIT_PAGECACHE_SYNC_READAHEAD_DUMP	BIT(5)
#define BIT_PAGECACHE_ASYNC_READAHEAD_DUMP	BIT(6)
#define BIT_DO_PAGECACHE_READAHEAD_DUMP		BIT(7)
#define BIT_MM_SHRINK_INACTIVE_DUMP		BIT(8)
#define BIT_MM_SHRINK_ACTIVE_DUMP		BIT(9)
#define BIT_GENERIC_WRITE_DUMP			BIT(10)
#define BIT_FSYNC_SYSCALL_DUMP			BIT(11)
#define BIT_WRITEBACK_DUMP			BIT(12)

#define BIT_PGCACHE_DUMP_FULLPATH_ENABLE	BIT(16)
#define BIT_PGCACHE_DUMP_ERROR_PRINT		BIT(17)
#define BIT_PGCACHE_DUMP_DENTRY_STATUS		BIT(18)

#define DUMP_PATH_LENGTH		200
#define DUMP_STAT_LENGTH		200

#define PGCACHE_DUMP_HEAD_LINE		"[PGCACHE_LOG],"

#define stat_inc_hit_count()
#define stat_inc_miss_count()
#define stat_inc_syncread_pages_count(cnt)
#define stat_inc_asyncread_pages_count(cnt)
#define stat_inc_mmap_hit_count()
#define stat_inc_mmap_miss_count()
#define stat_inc_mmap_syncread_pages_count(cnt)
#define stat_inc_mmap_asyncread_pages_count(cnt)
#define stat_inc_wb_count()
#define stat_inc_wb_pages_count(cnt)
#define stat_inc_dirty_pages_count()
#define stat_dec_dirty_pages_count()
#define stat_inc_shrink_pages_count()

#endif /* _LINUX_PAGECACHE_DEBUG_H */
