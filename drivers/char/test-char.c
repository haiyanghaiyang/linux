// SPDX-License-Identifier: GPL-2.0+

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

static int test_char_probe(struct platform_device *dev)
{
	int retval;
	int irq;
	u32 dev_id;
	const char *mailbox_name;
	struct regmap *ctrl_base;
	struct resource *res_irq;
	struct resource *res_mem;
	int res_index;

	struct device_node *node = dev->dev.of_node;

	printk("test char probe\n");

	retval = of_property_read_string(node, "mailbox", &mailbox_name);
	if (!retval)
		printk("test char: mailbox name is %s\n", mailbox_name);
	else {
		printk("test char: mailbox name is not found\n");
		return 0;
	}

	retval = of_property_read_u32_index(node, "dev-id", 0, &dev_id);
	if (retval)
		printk("test char: cannot get dev-id\n");
	else
		printk("test char: dev-id = %d\n", dev_id);

	irq = platform_get_irq(dev, 0);
	if (irq < 0)
		printk("test char: No IRQ\n");
	else
		printk("test char: IRQ = %d\n", irq);

#if 0
	if (of_property_read_bool(node, "syscon-spi-cs")) {
		ctrl_base = syscon_regmap_lookup_by_phandle(node, "syscon-spi-cs");
		if (IS_ERR(ctrl_base))
			printk("ctrl_base is error\n");
		else
			printk("ctrl_base is successful\n");
	}
#endif

	res_irq = platform_get_resource(dev, IORESOURCE_IRQ, 0);
	if (!res_irq)
		printk("test char: get resource No IRQ\n");
	else
		printk("test char: get resource IRQ = %lld\n", res_irq->start);

	for (res_index = 0;;res_index++) {
		res_mem = platform_get_resource(dev, IORESOURCE_MEM, res_index);
		if (!res_mem) {
			printk("test char: get resource No mem\n");
			break;
		}
		else
			printk("test char: get resource mem start = %llx end = %llx\n", res_mem->start, res_mem->end);
	}

	return 0;
}

static int test_char_remove(struct platform_device *dev)
{
	printk("test char remove\n");

	return 0;
}

static int test_char_suspend(struct platform_device *dev, pm_message_t state)
{
	printk("test char suspend: state = %d\n", state.event);

	return 0; //return -1 to prevent suspend
}

static int test_char_resume(struct platform_device *dev)
{
	printk("test char resume\n");

	return 0;
}

static const struct of_device_id of_test_char_ids[] = {
	{ .compatible = "test,char" },
	{}
};

static struct platform_device *test_char_platform_device;

static struct platform_driver test_char_device_driver = {
	.probe		= test_char_probe,
	.remove		= test_char_remove,
#ifdef CONFIG_PM
	.suspend	= test_char_suspend,
	.resume		= test_char_resume,
#endif
	.driver		= {
		.name	= "test char",
		.of_match_table = of_test_char_ids,
    },
};

static int __init test_char_init(void)
{
	int retval;

	printk("test char init\n");

	test_char_platform_device = platform_device_alloc("test char", -1);
	if (!test_char_platform_device)
		return -ENOMEM;

	retval = platform_device_add(test_char_platform_device);
	if (retval < 0) {
		platform_device_put(test_char_platform_device);
		return retval;
	}

	retval = platform_driver_register(&test_char_device_driver);
	if (retval < 0)
		platform_device_unregister(test_char_platform_device);

	return retval;
}

static void __exit test_char_exit(void)
{
	printk("test char exit\n");

	platform_driver_unregister(&test_char_device_driver);
	platform_device_unregister(test_char_platform_device);
	platform_device_put(test_char_platform_device);
}

module_init(test_char_init);
module_exit(test_char_exit);
