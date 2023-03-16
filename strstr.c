/*********************************************************************************
 *      Copyright:  (C) 2022 Zhu Lijun
 *                  All rights reserved.
 *
 *       Filename:  strstr.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/17/22)
 *         Author:  Zhu Lijun <3262465970@qq.com>
 *      ChangeLog:  1, Release initial version on "10/17/22 11:27:55"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
 
int main() {
	    char *s = "GoldenGlobalView";
		    char *l = "lob";
			    char *p;
				    p = strstr(s, l);
					    if (p)
						{    
							printf("%s\n", p);
						}
						else
						{	
								printf("NotFound!\n");
						}	     
		

							getchar();
			    return 0;
}
