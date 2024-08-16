#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/list.h>
#include <linux/mm_types.h>
#include <asm/pgtable.h>
#include<linux/mm.h>
#include <linux/maple_tree.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het");
MODULE_DESCRIPTION("Module gives total VMAs and mapped PAS for given PID");

// Take PID from command line
static int PID;
module_param(PID, int, S_IRUGO);

// Calculates total mapped Physical address space
unsigned long int calculate_physical_address_space(struct mm_struct * mem_map)
{
    unsigned long int file_pages = get_mm_counter(mem_map, MM_FILEPAGES);
    unsigned long int anonymous_pages = get_mm_counter(mem_map, MM_ANONPAGES);
    unsigned long int swap_pages = get_mm_counter(mem_map, MM_SWAPENTS);
    unsigned long int shared_pages = get_mm_counter(mem_map, MM_SHMEMPAGES);

    printk(KERN_INFO "File Pages: %lu\n",file_pages);
    printk(KERN_INFO "Anonymous Pages: %lu\n",anonymous_pages);
    printk(KERN_INFO "Swap Pages: %lu\n", swap_pages);
    printk(KERN_INFO "Shared Pages: %lu\n",shared_pages);
    return file_pages + anonymous_pages + swap_pages + shared_pages;
}

static int __init count_total_VMAs_init(void) {
    
    struct task_struct *x;
    for_each_process(x)
    {
        // for given PID find it's sum of all VMAs and physical memory size
        if(x->pid == PID)
        {
            printk(KERN_INFO "Sum of all VMAs: %lu KB\n", x->mm->total_vm * 4);
            printk(KERN_INFO "Total Mapped Physical Address Space: %lu KB\n", calculate_physical_address_space(x->mm) * 4);
        }
    }
    return 0;
}

static void __exit count_total_VMAs_exit(void) {
    printk("Module removed\n");
}

module_init(count_total_VMAs_init);
module_exit(count_total_VMAs_exit);
