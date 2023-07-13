; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPotoProcessor
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "h264player.h"
LastPage=0

ClassCount=5
Class1=CH264PlayerApp
Class2=CAboutDlg
Class3=CH264PlayerDlg

ResourceCount=9
Resource1=IDD_DIALOGBAR
Resource2=IDR_MENU
Resource3=IDD_FORMVIEW
Resource4=IDD_OLE_PROPPAGE_LARGE (English (U.S.))
Resource5=IDD_POTOPROCESSOR
Resource6=IDD_H264PLAYER_DIALOG
Class4=CPotoProcessor
Resource7=IDD_ABOUTBOX
Resource8=IDR_MENU_POTOPROCESSOR
Class5=CImageShow
Resource9=IDR_TOOLBAR

[CLS:CH264PlayerApp]
Type=0
BaseClass=CWinApp
HeaderFile=H264Player.h
ImplementationFile=H264Player.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=H264PlayerDlg.cpp
ImplementationFile=H264PlayerDlg.cpp
LastObject=CAboutDlg
Filter=D

[CLS:CH264PlayerDlg]
Type=0
BaseClass=CDialog
HeaderFile=H264PlayerDlg.h
ImplementationFile=H264PlayerDlg.cpp
LastObject=IDC_SHOW
Filter=W
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_H264PLAYER_DIALOG]
Type=1
Class=CH264PlayerDlg
ControlCount=5
Control1=IDC_SHOW,static,1342181390
Control2=IDC_PROGRESS,msctls_progress32,1350565888
Control3=IDC_PLAY,button,1342242816
Control4=IDC_STOP,button,1342242816
Control5=IDC_CUTPIC,button,1342242816

[MNU:IDR_MENU]
Type=1
Class=?
Command1=IDR_FILE_OPEN
Command2=IDCANCEL
Command3=IDC_PLAY
Command4=IDR_VIEW_ORIGINALSIZE
Command5=IDR_VIEW_FULLSCRREN
Command6=IDR_ABOUTBOX
CommandCount=6

[DLG:IDD_FORMVIEW]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_OLE_PROPPAGE_LARGE (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_POTOPROCESSOR]
Type=1
Class=CPotoProcessor
ControlCount=51
Control1=IDC_RADIO_CANNY,button,1342180361
Control2=IDC_SLIDER_CANNY,msctls_trackbar32,1342242840
Control3=IDC_STATIC_THRESH,static,1342177280
Control4=IDC_EDIT_THRESH,edit,1350633600
Control5=IDC_RADIO_BLUR,button,1342177289
Control6=IDC_RADIO_GAUSSIAN,button,1342177289
Control7=IDC_RADIO_MEDIAN,button,1342177289
Control8=IDC_SLIDER_PARA1,msctls_trackbar32,1342242840
Control9=IDC_STATIC_PARA1,static,1342177280
Control10=IDC_EDIT_PARA1,edit,1350633600
Control11=IDC_SLIDER_PARA2,msctls_trackbar32,1342242840
Control12=IDC_STATIC_PARA2,static,1342177280
Control13=IDC_EDIT_PARA2,edit,1350633600
Control14=IDC_POTO_SHOW,static,1342181380
Control15=IDC_SLIDER_LAPLACE,msctls_trackbar32,1342242840
Control16=IDC_EDIT_APERTUR_SIZE,edit,1350633600
Control17=IDC_RADIO_LAPLACE,button,1342177289
Control18=IDC_RADIO_PYRSEGMENT,button,1342177289
Control19=IDC_SLIDER_PYRSEGMENT1,msctls_trackbar32,1342242840
Control20=IDC_EDIT_PYRSEGMENT1,edit,1350633600
Control21=IDC_SLIDER_PYRSEGMENT2,msctls_trackbar32,1342242840
Control22=IDC_EDIT_PYRSEGMENT2,edit,1350633600
Control23=IDC_STATIC_PYRSEGMENT1,static,1342177280
Control24=IDC_STATIC_PYRSEGMENT2,static,1342177280
Control25=IDC_BTN_STHENGTHEN,button,1342251008
Control26=IDC_STATIC_COMBOBOX,button,1342177287
Control27=IDC_STATIC_LINE1,static,1342181392
Control28=IDC_BTN_THRSHOLD,button,1342251008
Control29=IDC_BTN_FACEDETECTION,button,1342251008
Control30=IDC_BTN_RECOVERORIMAGE,button,1342254848
Control31=IDC_STATIC_LINE3,static,1342181392
Control32=IDC_RADIO_EXPEND,button,1342177289
Control33=IDC_RADIO_ERODE,button,1342177289
Control34=IDC_STATIC_LINE4,static,1342181392
Control35=IDC_SLIDER_EXPEND,msctls_trackbar32,1342242840
Control36=IDC_STATIC_LINE5,static,1342181392
Control37=IDC_STATIC_LINE6,static,1342181392
Control38=IDC_BTN_OPEN,button,1342251008
Control39=IDC_STATIC_LINE2,static,1342181392
Control40=IDC_SLIDER_BRIGHTNESS,msctls_trackbar32,1342242840
Control41=IDC_SLIDER_CONTRAST,msctls_trackbar32,1342242840
Control42=IDC_SLIDER_LIGHTNESS,msctls_trackbar32,1342242840
Control43=IDC_EDIT_BRIGHTNESS,edit,1350633600
Control44=IDC_EDIT_CONTRAST,edit,1350633600
Control45=IDC_EDIT_LIGHTNESS,edit,1350633600
Control46=IDC_EDIT_TIMES,edit,1350633600
Control47=IDC_STATIC_TIMES,static,1342308352
Control48=IDC_BTN_SAVE,button,1342242816
Control49=IDC_RADIO_BRIGHTNESS,button,1342308361
Control50=IDC_RADIO_CONTRAST,button,1342177289
Control51=IDC_STATIC_LIGHTNESS,static,1342308352

[MNU:IDR_MENU_POTOPROCESSOR]
Type=1
Class=?
Command1=IDR_FILE_OPEN
Command2=IDCANCEL
CommandCount=2

[CLS:CPotoProcessor]
Type=0
HeaderFile=PotoProcessor.h
ImplementationFile=PotoProcessor.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_TIMES
VirtualFilter=dWC

[TB:IDR_TOOLBAR]
Type=1
Class=?
CommandCount=0

[DLG:IDD_DIALOGBAR]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342177284

[CLS:CImageShow]
Type=0
HeaderFile=ImageShow.h
ImplementationFile=ImageShow.cpp
BaseClass=CStatic
Filter=W
LastObject=CImageShow
VirtualFilter=WC

