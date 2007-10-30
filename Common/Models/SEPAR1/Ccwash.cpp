//================== SysCAD - Copyright Kenwalt (Pty) Ltd ===================
// $Nokeywords: $
//===========================================================================
 
#include "stdafx.h"
#include "sc_defs.h"
#define  __CCWASH_CPP
#include "ccwash.h"

//==========================================================================
/*#D:#T:Counter Current Washer
#X:#h<General Description>#n
The counter current washer perfectly mixes the incoming wash water and side streams.
Then based on the feed stream, Scandrett efficiency and underflow solids content,
the amount, temperature and composition of the underflow and overflow are calculated.  
The Scandrett efficiency is based on the concentrations of the user defined specie 
within the counter current washer.  The model assumes there are no solids in the 
overflow.
#n#n
The model allows reactions to be specified via a reaction file.  The reactions occur 
before washing and any solids generated by the reaction(s) report to the underflow.
#n#n
The temperature of the liquids is calculated after any reactions and/or heat loss has 
occurred.
#n#n
The user specifies:#n
 - Scandrett wash efficiency as a fraction,#n
 - The specie used in the Scandrett calculation,#n
 - solids content of the underflow in g/L, or as % solids,#n
 - heat loss.
#n#n
#n#h<References>#n
H.F. Scandrett, Equations for Calculating Recovery of Soluble Values in a Countercurrent 
Decantation Washing System.
#n#n
#n#h<Inputs and Outputs>#n#w
Label    Input/Output  Minimum number  Maximum number#n
mud          Input          1              20#n
wash water   Input          1              20#n
overflow     Output         1              1#n
underflow    Output         1              1#n
side stream  Input          0              10#n
#a
For the settler to function the user must specify at least one mud stream, one wash water 
stream, and the overflow and underflow streams.  Up to 20 mud, 20 wash water and 10 side 
streams are allowed.
#n#n
#n#h<Model variables>#n
#i<Scandrett wash efficiency> : #n
#i<ReqdSpecie> : #n
#i<Underflow density> : #n
#i<Heat loss> : #n
#i<Reactions> :
#n#n
#n#h<Assumptions & Limitations>#n
No solids in overflow.
#n#n
#n#h<Physical Equations>#n
Scandrett Wash Efficiency
#n#m<ccwashe2.bmp>#n
#n
#n#h<Other>#n
Default model prefix:CCW#n
Short name:CCWasher#n
Model type:Unit#n
#G:Units
*/
//==========================================================================

const byte ioidMud        = 0;
const byte ioidWWater     = 1;
const byte ioidSStream    = 2;
const byte ioidOFlw       = 3;
const byte ioidUFlw       = 4;
const byte ioidVent       = 5;

static IOAreaRec CCWasherIOAreaList[] =
  {{"Mud",        "Mud"      ,    ioidMud    ,  LIO_In0,  nc_MLnk, 1, 20, IOPipeEntry, 1.0f},
   {"Wash_water", "WashWater" ,   ioidWWater ,  LIO_In1,  nc_MLnk, 1, 10, IOPipeEntry, (float)0.5},
   {"Side_stream","SideStream" ,  ioidSStream , LIO_In2,  nc_MLnk, 0, 10, IOPipeEntry, (float)0.5},
   {"Overflow",   "Overflow" ,    ioidOFlw ,    LIO_Out0, nc_MLnk, 1,  1, IOPipeEntry, (float)0.98},
   {"Underflow",  "Underflow" ,   ioidUFlw ,    LIO_Out1, nc_MLnk, 1,  1, IOPipeEntry, (float)0.5},
   {"",           "Vent"        , ioidVent,     LIO_Out , nc_MLnk, 0,  1, IOPipeEntry, 1.0f},
    SPILL2AREA,
    VENT2AREA(ioidVent),
   {NULL}}; //This lists the areas of the model where links can be attached.


/*static double Drw_CCWasher[] = { DD_Poly, -15,15, -15,0, 0,-10, 15,0, 15,15, -15,15,
                                 DD_TagPos, 0, -18.5,
                                 DD_End };*/
