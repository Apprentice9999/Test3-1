#include "stdafx.h"

#define __MAKEUPBLOCK_CPP
#include "MakeupBlock.h"
//#include "optoff.h"

#if WITHBLOCKEVALUATOR

//=========================================================================
//
//
//
//=========================================================================

XID xidMakeupEnable = AdjustXID(1);
XID xidMakeupMdlNm  = AdjustXID(2);

//============================================================================
//
//
//
//============================================================================

const pchar CMakeupBlock::GroupName="MakeupBlock";

IMPLEMENT_MAKEUPBLOCK(CMakeupBlock, "MB_None", "", TOC_ALL|TOC_GRP_GENERAL|TOC_STD_KENWALT, "None",  " ");

CMakeupBlock::CMakeupBlock(pTagObjClass pClass_, pchar Tag_, TaggedObject* pAttach, TagObjAttachment eAttach) :
TaggedObject(pClass_, Tag_, pAttach, eAttach)
  {
  m_pMakeupBase = NULL;
  }

//--------------------------------------------------------------------------

CMakeupBlock::~CMakeupBlock()
  {
  };

//--------------------------------------------------------------------------

void CMakeupBlock::BuildDataDefn(DataDefnBlk& DDB)
  {
  if (DDB.BeginStruct(this, NULL, NULL, DDB_NoPage))
    {
    }
  DDB.EndStruct();
  };

//--------------------------------------------------------------------------

void CMakeupBlock::EvalProducts(SpConduit &Qf, double Po, double FinalTEst)
  {
  };

void CMakeupBlock::EvalProductsPipe(SpConduit & Qf, double Len, double Diam, double Po, double FinalTEst)
  {
  };

//============================================================================
//
//
//
//============================================================================

CMakeupBase::CMakeupBase(TaggedObject * pAttach, int Index) : CBlockEvalBase(BEId_Makeup, Index),
m_SrcIO(eDIO_Makeup, dynamic_cast<FlwNode*>(pAttach), false, true, 
        "Makeup", IOId_Makeup2Area+Index, IOId_AreaMakeupO, "MakeupSrc", "MakeupSrc_1")
  { 
  m_pMakeupB=NULL; 
  m_pNd=pAttach; 
  m_fEnabled=false;
  m_fFixed=false; 
  //m_Index=Index; 
  }

CMakeupBase::~CMakeupBase()  
  { 
  delete m_pMakeupB; 
  };

Strng CMakeupBase::Name()
  {
  Strng S;
  S.Set("M%i", m_Index+1);
  return S;
  }

flag CMakeupBase::Open(TagObjClass * pMakeupClass, flag Fixed)
  {
  m_fEnabled=True;

  if (m_pMakeupB && (pMakeupClass==NULL || m_pMakeupB->Class()==pMakeupClass))
    return True;

  delete m_pMakeupB;
  m_fFixed=Fixed;

  if (pMakeupClass)
    m_pMakeupB=(CMakeupBlock*)pMakeupClass->Construct(NULL, "Makeup", m_pNd, TOA_Embedded);//pNd);
  else
    m_pMakeupB=(CMakeupBlock*)CMakeupBlockClass.Construct(NULL, "Makeup", m_pNd, TOA_Embedded);//pNd);
  m_pNd->StructureChanged(NULL);
  m_pMakeupB->m_pMakeupBase=this;

  m_fEnabled=m_pMakeupB!=NULL;
  return m_fEnabled;
  };

//--------------------------------------------------------------------------

void CMakeupBase::Close()
  {
  if (m_fFixed)
    return;
  m_fEnabled=False;
  }

//--------------------------------------------------------------------------

void CMakeupBase::Add_OnOff(DataDefnBlk &DDB, dword Flags, int UserInfo)
  {
  Strng S;
  S.Set("Makeup%i", m_Index+1);
  DDB.PushUserInfo(UserInfo);
  DDB.Byte(S(), "",  DC_,    "",     xidMakeupEnable,  m_pNd, Flags, GetOnOffValLst());
  DDB.PopUserInfo();
  }

//--------------------------------------------------------------------------

void CMakeupBase::BuildDataDefn(DataDefnBlk &DDB, char* pTag, char* pTagComment, DDBPages PageIs, dword UserInfo)
  {
  DDB.PushUserInfo(UserInfo);
  if (pTag==NULL)
    pTag="Xx";
  DDEF_Flags Old=DDB.GetVisibility();
  DDB.Visibility(SHM_All, m_fEnabled);
  if (Enabled())//pHL)
    {
    DDB.Text("");
    if (DDB.BeginObject(m_pNd, Name()(), "EB_Makeup", pTagComment, PageIs))
      {
      if (m_SrcIO.Enabled)
        m_SrcIO.BuildDataDefn(DDB, NULL, DDB_NoPage, UserInfo+102, 0);//DFIO_ShowQm);

      DDBValueLstMem DDB0;
      TagObjClass::GetSDescValueLst(CMakeupBlock::GroupName, DDB0);
      DDB.String  ("Model",      "",       DC_    , "",      xidMakeupMdlNm  , m_pNd,m_fFixed ? 0 : isParmStopped|SetOnChange, DDB0());

      if (m_pMakeupB)
        {
        //DDB.SuppressNextBlock();
        m_pMakeupB->BuildDataDefn(DDB);
        }
      }
    DDB.EndObject();
    }
  DDB.SetVisibility(Old);
  DDB.PopUserInfo();
  }

//--------------------------------------------------------------------------

