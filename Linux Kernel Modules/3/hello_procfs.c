#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include<linux/module.h>

static int proc_show_helloworld(struct seq_file *file, void *v) {
    seq_printf(file, "Hello World!\n");
    return 0;
}

static int proc_open_helloworld(struct inode *inode_pointer, struct file *file) {
    return single_open(file, proc_show_helloworld, NULL);
}

static const struct proc_ops proc_file_operations = {
    .proc_open=proc_open_helloworld,
    .proc_read=seq_read,
    .proc_release = single_release,
};

static int __init init_helloworld(void)
{
    if(!proc_create("hello_procfs",0,NULL, &proc_file_operations))
    {
        printk(KERN_ERR "Error Creating proc entry");
        return -1;
    }
    return 0;
}

static void __exit exit_helloworld(void)
{
    remove_proc_entry("hello_procfs", NULL);
}

module_init(init_helloworld);
module_exit(exit_helloworld);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het");
MODULE_DESCRIPTION("hello world in proc directory");