static double Drw_CCWasher[] = { DD_Poly, -25,5.7, -25,2.7, -22,2.7, -22,5.7, 22,5.7, 22,2.7, 25,2.7,
							           				 25,5.7, 25,2.7, 22,2.7, 22,1.7, 0,-7.5, -22,1.7, -22,2.7, -22,1.7, 22,1.7, 
                                 DD_Poly, -3,5.7, -3,7.5, 3,7.5, 3,5.7,
                                 DD_TagPos, 0, -11,
                                 DD_End };

enum MEM_MixingEffMethods { MEM_Scandrett, MEM_Mixing };

//--------------------------------------------------------------------------

IMPLEMENT_MODELUNIT(CCWasher, "CCWasher", "1", Drw_CCWasher, "WasherDecanter", "CCW", TOC_PROBAL|TOC_GRP_SEPAR|TOC_STD_KENWALT,
                    "Separation:Counter Current Washer(1)",
                    "The counter current washer mixes perfectly the incoming wash water " 
                    "and side streams. Then based on the feed stream, Scandrett efficiency, " 
                    "user defined specie and underflow solids content, calculates the " 
                    "amount, temperature and composition of the underflow and overflow.  "  
                    "The model assumes there are no solids in the overflow. " 
                    " " 
                    "The temperature of the liquors is calculated after any reactions and heat "
                    "loss has occurred."
                    " " 
                    "The user specifies: " 
                    "� Scandrett wash efficiency as a fraction, " 
                    "� solids content of the underflow at 25�C in g/L, " 
                    "� heat loss. " 
                    " "
                    "The model converts the solids content at 25�C to the solids content " 
                    "at the operating temperature to determine the composition of the " 
                    "underflow and overflow. " )    

CCWasher::CCWasher(pTagObjClass pClass_, pchar TagIn, pTaggedObject pAttach, TagObjAttachment eAttach) :
  MN_Surge(pClass_, TagIn, pAttach, eAttach),
  Qm("Qm", this),
  Qw("Qw", this),
  Qt("Qt", this)
  {
  AttachIOAreas(CCWasherIOAreaList, &PipeEntryGroup);
  Contents.SetClosed(False);

  SetAllowedModes(true, NM_All|SM_Direct|/*SM_Inline|*//*SM_Buffered|*/HM_All);

  iEffMethod           = MEM_Scandrett;
  MixEff               = 1.0;
  ScandrettEff         = 0.75;       // Default Scrandrett wash efficiency as a fraction
  RqdUFSolidsConc25    = 520.0;      // Underflow solids at 25 dC in g/L
  RqdUFSolids          = 0.5;        // Underflow solids as % solids w/w
  Reqd_UFlowSolidsCorr = 0.0;
  SA                   = False;
	UFSolids             = RqdUFSolids;
  //OFSolids             = 0.1;
	UFCaustic            = 200.0;
	OFCaustic            = 150.0;
	iScanEffSpecie       = 0;
  ActUFSolidsConc25    = RqdUFSolidsConc25;
  //ActOFSolidsConc25    = 0.0;
  ActScandrettEff      = 0.0;

  //m_EHX.Open(&CEHX_LossPerQmClass);

  ByPassGuess=dNAN;
  WashByPassGuess=dNAN;

  fOn=True;
  }

// -------------------------------------------------------------------------

