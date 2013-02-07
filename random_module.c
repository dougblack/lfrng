

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/proc_fs.h>

#define seed_name "lfrng_seed"
#define min_name "lfrng_min"
#define max_name "lfrng_max"
#define rand_name "lfrng_rand"

static unsigned long long MULTIPLIER  = 764261123;
static unsigned long long PMOD        = 2147483647;
static unsigned long long mult_n;
double random_low, random_hi;

struct proc_dir_entry *seed_entry;
struct proc_dir_entry *min_entry;
struct proc_dir_entry *max_entry;
struct proc_dir_entry *rand_entry;



int read_rand(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
   int ret;

   printk(KERN_INFO "procfile_read (/proc/%s) called \n", seed_name);

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
   seed_entry = create_proc_entry(seed_name, 0644, NULL);
   max_entry = create_proc_entry(max_name, 0644, NULL);
   min_entry = create_proc_entry(min_name, 0644, NULL);
   rand_entry = create_proc_entry(rand_name, 0644, NULL);

   if (seed_entry == NULL || max_entry == NULL || min_entry == NULL || rand_entry == NULL) {
      remove_proc_entry(seed_name, NULL);
      remove_proc_entry(min_name, NULL);
      remove_proc_entry(max_name, NULL);
      remove_proc_entry(rand_name, NULL);
      printk(KERN_ALERT "Error: Could not initialize lfrng procs.");
      return -ENOMEM;
   }

//   seed_entry->write_proc = write_seed;
   seed_entry->owner = THIS_MODULE;
   seed_entry->mode = S_IFREG | S_IRUGO;
   seed_entry->uid = 0;
   seed_entry->gid = 0;
   seed_entry->size = 37;

//   min_entry->write_proc = write_min;
   min_entry->owner = THIS_MODULE;
   min_entry->mode = S_IFREG | S_IRUGO;
   min_entry->uid = 0;
   min_entry->gid = 0;
   min_entry->size = 37;

//   max_entry->write_proc = write_max;
   max_entry->owner = THIS_MODULE;
   max_entry->mode = S_IFREG | S_IRUGO;
   max_entry->uid = 0;
   max_entry->gid = 0;
   max_entry->size = 37;

   rand_entry->read_proc = read_rand;
   rand_entry->owner = THIS_MODULE;
   rand_entry->mode = S_IFREG | S_IRUGO;
   rand_entry->uid = 0;
   rand_entry->gid = 0;
   rand_entry->size = 37;

   printk(KERN_INFO "/proc/%s created\n", seed_name);
   return 0;
}

void cleanup_module()
{
   remove_proc_entry(rand_name, NULL);
   remove_proc_entry(seed_name, NULL);
   remove_proc_entry(max_name, NULL);
   remove_proc_entry(min_name, NULL);
   printk(KERN_INFO "/proc/%s removed\n", seed_name);
}

