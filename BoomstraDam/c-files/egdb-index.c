/*
 * Copyright 1996-2004 by Michel D. Grimminck
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 This files takes care of the index-function for the endgame databases.
 NB The indices are not compible with dragon version 1.1 and earlier.

 The index function has some (space) inefficiencies. This is a trade-off against
 simplicity and computation speed. 
*/

#include <stdio.h>
#include "egdb-var.h"
#include "egdb-const.h"
#include "BoomstraDam.v1.2.h"


int list[8][12];
//    
///*
//DBINDEX fac(int a)
//{
//    int i;
//    DBINDEX result=1;
//    if (a<2) return(1);
//    for(i=2;i<=a;i++) {
//        result*=i;
//    }
//    return(result);
//}
//*/

DBINDEX MULT[64][64];
int I2[64];
int I3[64];
int I4[64];
int I5[64];

int SLW[50]={
    0,0,0,0,0,
    40,40,40,40,40,
    35,35,35,35,35,
    30,30,30,30,30,
    25,25,25,25,25,
    20,20,20,20,20,
    15,15,15,15,15,
    10,10,10,10,10,
    5,5,5,5,5,
    0,0,0,0,0
};

int SLB[50]={
    0,0,0,0,0,
    5,5,5,5,5,
    10,10,10,10,10,
    15,15,15,15,15,
    20,20,20,20,20,
    25,25,25,25,25,
    30,30,30,30,30,
    35,35,35,35,35,
    40,40,40,40,40,
    0,0,0,0,0
};

DBINDEX mult(int top,int bot)
{
	DBINDEX res=1LL;
	DBINDEX i;
	
	if (bot<0) return 0;
	for (i=bot+1;i<=top;i++) res*=i;
	for (i=2;i<=(top-bot);i++) res/=i;
	return res;
}

void init_index()
{
    int i,j,i0;
    int a;
    
    for (i=0;i<=50;i++) {
        i0=i-10;
        if (i0<0) i0=0;
        for (j=0;j<12;j++) MULT[i][j]=0;
        for (j=i0;j<=i;j++) {
            MULT[i][j+12]=mult(i,j);
        }
    }
    for (a=0;a<=50;a++) {
        I2[a]= a*(a-1)/2;
        I3[a]= a*(a-1)*(a-2)/6;
        I4[a]= a*(a-1)*(a-2)*(a-3)/24;
        I5[a]= a*(a-1)*(a-2)*(a-3)*(a-4)/120;
    }
}

DBINDEX db_count(int wman,int wcrown,int bman,int bcrown,int ws,int bs)
/* returns the number of positions in our index function. (there are a large number of illegal position
   between these, but that is to make the indexing faster
*/
{
    DBINDEX count=0;
	DBINDEX cwm;
	DBINDEX cwc;
	DBINDEX cbm;
	DBINDEX cbc;
	int small=false;
	
	if (wman+wcrown+bman+bcrown<=5) small=true;
	
	cwc=mult(50,50-wcrown);
	cbc=mult(50,50-bcrown);
	cwm=1;
	if (wman>0) {
	    if (small==false) cwm=mult(5*ws+5,5*ws+5-wman)-mult(5*ws,5*ws-wman);
	    if (small==true) cwm=mult(45,45-wman);
	}
	cbm=1;
	if (bman>0) {
	    if (small==false) cbm=mult(5*bs+5,5*bs+5-bman)-mult(5*bs,5*bs-bman);
	    if (small==true) cbm=mult(45,45-bman);
	}
	
    return(cwc*cbc*cwm*cbm);
}
//#define I2(a) a*(a-1)/2
//#define I3(a) a*(a-1)*(a-2)/6
//#define I4(a) a*(a-1)*(a-2)*(a-3)/24
//#define I5(a) a*(a-1)*(a-2)*(a-3)*(a-4)/120

DBINDEX alt_database_linear_index(int color, int wman,unsigned long * wman_idx, 
							          int wcrown,unsigned long * wcrown_idx, 
									  int bman,unsigned long * bman_idx, 
									  int bcrown,unsigned long * bcrown_idx) 
									  
