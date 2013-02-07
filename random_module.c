

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define procfs_name "lfrng"

static unsigned long long MULTIPLIER  = 764261123;
static unsigned long long PMOD        = 2147483647;
static unsigned long long mult_n;
double random_low, random_hi;

struct proc_dir_entry *lfrng_entry;



int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
   int ret;

   printk(KERN_INFO "procfile_read (/proc/%s) called \n", procfs_name);

   if (offset > 0) {
      ret = 0;
   } else {
      //ret = our_random();
      ret = sprintf(buffer, "HELLO\n");
   }

   return ret;
}

int init_module()
{
   lfrng_entry = create_proc_entry(procfs_name, 0644, NULL);

   if (lfrng_entry == NULL) {
      remove_proc_entry(procfs_name, NULL);
      printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
      return -ENOMEM;
   }

   lfrng_entry->read_proc = procfile_read;
   lfrng_entry->owner = THIS_MODULE;
   lfrng_entry->mode = S_IFREG | S_IRUGO;
   lfrng_entry->uid = 0;
   lfrng_entry->gid = 0;
   lfrng_entry->size = 37;

   printk(KERN_INFO "/proc/%s created\n", procfs_name);
   return 0;
}

void cleanup_module()
{
   remove_proc_entry(procfs_name, NULL);
   printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
