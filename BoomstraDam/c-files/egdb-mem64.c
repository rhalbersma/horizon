/* mem64.c: low-performance 64-bit memory manager with page compression */

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


#include <stdio.h>
#include "egdb-const.h"
#include <time.h>
#include "malloc.h"
#include <stdlib.h>
#include "BoomstraDam.v1.2.h"


#ifdef USE_ZLIB
    #include "/usr/include/zlib.h"
#endif

#define PAGESIZE (1024LL*1024LL)        /* size of a page */
#define MAXPAGES 1200LL        /* maximum number of pages in ram */
#define MAXFILESIZE 1800000000LL   /* when larges than this, files will be split into blocks */
#define MAXHANDLE 22000   /* maximal number of memory64 handles */
//#define CLEANFILES "rm -f tmp/mem64*"

INT64 mem64_allocatedAmount[MAXHANDLE];   /* allocated amount in bytes for handle */
int mem64_numberOfPages[MAXHANDLE];       /* number of pages for handle */
char **mem64_pointerList[MAXHANDLE];       /* pointer to list with memory pointers */
int *mem64_pageNumber[MAXHANDLE];         /* pointer to list with pagenumber */

char *mem64[MAXPAGES];          /* pointer to PAGESIZE amount of memory */
INT64 mem64_increment[MAXPAGES];  /* last time this page was accessed */
int mem64_handle[MAXPAGES];     /* the handle associatied with this page */
int mem64_hpage[MAXPAGES];      /* the pagenumber within handle=index/pagesize */
int mem64_dirty[MAXPAGES];      /* page is dirty */
char pagefile[255];

INT64 mem64_diskActivity=0LL;
INT64 increment=0LL;

void display_board()
{ // dummy
}


void mem64_init(int showinfo)
{
    int i;
    
    //system(CLEANFILES);  // removed fb
    //printf("Pagefile: %s\n",pagefile);
    for (i=0;i<MAXHANDLE;i++) {
        mem64_allocatedAmount[i]=0;
        mem64_numberOfPages[i]=0;
        mem64_pointerList[i]=NULL;
    }
    mem64_diskActivity=0LL;
    for (i=0;i<MAXPAGES;i++) {
        mem64[i]=NULL;
        mem64_handle[i]=-1;
        mem64_hpage[i]=-1;
        mem64_increment[i]=0LL;
    }
    increment=0LL;
}

int mem64_getFreePage(); // forward declaration
void writePage(int);

int mem64_allocate(INT64 amount)
/* returns a mem64 handle. if handle<0 the memory allocation failed */
{
    INT64 pages;
    int handle;
    int fp;
    int i;
    char **base;
    int *numbers;    
    /* get a new handle */
    handle=-1;
    for(i=0;i<MAXHANDLE;i++) {
        if (mem64_allocatedAmount[i]==0LL) {
            handle=i;
            break;
        }
	}
    if (handle<0) return(handle);
    
    pages=(amount+PAGESIZE-1)/PAGESIZE;
    //printf(("pages: %i, handle:%i\n",pages,handle);
    
    mem64_allocatedAmount[handle]=amount;
    
    base=(char **)malloc((size_t)pages*sizeof(char *));
    if (base==NULL) {
        printf("Fatal: malloc failure on _base_\n");
        exit(1);
    } //else printf("allocated base = %p\n", base);
    numbers=(int *)malloc((size_t)pages*sizeof(int));
    if (numbers==NULL) {
        printf("Fatal: malloc failure on _numbers_\n");
        exit(1);
	}//else printf("allocated numbers = %p\n", numbers);

    //printf("pointerlist=%i  %p\n",(pages*sizeof(char *)), base); printf(" %p\n", base);
    //printf("pointerlist= %d %p\n", pages, base); printf(" %p\n", base);
	//printf("pointerlist=%i  %p\n",pages*sizeof(int *),numbers);
    mem64_pointerList[handle]=base;
    mem64_pageNumber[handle]=numbers;
    mem64_numberOfPages[handle]=(int)pages;
    for(i=0;i<pages;i++) {
        fp=mem64_getFreePage();
        if (mem64[fp]==NULL) { /* mem64 has not yet been allocated */
            mem64[fp]=(char *) malloc(PAGESIZE);
            if (mem64[fp]==0) {
                printf("Fatal: malloc failure on _mem64[%i]_\n",fp);
                exit(1);
            }
		}
        
        //printf(("i= %i, fp= %i mem64[fp]= %p base[i]= %p\n",i,fp,mem64[fp],base[i]);
        base[i]=mem64[fp];
        mem64_handle[fp]=handle;
        mem64_hpage[fp]=i;
        mem64_increment[fp]=(increment++);
        mem64_dirty[fp]=true;
        numbers[i]=fp;
	}
    /*printf("sadfdf\n");*/
        
    return (handle);
}