flag CMakeupBase::DataXchg(DataChangeBlk & DCB)
  {
  if (DCB.dwUserInfo%1000==102)
    {
    if (m_SrcIO.DataXchg(DCB))
      return 1;
    }

  switch (DCB.lHandle)
    {
    case xidMakeupEnable:
      if (DCB.rB)
        {
        if (*DCB.rB)
          Open(*DCB.rB);
        else
          Close();
        }
      DCB.B=OpenStatus();// (Enabled());
      return 1;
    case xidMakeupMdlNm:
      if (DCB.rpC && !m_fFixed)
        {
        int WasEnabled=m_fEnabled;
        TagObjClass * pC=CMakeupBlockClass.FindGrpShortDesc(DCB.rpC);
        if (pC)
          Open(pC);
        else
          Close();
        }
      DCB.pC = m_pMakeupB ? m_pMakeupB->ShortDesc() : "";
      return 1;
    }
  return 0;
  }

//============================================================================
//
//
//
//============================================================================

#define DllImportExport /* */

class DllImportExport CXBlk_Makeup: public CMakeupBlock
  {
  public:
    CXBlk_Makeup(TagObjClass* pClass_, pchar Tag_, TaggedObject* pAttach, TagObjAttachment eAttach);
    virtual ~CXBlk_Makeup();

    void           SetUpDDBSpcs();
    void           SetUpDDBCmps();
    virtual void   BuildDataDefn(DataDefnBlk& DDB);
    virtual flag   DataXchg(DataChangeBlk & DCB);
    virtual flag   ValidateData(ValidateDataBlk & VDB);

    virtual void   EvalProducts(SpConduit & Fo, double Po, double FinalTEst=dNAN);
    virtual void   EvalProductsPipe(SpConduit & Fo, double Len, double Diam, double Po, double FinalTEst=dNAN);

    double         GetSetPoint();
    double         GetRawMeas(SpConduit &QPrd);
    double         GetMeasVal(SpConduit &QIn, SpConduit &QPrd);

  public:
    enum eSource
      {
      Src_Self,
      Src_Remote,
      };

    enum eType
      { 
      //Type_None, 
      Type_MassFlow, 
      Type_MassRatio, 
      Type_MassMult, 
      Type_MoleFlow, 
      Type_MoleRatio, 
      Type_MoleMult, 
      Type_VolumeFlow, 
      Type_VolumeRatio, 
      Type_VolumeMult, 
      Type_NVolumeFlow, 
      Type_NVolumeRatio,
      Type_NVolumeMult,
      Type_MassFrac,
      Type_MoleFrac,
      Type_VolumeFrac,
      Type_NVolumeFrac,
      Type_Conc,
      };

    enum eSelect
      {
      Slct_All,
      Slct_Occur,
      Slct_Phase,
      Slct_Specie,
      Slct_Component,
      };

    enum eTemp
      {
      Temp_Inlet,
      Temp_Source,
      Temp_Std,
      Temp_Const,
      Temp_Mixture,
      };

    eSource         m_eSource;
    double          m_QmMin;
    double          m_QmMax;
    eType           m_eType;
    eSelect         m_eSelect;
    PhMask          m_Phases; 
    CIArray         m_Species;
    CIArray         m_Comps;
    bool            m_DDBSpcsOK;
    DDBValueLstMem  m_DDBSpcAdd;
    DDBValueLstMem  m_DDBSpcRem;
    int             m_nLastSpcStr;
    CStringArray    m_SpcStr;
    bool            m_DDBCmpsOK;
    DDBValueLstMem  m_DDBCmpAdd;
    DDBValueLstMem  m_DDBCmpRem;
    int             m_nLastCmpStr;
    CStringArray    m_CmpStr;

    double          m_QmRqd;
    double          m_QmRatio;
    double          m_QmMult;
    double          m_MassFrac;
    double          m_QMlRqd;
    double          m_QMlRatio;
    double          m_QMlMult;
    double          m_MoleFrac;
    double          m_QvRqd;
    double          m_QvRatio;
    double          m_QvMult;
    double          m_VolFrac;
    double          m_NQvRqd;
    double          m_NQvRatio;
    double          m_NQvMult;
    double          m_NVolFrac;
    double          m_Conc;

    eTemp           m_eRqdTemp;
    double          m_RqdTemp;

    //results
    double          m_dSetPoint;
    double          m_dMeas;
    double          m_dResult;
    double          m_dQmMakeup;
    double          m_dQmFeed;
    double          m_dQmProd;
    double          m_dHeatFlow;
    double          m_dTempKFeed;
    double          m_dTempKProd;

    CToleranceBlock m_QmTol;

  };

DEFINE_MAKEUPBLOCK(CXBlk_Makeup);

//============================================================================
//
//
//
//============================================================================

XID xidMkSelect  = AdjustXID(1000);
XID xidMkAll     = AdjustXID(1001);
XID xidMkSolids  = AdjustXID(1002);
XID xidMkLiquids = AdjustXID(1003);
XID xidMkGasses  = AdjustXID(1004);
XID xidMkSpcCnt  = AdjustXID(1006);
XID xidMkAddSpc  = AdjustXID(1007);
XID xidMkRemSpc  = AdjustXID(1008);
XID xidMkCmpCnt  = AdjustXID(1009);
XID xidMkAddCmp  = AdjustXID(1010);
XID xidMkRemCmp  = AdjustXID(1011);
XID xidMkPhase   = AdjustXID(1200);


