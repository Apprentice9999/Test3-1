// CmdCallback.h: interface for the CScdCmdIF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CmdCallback_H__CDB1041F_E9C8_4091_AC94_BF17D6820196__INCLUDED_)
#define AFX_CmdCallback_H__CDB1041F_E9C8_4091_AC94_BF17D6820196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\com\ScdCOM\ScdCOMIF.h"

class CScdCmdIF : public CScdCOMBase  
{
public:
	CLSID& clsidGetCLSID();
	LPWSTR pwszGetProgID();
	LPWSTR pwszGetVIProgID();
	LPWSTR pwszGetClassDesc();

	HRESULT get_UpNAbout(VARIANT_BOOL *pVal);
	HRESULT WaitUpNAbout(double TimeOut);
//	HRESULT get_SyncCallsOn(VARIANT_BOOL *pVal);
//	HRESULT put_SyncCallsOn(VARIANT_BOOL newVal);
//	HRESULT get_SyncCallsTimeOut(double *pVal);
//	HRESULT put_SyncCallsTimeOut(double newVal);
	HRESULT LoadProject(LPCTSTR bsProjectName);
	HRESULT SaveProject(LPCTSTR bsProjectName);
	HRESULT LoadSnapshot(LPCTSTR bsSnapshotName);
	HRESULT SaveSnapshot(LPCTSTR bsSnapshotName);
	HRESULT LoadScenario(LPCTSTR bsScenarioName);
	HRESULT SaveScenario(LPCTSTR bsScenarioName);
	HRESULT LoadBacktrack(LPCTSTR bsBacktrackName);
	HRESULT SaveBacktrack(LPCTSTR bsBacktrackName);
	HRESULT CloseProject();
	HRESULT SetTag(LPCTSTR Tag, VARIANT *Value);
	HRESULT GetTag(LPCTSTR Tag, VARIANT *Value);
	HRESULT put_TagValue(LPCTSTR Tag, VARIANT Value);
	HRESULT get_TagValue(LPCTSTR Tag, VARIANT *Value);
	HRESULT StartDynamicSolve();
	HRESULT StartProbalSolve();
	HRESULT Start();
	HRESULT Step(long IterMark, double WaitForNext);
	HRESULT Stop();
	HRESULT Idle();
	HRESULT Wait(double Seconds);
	HRESULT WaitCount(long Iterations);
	HRESULT WaitTillSteadyState();
	HRESULT WaitTillStop();
	HRESULT ResetWait();
	HRESULT GenerateTagReport(LPCTSTR Filename, LPCTSTR Reportname);
	HRESULT GenerateTrendReport(LPCTSTR Filename, LPCTSTR Reportname);
	HRESULT ProcessSetTags(LPCTSTR Filename, LPCTSTR Reportname);
	HRESULT SaveAndCloseReport(LPCTSTR Filename);
	HRESULT ExecuteMacro(LPCTSTR Filename, LPCTSTR Macroname);
	HRESULT get_CmpFilename(BSTR *pVal);
	HRESULT put_CmpFilename(BSTR newVal);
	HRESULT get_CmpSort(VARIANT_BOOL *pVal);
	HRESULT put_CmpSort(VARIANT_BOOL newVal);
	HRESULT get_CmpMaxCount(long *pVal);
	HRESULT put_CmpMaxCount(long newVal);
	HRESULT get_CmpRelativeTolerance(double *pVal);
	HRESULT put_CmpRelativeTolerance(double newVal);
	HRESULT get_CmpAbsoluteTolerance(double *pVal);
	HRESULT put_CmpAbsoluteTolerance(double newVal);
	HRESULT get_CmpShowDifferentStrings(VARIANT_BOOL *pVal);
	HRESULT put_CmpShowDifferentStrings(VARIANT_BOOL newVal);
	HRESULT get_CmpShowMissingTags(VARIANT_BOOL *pVal);
	HRESULT put_CmpShowMissingTags(VARIANT_BOOL newVal);
	HRESULT CompareReset();
	HRESULT CompareScenarios(LPCTSTR Filename1, LPCTSTR Filename2);
	HRESULT CompareScenarioToCurrent(LPCTSTR Filename);
	HRESULT TestScenario(LPCTSTR Filename);
	HRESULT RestartHistorian();
  HRESULT Sleep(double Seconds);
  HRESULT Exit();
//  HRESULT get_EventsOn(VARIANT_BOOL *pVal);
//  HRESULT put_EventsOn(VARIANT_BOOL newVal);
  HRESULT get_IsStopped(/*[out, retval]*/ VARIANT_BOOL *pVal);
  HRESULT get_IsIdling(/*[out, retval]*/ VARIANT_BOOL *pVal);
  HRESULT get_IsRunning(/*[out, retval]*/ VARIANT_BOOL *pVal);
  HRESULT get_StepSize(double *pVal);
  HRESULT put_StepSize(double newVal);
  HRESULT get_RealTimeOn(/*[out, retval]*/ VARIANT_BOOL *pVal); 
  HRESULT put_RealTimeOn(/*[in]*/ VARIANT_BOOL newVal);
  HRESULT get_RealTimeMultiplier(/*[out, retval]*/ double *pVal);
  HRESULT put_RealTimeMultiplier(/*[in]*/ double newVal);
  HRESULT get_SequenceNo(/*[out, retval]*/ long *pVal);
  HRESULT put_SequenceNo(/*[in]*/ long newVal);
  HRESULT SetAppWndState(/*[in]*/ long ReqdState);
  HRESULT VariableSetReplayItem(/* [in] */ long SequenceNo,/* [in] */ LPCTSTR Tag,/* [in] */ VARIANT Value,/* [in] */ LPCTSTR CnvTxt);
  HRESULT VariableSetReplayCtrl(/* [in] */ long Command, /* [in] */ long Param);
	

//
// Need to provide implementations for these
//

//  HRESULT get_TagValue(LPCTSTR Tag, VARIANT *pVal) { return(S_FALSE); };
//   HRESULT put_TagValue(LPCTSTR Tag, VARIANT newVal) { return(S_FALSE); };
   HRESULT get_TagType(LPCTSTR Tag, long *pVal) { return(S_FALSE); };

