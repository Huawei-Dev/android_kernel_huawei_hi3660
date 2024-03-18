#ifndef _HISI_CPUFREQ_DT_H
#define _HISI_CPUFREQ_DT_H

#include <linux/version.h>

#ifdef CONFIG_HISI_CPUFREQ_DT
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int hisi_cpufreq_set_supported_hw(struct cpufreq_policy *policy);
void hisi_cpufreq_put_supported_hw(struct cpufreq_policy *policy);
#else
struct opp_table *hisi_cpufreq_set_supported_hw(struct device *cpu_dev);
void hisi_cpufreq_put_supported_hw(struct opp_table *opp_table);
#endif
void hisi_cpufreq_get_suspend_freq(struct cpufreq_policy *policy);
int hisi_cpufreq_init(void);
#endif

#endif /* _HISI_CPUFREQ_DT_H */
