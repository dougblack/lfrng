

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm/uaccess.h>

#define thread_id_name "lfrng_seed"
#define min_name "lfrng_min"
#define max_name "lfrng_max"
#define nthreads_name "nthreads"

#define PROCFS_MAX_SIZE 1024

// PROC ENTRIES
struct proc_dir_entry *nthreads_entry;
struct proc_dir_entry *thread_id_entry;

static unsigned long long MULTIPLIER  = 764261123;
static unsigned long long PMOD        = 2147483647;
static unsigned long long mult_n;
double random_low, random_hi;

int nthreads_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
   int ret;

   printk(KERN_INFO "procfile_read (/proc/%s) called \n", nthreads_name);

   if (offset > 0) {
      ret = 0;
   } else {
      struct task_struct *task;
      int number_of_tasks = 1;
      for (task = next_thread(current); task != current; task = next_thread(task)) {
         number_of_tasks++; 
      }
      ret = sprintf(buffer, "Number of tasks: %d\n", number_of_tasks);
   }
   return ret;
}

int thread_id_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
   int ret;

   printk(KERN_INFO "procfile_read (/proc/%s) called \n", nthreads_name);

   if (offset > 0) {
      ret = 0;
   } else {
      int process_id = current->pid;
      ret = sprintf(buffer, "CURRENT PID: %d\n", process_id);
   }
   return ret;
}

int init_module()
{
   nthreads_entry = create_proc_entry(nthreads_name, 0644, NULL);
   thread_id_entry = create_proc_entry(thread_id_name, 0644, NULL);

   if (nthreads_entry == NULL || thread_id_entry == NULL) {
      remove_proc_entry(thread_id_name, NULL);
      remove_proc_entry(nthreads_name, NULL);
      printk(KERN_ALERT "Error: Could not initialize lfrng procs.");
      return -ENOMEM;
   }

   nthreads_entry->read_proc = nthreads_read;
   nthreads_entry->owner = THIS_MODULE;
   nthreads_entry->mode = S_IFREG | S_IRUGO;
   nthreads_entry->uid = 0;
   nthreads_entry->gid = 0;
   nthreads_entry->size = 37;

   //thread_id_entry->read_proc = thread_num_read;
   thread_id_entry->owner = THIS_MODULE;
   thread_id_entry->mode = S_IFREG | S_IRUGO;
   thread_id_entry->uid = 0;
   thread_id_entry->gid = 0;
   thread_id_entry->size = 37;

   printk(KERN_INFO "/proc/%s created\n", thread_id_name);
   return 0;
}

void cleanup_module()
{
   remove_proc_entry(nthreads_name, NULL);
   remove_proc_entry(thread_id_name, NULL);
   printk(KERN_INFO "/proc/%s removed\n", thread_id_name);
}