/* reads the 'board' array and determens the index function (not in bytes)
*/
{
    // index=blackmanindex.whitemanindex.blackcrownindex.whitecrownindex
    #define WM white|man
    #define WC white|crown
    #define BM black|man
    #define BC black|crown
    
    int npwm=0,npbm=0,npwc=0,npbc=0;  //number of white man, crown, etc.
    int p;
    int ws=0,bs=0;     // white,black slice
    int iwc=0;   // local index white crown
    int ibc=0;   // local index black crown
    int iwm=0;
    int ibm=0;
    int small=false;
    
    DBINDEX p1,p2,p3;
    DBINDEX index;
    
	if (color == white)
	{	for (p=0; p<wman;  p++){ list[white|man][npwm++]  = 49 - wman_idx[p]; if (SLW[wman_idx[p]]>ws) ws=SLW[wman_idx[p]];}
		for (p=0; p<wcrown; p++) list[white|crown][npwc++]= 49 - wcrown_idx[p];
		for (p=0; p<bman;  p++){ list[black|man][npbm++]  = bman_idx[p];if (SLB[bman_idx[p]]>bs) bs=SLB[bman_idx[p]];};
		for (p=0; p<bcrown; p++) list[black|crown][npbc++]= bcrown_idx[p];
	}
	else
	{	for (p=bman-1; p>=0;  p--) {list[white|man][npwm++]  = bman_idx[p];if (SLB[49-bman_idx[p]]>bs) bs=SLB[49-bman_idx[p]];};
		for (p=bcrown-1; p>=0; p--) list[white|crown][npwc++]= bcrown_idx[p];
		for (p=wman-1; p>=0;  p--) {list[black|man][npbm++]  = 49 - wman_idx[p];if (SLW[49-wman_idx[p]]>ws) ws=SLW[49-wman_idx[p]];};
		for (p=wcrown-1; p>=0; p--) list[black|crown][npbc++]= 49 - wcrown_idx[p];
	}

	printf("alt-mode:\n");
	if (npwm) {printf("wm: "); for (int i=0; i<npwm; i++) printf("%d, ", list[WM][i]); printf("\n");};
	if (npwc) {printf("wc: "); for (int i=0; i<npwc; i++) printf("%d, ", list[WC][i]); printf("\n");};
	if (npbm) {printf("bm: "); for (int i=0; i<npbm; i++) printf("%d, ", list[BM][i]); printf("\n");};
	if (npbc) {printf("bc: "); for (int i=0; i<npbc; i++) printf("%d, ", list[BC][i]); printf("\n");};

    //if (color==white) for(p=;p<50;p++) switch(board[map[p]]) {
    //        case white|man:{
    //                list[white|man][npwm++]=49-p;
    //                if (SLW[p]>ws) ws=SLW[p];
    //                break;
    //                }
    //        case white|crown:{
    //                list[white|crown][npwc++]=49-p;break;}
    //        case black|man:{
    //                list[black|man][npbm++]=p;
    //                if (SLB[p]>bs) bs=SLB[p];
    //                break;
    //                }
    //        case black|crown:{
    //                list[black|crown][npbc++]=p;break;}
    //        }
    //else for(p=49;p>=0;p--) switch(board[map[p]]) {
    //        case black|man:{
    //                list[white|man][npwm++]=p;
    //                if (SLW[49-p]>ws) ws=SLW[49-p];
    //                break;
    //                }
    //        case black|crown:{
    //                list[white|crown][npwc++]=p;break;}
    //        case white|man:{
    //                list[black|man][npbm++]=49-p;
    //                if (SLB[49-p]>bs) bs=SLB[49-p];
    //                break;
    //                }
    //        case white|crown:{
    //                list[black|crown][npbc++]=49-p;break;}
    //        }
    if (npwm+npwc+npbm+npbc<=5) small=true;

	//printf("nr of white crown = %d black = %d\n", npwc, npbc);
	//printf("nr of white man   = %d black = %d\n", npwm, npbm);

	//printf("white crown: %d, black crown: %d\n", list[white|crown][0], list[black|crown][0]);
     
    p3=MULT[50][50-npwc+12];
    p2=MULT[50][50-npbc+12]*p3;
    p1=p2;
    if (npwm>0) {
        if (small==false) { p1=(MULT[ws+5][ws+5-npwm+12]-MULT[ws][ws-npwm+12])*p2; }
        else { p1=MULT[45][45-npwm+12]*p2; }
    }	
//printf("%i %i %i\n",p1,npwm,small);
    switch(npwc) {
    	case 0: {
    	    iwc=0;
    	    break;
    	}
    	case 1: {
        	iwc=list[WC][0];
        	break;
    	}
    	case 2: {
    	    iwc=I2[list[WC][0]]+list[WC][1];
    	    break;
    	}
    	case 3: {
    	    iwc=I3[list[WC][0]]+I2[list[WC][1]]+list[WC][2];
    	    break;
    	}
    	case 4: {
    	    iwc=I4[list[WC][0]]+I3[list[WC][1]]+I2[list[WC][2]]+list[WC][3];
    	    break;
    	}
    	case 5: {
    	    iwc=I5[list[WC][0]]+I4[list[WC][1]]+I3[list[WC][2]]+I2[list[WC][3]]+list[WC][4];
    	    break;
    	}
    }
    
    switch(npbc) {
    	case 0: {
    	    ibc=0;
    	    break;
    	}
    	case 1: {
        	ibc=list[BC][0];
        	break;
    	}
    	case 2: {
    	    ibc=I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    	case 3: {
    	    ibc=I3[list[BC][2]]+I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    	case 4: {
    	    ibc=I4[list[BC][3]]+I3[list[BC][2]]+I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    	case 5: {
    	    ibc=I5[list[BC][4]]+I4[list[BC][3]]+I3[list[BC][2]]+I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    }

    switch(npwm) {
    	case 0: {
    	    iwm=0;
    	    break;
    	}
    	case 1: {
        	iwm=list[WM][0];
        	break;
    	}
    	case 2: {
    	    iwm=I2[list[WM][0]]+list[WM][1];
    	    break;
    	}
    	case 3: {
    	    iwm=I3[list[WM][0]]+I2[list[WM][1]]+list[WM][2];
    	    break;
    	}
    	case 4: {
    	    iwm=I4[list[WM][0]]+I3[list[WM][1]]+I2[list[WM][2]]+list[WM][3];
    	    break;
    	}
    	case 5: {
    	    iwm=I5[list[WM][0]]+I4[list[WM][1]]+I3[list[WM][2]]+I2[list[WM][3]]+list[WM][4];
    	    break;
    	}
    }
   	if (npwm>0 && small==false) iwm-=(int)MULT[ws][ws-npwm+12];

    switch(npbm) {
    	case 0: {
    	    ibm=0;
    	    break;
    	}
    	case 1: {
        	ibm=list[BM][0];
        	break;
    	}
    	case 2: {
    	    ibm=I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    	case 3: {
    	    ibm=I3[list[BM][2]]+I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    	case 4: {
    	    ibm=I4[list[BM][3]]+I3[list[BM][2]]+I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    	case 5: {
    	    ibm=I5[list[BM][4]]+I4[list[BM][3]]+I3[list[BM][2]]+I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    }
   	
   	if (npbm>0 && small==false) ibm-=(int)MULT[bs][bs-npbm+12];

    
    index=(ibm*p1)+(iwm*p2)+(ibc*p3)+iwc;
	if (index < 0) {
    printf(": %llu  %llu  %llu  %llu, slw=%i, slb=%i\n",ibm,iwm,ibc,iwc,ws,bs);
    printf("id:%llu\n",index);
	}

	return(index);
}
DBINDEX database_linear_index(int color, int wman,unsigned long * wman_idx, 
							          int wcrown,unsigned long * wcrown_idx, 
									  int bman,unsigned long * bman_idx, 
									  int bcrown,unsigned long * bcrown_idx) 
									  
/* reads the 'board' array and determens the index function (not in bytes)
*/
{
    // index=blackmanindex.whitemanindex.blackcrownindex.whitecrownindex
    #define WM white|man
    #define WC white|crown
    #define BM black|man
    #define BC black|crown
    
    int npwm=0,npbm=0,npwc=0,npbc=0;  //number of white man, crown, etc.
    int p;
    int ws=0,bs=0;     // white,black slice
    int iwc=0;   // local index white crown
    int ibc=0;   // local index black crown
    int iwm=0;
    int ibm=0;
    int small=false;
    
    DBINDEX p1,p2,p3;
    DBINDEX index;
    

	//for(p=0;p<50;p++) printf("map[%d]=%d\n", p, map[p]);
    if (color==white) for(p=0;p<50;p++) switch(board[map[p]]) {
            case white|man:{
					//printf("white man on: %d\n", p);
                    list[white|man][npwm++]=49-p;
                    if (SLW[p]>ws) ws=SLW[p];
                    break;
                    }
            case white|crown:{
                    list[white|crown][npwc++]=49-p;break;}
            case black|man:{
					//printf("black man on: %d\n", p);
                    list[black|man][npbm++]=p;
                    if (SLB[p]>bs) bs=SLB[p];
                    break;
                    }
            case black|crown:{
                    list[black|crown][npbc++]=p;break;}
            }
    else for(p=49;p>=0;p--) switch(board[map[p]]) {
            case black|man:{
                    list[white|man][npwm++]=p;
                    if (SLW[49-p]>ws) ws=SLW[49-p];
                    break;
                    }
            case black|crown:{
                    list[white|crown][npwc++]=p;break;}
            case white|man:{
                    list[black|man][npbm++]=49-p;
                    if (SLB[49-p]>bs) bs=SLB[49-p];
                    break;
                    }
            case white|crown:{
                    list[black|crown][npbc++]=49-p;break;}
            }
	printf("norm-mode:\n");
	if (npwm) {printf("wm: "); for (int i=0; i<npwm; i++) printf("%d, ", list[WM][i]); printf("\n");};
	if (npwc) {printf("wc: "); for (int i=0; i<npwc; i++) printf("%d, ", list[WC][i]); printf("\n");};
	if (npbm) {printf("bm: "); for (int i=0; i<npbm; i++) printf("%d, ", list[BM][i]); printf("\n");};
	if (npbc) {printf("bc: "); for (int i=0; i<npbc; i++) printf("%d, ", list[BC][i]); printf("\n");};
    if (npwm+npwc+npbm+npbc<=5) small=true;

	//printf("white man: %2d, %2d, %2d, bm: %2d\n", list[WM][0], list[WM][1], list[WM][2], list[BM][0]);

    p3=MULT[50][50-npwc+12];
    p2=MULT[50][50-npbc+12]*p3;
    p1=p2;
    if (npwm>0) {
        if (small==false) { p1=(MULT[ws+5][ws+5-npwm+12]-MULT[ws][ws-npwm+12])*p2; }
        else { p1=MULT[45][45-npwm+12]*p2; }
    }	
    //printf("%i %i %i\n",p1,npwm,small);
    switch(npwc) {
    	case 0: {
    	    iwc=0;
    	    break;
    	}
    	case 1: {
        	iwc=list[WC][0];
        	break;
    	}
    	case 2: {
    	    iwc=I2[list[WC][0]]+list[WC][1];
    	    break;
    	}
    	case 3: {
    	    iwc=I3[list[WC][0]]+I2[list[WC][1]]+list[WC][2];
    	    break;
    	}
    	case 4: {
    	    iwc=I4[list[WC][0]]+I3[list[WC][1]]+I2[list[WC][2]]+list[WC][3];
    	    break;
    	}
    	case 5: {
    	    iwc=I5[list[WC][0]]+I4[list[WC][1]]+I3[list[WC][2]]+I2[list[WC][3]]+list[WC][4];
    	    break;
    	}
    }
    
    switch(npbc) {
    	case 0: {
    	    ibc=0;
    	    break;
    	}
    	case 1: {
        	ibc=list[BC][0];
        	break;
    	}
    	case 2: {
    	    ibc=I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    	case 3: {
    	    ibc=I3[list[BC][2]]+I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    	case 4: {
    	    ibc=I4[list[BC][3]]+I3[list[BC][2]]+I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    	case 5: {
    	    ibc=I5[list[BC][4]]+I4[list[BC][3]]+I3[list[BC][2]]+I2[list[BC][1]]+list[BC][0];
    	    break;
    	}
    }

    switch(npwm) {
    	case 0: {
    	    iwm=0;
    	    break;
    	}
    	case 1: {
        	iwm=list[WM][0];
        	break;
    	}
    	case 2: {
    	    iwm=I2[list[WM][0]]+list[WM][1];
    	    break;
    	}
    	case 3: {
    	    iwm=I3[list[WM][0]]+I2[list[WM][1]]+list[WM][2];
    	    break;
    	}
    	case 4: {
    	    iwm=I4[list[WM][0]]+I3[list[WM][1]]+I2[list[WM][2]]+list[WM][3];
    	    break;
    	}
    	case 5: {
    	    iwm=I5[list[WM][0]]+I4[list[WM][1]]+I3[list[WM][2]]+I2[list[WM][3]]+list[WM][4];
    	    break;
    	}
    }
   	if (npwm>0 && small==false) iwm-=(int)MULT[ws][ws-npwm+12];

    switch(npbm) {
    	case 0: {
    	    ibm=0;
    	    break;
    	}
    	case 1: {
        	ibm=list[BM][0];
        	break;
    	}
    	case 2: {
    	    ibm=I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    	case 3: {
    	    ibm=I3[list[BM][2]]+I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    	case 4: {
    	    ibm=I4[list[BM][3]]+I3[list[BM][2]]+I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    	case 5: {
    	    ibm=I5[list[BM][4]]+I4[list[BM][3]]+I3[list[BM][2]]+I2[list[BM][1]]+list[BM][0];
    	    break;
    	}
    }
   	
   	if (npbm>0 && small==false) ibm-=(int)MULT[bs][bs-npbm+12];


    index=(ibm*p1)+(iwm*p2)+(ibc*p3)+iwc;
	if (index < 0) {
    printf(": %llu  %llu  %llu  %llu, slw=%i, slb=%i\n",ibm,iwm,ibc,iwc,ws,bs);
    printf("id:%llu\n",index);
	}
	return(index);
}