IMPLEMENT_MAKEUPBLOCK(CXBlk_Makeup, "MB_Simple", "", TOC_ALL|TOC_GRP_GENERAL|TOC_STD_KENWALT, "SimpleControl",  " ");

CXBlk_Makeup::CXBlk_Makeup(pTagObjClass pClass_, pchar Tag_, TaggedObject* pAttach, TagObjAttachment eAttach) :
CMakeupBlock(pClass_, Tag_, pAttach, eAttach),
m_QmTol(TBF_DynSys, "Makeup:QmFndEPS", 1.0e-8, 1.0e-8, 200)
  {
  m_eSource   = Src_Remote;
  m_eType     = Type_MassRatio;
  m_eSelect   = Slct_All;
  m_Phases    = som_ALL;
  m_DDBSpcsOK = false;
  m_DDBCmpsOK = false;

  m_QmMin     = 0.0;
  m_QmMax     = 10000.0/3.6;

  m_QmRqd     = 0;
  m_QMlRqd    = 0;
  m_QvRqd     = 0;
  m_NQvRqd    = 0;
  m_QmRatio   = 0;
  m_QMlRatio  = 0;
  m_QvRatio   = 0;
  m_NQvRatio  = 0;
  m_QmMult    = 1.0;
  m_QMlMult   = 1.0;
  m_QvMult    = 1.0;
  m_NQvMult   = 1.0;
  m_MassFrac  = 0;
  m_MoleFrac  = 0;
  m_VolFrac   = 0;
  m_NVolFrac  = 0;
  m_Conc      = 0;

  m_eRqdTemp  = Temp_Inlet;
  m_RqdTemp   = C2K(25);

  m_dSetPoint   = 0.0;
  m_dMeas       = 0.0;
  m_dResult     = 0.0;
  m_dQmMakeup   = 0.0;
  m_dQmFeed     = 0.0;
  m_dQmProd     = 0.0;
  m_dHeatFlow   = 0.0;
  m_dTempKFeed  = C2K(0.0);
  m_dTempKProd  = C2K(0.0);

  }

//--------------------------------------------------------------------------

CXBlk_Makeup::~CXBlk_Makeup()
  {
  }

//--------------------------------------------------------------------------

void CXBlk_Makeup::SetUpDDBSpcs()
  {

  if (!m_DDBSpcsOK || m_DDBSpcAdd.Length()+m_DDBSpcRem.Length()<SDB.Count())
    {
    m_DDBSpcAdd.Empty();
    m_DDBSpcRem.Empty();
    m_DDBSpcAdd.Add(-2, "AllAvailable");
    m_DDBSpcAdd.Add(-1, " - ");
    m_DDBSpcRem.Add(-2, "AllAvailable");
    m_DDBSpcRem.Add(-1, " - ");

    bool InList[MaxSpeciesEx];
    for (int s=0; s<SDB.Count(); s++)
      InList[s]=false;

    bool NeedsComma=false;
    m_nLastSpcStr=0;
    m_SpcStr.SetSize(Max(m_nLastSpcStr+1, m_SpcStr.GetSize()));
    m_SpcStr[m_nLastSpcStr]="Species:";
    for (int j=0; j<m_Species.GetSize(); j++)
      {
      int s=m_Species[j];
      InList[s]=true;
      m_DDBSpcRem.Add(s, SDB[s].SymOrTag());

      if (NeedsComma)
        m_SpcStr[m_nLastSpcStr]+=",";
      m_SpcStr[m_nLastSpcStr]+=SDB[s].SymOrTag();
      if (m_SpcStr[m_nLastSpcStr].GetLength()>35)
        {
        m_nLastSpcStr++;
        m_SpcStr.SetSize(Max(m_nLastSpcStr+1, m_SpcStr.GetSize()));
        m_SpcStr[m_nLastSpcStr]="        ";
        NeedsComma=false;
        }
      else
        NeedsComma=true;
      }

    for (int s=0; s<SDB.Count(); s++)
      {
      if (!InList[s])
        m_DDBSpcAdd.Add(s,SDB[s].SymOrTag());
      }
    }

  m_DDBSpcsOK = true;
  }

//--------------------------------------------------------------------------

void CXBlk_Makeup::SetUpDDBCmps()
  {
  if (!m_DDBCmpsOK || m_DDBCmpAdd.Length()+m_DDBCmpRem.Length()<CDB.No())
    {
    m_DDBCmpAdd.Empty();
    m_DDBCmpRem.Empty();
    m_DDBCmpAdd.Add(-2, "AllAvailable");
    m_DDBCmpAdd.Add(-1, " - ");
    m_DDBCmpRem.Add(-2, "AllAvailable");
    m_DDBCmpRem.Add(-1, " - ");

    bool InList[MaxSpeciesEx];
    for (int s=0; s<CDB.No(); s++)
      InList[s]=false;

    bool NeedsComma=false;
    m_nLastCmpStr=0;
    m_CmpStr.SetSize(Max(m_nLastCmpStr+1, m_CmpStr.GetSize()));
    m_CmpStr[m_nLastCmpStr]="Comps:";
    for (int j=0; j<m_Comps.GetSize(); j++)
      {
      int s=m_Comps[j];
      InList[s]=true;
      m_DDBCmpRem.Add(s, CDB[s].SymOrTag());

      if (NeedsComma)
        m_CmpStr[m_nLastCmpStr]+=",";
      m_CmpStr[m_nLastCmpStr]+=CDB[s].SymOrTag();
      if (m_CmpStr[m_nLastCmpStr].GetLength()>35)
        {
        m_nLastCmpStr++;
        m_CmpStr.SetSize(Max(m_nLastCmpStr+1, m_CmpStr.GetSize()));
        m_CmpStr[m_nLastCmpStr]="        ";
        NeedsComma=false;
        }
      else
        NeedsComma=true;
      //for (int ss=CDB[s].
      }

    for (int s=0; s<CDB.No(); s++)
      {
      if (!InList[s])
        m_DDBCmpAdd.Add(s,CDB[s].SymOrTag());
      }
    }

  m_DDBCmpsOK = true;
  }