   HRESULT InsertNode(/*[in]*/ LPCTSTR Class, /*[in]*/ LPCTSTR Tag, /*[in]*/ double X, /*[in]*/ double Y, /*[in]*/ double Z, /*[in]*/ double XScale, /*[in]*/ double YScale, /*[in]*/ double Rotation ) { return(S_FALSE); };
   HRESULT InsertLink(/*[in]*/ LPCTSTR Class, /*[in]*/ LPCTSTR Tag, /*[in]*/ LPCTSTR SrcTag, /*[in]*/ LPCTSTR SrcIO, /*[in]*/ LPCTSTR DstTag, /*[in]*/ LPCTSTR DstIO, /*[in]*/ double Xs, /*[in]*/ double Ys, /*[in]*/ double Zs, /*[in]*/ double Xd, /*[in]*/ double Yd, /*[in]*/ double Zd) { return(S_FALSE); };
   HRESULT get_GrfWndIndex(/*[out, retval]*/ long *pVal) { return(S_FALSE); };
   HRESULT put_GrfWndIndex(/*[in]*/ long newVal) { return(S_FALSE); };
   HRESULT ZoomWindowFull() { return(S_FALSE); };
   HRESULT CreateUnit(/*[in]*/ LPCTSTR Class, /*[in]*/ LPCTSTR Tag ) { return(S_FALSE); };
   HRESULT CreateUnitGrf(/*[in]*/ LPCTSTR Tag, /*[in]*/ double X, /*[in]*/ double Y, /*[in]*/ double Z, /*[in]*/ LPCTSTR Symbol, /*[in]*/ double XScale, /*[in]*/ double YScale, /*[in]*/ double Rotation ) { return(S_FALSE); };
   HRESULT CreateLink(/*[in]*/ LPCTSTR Class, /*[in]*/ LPCTSTR Tag, /*[in]*/ LPCTSTR SrcTag, /*[in]*/ LPCTSTR SrcIO, /*[in]*/ LPCTSTR DstTag, /*[in]*/ LPCTSTR DstIO) { return(S_FALSE); };
   HRESULT CreateLinkGrf(/*[in]*/ LPCTSTR Tag, /*[in]*/ long DrawLineMethod) { return(S_FALSE); };
   HRESULT CreateLinkGrfLine(/*[in]*/ LPCTSTR Tag, /*[in]*/ double Xs, /*[in]*/ double Ys, /*[in]*/ double Zs, /*[in]*/ double Xd, /*[in]*/ double Yd, /*[in]*/ double Zd) { return(S_FALSE); };


  CScdCmdIF();
	virtual ~CScdCmdIF();

};

#endif // !defined(AFX_CmdCallback_H__CDB1041F_E9C8_4091_AC94_BF17D6820196__INCLUDED_)
