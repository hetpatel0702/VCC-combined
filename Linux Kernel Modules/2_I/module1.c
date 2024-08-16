#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <asm/pgtable.h>
#include <linux/init.h>
#include <linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/list.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <asm-generic/io.h>
#include "declarations_ioctl.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het Patel");
MODULE_DESCRIPTION("Module giving back PA for a given VA");

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static int PID = 0;
static unsigned long int VA = 0;
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}

// Get Physical Address for the given PID and virtual address
static long get_physical_address(void)
{
    struct task_struct *x;
    printk("Given PID: %d and VA: %lu\n",PID,VA);

    for_each_process(x)
    {
        if(x->pid == PID)
        {
            pgd_t *pgd_entry_pointer = pgd_offset(x->mm, VA);

            if (pgd_none(*pgd_entry_pointer)) {
                printk(KERN_ERR "Invalid PGD Entry\n");
                return 0;
            }

            p4d_t *p4d_entry_pointer = p4d_offset(pgd_entry_pointer, VA);
            if(p4d_none(*p4d_entry_pointer))
            {
                printk(KERN_ERR "Invalid P4D Entry\n");
                return 0;
            }
        
            pud_t *pud_entry_pointer = pud_offset(p4d_entry_pointer, VA);
            if(pud_none(*pud_entry_pointer))
            {
                printk(KERN_ERR "Invalid PUD Entry\n");
                return 0;
            }
        
            pmd_t *pmd_entry_pointer = pmd_offset(pud_entry_pointer, VA);
            if(pmd_none(*pmd_entry_pointer))
            {
                printk(KERN_ERR "Invalid PMD Entry\n");
                return 0;
            }
        
            pte_t *pte_entry_pointer = pte_offset_kernel(pmd_entry_pointer, VA);
            if(pte_none(*pte_entry_pointer))
            {
                printk(KERN_ERR "Invalid PTE Entry\n");
                return 0;
            }
            
            int page_offset = (~PAGE_MASK) & VA;
            int frame_number = pte_val(*pte_entry_pointer) & PAGE_MASK;
            long int PA = frame_number | page_offset;
            return PA;
        }
    }
    return 0;
}

static long ioctl_function(struct file *f, unsigned int cmd, unsigned long arg)
{    
    switch (cmd)
    {
        // User passes PID and virtual address to the kernel
        case GET_PHYSICAL_ADDRESS:
            virt_addr v;
            if (copy_from_user(&v, (virt_addr *)arg, sizeof(virt_addr)))
            {
                return -1;
            }
            PID = v.PID;
            VA = v.virtual_address;

            // get the physical address from virtual address
            v.physical_address = get_physical_address();

            // Give physical address to user
            if (copy_to_user((virt_addr *)arg, &v, sizeof(virt_addr)))
            {
                return -1;
            }
            break;
        
        // User Passes Physical address and value to kernel
        case PASS_PHYSICAL_ADDRESS_AND_VALUE:
            phys_value p;
            if (copy_from_user(&p, (phys_value *)arg, sizeof(phys_value)))
            {
                return -1;
            }

            // get kernel virtual address from the physical address using phys_to_virt
            char * kernel_virt_addr = phys_to_virt(p.physical_address);
            printk("Virtual Address: %lu\n", kernel_virt_addr);

            // Change value to 5
            *kernel_virt_addr = 5;
            break;
    }
    return 0;
}
 
static struct file_operations query_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = ioctl_function
};
 
static int __init ioctl_value_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "declarations_ioctl")) < 0)
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
 
static void __exit ioctl_value_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}
 
module_init(ioctl_value_init);
module_exit(ioctl_value_exit);