int mem64_getFreePage()
{
    int i;
    int fp;
    INT64 oldestPage=9000000000000000000LL;
    int oldest=-1;
    
    fp=-1;
    oldest=-1;
    for (i=0;i<MAXPAGES;i++) {
        if (mem64[i]==NULL) {
            fp=i;
            break;
        }
        if (mem64_increment[i]<oldestPage) {
            oldestPage=mem64_increment[i];
            oldest=i;
        }
    }
    if (fp>=0) return (fp);
    /* no page found, write the oldest to disk or discard it if is it not changed */
    writePage(oldest);
    return(oldest);
}

void writePage(int page)
{
    char pg[100];
    int handle,hpage;
    FILE *out;
    void **base;
#ifdef USE_ZLIB 
    gzFile gzOut;
#endif
    
    handle=mem64_handle[page];
    hpage=mem64_hpage[page];
    
    /* save page to disk */
    /*printf("saving page %i, handle %i/%i\n",page,handle,hpage,mem64[page]);*/
    if (mem64_dirty[page]==true) { 
        sprintf_s(pg,100,pagefile,handle,hpage);
        /*printf("%s\n",pagefile);*/
        #ifdef USE_ZLIB 
            gzOut = gzopen(pg, "wb1");
            if (gzOut==NULL) {
                printf("fatal: gzwrite error on %s\n",pg);
                exit(1);
            }
            br=gzwrite(gzOut, mem64[page], PAGESIZE);
            if (br==0) { 
                printf("fatal: gzwrite (2) error on %s\n",pg);
                exit(1);
            }
            gzclose(gzOut);
            mem64_diskActivity+=br;
        #else
            fopen_s(&out,pg,"wb");
                if (out==NULL) {
                     printf("fatal: write error on %s\n",pg);
                     exit(1);
               }
            fwrite(mem64[page],1,PAGESIZE,out);
            fclose(out);
            mem64_diskActivity+=PAGESIZE;
        #endif
    }
        /*for (j=0;j<PAGESIZE;j++) printf("%i",mem64[page][j]);*/
    /*printf("\n");*/
    /* mark page as unused */
    base=(void **)mem64_pointerList[handle];
    base[hpage]=0;
    mem64_pageNumber[handle][hpage]=-1;
    mem64_handle[page]=-1;
    mem64_hpage[page]=-1;
    mem64_dirty[page]=false;
}

