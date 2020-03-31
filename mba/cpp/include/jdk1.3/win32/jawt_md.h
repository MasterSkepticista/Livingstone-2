/*
 * @(#)jawt_md.h	1.2 99/05/27
 *
 * Copyright 1999 by Sun Microsystems, Inc.,
 * 901 San Antonio Road, Palo Alto, California, 94303, U.S.A.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Sun Microsystems, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Sun.
 */

#ifndef _JAVASOFT_JAWT_MD_H_
#define _JAVASOFT_JAWT_MD_H_

#include <windows.h>
#include "jawt.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Win32-specific declarations for AWT native interface.
 * See notes in jawt.h for an example of use.
 */
typedef struct jawt_Win32DrawingSurfaceInfo {
    /* Native window, DDB, or DIB handle */
    union {
        HWND hwnd;
        HBITMAP hbitmap;
        void* pbits;
    };
    /*
     * This HDC should always be used instead of the HDC returned from
     * BeginPaint() or any calls to GetDC().
     */
    HDC hdc;
    HPALETTE hpalette;
} JAWT_Win32DrawingSurfaceInfo;

#ifdef __cplusplus
}
#endif

#endif /* !_JAVASOFT_JAWT_MD_H_ */
