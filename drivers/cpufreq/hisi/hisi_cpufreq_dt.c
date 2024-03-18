/*
 * Hisilicon Platforms CPUFREQ-DT support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/cpufreq.h>
#include <linux/of_platform.h>
#include <linux/pm_opp.h>
#include <linux/cpumask.h>

#include <linux/of_address.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/hisi/hisi_cpufreq_req.h>
#include <linux/hisi/hifreq_hotplug.h>
#include <linux/version.h>

#define CREATE_TRACE_POINTS
#include <trace/events/hisi_cpufreq_req.h>

#define VERSION_ELEMENTS	1
static unsigned int cpufreq_dt_version = 0;

static int cpufreq_freq_req_policy_notifier(struct notifier_block *nb,
					    unsigned long event, void *data)
{
	struct cpufreq_req *req = container_of(nb, struct cpufreq_req, nb);
	struct cpufreq_policy *policy = (struct cpufreq_policy *) data;

	if (event != CPUFREQ_ADJUST ||
	    !cpumask_test_cpu(req->cpu, policy->cpus))
		return NOTIFY_DONE;

	trace_cpufreq_req_notify(req->cpu, (void *)req, req->freq,
				 policy->min, policy->max);

	cpufreq_verify_within_limits(policy, req->freq,
				     policy->cpuinfo.max_freq);

	return NOTIFY_OK;
}

int hisi_cpufreq_init_req(struct cpufreq_req *req, int cpu)
{
	if (unlikely(IS_ERR_OR_NULL(req)))
		return -1;

	req->cpu = cpu;
	req->freq = 0;
	req->nb.priority = 100;
	req->nb.notifier_call = cpufreq_freq_req_policy_notifier;
	return cpufreq_register_notifier(&req->nb, CPUFREQ_POLICY_NOTIFIER);
}
EXPORT_SYMBOL(hisi_cpufreq_init_req);

void hisi_cpufreq_update_req(struct cpufreq_req *req, unsigned int freq)
{
	if (unlikely(IS_ERR_OR_NULL(req)))
		return;

	req->freq = freq;
	trace_cpufreq_req_update(req->cpu, (void *)req, freq);
	cpufreq_update_policy(req->cpu);
}
EXPORT_SYMBOL(hisi_cpufreq_update_req);

void hisi_cpufreq_exit_req(struct cpufreq_req *req)
{
	if (unlikely(IS_ERR_OR_NULL(req)))
		return;

	cpufreq_unregister_notifier(&req->nb, CPUFREQ_POLICY_NOTIFIER);
}
EXPORT_SYMBOL(hisi_cpufreq_exit_req);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int hisi_cpufreq_set_supported_hw(struct cpufreq_policy *policy)
{
	int ret, cpu;
	struct device *cpu_dev;

	/* find first cpu of policy->cpus */
	cpu = cpumask_any(policy->cpus);
	cpu_dev = get_cpu_device(cpu);
	if (!cpu_dev) {
		pr_err("%s Failed to get cpu %d device!\n", __func__, cpu);
		return -ENODEV;
	}

	ret = dev_pm_opp_set_supported_hw(cpu_dev, &cpufreq_dt_version, VERSION_ELEMENTS);
	if (ret)
		pr_err("%s Failed to set supported hardware\n", __func__);

	return ret;
}

void hisi_cpufreq_put_supported_hw(struct cpufreq_policy *policy)
{
	int cpu = 0, j;
	struct device *cpu_dev;

	/* find last cpu of policy->related_cpus */
	for_each_cpu(j, policy->related_cpus) {
		cpu = j;
	}
	cpu_dev = get_cpu_device(cpu);
	if (!cpu_dev) {
		pr_err("%s Failed to get cpu %d device!\n", __func__, cpu);
		return;
	}

	dev_pm_opp_put_supported_hw(cpu_dev);
}
#else
struct opp_table *hisi_cpufreq_set_supported_hw(struct device *cpu_dev)
{
	return dev_pm_opp_set_supported_hw(cpu_dev, &cpufreq_dt_version, VERSION_ELEMENTS);
}

void hisi_cpufreq_put_supported_hw(struct opp_table *opp_table)
{
	dev_pm_opp_put_supported_hw(opp_table);
}
#endif

static int hisi_cpufreq_get_dt_version(void)
{
	const char *target_cpu;
	int ret, index;
	struct device_node *np;

	np = of_find_compatible_node(NULL, NULL, "hisi,targetcpu");
	if (!np) {
		pr_err("%s Failed to find compatible node:targetcpu\n", __func__);
		return -ENODEV;
	}

	ret = of_property_read_string(np, "target_cpu", &target_cpu);
	if (ret) {
		pr_err("%s Failed to read target_cpu\n", __func__);
		of_node_put(np);
		return ret;
	}
	of_node_put(np);

	np = of_find_compatible_node(NULL, NULL, "hisi,supportedtarget");
	if (!np) {
		pr_err("%s Failed to find compatible node:supportedtarget\n", __func__);
		return -ENODEV;
	}

	ret = of_property_match_string(np, "support_name", target_cpu);
	if (ret < 0) {
		pr_err("%s Failed to get support_name\n", __func__);
		of_node_put(np);
		return ret;
	}
	of_node_put(np);

	index = ret;
	cpufreq_dt_version = BIT(index);

	return 0;
}

void hisi_cpufreq_get_suspend_freq(struct cpufreq_policy *policy)
{
	struct device_node *np;
	unsigned int value;
	int cluster, ret;

	np = of_find_compatible_node(NULL, NULL, "hisi,suspend-freq");
	if (!np)
		return;

	cluster = topology_physical_package_id(policy->cpu);
	ret = of_property_read_u32_index(np, "suspend_freq", cluster, &value);
	of_node_put(np);

	/* This overides the suspend opp */
	if (!ret)
		policy->suspend_freq = value;
}

int hisi_cpufreq_init(void)
{
	int ret = 0;

	if (!of_find_compatible_node(NULL, NULL, "arm,generic-bL-cpufreq"))
		return -ENODEV;

	ret = hisi_cpufreq_get_dt_version();
	if (ret)
		return -EINVAL;

	return ret;
}