void CCWasher::BuildDataDefn(DataDefnBlk & DDB)
  {
  DDB.BeginStruct(this);

  BuildDataDefnElevation(DDB);

  DDB.Visibility();
	DDB.Text    ("");
  static DDBValueLst DDB0[]={
    {(int)MEM_Scandrett, "ScandrettEff"},
    {(int)MEM_Mixing, "MixingEff", MDD_Hidden }, //this has bugs, keep hidden!
    {0}};
  static DDBValueLst DDB1[]={
    {(int)True,  "g/L_Solids"},
    {(int)False, "%_Solids" },
    {0}};
  if (PrjFileVerNo()<49)
    {
    DDB.Text    ("Requirements");
    DDB.CheckBox("On",                  "",             DC_,     "",    &fOn,                this, isParm);
  	DDB.Byte    ("Eff_Specie",          "RqdSpecie",    DC_,     "",    &iScanEffSpecie,     this, isParm, SDB.DDBLiqSpList());
    DDB.Double  ("Scandrett_wash_eff",  "ScandrettEff", DC_Frac, "%",   &ScandrettEff,       this, isParm);
    DDB.Bool    ("U/FMethod",           "SolidSpec",    DC_,      "",   &SA,                 this, isParm, DDB1);
    if (SA)
      DDB.Double("Reqd_UFlowSolids",    "R_USol",       DC_Conc, "g/L", &RqdUFSolidsConc25,   this, isParm);
    else
      DDB.Double("U/F_Solids",          "U/FSolids",    DC_Frac, "%",   &RqdUFSolids,        this, isParm);
    DDB.Text    ("Results");
	  DDB.Double  ("UnderFlow_Solids",    "UFSolids",     DC_Frac, "%",   &UFSolids,		  	   this, isResult|0);
	  DDB.Double  ("UnderFlow_Specie",    "UFSpecie",     DC_Conc, "g/L", &UFCaustic,					 this, isResult|0);
	  DDB.Double  ("OverFlow_Specie",     "OFSpecie",     DC_Conc, "g/L", &OFCaustic,					 this, isResult|InitHidden);
    }
  else
    {
    DDB.Text    ("Requirements");
    DDB.CheckBox("On",                      "",                DC_,     "",       &fOn,                this, isParm);
	  DDB.Byte    ("",                        "ScanEffSpecie",   DC_,     "",       &iScanEffSpecie,     this, isParm, SDB.DDBLiqSpList());
    DDB.Byte    ("Wash_Eff_Method",         "EffMethod",       DC_,      "",      &iEffMethod,         this, isParm, DDB0);
    DDB.Visibility(NSHM_All, iEffMethod==MEM_Mixing);
    DDB.Double  ("Mixing_Efficiency",       "MixingEff",       DC_Frac, "%",      &MixEff,             this, isParm);
    DDB.Visibility(NSHM_All, iEffMethod==MEM_Scandrett);
    DDB.Double  ("Rqd_Scandrett_Wash_Eff",  "RqdScandrettEff", DC_Frac, "%",      &ScandrettEff,       this, isParm);
    DDB.Visibility();
    DDB.Bool    ("UnderFlow_Method",        "Method",          DC_,      "",      &SA,                 this, isParm, DDB1);
    DDB.Visibility(NSHM_All, SA);
    DDB.Double  ("Rqd_U/F_SolidsConc@25",   "RqdUFSolConc25",  DC_Conc, "g/L",    &RqdUFSolidsConc25,  this, isParm);
    DDB.Visibility(NSHM_All, !SA);
    DDB.Double  ("Rqd_UnderFlow_Solids",    "RqdUFSolids",     DC_Frac, "%",      &RqdUFSolids,        this, isParm);
    DDB.Visibility();
    DDB.Text    ("Results");
	  DDB.Double  ("UnderFlow_Solids",        "UFSolids",        DC_Frac, "%",      &UFSolids,           this, isResult|0);
    //DDB.Double  ("OverFlow_Solids",         "OFSolids",        DC_Frac, "%",      &OFSolids,           this, isResult|InitHidden);
    DDB.Double  ("UnderFlow_SolidsConc@25", "UFSolConc25",     DC_Conc, "g/L",    &ActUFSolidsConc25,  this, isResult|InitHidden);
    //DDB.Double  ("OverFlow_SolidsConc@25",  "OFSolConc25",     DC_Conc, "g/L",    &ActOFSolidsConc25,  this, isResult|InitHidden);
    DDB.Double  ("Scandrett_Wash_Eff",      "ScandrettEff",    DC_Frac, "%",      &ActScandrettEff,    this, isResult);//|noFile|noSnap);
	  DDB.Double  ("UnderFlow_Specie",        "UFSpecie",        DC_Conc, "g/L",    &UFCaustic,				   this, isResult|0);
	  DDB.Double  ("OverFlow_Specie",         "OFSpecie",        DC_Conc, "g/L",    &OFCaustic,				   this, isResult|0);
	  DDB.Double  ("ByPassGuess",             "",                DC_,     "",       &ByPassGuess,				 this, noView|isParm|NAN_OK);
	  DDB.Double  ("WashByPassGuess",         "",                DC_,     "",       &WashByPassGuess,		 this, noView|isParm|NAN_OK);
    }

  DDB.Text    ("");
  m_RB.Add_OnOff(DDB);
  m_EHX.Add_OnOff(DDB);

  DDB.Text    ("");
  BuildDataDefnShowIOs(DDB);

  m_RB.BuildDataDefn(DDB);
  m_EHX.BuildDataDefn(DDB);

  DDB.Visibility();

  if (SolveSurgeMethod())
    {
    DDB.Object(&Contents, this, NULL, NULL, DDB_RqdPage);
    DDB.Object(&m_PresetImg, this, NULL, NULL, DDB_RqdPage);
    }

  DDB.EndStruct();
  }