void loadPage(int page,int handle,int hpage)
{
    char pg[100];
    FILE *in;
    void **base;
#ifdef USE_ZLIB 
    gzFile zin;
#endif
    
    /* save page to disk */
    /*printf("loading page %i, handle %i/%i\n",page,handle,hpage);*/
    sprintf_s(pg,100,pagefile,handle,hpage);
    /*printf("%s %i\n",pagefile,mem64[page]);*/
    
    #ifdef USE_ZLIB 
    zin = gzopen(pg, "rb");
	if (zin == NULL) {
        printf("fatal: read error on %s\n",pg);
        exit(1);
    }
    amountread = gzread(zin, mem64[page], sizeof(unsigned char)*PAGESIZE);
    if (gzclose(zin) != Z_OK) printf("\n\nfailed gzclose\n\n");
    mem64_diskActivity+=amountread;
    #else
    errno_t err = fopen_s(&in, pg,"rb");
    if ((err)) {
        printf("fatal: read error on %s\n",pg);
        exit(1);
    }
    fread(mem64[page],1,PAGESIZE,in);
    fclose(in);
    mem64_diskActivity+=PAGESIZE;
    #endif
    
    /* mark page as in use */
    base=(void **)mem64_pointerList[handle];
    base[hpage]=mem64[page];
    mem64_pageNumber[handle][hpage]=page;
    mem64_handle[page]=handle;
    mem64_hpage[page]=hpage;
    mem64_increment[page]=(increment++);
    mem64_dirty[page]=false;
}
//void decompressGZfile(char *fIn,char *fOut)
//{
//    #define BUFFERSIZE 32768
//    gzFile zIn;
//    FILE *out;
//    char buffer[BUFFERSIZE];
//    int amountread;
//    int j;
//    
//    zIn = gzopen(fIn, "rb");
//	if (zIn == NULL) {
//        printf("fatal: read error on %s\n",fIn);
//        exit(1);
//    }
//    out=fopen(fOut,"wb");
//    if (out==NULL) {
//        printf("fatal: read error on %s\n",fOut);
//        exit(1);
//    }
//    
//    for (j=0;true;j++) {
//        amountread = gzread(zIn, buffer, sizeof(unsigned char)*BUFFERSIZE);
//        if (amountread==0) break;
//        fwrite(buffer,1,amountread,out);
//    }
//    fclose(out);
//    gzclose(zIn);
//}
//
char *mem64_pointer(int handle,INT64 index,int markAsDirty)
{
    INT64 hpage;
    char **base;
    char *memBlock;
    int fp;
    int page;
    
    if (handle>MAXHANDLE || handle<0) {
        printf("Illegal mem64 handle: %i\n",handle);
        display_board();
        return NULL;
    }
    hpage=index/PAGESIZE;
    if (index<0 || hpage<0) {
        printf("Illegal index handle: %llu %llu\n",index,hpage);
        return NULL;
    }
    base=mem64_pointerList[handle];
    memBlock=base[hpage];
    //printf(("bla %i %llu  %p  memblock:%p\n",handle,index,base,memBlock);
    //address=base[page];
    
    if (memBlock==NULL) {
        fp=mem64_getFreePage();
        loadPage(fp,handle,(int)hpage);
        memBlock=mem64[fp];
    }
    page=mem64_pageNumber[handle][hpage];
    mem64_increment[page]=(increment++);

	if (markAsDirty==true) {
        mem64_dirty[page]=true;
    }

	return memBlock+index%PAGESIZE;
}


int mem64_load(int handle,char *filename)
/* returns true on success */
{
    FILE *data_file;
    INT64 index=0;
    char *p; // = (char *)malloc((size_t)PAGESIZE);
    int j;
    int amountread=0;

	//if (i==0) sprintf_s(fname,256,"%s",filename);
    //if (i>0) sprintf_s(fname,256,"%s-%i",filename,i);
    errno_t err = fopen_s(&data_file, filename, "rb");
    if ((err))
	{	printf("Error in opening raw data file: %s errno = %d\n", filename, errno);				
		return false;
	}

	for (j=0;true;j++) {
        p=mem64_pointer(handle,index,false); // true?? changed fb
		//printf("try to read in: %p, size = %d, count = %d, stream = %p\n", p, (size_t)sizeof(unsigned char), (size_t)1024, data_file);
		amountread = (int)fread(p, sizeof(unsigned char), (size_t)PAGESIZE, data_file);
        index+=amountread;
        mem64_diskActivity+=amountread;
		if (amountread != PAGESIZE)
		{	//printf("read: %d\n, amountread);
			if (feof(data_file)) break;
			else printf("Error during read: errno = %d\n", errno);
		}
    }

	fclose(data_file);
    if (index==0) {
        printf("load failure on %s\n",filename);
        return(false);
    }
    return(true);
}
