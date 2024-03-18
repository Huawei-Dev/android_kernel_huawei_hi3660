#ifndef _OASES_UTIL_H
#define _OASES_UTIL_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/printk.h>

/* general printk */
#define oases_debug(fmt, ...)				\
do {							\
	pr_debug("[oases][%s:%d] " fmt,			\
		__func__, __LINE__, ## __VA_ARGS__);	\
} while(0)

#define oases_info(fmt, ...)				\
do {							\
	pr_info("[oases][%s:%d] " fmt,			\
		__func__, __LINE__, ## __VA_ARGS__);	\
} while(0)

#define oases_error(fmt, ...)				\
do {							\
	pr_err("[oases][%s:%d] " fmt, 			\
		__func__, __LINE__, ## __VA_ARGS__);	\
} while(0)

struct oases_patch_info;

int oases_is_null(const void *, int);
int oases_valid_name(const char *id, int maxlen);

void oases_module_lock(void);
void oases_module_unlock(void);
void *oases_ref_module(const char *name);
int oases_ref_module_ptr(void *module);
void oases_unref_module(void *module);

int oases_insn_patch(const char *id);
int oases_insn_unpatch(const char *id);
int oases_remove_patch(struct oases_patch_info *info);

#endif /* OASES_UTIL_H */