//--------------------------------------------------------------------------

flag CCWasher::DataXchg(DataChangeBlk & DCB)
  {
  if (MN_Surge::DataXchg(DCB)) 
    return 1;

  return 0;
  }

//---------------------------------------------------------------------------

flag CCWasher::ValidateData(ValidateDataBlk & VDB)
  {
  return MN_Surge::ValidateData(VDB);
  }

//--------------------------------------------------------------------------

void CCWasher::EvalJoinPressures(long JoinMask)
  {
  switch (NetMethod())
    {
    case NM_Probal:
      {
      for (int j=0; j<NoProcessJoins(); j++)
        {
        double P = PBPress();//AtmosPress(IODatum_Term(j));
        SetJoinPressure(j, P);
        }
      break;
      }
    case NM_Dynamic:
      MN_Surge::EvalJoinPressures(JoinMask);
      break;
    }
  }
 
//--------------------------------------------------------------------------

void CCWasher::EvalJoinFlows(int JoinNo)
  {
  switch (NetMethod())
    {
    case NM_Probal:
    case NM_Dynamic:
      break;
    }
  }
 
//--------------------------------------------------------------------------

void CCWasher::EvalSteadyState()
  {
  };

//--------------------------------------------------------------------------
// x is bypass fraction
//

class CCWashEffFnd : public MRootFinderBase
  {
  public:
    SpConduit &Qm, &Qw, &Qu, &Qo, &Qt;
    double  U, Sf, Su, Sw, So, Lf, Lw, Solids2Over,
            Cf, Cw, FT, &UCorr, Y, X, HTot, POut;
    byte    SA, iScanEffSpecie;
    static CToleranceBlock s_Tol;
    CCWashEffFnd (double Cf_, double Cw_,
      SpConduit &Qm_, SpConduit &Qw_, 
      SpConduit &Qu_, SpConduit &Qo_, 
      SpConduit &Qt_, 
      double RqdUFSolidsConc25_, double Solids2Over_, double &Reqd_UFlowSolidsCorr_, byte SA_, byte iScanEffSpecie_, double POut_);
    double Function(double x);
  };

//--------------------------------------------------------------------------

CToleranceBlock CCWashEffFnd::s_Tol(TBF_Both, "CCWasher:WashEff", 0.0, 1.0e-8);
    
CCWashEffFnd::CCWashEffFnd (double Cf_, double Cw_,
                SpConduit &Qm_, SpConduit &Qw_, 
                SpConduit &Qu_, SpConduit &Qo_, 
                SpConduit &Qt_, 
                double RqdUFSolidsConc25_, double Solids2Over_, double &Reqd_UFlowSolidsCorr_, byte SA_, byte iScanEffSpecie_, double POut_):
  MRootFinderBase("CCWasher", s_Tol),//1.0e-8), 
  Qm(Qm_), Qw(Qw_), Qu(Qu_), Qo(Qo_), Qt(Qt_), UCorr(Reqd_UFlowSolidsCorr_)
  {
  U = RqdUFSolidsConc25_;

  Solids2Over = Solids2Over_;
  Cf = Cf_;
  Cw = Cw_;
  Sf = Qm.QMass(som_Sol);
  Sw = Qw.QMass(som_Sol);
  Su = (Sf + Sw)*(1.0 - Solids2Over);
  So = (Sf + Sw)*Solids2Over;
  Lf = Qm.QMass(som_Liq);
  Lw = Qw.QMass(som_Liq);

  Qt.QSetF(Qm, som_ALL, 1.0, Std_P);
  Qt.QAddF(Qw, som_ALL, 1.0);
  FT  = Qt.Temp();
  HTot=Qm.totHf()+Qw.totHf();

  SA = SA_;
	iScanEffSpecie = iScanEffSpecie_;
  POut = POut_;
  }

