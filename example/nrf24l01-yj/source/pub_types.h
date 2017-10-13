#ifndef __C_PUBLIC_TYPES_H
#define __C_PUBLIC_TYPES_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef signed long long 	t_s64;
typedef signed long			t_s32;
typedef signed short		t_s16;
typedef signed char			t_s8;

typedef unsigned long long 	t_u64;
typedef unsigned long		t_u32;
typedef unsigned short		t_u16;
typedef unsigned char		t_u8;

typedef float				t_f32;
typedef double				t_f64;

typedef volatile t_s64		t_vs64;
typedef volatile t_s32  	t_vs32;
typedef volatile t_s16  	t_vs16;
typedef volatile t_s8   	t_vs8;

typedef volatile t_u64		t_vu64;
typedef volatile t_u32  	t_vu32;
typedef volatile t_u16  	t_vu16;
typedef volatile t_u8   	t_vu8;


typedef void				t_void;

typedef t_u32				t_tick;

typedef void*				t_handle;

typedef t_u8				t_rw;

#define m_null				(0)

#define m_write				(1)
#define m_read				(0)

typedef enum{m_false=0,m_true=1}t_bool;

#define GET_DWORD_LSB(buf)	(((t_u32)((buf)[0])) | (((t_u32)((buf)[1]))<<8) | (((t_u32)((buf)[2]))<<16) | (((t_u32)((buf)[3]))<<24))
#define GET_DWORD_MSB(buf)	(((t_u32)((buf)[3])) | (((t_u32)((buf)[2]))<<8) | (((t_u32)((buf)[1]))<<16) | (((t_u32)((buf)[0]))<<24))

#endif

