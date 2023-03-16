/*********************************************************************************
 *      Copyright:  (C) 2022 Zhu Lijun
 *                  All rights reserved.
 *
 *       Filename:  ds18b20.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/16/22)
 *         Author:  Zhu Lijun <3262465970@qq.com>
 *      ChangeLog:  1, Release initial version on "10/16/22 11:11:01"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int get_temperature(float *temper);
int main (int argc,char **argv)
{
	float 		temper;
	int 		rv;
	rv =get_temperature(&temper);
	
	if (rv<0)
	{
		printf("get temperature failure retunrn value:%d",rv);
	}
}

int get_temperature(float *temper)
{
	int             fd=-1;
	char            buf[128];
	char            *ptr=NULL;
	DIR             *dirp=NULL;
	char            w1_path[64]="/sys/bus/w1/devices/";
	struct dirent   *direntp=NULL;

	char            chip_sn[32];

	char            ds18b20_path[64];

	int             found=0;


	dirp=opendir(w1_path);

	if(!dirp)

	{	
		printf("open folser %s failure:%s\n",w1_path,strerror(errno));

		return -1;

	}



	while(NULL !=(direntp=readdir(dirp)))

	{

		if(strstr(direntp->d_name,"28-"))

				{

				strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));

				found=1;

				}
	}


				closedir(dirp);

				if(!found)

				{

					printf("can not find de18b20 chipset\n");

					return -2;

				}

		//snprintf(ds18b20_path,sizeof(ds18b20_path),"%s/%s/w1_slave",w1_path,chip_sn);

		//printf("w1_path: %s\n",w1_path);
       strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));

	   strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

		//printf("w1_path: %s\n",w1_path);


       if((  fd=open(w1_path,O_RDONLY))<0)
	   {
	    printf("Open file failure:%s\n",strerror(errno));
		return -1;
			}

		memset(buf,0,sizeof(buf));
			
		if(read(fd,buf,sizeof(buf))<0)
		{
			printf("read data from fd=%d failure:%s\n",fd,strerror(errno));
			return -2; 
		}
		
		
		//printf("buf:%s\n",buf);
				ptr	=strstr(buf,"t=");
				
				if(!ptr)
				{	
		printf("can not find t= string\n");
				return -1;
				}
				ptr+=2;
		//		printf("ptr:%s\n",ptr);
				atof(ptr); 
				*temper=atof(ptr)/1000; 
				printf("temper is: %f\n",*temper);
				write(fd,buf,sizeof(buf));
				close(fd);
				return     0;
	}
				







