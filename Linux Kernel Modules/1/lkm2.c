#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het");
MODULE_DESCRIPTION("Module printing PID and process state of children of given PID");

// Take Parent PID from command line
static int parent_PID;
module_param(parent_PID, int, S_IRUGO);

// Load Module
static int __init print_children_info_init(void) {
    
    struct task_struct *x;
    printk("Printing children PIDs and their states of process %d\n", parent_PID);
    
    // Iterate through every process's task_struct
    for_each_process(x)
    {
        // if current process's parent matches with the given PID then it's the child of given PID
        if(x->real_parent->pid == parent_PID)
        {
            printk(KERN_INFO "Child PID:%d State:%d\n", x->pid, x->__state);
        }
    }
    return 0;
}

// Remove Module
static void __exit print_children_info_exit(void) {
    printk("module removed\n");
}

module_init(print_children_info_init);
module_exit(print_children_info_exit);