//--------------------------------------------------------------------------

void CXBlk_Makeup::BuildDataDefn(DataDefnBlk& DDB)
  {

  //if (DDB.BeginStruct(this, "MakeupQm", NULL, DDB_NoPage))
  //  {
  static DDBValueLst DDBCtrl[] =
    {                         
      {Type_MassFlow,       "MassFlow"          },
      {Type_MassRatio,      "MassRatio"         },
      {Type_MassMult,       "MassMultiplier"    },
      {Type_MoleFlow,       "MoleFlow"          },
      {Type_MoleRatio,      "MoleRatio"         },
      {Type_MoleMult,       "MoleMultiplier"    },
      {Type_VolumeFlow,     "VolumeFlow"        },
      {Type_VolumeRatio,    "VolumeRatio"       },
      {Type_VolumeMult,     "VolumeMultiplier"  },
      {Type_NVolumeFlow,    "NVolumeFlow"       },
      {Type_NVolumeRatio,   "NVolumeRatio"      },
      {Type_NVolumeMult,    "NVolumeMultiplier" },
      {Type_MassFrac,       "MassFrac"          },
      {Type_MoleFrac,       "MoleFrac"          },
      {Type_VolumeFrac,     "VolumeFrac"        },
      {Type_NVolumeFrac,    "NVolumeFrac"       },
      {Type_Conc,           "Concentration"     },
      {}
    };
  static DDBValueLst DDBSelect[] =
    {                         
      {Slct_All,            "All"             },
      {Slct_Occur,          "Occurence"       },
      {Slct_Phase,          "Phase"           },
      {Slct_Specie,         "Specie"          },
      {Slct_Component,      "Component"       },
      {}
    };
  static DDBValueLst DDBSource[] =
    {                         
      {Src_Self,            "Self"            },
      {Src_Remote,          "Remote"          },
      {}
    };

  DDB.Text(" ");
  DDB.Text("Requirements");
  DDB.Long       ("", "Type",             DC_,  "", (long*)&m_eType,  this, isParmStopped|SetOnChange, DDBCtrl);
  DDB.Visibility(SHM_All, m_eType==Type_MassFlow);
  DDB.Double("", "QmRqd",    DC_Qm, "kg/s", &m_QmRqd,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MoleFlow);
  DDB.Double("", "QMlRqd",   DC_QKgMl, "kmol/s", &m_QMlRqd,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_VolumeFlow);
  DDB.Double("", "QvRqd",    DC_Qv, "m^3/s", &m_QvRqd,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_NVolumeFlow);
  DDB.Double("", "NQvRqd",    DC_NQv, "Nm^3/s", &m_NQvRqd,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MassRatio);
  DDB.Double("", "QmRatio",    DC_Frac, "%", &m_QmRatio,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MoleRatio);
  DDB.Double("", "QmRatio",    DC_Frac, "%", &m_QMlRatio,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_VolumeRatio);
  DDB.Double("", "QvRatio",    DC_Frac, "%", &m_QvRatio,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_NVolumeRatio);
  DDB.Double("", "NQvRatio",    DC_Frac, "%", &m_NQvRatio,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MassMult);
  DDB.Double("", "QmMult",    DC_Frac, "%", &m_QmMult,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MoleMult);
  DDB.Double("", "QmMult",    DC_Frac, "%", &m_QMlMult,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_VolumeMult);
  DDB.Double("", "QvMult",    DC_Frac, "%", &m_QvMult,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_NVolumeMult);
  DDB.Double("", "NQvMult",    DC_Frac, "%", &m_NQvMult,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MassFrac);
  DDB.Double("", "MassFrac",    DC_Frac, "%", &m_MassFrac,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_MoleFrac);
  DDB.Double("", "MoleFrac",    DC_Frac, "%", &m_MoleFrac,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_VolumeFrac);
  DDB.Double("", "VolumeFrac",    DC_Frac, "%", &m_VolFrac,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_NVolumeFrac);
  DDB.Double("", "NVolumeFrac",    DC_Frac, "%", &m_NVolFrac,  this, isParm);
  DDB.Visibility(SHM_All, m_eType==Type_Conc);
  DDB.Double("", "Concentration",  DC_Conc, "g/L", &m_Conc,  this, isParm);

  DDB.Visibility();
  DDB.Text(" ");
  DDB.Long       ("", "Selection",        DC_,  "", xidMkSelect,  this, isParm|SetOnChange, DDBSelect);
  //DDB.Text("Selection");
  if (DDB.ForFileSnpScn())
    {
    switch (m_eSelect)
      {
      case Slct_All:
      case Slct_Occur:
      case Slct_Phase:
        DDB.Long("", "PhaseMask",               DC_,  "", (long*)&m_Phases,     this, isParm|InitHidden);
        break;
      case Slct_Specie:
        {
        SetUpDDBSpcs();

        DDB.Long  ("NSpcs",       "", DC_,      "",   xidMkSpcCnt, this, isParm);
        if (DDB.BeginArray(this, "Spcs", "MU_SpIndex", m_Species.GetSize(), 0, DDB_NoPage, isParm))
          {
          for (int s=0; s<m_Species.GetSize(); s++)
            {
            DDB.BeginElement(this, s);
            DDB.Int("Spc", "", DC_, "", &m_Species[s], this, isParmConstruct, &SDB.DDBSpList);
            }
          };
        DDB.EndArray();
        break;
        }
      case Slct_Component:
        {
        SetUpDDBCmps();

        DDB.Long  ("NCmps",       "", DC_,      "",   xidMkCmpCnt, this, isParm);
        if (DDB.BeginArray(this, "Cmps", "MU_CmpIndex", m_Comps.GetSize(), 0, DDB_NoPage, isParm))
          {
          for (int s=0; s<m_Comps.GetSize(); s++)
            {
            DDB.BeginElement(this, s);
            DDB.Int("Cmp", "", DC_, "", &m_Comps[s], this, isParmConstruct, &CDB.DDBCompList);
            }
          };
        DDB.EndArray();
        break;
        }
      }  
    }
  else
    {
    switch (m_eSelect)
      {
      case Slct_All:
        break;
      case Slct_Occur:
        {
        DDB.CheckBoxBtn("", "Solids",           DC_,  "", xidMkSolids,  this, isParm);
        DDB.CheckBoxBtn("", "Liquids",          DC_,  "", xidMkLiquids, this, isParm);
        DDB.CheckBoxBtn("", "Gasses",           DC_,  "", xidMkGasses,  this, isParm);
        break;
        }
      case Slct_Phase:
        {
        for (int o=CDB.PhaseFirst(BOT_Solid); o<=CDB.PhaseLast(BOT_Gas); o++)
          {
          CPhaseInfo & P=CDB.PhaseInfo(o);
          Strng T,S;
          T.Set("(%s)", P.m_Tag());
          S.Set("(%s)", P.m_Sym());
          DDB.CheckBoxBtn(T(), S(), DC_,  "", xidMkPhase+o,  this, isParm);
          }
        break;
        }
      case Slct_Specie:
        {
        SetUpDDBSpcs();
        for (int s=0; s<=m_nLastSpcStr; s++)
          DDB.Text((LPTSTR)(LPCTSTR)m_SpcStr[s]);
        DDB.Long  ("Add",    "", DC_, "", xidMkAddSpc, this, (m_DDBSpcAdd.Length()>1?isParm:0)|SetOnChange, &m_DDBSpcAdd);
        DDB.Long  ("Remove", "", DC_, "", xidMkRemSpc, this, (m_DDBSpcRem.Length()>1?isParm:0)|SetOnChange, &m_DDBSpcRem);
        break;
        }
      case Slct_Component:
        {
        SetUpDDBCmps();
        for (int s=0; s<=m_nLastCmpStr; s++)
          DDB.Text((LPTSTR)(LPCTSTR)m_CmpStr[s]);
        DDB.Long  ("Add",    "", DC_, "", xidMkAddCmp, this, (m_DDBCmpAdd.Length()>1?isParm:0)|SetOnChange, &m_DDBCmpAdd);
        DDB.Long  ("Remove", "", DC_, "", xidMkRemCmp, this, (m_DDBCmpRem.Length()>1?isParm:0)|SetOnChange, &m_DDBCmpRem);
        break;
        }
      }  
    }

  DDB.Visibility();
  DDB.Text(" ");
  DDB.Long       ("", "Source",           DC_,  "", (long*)&m_eSource,  this, isParm|SetOnChange, DDBSource);
  DDB.Double     ("QmMin", "",            DC_Qm, "kg/s", &m_QmMin, this, isParm);
  DDB.Double     ("QmMax", "",            DC_Qm, "kg/s", &m_QmMax, this, isParm);

  static DDBValueLst DDBTemp[] = 
    {
      {Temp_Inlet,   "InletTemp"},
      {Temp_Source,  "SourceTemp"},
      {Temp_Std,     "StdTemp"},
      {Temp_Const,   "Const"},
      {Temp_Mixture, "Mixture"},  // Needs work - a solve loop for volume
      {0}
    };

  DDB.Text(" ");
  DDB.Long  ("FinalTemp",      "",  DC_,  "", (long*)&m_eRqdTemp, this, isParm/*|DDEF_WRITEPROTECT*/, DDBTemp);
  DDB.Visibility(SHM_All, m_eRqdTemp==Temp_Const);
  DDB.Double("RqdTemp",        "",  DC_T, "C", &m_RqdTemp, this, isParm/*|DDEF_WRITEPROTECT*/);
  DDB.Visibility();

  DDB.Text(" ");
  DDB.Text("Results");
  if (0)
    {
    Strng CnvTxt;
    //CnvTxt = "??"; todo, get currently used cnv text based on type
    DDB.Double ("Meas",              "", DC_,     "",        &m_dMeas,       this, isResult);
    //DDB.TagComment(CnvTxt());
    DDB.Double ("SetPoint",          "", DC_,     "",        &m_dSetPoint,   this, isResult);
    //DDB.TagComment(CnvTxt());
    DDB.Double ("Result",            "", DC_,     "",        &m_dResult,     this, isResult);
    //DDB.TagComment(CnvTxt());
    }
  else
    {
    CCnvIndex CnvUsed;
    Strng CnvTxt;
    switch (m_eType)
      {                         
      case Type_MassFlow    : CnvUsed=DC_Qm; CnvTxt="kg/s"; break;
      case Type_MoleFlow    : CnvUsed=DC_QKgMl; CnvTxt="kmol/s"; break;
      case Type_VolumeFlow  : CnvUsed=DC_Qv; CnvTxt="m^3/s"; break;
      case Type_NVolumeFlow : CnvUsed=DC_NQv; CnvTxt="Nm^3/s"; break;
      case Type_Conc        : CnvUsed=DC_Conc; CnvTxt="g/L"; break;
      default               : CnvUsed=DC_Frac; CnvTxt="%"; break;
      }
    DDB.Visibility(SHM_All, m_eType==Type_MassFlow || m_eType==Type_MoleFlow || m_eType==Type_VolumeFlow || m_eType==Type_NVolumeFlow || 
      m_eType==Type_MassFrac || m_eType==Type_MoleFrac || m_eType==Type_VolumeFrac || m_eType==Type_NVolumeFrac);
    DDB.Double ("Meas",              "", CnvUsed, CnvTxt(),  &m_dMeas,       this, isResult|noFileAtAll);
    DDB.Visibility();
    DDB.Double ("SetPoint",          "", CnvUsed, CnvTxt(),  &m_dSetPoint,   this, isResult|noFileAtAll);
    DDB.Double ("Result",            "", CnvUsed, CnvTxt(),  &m_dResult,     this, isResult|noFileAtAll);
    }
  DDB.Double ("QmMakeup",          "", DC_Qm,   "kg/s",    &m_dQmMakeup,   this, isResult);
  DDB.Double ("QmFeed",            "", DC_Qm,   "kg/s",    &m_dQmFeed,     this, isResult);
  DDB.Double ("QmProd",            "", DC_Qm,   "kg/s",    &m_dQmProd,     this, isResult);
  DDB.Double ("HeatFlow",          "", DC_Pwr,  "kW",      &m_dHeatFlow,   this, isResult);
  DDB.Double ("TempFeed",          "", DC_T,    "C",       &m_dTempKFeed,  this, isResult);//|noFileAtAll);
  DDB.Double ("TempProd",          "", DC_T,    "C",       &m_dTempKProd,  this, isResult);//|noFileAtAll);

  };

