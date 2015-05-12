/*
 This file is part of program wsprd, a detector/demodulator/decoder
 for the Weak Signal Propagation Reporter (WSPR) mode.
 
 File name: wsprd_utils.c

 Copyright 2001-2015, Joe Taylor, K1JT

 Most of the code is based on work by Steven Franke, K9AN, which
 in turn was based on earlier work by K1JT.

 Copyright 2014-2015, Steven Franke, K9AN

 License: GNU GPL v3
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef int32_t
#define int32_t int
#endif

void unpack50( signed char *dat, int32_t *n1, int32_t *n2 )
{
  int32_t i,i4;
    
  i=dat[0];
  i4=i&255;
  *n1=i4<<20;
    
  i=dat[1];
  i4=i&255;
  *n1=*n1+(i4<<12);
    
  i=dat[2];
  i4=i&255;
  *n1=*n1+(i4<<4);
    
  i=dat[3];
  i4=i&255;
  *n1=*n1+((i4>>4)&15);
  *n2=(i4&15)<<18;
    
  i=dat[4];
  i4=i&255;
  *n2=*n2+(i4<<10);

  i=dat[5];
  i4=i&255;
  *n2=*n2+(i4<<2);
    
  i=dat[6];
  i4=i&255;
  *n2=*n2+((i4>>6)&3);
}

void unpackcall( int32_t ncall, char *call )
{
  char c[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E',
	    'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
	    'U','V','W','X','Y','Z',' '};
  int32_t n;
  int i;
  char tmp[7];
    
  n=ncall;
  strcpy(call,"......");
  if (n < 262177560 ) {
    i=n%27+10;
    tmp[5]=c[i];
    n=n/27;
    i=n%27+10;
    tmp[4]=c[i];
    n=n/27;
    i=n%27+10;
    tmp[3]=c[i];
    n=n/27;
    i=n%10;
    tmp[2]=c[i];
    n=n/10;
    i=n%36;
    tmp[1]=c[i];
    n=n/36;
    i=n;
    tmp[0]=c[i];
    tmp[6]='\0';
// remove leading whitespace
    for(i=0; i<5; i++) {
      if( tmp[i] != c[36] )
	break;
    }
    sprintf(call,"%-6s",&tmp[i]);
// remove trailing whitespace
    for(i=0; i<6; i++) {
      if( call[i] == c[36] ) {
	call[i]='\0';
      }
    }
  }
}

void unpackgrid( int32_t ngrid, char *grid)
{
  char c[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E',
	    'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
	    'U','V','W','X','Y','Z',' '};
  int dlat, dlong;
    
  ngrid=ngrid>>7;
  if( ngrid < 32400 ) {
    dlat=(ngrid%180)-90;
    dlong=(ngrid/180)*2 - 180 + 2;
    if( dlong < -180 )
      dlong=dlong+360;
    if( dlong > 180 )
      dlong=dlong+360;
    int nlong = 60.0*(180.0-dlong)/5.0;
    int n1 = nlong/240;
    int n2 = (nlong - 240*n1)/24;
    grid[0] = c[10+n1];
    grid[2]=  c[n2];

    int nlat = 60.0*(dlat+90)/2.5;
    n1 = nlat/240;
    n2 = (nlat-240*n1)/24;
    grid[1]=c[10+n1];
    grid[3]=c[n2];
  } else {
    strcpy(grid,"XXXX");
  }
}

void unpackpfx( int32_t nprefix, char *call)
{
  char nc, pfx[4]="", tmpcall[7]="";
  int i;
  int32_t n;
    
  strcpy(tmpcall,call);

  if( nprefix < 60000 ) {
// add a prefix of 1 to 3 characters
    n=nprefix;
    for (i=2; i>=0; i--) {
      nc=n%37;
      if( (nc >= 0) & (nc <= 9) ) {
	pfx[i]=nc+48;
      }
      else if( (nc >= 10) & (nc <= 35) ) {
	pfx[i]=nc+55;
      }
      else {
	pfx[i]=' ';
      }
      n=n/37;
    }

    strcpy(call,pfx);
    strncat(call,"/",1);
    strncat(call,tmpcall,strlen(tmpcall));

  } else {
// add a suffix of 1 or 2 characters
    nc=nprefix-60000;
    if( (nc >= 0) & (nc <= 9) ) {
      pfx[0]=nc+48;
      strcpy(call,tmpcall);
      strncat(call,"/",1);
      strncat(call,pfx,1);
    }
    else if( (nc >= 10) & (nc <= 35) ) {
      pfx[0]=nc+55;
      strcpy(call,tmpcall);
      strncat(call,"/",1);
      strncat(call,pfx,1);
    }
    else if( (nc >= 36) & (nc <= 125) ) {
      pfx[0]=(nc-26)/10+48;
      pfx[1]=(nc-26)%10+48;
      strcpy(call,tmpcall);
      strncat(call,"/",1);
      strncat(call,pfx,2);
    }
  }  
}

void deinterleave(unsigned char *sym)
{
  unsigned char tmp[162];
  unsigned char p, i, j;

  p=0;
  i=0;
  while (p<162) {
    j=((i * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
    if (j < 162 ) {
      tmp[p]=sym[j];
      p=p+1;
    }
    i=i+1;
  }
  for (i=0; i<162; i++) {
        sym[i]=tmp[i];
  }
}

// used by qsort
int floatcomp(const void* elem1, const void* elem2)
{
  if(*(const float*)elem1 < *(const float*)elem2)
    return -1;
  return *(const float*)elem1 > *(const float*)elem2;
}
