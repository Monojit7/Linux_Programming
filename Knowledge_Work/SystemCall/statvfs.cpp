#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>

int
get_fs_usage (char const *file, char const *disk)
{

      struct statvfs vfsd;

      if (statvfs (file, &vfsd) < 0)
        return -1;

      /* f_frsize isn't guaranteed to be supported.  */


      printf("[statvfs]:  file : %s, disk: %s, fsu_bfree: %d, fsu_blocks: %d, fsu_bavail: %d, fsu_file: %d, fsu_ffree: %d, filesystem id: %x \n", file, disk, vfsd.f_bfree, vfsd.f_blocks, vfsd.f_bavail,vfsd.f_files, vfsd.f_ffree, vfsd.f_fsid);

struct statfs fsd;

if( statfs(file, &fsd) < 0 )
  return -1;


printf("[statfs]: filesystem type: %x \n", fsd.f_type);

return 0;

}




int main(int argc, char** argv)
{

printf("Exceuting Main of statvfs \n");


printf("calling get fs usage \n");
get_fs_usage (argv[1], argv[2]); 

}
