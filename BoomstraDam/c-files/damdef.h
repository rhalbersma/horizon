/*
  This file is Copyright (C) 1999 Harm Jetten.
  Modifications Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef _DAMDEF_H
#define _DAMDEF_H

#undef UNICODE

#include <windows.h>
#include <windowsx.h>
#ifdef __LCC__
#define stricmp strcmpi
#include <shellapi.h>
#endif
#ifdef __GCC__
#define NONAMELESSUNION
#endif
#ifdef _MSC_VER
#pragma warning(disable: 4145)  /* relational expression as switch expression */
#endif
#include <commctrl.h>
#include <mmsystem.h>
#include <winsock.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
//#include "resource.h"
//#include "context.h"
//#include "extra.h"

#ifndef USEDEF          /* defined as "static" by .lclintrc to    */
#define USEDEF          /* avoid "used before definition" warning */
#endif
#undef  FALSE           /* for lclint */
#define FALSE ((BOOL)0)
#undef  TRUE
#define TRUE ((BOOL)1)

#ifndef GetStockFont    /* in case not defined in windowsx.h (gcc) */
#define GetStockFont(i)        ((HFONT)GetStockObject(i))
#define GetStockPen(i)         ((HPEN)GetStockObject(i))
#define SelectBitmap(hdc, hbm) ((HBITMAP)SelectObject((hdc), (HGDIOBJ)(hbm)))
#define SelectBrush(hdc, hbr)  ((HBRUSH)SelectObject((hdc), (HGDIOBJ)(hbr)))
#define SelectFont(hdc, hfont) ((HFONT)SelectObject((hdc), (HGDIOBJ)(hfont)))
#define SelectPen(hdc, hpen)   ((HPEN)SelectObject((hdc), (HGDIOBJ)(hpen)))
#define DeleteBitmap(hbm)      DeleteObject((HGDIOBJ)(hbm))
#define DeleteBrush(hbr)       DeleteObject((HGDIOBJ)(hbr))
#define DeleteFont(hfont)      DeleteObject((HGDIOBJ)(hfont))
#define DeletePen(hpen)        DeleteObject((HGDIOBJ)(hpen))
#endif

#define mymalloc(size) (GlobalLock(GlobalAlloc(GMEM_MOVEABLE, (size))))
#define myfree(block)  (GlobalUnlock((HGLOBAL)GlobalHandle(block)), \
                       (BOOL)GlobalFree((HGLOBAL)GlobalHandle(block)))

/* private messages */
#define PM_RESTORE      (WM_USER)      /* restore main window */
#define PM_WSGETLOCAL   (WM_USER + 1)  /* local hostname notification */
#define PM_WSGETREMOTE  (WM_USER + 2)  /* remote hostname notification */
#define PM_WSSELECT     (WM_USER + 3)  /* select notification */
#define PM_WSGETBYADDR  (WM_USER + 4)  /* remote hostname by address */
#define PM_RCVREMOTE    (WM_USER + 5)  /* handle received DamExchange message */
#define PM_COPYCLIP     (WM_USER + 6)  /* copy board to clipboard */
#define PM_COMMNOTIFY   (WM_USER + 7)  /* serial port event notification */

/* additional multi_ret codes */
#define ER_MTOV   (WM_USER + 1000)     /* move tree overflow */
#define ER_GROV   (WM_USER + 1001)     /* game record overflow */
#define DRAWPOS   (WM_USER + 1002)     /* drawn position reached */
#define GAMEREQ   (WM_USER + 1003)     /* gamereq received */
#define GAMEACC   (WM_USER + 1004)     /* gameacc received */
#define GAMEEND   (WM_USER + 1005)     /* gameend received/sent */
#define TAKEBACK  (WM_USER + 1006)     /* execute received takeback */
#define TAKEBRCV  (WM_USER + 1007)     /* takeback while in dialog */
#define TAKEFORW  (WM_USER + 1008)     /* execute received forward */
#define LOADPREV  (WM_USER + 1009)     /* load previous pdn/doc game */
#define LOADNEXT  (WM_USER + 1010)     /* load next pdn/doc game */

#define PASSFILE  (WM_USER + 1050)     /* pass name of file to open */
#define PASSINST  (WM_USER + 1051)     /* pass instance number */

#define XCOMPARE  (WM_USER + 1090)     /* compare with expert moves */
#define XCHKEND   (WM_USER + 1091)     /* check endgame databases */

#ifdef EXTRA
#define PEGCOUNT(x) ((x)++) /* for counting of various statistics */
#else
#define PEGCOUNT(x)
#endif

#define EQUAL   0

//#define abs(x)  ((x)>0?(x):-(x))
#define hi(x)   ((x) / 256)
#define lo(x)   ((x) & 0xff)
#define col(x)  ((x) & 1)

#define W  0            /* the colors */
#define B  1

#define M  2            /* the piece types */
#define K  4

#define E  1            /* empty square */

#define MW (M + W)      /* the pieces */
#define MB (M + B)
#define KW (K + W)
#define KB (K + B)

//#include "dambook.h"
//#include "damclock.h"
//#include "damcom.h"
//#include "damdlg.h"
//#include "damend.h"
//#include "dameval.h"
//#include "damfile.h"
//#include "damhash.h"
//#include "damlearn.h"
//#include "dammain.h"
//#include "dammove.h"
//#include "dampdn.h"
//#include "damprint.h"
//#include "damshow.h"
//#include "damtcp.h"

#endif
