#ifndef FILE_HANDLING_H_
#define FILE_HANDLING_H_

#include "asf.h"
#include <string.h>

// common variable to access mounted file system
extern FATFS fs;

//mounts a file system which can be accessed through the fs var
//also executes #define FS_MOUNTED if mount is succesful for future reference
int fs_mount(void);

#endif /* FILE_HANDLING_H_ */