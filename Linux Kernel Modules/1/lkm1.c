#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het");
MODULE_DESCRIPTION("Module listing running or runnable processes");

//Load Module
static int running_process_init(void) {
    
    struct task_struct *task;
    printk(KERN_INFO "Running or runnable Processes\n");

    //Iterating through all the process task_structs
    for_each_process(task) {

        // If process is currently running
        if(task->__state == TASK_RUNNING)
        {
            printk("PID: %d\n", task->pid);
        }
    }
    return 0;
}

// Remove Module
static void running_process_exit(void) {
    printk("Module Removed\n");
}

module_init(running_process_init);
module_exit(running_process_exit);
