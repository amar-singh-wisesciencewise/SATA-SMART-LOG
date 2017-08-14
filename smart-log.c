#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<scsi/sg.h>
#include<scsi/scsi_ioctl.h>
#include<string.h>
#include <string.h>

#define BUFFER_SIZE 512
#define SENSE_BUFFER_SIZE 32
#define CMD_LENGTH 12

int main(int argc, char* argv[]){
	int fd = 0;
	unsigned char buffer[BUFFER_SIZE];
	unsigned char sense_buffer[SENSE_BUFFER_SIZE];
///////////////////forming SMART READ CDB/////////////////
	unsigned char smart_read_cdb[CMD_LENGTH] = {
				0xa1,
				0x0c,
				0x0e,
				0xd0,
				1,
				0,
				0x4f,
				0xc2,
				0,
				0xb0,
				0,
				0};

	sg_io_hdr_t io_hdr;
	char* device_file_name = 0;

	if(argc<2){
		printf("please enter a device file\n");
		return 1;
	}else device_file_name = argv[1];

/////////////opening device file//////////////////

	if((fd = open(device_file_name, O_RDONLY)) < 0 ){
		printf("could not open device file\n");
		return 1;
	}
//////////////////initialize your buffer///////////////

	for(register int i = 0 ; i < BUFFER_SIZE; buffer[i++] = 0 );
	
//////////prepare the CDB ////////////////////////////
	memset(&io_hdr,0,sizeof(sg_io_hdr_t));
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = CMD_LENGTH;
	io_hdr.mx_sb_len = SENSE_BUFFER_SIZE;
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len = BUFFER_SIZE;
	io_hdr.dxferp = buffer;
	io_hdr.cmdp = smart_read_cdb;
	io_hdr.sbp = sense_buffer;
	io_hdr.timeout = 20000;

/////////////// calling IOCTL///////////////////////////
	if(ioctl(fd,SG_IO,&io_hdr) < 0){
		printf("IOCTL CALL FAILED\n");
		return 1;
	}

////////////////////extracting SMART attributes/////////////
	double raw;
	int id;
	
	printf("ID\t\tONLINE-OFFLINE\t\tRAW-VALUE\n");
	for(register int i = 2;i < 361 ; i = i+12 ){
		id = (int)buffer[i];
		printf("%d\t\t", id);
		
		if(buffer[i+1] & 0x00000002)
			printf("ONLINE+OFFLINE\t\t");
		else printf("OFFLINE       \t\t");
			
		raw = 0;
		raw = (long long int)((buffer[i+5])|(buffer[i+6]<<8)|(buffer[i+7]<<16)|(buffer[i+8]<<24)|((long long int)buffer[i+9]<<32)|((long long int)buffer[i+10]<<40));	
		printf("%lld",(long long int)raw);
		printf("\n");	
	}
return 0;
}