//--------------------------------------------------------------------------

flag CXBlk_Makeup::DataXchg(DataChangeBlk & DCB)
  {
  switch (DCB.lHandle)
    {
    case xidMkSelect:
      if (DCB.rL)
        m_eSelect = (eSelect)*DCB.rL;
      DCB.L = m_eSelect; 
      return 1;
    case xidMkAll:
      if (DCB.rB)
        m_Phases = *DCB.rB ? som_ALL:0;
      DCB.B = (m_Phases==som_ALL) ? 1 : (m_Phases==0) ? 0 : 2; 
      return 1;
    case xidMkSolids:
      if (DCB.rB)
        m_Phases = (m_Phases&~som_Sol) | (*DCB.rB ? som_Sol:0);
      DCB.B = ((m_Phases&som_Sol)==som_Sol)? 1 : ((m_Phases&som_Sol)==0) ? 0 : 2; 
      return 1;
    case xidMkLiquids:
      if (DCB.rB)
        m_Phases = (m_Phases&~som_Liq) | (*DCB.rB ? som_Liq:0);
      DCB.B = ((m_Phases&som_Liq)==som_Liq)? 1 : ((m_Phases&som_Liq)==0) ? 0 : 2; 
      return 1;
    case xidMkGasses:
      if (DCB.rB)
        m_Phases = (m_Phases&~som_Gas) | (*DCB.rB ? som_Gas:0);
      DCB.B = ((m_Phases&som_Gas)==som_Gas)? 1 : ((m_Phases&som_Gas)==0) ? 0 : 2; 
      return 1;

    case xidMkSpcCnt:
      if (DCB.rL)
        {
        m_Species.SetSize(*DCB.rL);
        m_DDBSpcsOK = false;
        }
      DCB.L=m_Species.GetSize();
      return 1;
    case xidMkAddSpc:
      if (DCB.rL)
        {
        if (*DCB.rL>=0)
          {
          for (int j=0; j<m_Species.GetSize(); j++)
            {
            if (m_Species[j]>*DCB.rL)
              {
              m_Species.InsertAt(j, *DCB.rL);
              m_DDBSpcsOK=false;
              DCB.L=-1;
              return 1;
              }
            }
          m_Species.Add(*DCB.rL);
          m_DDBSpcsOK=false;
          }
        else if (*DCB.rL==-2)
          {
          m_Species.SetSize(0);
          for (int i=0; i<SDB.Count(); i++)
             m_Species.Add(i);
          m_DDBSpcsOK=false;
          }
        }
      DCB.L=-1;
      return 1;
    case xidMkRemSpc:
      if (DCB.rL)
        {
        if (*DCB.rL>=0)
          {
          for (int j=0; j<m_Species.GetSize(); j++)
            {
            if (m_Species[j]==*DCB.rL)
              {
              m_Species.RemoveAt(j);
              m_DDBSpcsOK=false;
              break;
              }
            }
          }
        else if (*DCB.rL==-2)
          {
          m_Species.SetSize(0);
          m_DDBSpcsOK=false;
          }
        }
      DCB.L=-1;
      return 1;

    case xidMkCmpCnt:
      if (DCB.rL)
        {
        m_Comps.SetSize(*DCB.rL);
        m_DDBCmpsOK = false;
        }
      DCB.L=m_Comps.GetSize();
      return 1;
    case xidMkAddCmp:
      if (DCB.rL)
        {
        if (*DCB.rL>=0)
          {
          for (int j=0; j<m_Comps.GetSize(); j++)
            {
            if (m_Comps[j]>*DCB.rL)
              {
              m_Comps.InsertAt(j, *DCB.rL);
              m_DDBCmpsOK=false;
              DCB.L=-1;
              return 1;
              }
            }
          m_Comps.Add(*DCB.rL);
          m_DDBCmpsOK=false;
          }
        else if (*DCB.rL==-2)
          {
          m_Comps.SetSize(0);
          for (int i=0; i<CDB.No(); i++)
             m_Comps.Add(i);
          m_DDBCmpsOK=false;
          }
        }
      DCB.L=-1;
      return 1;
    case xidMkRemCmp:
      if (DCB.rL)
        {
        if (*DCB.rL>=0)
          {
          for (int j=0; j<m_Comps.GetSize(); j++)
            {
            if (m_Comps[j]==*DCB.rL)
              {
              m_Comps.RemoveAt(j);
              m_DDBCmpsOK=false;
              break;
              }
            }
          }
        else if (*DCB.rL==-2)
          {
          m_Comps.SetSize(0);
          m_DDBCmpsOK=false;
          }
        }
      DCB.L=-1;
      return 1;

    default:
      if (DCB.lHandle>=xidMkPhase && DCB.lHandle<xidMkPhase+CDB.PhaseCount())
        {
        int o=DCB.lHandle-xidMkPhase;
        CPhaseInfo & P=CDB.PhaseInfo(o);
        if (DCB.rB)
          m_Phases = (m_Phases&~P.m_PhMsk) | (*DCB.rB ? P.m_PhMsk:0);
        DCB.B = (m_Phases&P.m_PhMsk) ? 1 : 0; 
        return 1;
        }
    }
  return 0;
  }

