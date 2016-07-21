#ifndef __GRX_COMMON_H__
#define __GRX_COMMON_H__

/* context.h */
typedef struct _GrxFrame            GrxFrame;
typedef struct _GrxContext          GrxContext;
/* draw.h */
typedef struct _GrxFramedBoxColors  GrxFramedBoxColors;
typedef struct _GrxPoint            GrxPoint;
/* device_manager.h */
typedef struct _GrxDeviceManager    GrxDeviceManager;
/* events.h */
typedef union _GrxEvent             GrxEvent;
/* mode.h */
typedef struct _GrxFrameDriver      GrxFrameDriver;
typedef struct _GrxVideoDriver      GrxVideoDriver;
typedef struct _GrxVideoMode        GrxVideoMode;
typedef struct _GrxVideoModeExt     GrxVideoModeExt;
/* pattern.h */
typedef struct _GrxPixmap           GrxPixmap;
typedef union  _GrxPattern          GrxPattern;
typedef struct _GrxLinePattern      GrxLinePattern;
/* text.h */
typedef struct _GrxTextOptions       GrxTextOptions;
/* wideline.h */
typedef struct _GrxLineOptions      GrxLineOptions;

#endif /* __GRX_COMMON_H__ */
