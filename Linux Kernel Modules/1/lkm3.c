#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/list.h>
#include <linux/mm_types.h>
#include <asm/pgtable.h>
#include<linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het Patel");
MODULE_DESCRIPTION("Module Printing Physical address, given PID and virtual address");

// Take PID and VA from command line
static int PID;
static uintptr_t VA;
module_param(PID, int, S_IRUGO);
module_param(VA, ulong, S_IRUGO);

// Load Module
static int __init get_physical_address_init(void) {
    
    struct task_struct *x;
    printk("Given PID: %d and VA: %lu\n",PID,VA);
    for_each_process(x)
    {
        // Check if this task_struct is of given PID
        if(x->pid == PID)
        {
            // Get the pointer to the matched entry in PGD 
            pgd_t *pgd_entry_pointer = pgd_offset(x->mm, VA);

            // Check if PGD entry is empty or not
            if (pgd_none(*pgd_entry_pointer)) {
                printk(KERN_ERR "Invalid PGD Entry\n");
                return 0;
            }

            printk(KERN_INFO "PGD Entry: %lX\n", pgd_val(*pgd_entry_pointer));

            p4d_t *p4d_entry_pointer = p4d_offset(pgd_entry_pointer, VA);
            if(p4d_none(*p4d_entry_pointer))
            {
                printk(KERN_ERR "Invalid P4D Entry\n");
                return 0;
            }
            printk(KERN_INFO "P4D Entry: %lX\n", p4d_val(*p4d_entry_pointer));

            pud_t *pud_entry_pointer = pud_offset(p4d_entry_pointer, VA);
            if(pud_none(*pud_entry_pointer))
            {
                printk(KERN_ERR "Invalid pud\n");
                return 0;
            }
            printk(KERN_INFO "PUD Entry: %lX\n", pud_val(*pud_entry_pointer));

            pmd_t *pmd_entry_pointer = pmd_offset(pud_entry_pointer, VA);
            if(pmd_none(*pmd_entry_pointer))
            {
                printk(KERN_ERR "Invalid pmd\n");
                return 0;
            }
            printk(KERN_INFO "PMD Entry: %lX\n", pmd_val(*pmd_entry_pointer));

            pte_t *pte_entry_pointer = pte_offset_kernel(pmd_entry_pointer, VA);
            if(pte_none(*pte_entry_pointer))
            {
                printk(KERN_ERR "Invalid pte\n");
                return 0;
            }
            printk(KERN_INFO "PTE Entry: %lX\n", pte_val(*pte_entry_pointer));
            
            int page_offset = (~PAGE_MASK) & VA;
            int frame_number = pte_val(*pte_entry_pointer) & PAGE_MASK;
            long int PA = frame_number | page_offset;

            printk(KERN_INFO "Physical Address: %lX\n", PA);
        }
    }
    return 0;
}

// Remove module
static void __exit get_physical_address_exit(void) {
    printk("Module removed\n");
}

module_init(get_physical_address_init);
module_exit(get_physical_address_exit);