//--------------------------------------------------------------------------

flag CXBlk_Makeup::ValidateData(ValidateDataBlk & VDB)
  {
  switch (m_eSelect)
    {
    case Slct_Specie:
      {
      break;
      }
    case Slct_Component:
      {
      m_Species.SetSize(0);
      for (int i=0; i<m_Comps.GetCount(); i++)
        {
        for (int j=0; j<CDB[i].NSpecies(); j++)
          {
          int s=CDB[i].iSpecie(j);
          m_Species.Add(s);
          }
        }
      break;
      }
    default:
      if (m_Phases==som_ALL)
        {
        switch (m_eType)
          {                         
          case Type_MassFrac:
          case Type_VolumeFrac:
          case Type_NVolumeFrac:
            LogError(Tag(), 0, "Fraction Type Invalid");
            break;
          }
        }
      break;
    }

  return CMakeupBlock::ValidateData(VDB); 
  }

//--------------------------------------------------------------------------

double CXBlk_Makeup::GetSetPoint()
  {
  switch (m_eType)
    {                         
    case Type_MassFlow:       return m_QmRqd;
    case Type_MassRatio:      return m_QmRatio;
    case Type_MassMult:       return m_QmMult;
    case Type_MassFrac:       return m_MassFrac;
    case Type_MoleFlow:       return m_QMlRqd;
    case Type_MoleRatio:      return m_QMlRatio;
    case Type_MoleMult:       return m_QMlMult;
    case Type_MoleFrac:       return m_MoleFrac;
    case Type_VolumeFlow:     return m_QvRqd;
    case Type_VolumeRatio:    return m_QvRatio;
    case Type_VolumeMult:     return m_QvMult;
    case Type_VolumeFrac:     return m_VolFrac;
    case Type_NVolumeFlow:    return m_NQvRqd;
    case Type_NVolumeRatio:   return m_NQvRatio;
    case Type_NVolumeMult:    return m_NQvMult;
    case Type_NVolumeFrac:    return m_NVolFrac;
    case Type_Conc:           return m_Conc;
    };
  return dNAN;
  }