//---------------------------------------------

double CCWashEffFnd::Function(double x) 
  {
  X = x;
  // *************************** NBNB ***************************
  // These Temporary assignments are a compiler bug work around
  //dword PhS = som_Sol;
  Qt.QSetF(Qm, som_Sol, 1.0, POut);
  Qt.QAddF(Qw, som_Sol, 1.0);
  //dword PhL = som_Liq;
  Qt.QAddF(Qm, som_Liq, 1.0);
  Qt.QAddF(Qw, som_Liq, 1.0-x);
  Qt.SetTemp(FT);
  // *************************** NBNB ***************************

  const double RhoS = Range(1.0, Qt.Rho(som_Sol), 5000.0);
  const double RhoL = Qt.Rho(som_Liq);

  const double Lu   = RhoL*Su*(1.0/(U + UCorr)-1.0/RhoS);

  if (SA)
    Y = GEZ(1.0-(Lu/GTZ(Lf + (1.0 - x)*Lw)));
  else
    {
    const double solper = Range(0.1, U + UCorr, 1.0);
    const double solmas = Qt.QMass(som_Sol);
    const double LiqMas = solmas/solper - solmas;
    const double TotLiq = Max(1e-6, Qt.QMass(som_Liq));
    Y = Range(0.0, 1.0 - LiqMas/TotLiq, 1.0);
    }

  Qu.QSetF(Qt, som_Sol, 1.0, POut);
  Qu.QAddF(Qt, som_Liq, (1.0 - Y));

  Qo.QSetF(Qt, som_Liq, Y, POut);
  Qo.QAddF(Qw, som_Liq, x);

  Qu.SanityCheck();
  Qo.SanityCheck();

  if (1)
    {// Correct Enthalpy...
    Qu.SetTemp(FT);
    Qo.SetTemp(FT);
    double P = POut;
    double H = Qu.totHf()+Qo.totHf();
    double dT = 0.0, H0, T0;
    for (int Hiter=0; Hiter<10; Hiter++)
      {
      if (ConvergedVV(HTot, H, 1.0e-12, 1.0e-12))
        break;
      if (dT!=0.0)
        dT = (HTot-H)*(FT-T0)/NZ(H-H0);
      else
        dT = 0.1;
      T0 = FT;
      H0 = H;
      FT += dT;
      H = Qu.totHf(som_ALL, FT, P)+Qo.totHf(som_ALL, FT, P);
      }
    Qu.SetTemp(FT);
    Qo.SetTemp(FT);
    }

  const double Co = Qo.SpecieConc(Qo.Temp(), iScanEffSpecie, som_Liq);
  const double Cu = Qu.SpecieConc(Qu.Temp(), iScanEffSpecie, som_Liq);

  if (Converging() || TestingEstimate()) // Converging
    { 
    if (SA)  // Solids expressed as g/L.
      {
      double SolConc25  = Qu.PhaseConc(C_2_K(25.0), som_Sol, som_ALL);
      UCorr             = Range(-10.0, UCorr + U - SolConc25, 10.0);
      }
    else    // Solids expressed as % w/w.
      {
      double solid   = Qu.QMass(som_Sol);// + Qw.QMass(som_Sol);
      double Totmass = Max(1e-6, Qu.QMass(som_ALL));
      double SolUF   = solid/Totmass;
      UCorr          = Range(-0.1, UCorr + U - SolUF, 0.1);
      }
    }

  double ScandrettEff = (Cf - Cu)/NZ(Cf - Co);
  return ScandrettEff;
  }

