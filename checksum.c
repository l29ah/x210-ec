#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push)
#pragma pack(1)
struct HFCGP
{
	uint8_t CPRED:4;
	uint8_t FPRED:4;
	
};
struct header_data
{
	uint16_t signature;
	uint16_t reserved_0x02;
	uint8_t HCBAL_DAT;
	uint8_t HCBAH_DAT;
	uint16_t reserved_0x06;
	uint8_t HFCGML_DAT;
	uint8_t HFCGMH_DAT;
	uint8_t HFCGN_DAT;
	struct HFCGP HFCGP_DAT;
	uint16_t reserved_0x0c;
	uint8_t FL_SIZE_DAT;
	uint8_t BURST_DAT;
	uint8_t SPI_FL_DAT;
	uint8_t MISC_CTL;
	uint16_t reserved_0x12;
	uint32_t FIRMW_START;
	uint32_t FIRMW_SIZE;
	uint16_t reserved_0x1c[(0x40 - 0x1c)/2];
	uint16_t HEADER_CKSM;
	uint16_t reserved_0x42;
	uint32_t FIRMW_CKSM;
};
#pragma pack(pop)



void main(int argc,char ** argv)
{

	uint16_t csum=0;
	uint8_t buf[0x100];
	uint32_t csum2=0;
	size_t sz,k;
	uint8_t * buf2;

	if(argc!=2)
	{
		printf("usage:\n%s filename\n",argv[0]);
		exit(0);
	}
	FILE * fd=fopen(argv[1],"r+");
	if(!fd)
	{
		printf("failed to open a file %s\n",argv[1]);
		exit(1);
	}
	struct header_data *hd=(struct header_data *)&buf[0];
	sz=fread(buf,0x100,1,fd);
	if(sz!=1)
	{
		printf("failed to read a file %s\n",argv[1]);
		exit(1);
	}
	for(k=0;k<0x3f;k++)
		csum+=buf[k];
	printf("size=%ld\n\n",sizeof(struct header_data));
	
	printf(
		"signature=%04x\n"
		"reserved_0x02=%04x\n"
		"HCBAL_DAT=%02x\n"
		"HCBAH_DAT=%02x\n"
		"reserved_0x06=%04x\n"
		"HFCGML_DAT=%02x\n"
		"HFCGMH_DAT=%02x\n"
		"HFCGN_DAT=%02x\n"
		"HFCGP_DAT.CPRED=%02x\n"
		"HFCGP_DAT.FPRED=%02x\n"
		"reserved_0x0c=%04x\n"
		"FL_SIZE_DAT=%02x\n"
		"BURST_DAT=%02x\n"
		"SPI_FL_DAT=%02x\n"
		"MISC_CTL=%02x\n"
		"reserved_0x12=%04x\n"
		"FIRMW_START=%08x\n"
		"FIRMW_SIZE=%08x\n"
		"HEADER_CKSM=%04x\n"
		"reserved_0x42=%04x\n"
		"FIRMW_CKSM=%08x\n",
		(unsigned int)hd->signature,(unsigned int)hd->reserved_0x02,(unsigned int)hd->HCBAL_DAT,(unsigned int)hd->HCBAH_DAT,(unsigned int)hd->reserved_0x06,
		(unsigned int)hd->HFCGML_DAT,(unsigned int)hd->HFCGMH_DAT,(unsigned int)hd->HFCGN_DAT,(unsigned int)hd->HFCGP_DAT.CPRED,(unsigned int)hd->HFCGP_DAT.FPRED,
		(unsigned int)hd->reserved_0x0c,
		(unsigned int)hd->FL_SIZE_DAT,(unsigned int)hd->BURST_DAT,(unsigned int)hd->SPI_FL_DAT,(unsigned int)hd->MISC_CTL,(unsigned int)hd->reserved_0x12,(unsigned int)hd->FIRMW_START,
		(unsigned int)hd->FIRMW_SIZE,(unsigned int)hd->HEADER_CKSM,(unsigned int)hd->reserved_0x42,(unsigned int)hd->FIRMW_CKSM);
	printf("Calculated hearder crc=%04x\n",csum);
	fseek(fd,hd->FIRMW_START-0x020000,SEEK_SET);
	buf2=malloc(hd->FIRMW_SIZE*2);
	if(buf2==NULL)
	{
		printf("failed to allocate memory %u\n",hd->FIRMW_SIZE);
		exit(1);
	}
	sz=fread(buf2,hd->FIRMW_SIZE*2,1,fd);
	if(sz!=1)
	{
		printf("failed to read a file %s\n",argv[1]);
    	free(buf2);
		exit(1);
	}
	for(k=0;k<hd->FIRMW_SIZE*2;k++)
		csum2+=buf2[k];
	printf("Calculated firmware crc=%08x\n",csum2);
	if( (csum == hd->HEADER_CKSM) && (csum2 == hd->FIRMW_CKSM))
		printf("Checksums OK\n");
	else{
		hd->HEADER_CKSM=csum;
		hd->FIRMW_CKSM=csum2;
		fseek(fd,0,SEEK_SET);
		if(fwrite(hd,sizeof(struct header_data),1,fd)==1)
    		printf("Checksums Updated\n");
        else
            printf("Checksum update failed.\n");
	}
	free(buf2);
	
	fclose(fd);
}