#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define procfs_name "helloworld"

struct proc_dir_entry *The_Proc_File;

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;

    printk(KERN_INFO "procfile_read (/proc/%s) called \n", procfs_name);

    if (offset > 0) {
        ret = 0;
    } else {
        ret = sprintf(buffer, "HELLO\n");
    }

    return ret;
}

int init_module()
{
    The_Proc_File = create_proc_entry(procfs_name, 0644, NULL);

    if (The_Proc_File == NULL) {
        remove_proc_entry(procfs_name, NULL);
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
        return -ENOMEM;
    }

    The_Proc_File->read_proc = procfile_read;
    The_Proc_File->owner = THIS_MODULE;
    The_Proc_File->mode = S_IFREG | S_IRUGO;
    The_Proc_File->uid = 0;
    The_Proc_File->gid = 0;
    The_Proc_File->size = 37;

    printk(KERN_INFO "/proc/%s created\n", procfs_name);
    return 0;
}

void cleanup_module()
{
    remove_proc_entry(procfs_name, NULL);
    printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
