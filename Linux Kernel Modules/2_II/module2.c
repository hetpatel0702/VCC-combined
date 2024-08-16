#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/sched.h>
#include<linux/list.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <asm/io.h>
#include "declarations_ioctl_2.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het Patel");
MODULE_DESCRIPTION("Module which changes the parent of soldier to control station");

#define FIRST_MINOR 0
#define MINOR_CNT 1
 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}

static long change_parent_function_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    cs_pid p;
    switch (cmd)
    {
        // Change soldier's parent to control station
        case SEND_CONTROL_STATION_PID:
            
            // Get the control station's PID from user
            if (copy_from_user(&p, (cs_pid *)arg, sizeof(cs_pid)))
            {
                return -1;
            }
            
            int control_station_pid = p.PID;
            struct task_struct *task;
            struct task_struct *control_station_task_struct;

            // Get the task_struct of the control station
            for_each_process(task)
            {
                if(task->pid == control_station_pid)
                {
                    control_station_task_struct = task;
                    break;
                }
            }

            // Children entry in parent of soldier
            struct list_head *x = &current->parent->children;
            struct list_head *i;

            // Iterate through the children of parent of soldier
            list_for_each(i, x)
            {
                // Get the task_struct from the sibling pointer
                struct task_struct *y = list_entry(i, struct task_struct, sibling);
                
                // If current task_struct is of soldier's, then delete it
                if(y->pid == current->pid)
                {
                    list_del(&y->sibling);
                    break;
                }
            }

            // Add soldier's task_struct to control_station's children linked list
            list_add_tail(&current->sibling, &control_station_task_struct->children);

            // Change parent of soldier to control station
            current->parent = control_station_task_struct;
            current->real_parent = control_station_task_struct;
            break;
        default:
            return -EINVAL;
    }
    return 0;
}
 
static struct file_operations query_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = change_parent_function_ioctl
};
 
static int __init ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_ioctl")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &query_fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "car")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
 
    return 0;
}
 
static void __exit ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}
 
module_init(ioctl_init);
module_exit(ioctl_exit);
 
