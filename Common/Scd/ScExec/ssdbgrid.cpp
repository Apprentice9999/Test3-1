// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "ssdbgrid.h"

// Dispatch interfaces referenced by this interface
#include "font.h"
#include "Picture.h"
#include "SSColumns.h"
#include "ssgroups.h"
#include "ssselbookmarks.h"
#include "ssstylesets.h"
#include "ssactivecell.h"

/////////////////////////////////////////////////////////////////////////////
// CSSDBGrid

IMPLEMENT_DYNCREATE(CSSDBGrid, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CSSDBGrid properties

LPUNKNOWN CSSDBGrid::GetDataSource()
{
	LPUNKNOWN result;
	GetProperty(0x1, VT_UNKNOWN, (void*)&result);
	return result;
}

void CSSDBGrid::SetDataSource(LPUNKNOWN propVal)
{
	SetProperty(0x1, VT_UNKNOWN, propVal);
}

OLE_COLOR CSSDBGrid::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

CString CSSDBGrid::GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}

void CSSDBGrid::SetCaption(LPCTSTR propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

BOOL CSSDBGrid::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

COleFont CSSDBGrid::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CSSDBGrid::SetFont(LPDISPATCH propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

OLE_COLOR CSSDBGrid::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

OLE_HANDLE CSSDBGrid::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetBackColorEven()
{
	unsigned long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBackColorEven(unsigned long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetBackColorOdd()
{
	unsigned long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBackColorOdd(unsigned long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetForeColorEven()
{
	unsigned long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetForeColorEven(unsigned long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetForeColorOdd()
{
	unsigned long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetForeColorOdd(unsigned long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

long CSSDBGrid::GetCaptionAlignment()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetCaptionAlignment(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long CSSDBGrid::GetDividerType()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetDividerType(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long CSSDBGrid::GetScrollBars()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetScrollBars(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

BOOL CSSDBGrid::GetAllowAddNew()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowAddNew(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowDelete()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowDelete(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowUpdate()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowUpdate(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowRowSizing()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowRowSizing(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowGroupSizing()
{
	BOOL result;
	GetProperty(0xd, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowGroupSizing(BOOL propVal)
{
	SetProperty(0xd, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowColumnSizing()
{
	BOOL result;
	GetProperty(0xe, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowColumnSizing(BOOL propVal)
{
	SetProperty(0xe, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetRecordSelectors()
{
	BOOL result;
	GetProperty(0xf, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetRecordSelectors(BOOL propVal)
{
	SetProperty(0xf, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetGroupHeaders()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetGroupHeaders(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetColumnHeaders()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetColumnHeaders(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}

short CSSDBGrid::GetHeadLines()
{
	short result;
	GetProperty(0x12, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetHeadLines(short propVal)
{
	SetProperty(0x12, VT_I2, propVal);
}

COleFont CSSDBGrid::GetHeadFont()
{
	LPDISPATCH pDispatch;
	GetProperty(0x13, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CSSDBGrid::SetHeadFont(LPDISPATCH propVal)
{
	SetProperty(0x13, VT_DISPATCH, propVal);
}

long CSSDBGrid::GetHeadFont3D()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetHeadFont3D(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

short CSSDBGrid::GetCol()
{
	short result;
	GetProperty(0x15, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetCol(short propVal)
{
	SetProperty(0x15, VT_I2, propVal);
}

short CSSDBGrid::GetRow()
{
	short result;
	GetProperty(0x16, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetRow(short propVal)
{
	SetProperty(0x16, VT_I2, propVal);
}

long CSSDBGrid::GetDataMode()
{
	long result;
	GetProperty(0x17, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetDataMode(long propVal)
{
	SetProperty(0x17, VT_I4, propVal);
}

short CSSDBGrid::GetVisibleCols()
{
	short result;
	GetProperty(0x18, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetVisibleCols(short propVal)
{
	SetProperty(0x18, VT_I2, propVal);
}

short CSSDBGrid::GetVisibleRows()
{
	short result;
	GetProperty(0x19, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetVisibleRows(short propVal)
{
	SetProperty(0x19, VT_I2, propVal);
}

short CSSDBGrid::GetLeftCol()
{
	short result;
	GetProperty(0x1a, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetLeftCol(short propVal)
{
	SetProperty(0x1a, VT_I2, propVal);
}

short CSSDBGrid::GetCols()
{
	short result;
	GetProperty(0x1b, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetCols(short propVal)
{
	SetProperty(0x1b, VT_I2, propVal);
}

long CSSDBGrid::GetRows()
{
	long result;
	GetProperty(0x1c, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetRows(long propVal)
{
	SetProperty(0x1c, VT_I4, propVal);
}

CString CSSDBGrid::GetFieldDelimiter()
{
	CString result;
	GetProperty(0x1d, VT_BSTR, (void*)&result);
	return result;
}

void CSSDBGrid::SetFieldDelimiter(LPCTSTR propVal)
{
	SetProperty(0x1d, VT_BSTR, propVal);
}

CString CSSDBGrid::GetFieldSeparator()
{
	CString result;
	GetProperty(0x1e, VT_BSTR, (void*)&result);
	return result;
}

void CSSDBGrid::SetFieldSeparator(LPCTSTR propVal)
{
	SetProperty(0x1e, VT_BSTR, propVal);
}

long CSSDBGrid::GetFont3D()
{
	long result;
	GetProperty(0x1f, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetFont3D(long propVal)
{
	SetProperty(0x1f, VT_I4, propVal);
}

long CSSDBGrid::GetDividerStyle()
{
	long result;
	GetProperty(0x20, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetDividerStyle(long propVal)
{
	SetProperty(0x20, VT_I4, propVal);
}

BOOL CSSDBGrid::GetUseExactRowCount()
{
	BOOL result;
	GetProperty(0x21, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetUseExactRowCount(BOOL propVal)
{
	SetProperty(0x21, VT_BOOL, propVal);
}

unsigned long CSSDBGrid::GetBevelColorFace()
{
	unsigned long result;
	GetProperty(0x22, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBevelColorFace(unsigned long propVal)
{
	SetProperty(0x22, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetBevelColorFrame()
{
	unsigned long result;
	GetProperty(0x23, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBevelColorFrame(unsigned long propVal)
{
	SetProperty(0x23, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetBevelColorHighlight()
{
	unsigned long result;
	GetProperty(0x24, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBevelColorHighlight(unsigned long propVal)
{
	SetProperty(0x24, VT_I4, propVal);
}

unsigned long CSSDBGrid::GetBevelColorShadow()
{
	unsigned long result;
	GetProperty(0x25, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBevelColorShadow(unsigned long propVal)
{
	SetProperty(0x25, VT_I4, propVal);
}

long CSSDBGrid::GetBevelColorScheme()
{
	long result;
	GetProperty(0x26, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBevelColorScheme(long propVal)
{
	SetProperty(0x26, VT_I4, propVal);
}

short CSSDBGrid::GetGroupHeadLines()
{
	short result;
	GetProperty(0x27, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetGroupHeadLines(short propVal)
{
	SetProperty(0x27, VT_I2, propVal);
}

VARIANT CSSDBGrid::GetBookmark()
{
	VARIANT result;
	GetProperty(0x28, VT_VARIANT, (void*)&result);
	return result;
}

void CSSDBGrid::SetBookmark(const VARIANT& propVal)
{
	SetProperty(0x28, VT_VARIANT, &propVal);
}

BOOL CSSDBGrid::GetAllowDragDrop()
{
	BOOL result;
	GetProperty(0x29, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowDragDrop(BOOL propVal)
{
	SetProperty(0x29, VT_BOOL, propVal);
}

VARIANT CSSDBGrid::GetFirstRow()
{
	VARIANT result;
	GetProperty(0x2a, VT_VARIANT, (void*)&result);
	return result;
}

void CSSDBGrid::SetFirstRow(const VARIANT& propVal)
{
	SetProperty(0x2a, VT_VARIANT, &propVal);
}

float CSSDBGrid::GetDefColWidth()
{
	float result;
	GetProperty(0x2b, VT_R4, (void*)&result);
	return result;
}

void CSSDBGrid::SetDefColWidth(float propVal)
{
	SetProperty(0x2b, VT_R4, propVal);
}

float CSSDBGrid::GetRowHeight()
{
	float result;
	GetProperty(0x2c, VT_R4, (void*)&result);
	return result;
}

void CSSDBGrid::SetRowHeight(float propVal)
{
	SetProperty(0x2c, VT_R4, propVal);
}

short CSSDBGrid::GetVisibleGrps()
{
	short result;
	GetProperty(0x2d, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetVisibleGrps(short propVal)
{
	SetProperty(0x2d, VT_I2, propVal);
}

short CSSDBGrid::GetLeftGrp()
{
	short result;
	GetProperty(0x2e, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetLeftGrp(short propVal)
{
	SetProperty(0x2e, VT_I2, propVal);
}

BOOL CSSDBGrid::GetCheckBox3D()
{
	BOOL result;
	GetProperty(0x2f, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetCheckBox3D(BOOL propVal)
{
	SetProperty(0x2f, VT_BOOL, propVal);
}

short CSSDBGrid::GetLevelCount()
{
	short result;
	GetProperty(0x30, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetLevelCount(short propVal)
{
	SetProperty(0x30, VT_I2, propVal);
}

BOOL CSSDBGrid::GetRedraw()
{
	BOOL result;
	GetProperty(0x31, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetRedraw(BOOL propVal)
{
	SetProperty(0x31, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetComboDroppedDown()
{
	BOOL result;
	GetProperty(0x32, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetComboDroppedDown(BOOL propVal)
{
	SetProperty(0x32, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowGroupMoving()
{
	BOOL result;
	GetProperty(0x33, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowGroupMoving(BOOL propVal)
{
	SetProperty(0x33, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowGroupSwapping()
{
	BOOL result;
	GetProperty(0x34, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowGroupSwapping(BOOL propVal)
{
	SetProperty(0x34, VT_BOOL, propVal);
}

long CSSDBGrid::GetAllowColumnMoving()
{
	long result;
	GetProperty(0x35, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowColumnMoving(long propVal)
{
	SetProperty(0x35, VT_I4, propVal);
}

long CSSDBGrid::GetAllowColumnSwapping()
{
	long result;
	GetProperty(0x36, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowColumnSwapping(long propVal)
{
	SetProperty(0x36, VT_I4, propVal);
}

float CSSDBGrid::GetResizeHeight()
{
	float result;
	GetProperty(0x37, VT_R4, (void*)&result);
	return result;
}

void CSSDBGrid::SetResizeHeight(float propVal)
{
	SetProperty(0x37, VT_R4, propVal);
}

float CSSDBGrid::GetResizeWidth()
{
	float result;
	GetProperty(0x38, VT_R4, (void*)&result);
	return result;
}

void CSSDBGrid::SetResizeWidth(float propVal)
{
	SetProperty(0x38, VT_R4, propVal);
}

VARIANT CSSDBGrid::GetTagVariant()
{
	VARIANT result;
	GetProperty(0x39, VT_VARIANT, (void*)&result);
	return result;
}

void CSSDBGrid::SetTagVariant(const VARIANT& propVal)
{
	SetProperty(0x39, VT_VARIANT, &propVal);
}

short CSSDBGrid::GetSplitterPos()
{
	short result;
	GetProperty(0x3a, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetSplitterPos(short propVal)
{
	SetProperty(0x3a, VT_I2, propVal);
}

BOOL CSSDBGrid::GetSplitterVisible()
{
	BOOL result;
	GetProperty(0x3b, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetSplitterVisible(BOOL propVal)
{
	SetProperty(0x3b, VT_BOOL, propVal);
}

long CSSDBGrid::GetSelectTypeCol()
{
	long result;
	GetProperty(0x3c, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetSelectTypeCol(long propVal)
{
	SetProperty(0x3c, VT_I4, propVal);
}

long CSSDBGrid::GetSelectTypeRow()
{
	long result;
	GetProperty(0x3d, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetSelectTypeRow(long propVal)
{
	SetProperty(0x3d, VT_I4, propVal);
}

BOOL CSSDBGrid::GetSelectByCell()
{
	BOOL result;
	GetProperty(0x3e, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetSelectByCell(BOOL propVal)
{
	SetProperty(0x3e, VT_BOOL, propVal);
}

CString CSSDBGrid::GetHeadStyleSet()
{
	CString result;
	GetProperty(0x3f, VT_BSTR, (void*)&result);
	return result;
}

void CSSDBGrid::SetHeadStyleSet(LPCTSTR propVal)
{
	SetProperty(0x3f, VT_BSTR, propVal);
}

CString CSSDBGrid::GetStyleSet()
{
	CString result;
	GetProperty(0x40, VT_BSTR, (void*)&result);
	return result;
}

void CSSDBGrid::SetStyleSet(LPCTSTR propVal)
{
	SetProperty(0x40, VT_BSTR, propVal);
}

BOOL CSSDBGrid::GetMultiLine()
{
	BOOL result;
	GetProperty(0x41, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetMultiLine(BOOL propVal)
{
	SetProperty(0x41, VT_BOOL, propVal);
}

CString CSSDBGrid::GetActiveRowStyleSet()
{
	CString result;
	GetProperty(0x42, VT_BSTR, (void*)&result);
	return result;
}

void CSSDBGrid::SetActiveRowStyleSet(LPCTSTR propVal)
{
	SetProperty(0x42, VT_BSTR, propVal);
}

CPicture CSSDBGrid::GetPictureButton()
{
	LPDISPATCH pDispatch;
	GetProperty(0x43, VT_DISPATCH, (void*)&pDispatch);
	return CPicture(pDispatch);
}

void CSSDBGrid::SetPictureButton(LPDISPATCH propVal)
{
	SetProperty(0x43, VT_DISPATCH, propVal);
}

CPicture CSSDBGrid::GetPictureComboButton()
{
	LPDISPATCH pDispatch;
	GetProperty(0x44, VT_DISPATCH, (void*)&pDispatch);
	return CPicture(pDispatch);
}

void CSSDBGrid::SetPictureComboButton(LPDISPATCH propVal)
{
	SetProperty(0x44, VT_DISPATCH, propVal);
}

CPicture CSSDBGrid::GetPictureRecordSelectors()
{
	LPDISPATCH pDispatch;
	GetProperty(0x45, VT_DISPATCH, (void*)&pDispatch);
	return CPicture(pDispatch);
}

void CSSDBGrid::SetPictureRecordSelectors(LPDISPATCH propVal)
{
	SetProperty(0x45, VT_DISPATCH, propVal);
}

BOOL CSSDBGrid::GetAllowColumnShrinking()
{
	BOOL result;
	GetProperty(0x46, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowColumnShrinking(BOOL propVal)
{
	SetProperty(0x46, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetAllowGroupShrinking()
{
	BOOL result;
	GetProperty(0x47, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetAllowGroupShrinking(BOOL propVal)
{
	SetProperty(0x47, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetDataChanged()
{
	BOOL result;
	GetProperty(0x48, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetDataChanged(BOOL propVal)
{
	SetProperty(0x48, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetRowChanged()
{
	BOOL result;
	GetProperty(0x49, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetRowChanged(BOOL propVal)
{
	SetProperty(0x49, VT_BOOL, propVal);
}

BOOL CSSDBGrid::GetBalloonHelp()
{
	BOOL result;
	GetProperty(0x4a, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetBalloonHelp(BOOL propVal)
{
	SetProperty(0x4a, VT_BOOL, propVal);
}

long CSSDBGrid::GetCellNavigation()
{
	long result;
	GetProperty(0x4b, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetCellNavigation(long propVal)
{
	SetProperty(0x4b, VT_I4, propVal);
}

long CSSDBGrid::GetRowNavigation()
{
	long result;
	GetProperty(0x4c, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetRowNavigation(long propVal)
{
	SetProperty(0x4c, VT_I4, propVal);
}

short CSSDBGrid::GetGrp()
{
	short result;
	GetProperty(0x4d, VT_I2, (void*)&result);
	return result;
}

void CSSDBGrid::SetGrp(short propVal)
{
	SetProperty(0x4d, VT_I2, propVal);
}

long CSSDBGrid::GetRowSelectionStyle()
{
	long result;
	GetProperty(0x4e, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetRowSelectionStyle(long propVal)
{
	SetProperty(0x4e, VT_I4, propVal);
}

long CSSDBGrid::GetMaxSelectedRows()
{
	long result;
	GetProperty(0x4f, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetMaxSelectedRows(long propVal)
{
	SetProperty(0x4f, VT_I4, propVal);
}

BOOL CSSDBGrid::GetDroppedDown()
{
	BOOL result;
	GetProperty(0x96, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetDroppedDown(BOOL propVal)
{
	SetProperty(0x96, VT_BOOL, propVal);
}

long CSSDBGrid::GetBorderStyle()
{
	long result;
	GetProperty(0x12c, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetBorderStyle(long propVal)
{
	SetProperty(0x12c, VT_I4, propVal);
}

long CSSDBGrid::GetHwndEdit()
{
	long result;
	GetProperty(0x12d, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetHwndEdit(long propVal)
{
	SetProperty(0x12d, VT_I4, propVal);
}

CSSColumns CSSDBGrid::GetColumns()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3e9, VT_DISPATCH, (void*)&pDispatch);
	return CSSColumns(pDispatch);
}

void CSSDBGrid::SetColumns(LPDISPATCH propVal)
{
	SetProperty(0x3e9, VT_DISPATCH, propVal);
}

CSSGroups CSSDBGrid::GetGroups()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3ea, VT_DISPATCH, (void*)&pDispatch);
	return CSSGroups(pDispatch);
}

void CSSDBGrid::SetGroups(LPDISPATCH propVal)
{
	SetProperty(0x3ea, VT_DISPATCH, propVal);
}

CSSSelBookmarks CSSDBGrid::GetSelBookmarks()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3eb, VT_DISPATCH, (void*)&pDispatch);
	return CSSSelBookmarks(pDispatch);
}

void CSSDBGrid::SetSelBookmarks(LPDISPATCH propVal)
{
	SetProperty(0x3eb, VT_DISPATCH, propVal);
}

CSSStyleSets CSSDBGrid::GetStyleSets()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3ec, VT_DISPATCH, (void*)&pDispatch);
	return CSSStyleSets(pDispatch);
}

void CSSDBGrid::SetStyleSets(LPDISPATCH propVal)
{
	SetProperty(0x3ec, VT_DISPATCH, propVal);
}

BOOL CSSDBGrid::GetUseDefaults()
{
	BOOL result;
	GetProperty(0x3ed, VT_BOOL, (void*)&result);
	return result;
}

void CSSDBGrid::SetUseDefaults(BOOL propVal)
{
	SetProperty(0x3ed, VT_BOOL, propVal);
}

long CSSDBGrid::GetTabNavigation()
{
	long result;
	GetProperty(0x3ee, VT_I4, (void*)&result);
	return result;
}

void CSSDBGrid::SetTabNavigation(long propVal)
{
	SetProperty(0x3ee, VT_I4, propVal);
}

COleFont CSSDBGrid::GetPageHeaderFont()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3ef, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CSSDBGrid::SetPageHeaderFont(LPDISPATCH propVal)
{
	SetProperty(0x3ef, VT_DISPATCH, propVal);
}

COleFont CSSDBGrid::GetPageFooterFont()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3f0, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CSSDBGrid::SetPageFooterFont(LPDISPATCH propVal)
{
	SetProperty(0x3f0, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CSSDBGrid operations

void CSSDBGrid::DoClick()
{
	InvokeHelper(DISPID_DOCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::AddItem(LPCTSTR Item, const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Item, &Index);
}

void CSSDBGrid::RemoveItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CSSDBGrid::RowBookmark(long RowNum)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		RowNum);
	return result;
}

short CSSDBGrid::RowContaining(float Y)
{
	short result;
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x55, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Y);
	return result;
}

float CSSDBGrid::RowTop(short RowNum)
{
	float result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
		RowNum);
	return result;
}

void CSSDBGrid::Scroll(short Cols, long Rows)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Cols, Rows);
}

void CSSDBGrid::ReBind()
{
	InvokeHelper(0x58, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CSSDBGrid::ColContaining(float X, const VARIANT& Y)
{
	short result;
	static BYTE parms[] =
		VTS_R4 VTS_VARIANT;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		X, &Y);
	return result;
}

short CSSDBGrid::GrpContaining(float X)
{
	short result;
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x5a, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		X);
	return result;
}

VARIANT CSSDBGrid::GetBookmark(long RowNum)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		RowNum);
	return result;
}

void CSSDBGrid::RemoveAll()
{
	InvokeHelper(0x5d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::MoveFirst()
{
	InvokeHelper(0x5e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::MovePrevious()
{
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::MoveNext()
{
	InvokeHelper(0x60, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::MoveLast()
{
	InvokeHelper(0x61, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::MoveRecords(long Records)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x62, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Records);
}

BOOL CSSDBGrid::IsItemInList()
{
	BOOL result;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CSSDBGrid::WhereIs(float X, float Y)
{
	long result;
	static BYTE parms[] =
		VTS_R4 VTS_R4;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		X, Y);
	return result;
}

CSSActiveCell CSSDBGrid::ActiveCell()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CSSActiveCell(pDispatch);
}

short CSSDBGrid::ColPosition(short ColPos)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		ColPos);
	return result;
}

short CSSDBGrid::GrpPosition(short GrpPos)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		GrpPos);
	return result;
}

void CSSDBGrid::Reset()
{
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CSSDBGrid::IsAddRow()
{
	BOOL result;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

VARIANT CSSDBGrid::AddItemBookmark(long RowIndex)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		RowIndex);
	return result;
}

long CSSDBGrid::AddItemRowIndex(const VARIANT& Bookmark)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&Bookmark);
	return result;
}

void CSSDBGrid::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::Update()
{
	InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString CSSDBGrid::DeleteSelected()
{
	CString result;
	InvokeHelper(0x12e, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CSSDBGrid::AddNew()
{
	InvokeHelper(0x12f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::CancelUpdate()
{
	InvokeHelper(0x130, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CSSDBGrid::Export(long Type, long Flags, LPCTSTR ExportToFile, const VARIANT& HTMLTemplate, const VARIANT& OutputFileField)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x131, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type, Flags, ExportToFile, &HTMLTemplate, &OutputFileField);
}

void CSSDBGrid::PrintData(long Flags, BOOL ShowSetupDialog, BOOL ShowPrintDialog, const VARIANT& Reserved)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL VTS_BOOL VTS_VARIANT;
	InvokeHelper(0x132, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Flags, ShowSetupDialog, ShowPrintDialog, &Reserved);
}

void CSSDBGrid::SaveLayout(LPCTSTR FileName, long Flags)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x44c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Flags);
}

void CSSDBGrid::LoadLayout(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x44d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}