//--------------------------------------------------------------------------

double CXBlk_Makeup::GetRawMeas(SpConduit &QPrd)
  {
  switch (m_eType)
    {                         
    case Type_MassFlow:
    case Type_MassRatio:
    case Type_MassMult: 
    case Type_MassFrac:
      if (m_eSelect>=Slct_Specie)
        return QPrd.QMass(m_Species);
      return QPrd.QMass(m_Phases);

    case Type_MoleFlow:
    case Type_MoleRatio:
    case Type_MoleMult: 
    case Type_MoleFrac:
      if (m_eSelect>=Slct_Specie)
        return QPrd.QMole(m_Species);
      return QPrd.QMole(m_Phases);

    case Type_VolumeFlow:
    case Type_VolumeRatio:
    case Type_VolumeMult: 
    case Type_VolumeFrac:
      if (m_eSelect>=Slct_Specie)
        return QPrd.QVolume(m_Species);
      return QPrd.QVolume(m_Phases);

    case Type_NVolumeFlow:
    case Type_NVolumeRatio:
    case Type_NVolumeMult: 
    case Type_NVolumeFrac:
      if (m_eSelect>=Slct_Specie)
        return QPrd.QNVolume(m_Species);
      return QPrd.QNVolume(m_Phases);

    case Type_Conc:
      if (m_eSelect>=Slct_Specie)
        return QPrd.QMass(m_Species);
      return QPrd.QMass(m_Phases);
    }
  return dNAN;
  }

