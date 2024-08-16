#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include<linux/module.h>
#include <linux/mm.h>

// This array contains the memory related fields
unsigned long vm_events[NR_VM_EVENT_ITEMS];

static int proc_show_helloworld(struct seq_file *file, void *v) {
    
    // Populate the array
    all_vm_events(vm_events);
    
    // Print the page faults
    seq_printf(file, "Page Faults: %lu\n", vm_events[PGFAULT]);
    return 0;
}

static int proc_open_helloworld(struct inode *inode_pointer, struct file *file) {
    return single_open(file, proc_show_helloworld, NULL);
}

// Defines functions for a proc entry 
static const struct proc_ops proc_file_operations = {
    .proc_open=proc_open_helloworld,
    .proc_read=seq_read,
    .proc_release = single_release,
};

static int __init init_helloworld(void)
{
    // Create "get_pgfaults" entry in proc
    if(!proc_create("get_pgfaults",0,NULL, &proc_file_operations))
    {
        printk(KERN_ERR "Error Creating proc entry");
        return -1;
    }
    return 0;
}

// Remove module and delete entry
static void __exit exit_helloworld(void)
{
    remove_proc_entry("get_pgfaults", NULL);
}

module_init(init_helloworld);
module_exit(exit_helloworld);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het");
MODULE_DESCRIPTION("Page faults in proc directory");