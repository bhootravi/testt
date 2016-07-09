#include "file_handling.h"

FATFS fs;

//TODO set timeout
int fs_mount(void)
{
	Ctrl_status status;
	FRESULT res;
	/* Wait card present and ready */
	do {
		status = sd_mmc_test_unit_ready(0);
		if (CTRL_FAIL == status) {
			printf("Card install FAIL\n\r");
			printf("Please unplug and re-plug the card.\n\r");
			//TODO set timeout here
			while (CTRL_NO_PRESENT != sd_mmc_check(0));
		}
		//TODO set timeout here
	} while (CTRL_GOOD != status);
	
	/* Mount the file sytem */
	printf("Mount disk (f_mount)...\r\n");
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	if (FR_INVALID_DRIVE == res) {
		printf("[FAIL] res %d\r\n", res);
		return res;
	}
	#define FS_MOUNTED
	printf("mounting [OK]\r\n");
	return res;
}