//--------------------------------------------------------------------------
// x is required fraction of wash to overflow
//
class CCWashMixEffFnd : public MRootFinderBase
  {
  public:
    SpConduit &Qm, &Qw, &Qu, &Qo, &Qt;
    double LoLimit;
    static CToleranceBlock s_Tol;
    CCWashMixEffFnd(SpConduit &Qm_, SpConduit &Qw_, SpConduit &Qu_, SpConduit &Qo_, 
                    SpConduit &Qt_, double RqdMixEff_, byte SA_, double POut_);
    double Function(double x);
  protected:
    double RqdMixEff, POut;
    double QmLiq,QwLiq;
    byte   SA;
    double FT, HTot;
  };

CToleranceBlock CCWashMixEffFnd::s_Tol(TBF_Both, "CCWasher:WashMixEff", 0.0, 1.0e-8);

//--------------------------------------------------------------------------

CCWashMixEffFnd::CCWashMixEffFnd (
                SpConduit &Qm_, SpConduit &Qw_, 
                SpConduit &Qu_, SpConduit &Qo_, 
                SpConduit &Qt_, 
                double RqdMixEff_, byte SA_, double POut_):
  MRootFinderBase("CCWasher2", s_Tol),//1.0e-8), 
  Qm(Qm_), Qw(Qw_), Qu(Qu_), Qo(Qo_), Qt(Qt_)
  {
  RqdMixEff = RqdMixEff_;
  SA = SA_;
  POut = POut_;

  QmLiq = Qm.QMass(som_Liq);
  QwLiq = Qw.QMass(som_Liq);
  Qt.QSetF(Qm, som_ALL, 1.0, Std_P);
  Qt.QAddF(Qw, som_ALL, 1.0);
  //limit is where 100% of mud liq reports to uf...
  const double xlim = (RqdMixEff*(QmLiq/GTZ(QwLiq) + 1.0) - 1.0) / NZ(RqdMixEff - 1.0);
  LoLimit = Range(0.0, xlim, 0.99999);
  FT = Qt.Temp();
  HTot = Qm.totHf()+Qw.totHf();
  }

//---------------------------------------------

double CCWashMixEffFnd::Function(double x) 
  {//x is fraction of wash to overflow
  
  //1) All solids go to underflow
  //2) Try meet user requirements for UF conc/liqFrac by adjusting wash fraction to overflow
  //3) Of the liquids reporting to the UF, we want (100%-RqdMixEff%) of liquids to retain mud liquids composition,
  //   the rest is the wash liquids.

  Qu.QSetF(Qm, som_Sol, 1.0, POut);
  Qu.QAddF(Qw, som_Sol, 1.0);
  const double WashLiqToUF = (1.0 - x)*QwLiq;
  const double RqdMudLiqToUF = WashLiqToUF*(1.0/GTZ(RqdMixEff)-1.0);
  const double RqdMudFracToUF = Min(1.0, RqdMudLiqToUF/GTZ(QmLiq));
  Qu.QAddF(Qm, som_Liq, RqdMudFracToUF);
  Qu.QAddF(Qw, som_Liq, (1.0 - x));

  Qo.QSetF(Qm, som_Liq, (1.0-RqdMudFracToUF), POut);
  Qo.QAddF(Qw, som_Liq, x);

  if (1)
    {// Correct Enthalpy...
    Qu.SetTemp(FT);
    Qo.SetTemp(FT);
    double P = POut;
    double H = Qu.totHf()+Qo.totHf();
    double dT = 0.0, H0, T0;
    for (int Hiter=0; Hiter<10; Hiter++)
      {
      if (ConvergedVV(HTot, H, 1.0e-12, 1.0e-12))
        break;
      if (dT!=0.0)
        dT = (HTot-H)*(FT-T0)/NZ(H-H0);
      else
        dT = 0.1;
      T0 = FT;
      H0 = H;
      FT += dT;
      H = Qu.totHf(som_ALL, FT, P)+Qo.totHf(som_ALL, FT, P);
      }
    Qu.SetTemp(FT);
    Qo.SetTemp(FT);
    }

  if (SA)  // Solids expressed as g/L.
    {
    double SolConc25  = Qu.PhaseConc(C_2_K(25.0), som_Sol, som_ALL);
    return SolConc25;
    }
  else    // Solids expressed as % w/w.
    {
    double solid   = Qu.QMass(som_Sol);
    double Totmass = Max(1e-6, Qu.QMass(som_ALL));
    double SolUF   = solid/Totmass;
    return SolUF;
    }
  }
  