double CXBlk_Makeup::GetMeasVal(SpConduit &QIn, SpConduit &QPrd)
  {
  switch (m_eType)
    {                         
    case Type_MassFlow:
    case Type_MoleFlow:
    case Type_VolumeFlow:
    case Type_NVolumeFlow:
      return GetRawMeas(QPrd);

    case Type_MassRatio:
    case Type_MoleRatio:
    case Type_VolumeRatio:
    case Type_NVolumeRatio:
      return GetRawMeas(QPrd)/GTZ(GetRawMeas(QIn))-1.0;

    case Type_MassMult: 
    case Type_MoleMult: 
    case Type_VolumeMult: 
    case Type_NVolumeMult: 
      return GetRawMeas(QPrd)/GTZ(GetRawMeas(QIn));

    case Type_MassFrac:
    case Type_MoleFrac:
    case Type_VolumeFrac:
    case Type_NVolumeFrac:
      return GetRawMeas(QPrd)/GTZ(GetRawMeas(QIn));

    case Type_Conc:
      return GetRawMeas(QPrd)/GTZ(QPrd.QVolume());
    }
  return 0.0;
  }

//-------------------------------------------------------------------------

class CMkUpFnd : public MRootFinder
  {
  public:
    CMkUpFnd(CXBlk_Makeup * pMU, LPCTSTR pTag, SpConduit * pIn, SpConduit * pSrc, SpConduit * pPrd, double TRqd, double PRqd, CToleranceBlock & Tol) : \
      m_pMU(pMU), m_pTag(pTag), m_In(*pIn), m_Src(*pSrc), m_Prd(*pPrd), m_TRqd(TRqd), m_PRqd(PRqd), MRootFinder("MkUpFnd", Tol)
      { 
      };
    LPCTSTR ObjTag() { return m_pTag; };
    double Function(double Qm)
      {
      m_Prd.QCopy(m_In);
      m_Prd.QAddM(m_Src, som_ALL, Qm);
      m_Prd.SetTempPress(m_TRqd, m_PRqd);
      if (1)
        dbgpln("   Cnv %20.6f %20.6f %20.6f", Qm, m_pMU->GetMeasVal(m_In, m_Prd), m_pMU->GetSetPoint());
      return m_pMU->GetMeasVal(m_In, m_Prd);//pGrp->m_P_Err;
      };

  protected:
    CXBlk_Makeup  * m_pMU;
    LPCTSTR         m_pTag;
    SpConduit     & m_In;
    SpConduit     & m_Src;
    SpConduit     & m_Prd;
    double          m_TRqd;
    double          m_PRqd;
  };

//--------------------------------------------------------------------------

void CXBlk_Makeup::EvalProducts(SpConduit &QPrd, double Po, double FinalTEst)
  {
  m_dQmFeed = QPrd.QMass();
  m_dTempKFeed = QPrd.Temp();


  FlwNode *pNd=FindObjOfType((FlwNode*)NULL);

  //TaggedObject *p=pAttachedTo;
  //FlwNode *pNd=dynamic_cast<FlwNode*>(p);
  //while (pNd==NULL && p)
  //  {
  //  p=p->pAttachedTo;
  //  pNd=dynamic_cast<FlwNode*>(p);
  //  }
  ASSERT_ALWAYS(pNd, "Should always be part of a FlwNode");

  StkSpConduit QIn("QIn", "MkUp", pNd);

  QIn().QCopy(QPrd);

  m_dMeas = GetMeasVal(QIn(), QPrd);
  const double HzIn = QPrd.totHz();

  SpConduit &QSrc=SrcIO.Cd;

  // Copy to Src if Self
  if (m_eSource==Src_Self)
    QSrc.QSetF(QPrd, som_ALL, 1.0);

  double TSrc=QSrc.Temp();
  double TPrd=QPrd.Temp();
  double TReqd=TPrd;


  switch (m_eRqdTemp)
    {
    case Temp_Inlet:
      TReqd=TPrd;
      break;
    case Temp_Source:
      TReqd=TSrc;
      break;
    case Temp_Std:
      TReqd=StdT;
      break;
    case Temp_Mixture:
      TReqd=StdT; //??????
      break;
    }


  CMkUpFnd MkUpFnd(this, BaseTag(), &QIn(), &QSrc, &QPrd, TReqd, Po, m_QmTol);
  int iRet=MkUpFnd.FindRoot(GetSetPoint(), m_QmMin, m_QmMax, m_dQmMakeup);
  switch (iRet)
    {
    case RF_OK:         m_dQmMakeup = MkUpFnd.Result();   break;
    case RF_LoLimit:    m_dQmMakeup = MkUpFnd.Result();   break;
    case RF_HiLimit:    m_dQmMakeup = MkUpFnd.Result();   break;

    default: 
      _asm int 3;
      break;
    }

  m_dSetPoint = GetSetPoint();
  m_dResult = GetMeasVal(QIn(), QPrd);
  m_dQmProd = QPrd.QMass();
  m_dQmMakeup = m_dQmProd-m_dQmFeed;
  m_dTempKProd = QPrd.Temp();
  m_dHeatFlow = QPrd.totHz() - HzIn;
  };

void CXBlk_Makeup::EvalProductsPipe(SpConduit & Qf, double Len, double Diam, double Po, double FinalTEst)
  {
  EvalProducts(Qf, Po, FinalTEst);
  };

//=========================================================================
//
//
//
//=========================================================================

#endif