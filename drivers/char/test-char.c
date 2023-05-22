// SPDX-License-Identifier: GPL-2.0+

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>

static int test_char_probe(struct platform_device *dev)
{
	int retval;
	int irq;
	const char *mailbox_name;

	struct device_node *node = dev->dev.of_node;

	printk("test char probe\n");

	retval = of_property_read_string(node, "mailbox", &mailbox_name);
	if (!retval)
		printk("test char: mailbox name is %s\n", mailbox_name);
	else
		printk("test char: mailbox name is not found\n");

	irq = platform_get_irq(dev, 0);
	if (irq < 0)
		printk("test char: No IRQ\n");
	else
		printk("test char: IRQ = %d\n", irq);

	return 0;
}

static int test_char_remove(struct platform_device *dev)
{
	printk("test char remove\n");

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