//--------------------------------------------------------------------------

void CCWasher::EvalProducts(CNodeEvalIndex & NEI)
  {
  switch (SolveMethod())
    {
    case SM_Direct:
      {
      const int ioUFlw = IOWithId_Self(ioidUFlw);
      const int ioOFlw = IOWithId_Self(ioidOFlw);

      SpConduit & Qu = *IOConduit(ioUFlw);
      SpConduit & Qo = *IOConduit(ioOFlw);

      Qm.QZero();
      Qw.QZero();
      SigmaQInPMin(Qm, som_SL, Id_2_Mask(ioidMud));
      SigmaQInPMin(Qw, som_SL, Id_2_Mask(ioidWWater)|Id_2_Mask(ioidSStream));
      double CFeed = Qm.SpecieConc(Qm.Temp(), iScanEffSpecie, som_Liq);

      flag HasFlwIn = (Qm.QMass()>UsableMass || Qw.QMass()>UsableMass);
      if (fOn && HasFlwIn)
        {
        Qt.QZero();

        // Select Model Transfer 
        Qu.SelectModel(&Qm, False);
        Qo.SelectModel(&Qm, False);

        ScandrettEff      = Range(0.0, ScandrettEff, 1.0);
        MixEff            = Range(0.0, MixEff, 1.0);
        RqdUFSolidsConc25 = Range(1.0, RqdUFSolidsConc25, 5000.0);
        RqdUFSolids       = Range(0.1, RqdUFSolids, 1.0);
				Reqd_UFlowSolidsCorr = 0.0;

        double Cf = Qm.SpecieConc(Qm.Temp(), iScanEffSpecie, som_Liq);
				double Cw = Qw.SpecieConc(Qw.Temp(), iScanEffSpecie, som_Liq);
      
        m_RB.EvalProducts(Qm);
        m_EHX.EvalProducts(Qm);

        //double Qg = Qm.QMass(som_Gas);
        //Qg += Qw.QMass(som_Gas);
        //TODO CC Washer may NOT work for some reactions!

        double UFTemp = (SA ? RqdUFSolidsConc25 : RqdUFSolids);

        //double POut = AtmosPress(); //force outlet to Atmos P
        double POut = Std_P; //force outlet to Std_P
        if (iEffMethod==MEM_Scandrett)
          {
          CCWashEffFnd WEF(Cf, Cw, Qm, Qw, Qu, Qo, Qt, UFTemp, 0.0, Reqd_UFlowSolidsCorr, SA, iScanEffSpecie, POut);

          WEF.SetTarget(ScandrettEff);
          if (Valid(ByPassGuess))
            {
            WEF.SetEstimate(ByPassGuess, -1.0);
            ByPassGuess = dNAN;
            }
          flag Ok = false;
          int iRet=WEF.Start(0.0, 1.0);
          if (iRet==RF_EstimateOK) //estimate is good, solve not required
            {
            ByPassGuess = WEF.Result();
            Ok = true;
            }
          else
            {
            if (iRet==RF_BadEstimate)
              iRet = WEF.Start(0.0, 1.0); // Restart
            if (iRet==RF_OK)
              if (WEF.Solve_Brent()==RF_OK)
                {
                ByPassGuess = WEF.Result();
                Ok = true;
                }
            }
          SetCI(1, !Ok);
          }
        else //if (iEffMethod==MEM_Mixing)
          {
          CCWashMixEffFnd WF(Qm, Qw, Qu, Qo, Qt, MixEff, SA, POut);

          WF.SetTarget(UFTemp);
          if (Valid(WashByPassGuess))
            {
            WF.SetEstimate(WashByPassGuess, -1.0);
            WashByPassGuess = dNAN;
            }
          flag Ok = false;
          int iRet=WF.Start(WF.LoLimit, 1.0);
          if (iRet==RF_EstimateOK) //estimate is good, solve not required
            {
            WashByPassGuess = WF.Result();
            Ok = true;
            }
          else
            {
            if (iRet==RF_BadEstimate)
              iRet = WF.Start(WF.LoLimit, 1.0); // Restart
            if (iRet==RF_OK)
              if (WF.Solve_Brent()==RF_OK)
                {
                WashByPassGuess = WF.Result();
                Ok = true;
                }
            }
          if (SA)
            {
            SetCI(2, !Ok);
            ClrCI(3);
            }
          else
            {
            ClrCI(2);
            SetCI(3, !Ok);
            }
          }

        //put all vapours (if any) to vent (if present)
        const int iVent = IOWithId_Self(ioidVent);
        if (iVent>=0)
          {
          SpConduit & Cvent = *IOConduit(iVent);
          SigmaQInPMin(Cvent, som_Gas, Id_2_Mask(ioidMud)|Id_2_Mask(ioidWWater)|Id_2_Mask(ioidSStream));
          double Qg = Qm.QMass(som_Gas);
          Cvent.QAddM(Qm, som_Gas, Qg);
          Qg = Qw.QMass(som_Gas);
          Cvent.QAddM(Qw, som_Gas, Qg);
          }

        }
      else
        {
        Qo.QZero();
        Qu.QZero();
        const int iVent = IOWithId_Self(ioidVent);
        if (iVent>=0)
          {
          SpConduit & Cvent = *IOConduit(iVent);
          SigmaQInPMin(Cvent, som_Gas, Id_2_Mask(ioidMud)|Id_2_Mask(ioidWWater)|Id_2_Mask(ioidSStream));
          SigmaQInPMin(Qu, som_SL, Id_2_Mask(ioidMud));
          SigmaQInPMin(Qo, som_SL, Id_2_Mask(ioidWWater)|Id_2_Mask(ioidSStream));
          }
        else
          {
          SigmaQInPMin(Qu, som_ALL, Id_2_Mask(ioidMud));
          SigmaQInPMin(Qo, som_ALL, Id_2_Mask(ioidWWater)|Id_2_Mask(ioidSStream));
          }
        }
		
			UFCaustic = Qu.SpecieConc(Qu.Temp(), iScanEffSpecie, som_Liq);
		  OFCaustic = Qo.SpecieConc(Qo.Temp(), iScanEffSpecie, som_Liq);
 
			//double solid   = Qu.QMass(som_Sol);
      //double Totmass = Max(1e-6, Qu.QMass(som_ALL));
      //UFSolids = solid/Totmass;
      UFSolids = Qu.MassFrac(som_Sol);
      //OFSolids = Qo.MassFrac(som_Sol);
      ActUFSolidsConc25 = Qu.PhaseConc(C_2_K(25.0), som_Sol, som_ALL);
      //ActOFSolidsConc25 = Qo.PhaseConc(C_2_K(25.0), som_Sol, som_ALL);
      double Cuf = Qu.SpecieConc(Qu.Temp(), iScanEffSpecie, som_Liq);
      double Cof = Qo.SpecieConc(Qo.Temp(), iScanEffSpecie, som_Liq);
      ActScandrettEff = (CFeed - Cuf)/NZ(CFeed - Cof);
      break;
      }
    default:
      MN_Surge::EvalProducts(NEI);
    }
  }

//--------------------------------------------------------------------------

void CCWasher::SetState(eScdMdlStateActs RqdState)
  {
  MN_Surge::SetState(RqdState);
  switch (RqdState)
    {
    case MSA_PBInit   : 
    case MSA_ZeroFlows:
    case MSA_Empty    :
      ByPassGuess=dNAN;
      WashByPassGuess=dNAN;
      break;
    case MSA_PreSet   :
    case MSA_DynStatsRunInit: 
      break;
    default: break;
    }
  }

//--------------------------------------------------------------------------

flag CCWasher::CIStrng(int No, pchar & pS)
  {
  // NB check CBCount is large enough.
  switch (No - CBContext())
    {
    case 1: pS = "W\tWashing Efficiency not achieved"; return 1;
    case 2: pS = "W\tRequired underflow concentration not achieved"; return 1;
    case 3: pS = "W\tRequired underflow solids fraction not achieved"; return 1;
    default:                                               
      return MN_Surge::CIStrng(No, pS);
    }
  }

//==========================================================================
