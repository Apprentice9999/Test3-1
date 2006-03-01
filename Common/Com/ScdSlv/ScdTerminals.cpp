// ScdTerminals.cpp : Implementation of CScdTerminals

#include "stdafx.h"
#include "ScdTerminals.h"
#include "flwnode.h"


// CScdTerminals

STDMETHODIMP CScdTerminals::InterfaceSupportsErrorInfo(REFIID riid)
  {
  static const IID* arr[] = 
    {
    &IID_IScdTerminals
    };

  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
    }
  return S_FALSE;
  }

STDMETHODIMP CScdTerminals::Terminal( VARIANT  WhichOne,  IScdTerminal * *  pItem)
  {
  dllSCD_COMENTRY(eScdNodeCodes)
    {
    CHECK_ELEC_TOPO_OK;

    switch (WhichOne.vt)
      {
      case VT_I2:
      case VT_I4:
        {
        long N=m_ITerminals.GetSize();
        long i=(WhichOne.vt==VT_I2) ? WhichOne.iVal : WhichOne.lVal;
        if (i>=0 && i<N)
          {
          IScdTerminal * p;
          ::CoCreateInstance(CLSID_ScdTerminal, NULL, CLSCTX_ALL, IID_IScdTerminal, (void**)&p);
          p->SetObjectInfo((DWORD)m_Terminals[i]);
          *pItem=p; // User must release
          return S_OK;
          }
        return Scd.ReturnH(E_INVALIDARG, "Bad Index");
        }
      case VT_BSTR:
        {
        SCD_LclStr(Name, WhichOne.bstrVal);
        for (int i=0; i<m_Terminals.GetSize(); i++)
          {
          if (stricmp(m_Terminals[i]->m_sName, Name)==0)
            {
            IScdTerminal * p;
            ::CoCreateInstance(CLSID_ScdTerminal, NULL, CLSCTX_ALL, IID_IScdTerminal, (void**)&p);
            p->SetObjectInfo((DWORD)m_Terminals[i]);
            *pItem=p; // User must release
            return S_OK;
            }
          }
        return Scd.ReturnH(E_INVALIDARG, "Bad Tag");
        }
      default:
        return Scd.ReturnH(E_INVALIDARG);
      }
    *pItem=NULL;
    }		
  SCD_COMEXIT
  }
STDMETHODIMP CScdTerminals::get__NewEnum( LPUNKNOWN *  pVal)
  {
  dllSCD_COMENTRYGET(eScdNodeCodes, pVal)
    {
    CHECK_ELEC_TOPO_OK;

    long size=m_ITerminals.GetSize();

    VARIANT* pVar = new VARIANT[size];
    for(long i = 0; i < size; i++)
      {
      pVar[i].vt = VT_DISPATCH;
      pVar[i].pdispVal = m_ITerminals[i];
      }

    // Now make the enum.
    typedef CComObject< CComEnum< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy< VARIANT > > > enumVar;
    enumVar* pEnumVar = new enumVar;
    pEnumVar->Init(&pVar[0], &pVar[size], NULL, AtlFlagCopy);
    delete[] pVar;
    // return the enum.
    return pEnumVar->QueryInterface(IID_IUnknown, (void**)pVal);
    //Scd.ReturnH(E_NOTIMPL);
    }		
  SCD_COMEXIT
  }
STDMETHODIMP CScdTerminals::get_Count( long *  pVal)
  {
  dllSCD_COMENTRYGET(eScdNodeCodes, pVal)
    {
    CHECK_ELEC_TOPO_OK;

    *pVal=m_ITerminals.GetSize();
    }		
  SCD_COMEXIT
  }
STDMETHODIMP CScdTerminals::SetObjectInfo(DWORD  Node,  DWORD  Types,  DWORD  Dirn)
  {
  dllSCD_COMENTRY(eScdNodeCodes)
    {
    CHECK_SFE_OK;
    m_pNode       = (FlwNode*)(Node);
    m_eTypes      = (eScdElecTypes)Types;
    m_eDirns      = (eScdDirections)Dirn;
    m_lTopoCount  = gs_pSfeSrvr->FE_GetTopologyCount();
    m_lElectricsTopoCount=-1;
    BuildData();
    }		
  SCD_COMEXIT
  }
STDMETHODIMP CScdTerminals::Select( eScdElecTypes  Types, eScdDirections  Direction)
  {
  dllSCD_COMENTRY(eScdNodeCodes)
    {
    m_eTypes=(eScdElecTypes)Types;
    m_eDirns=(eScdDirections)Direction;
    }		
  SCD_COMEXIT
  }
//STDMETHODIMP CScdTerminals::get_Tag(BSTR* pVal)
//  {
//  dllSCD_COMENTRYGET(eScdNodeCodes, pVal)
//    {
//    CHECK_ELEC_TOPO_OK;
//    SCD_Copy2BSTR(pVal, m_pTerminals->m_sName);
//    }
//  SCD_COMEXIT
//  }

void CScdTerminals::BuildData()
  {
  long TopoCnt=gs_pSfeSrvr->FE_GetTopologyCount();
  long ElectricsTopoCount=m_pNode->GetElectricsTopoCount();
  if (m_lElectricsTopoCount!=ElectricsTopoCount)
    {
    int size=0;
    m_lTopoCount=TopoCnt;
    m_lElectricsTopoCount=ElectricsTopoCount;

    m_pNode->UpdateElectrics();
    CETermStripPtrs & TS=m_pNode->m_Electrics.m_TermStrips;
    
    m_Terminals.SetSize(0);
    for (int ts=0; ts<TS.GetCount(); ts++)
      {
      for (int i=0; i<TS[ts]->m_Terms.GetCount(); i++)
        {
        CETerminal * p=TS[ts]->m_Terms[i];
        m_Terminals.SetAtGrow(size++, p);
        }
      }
    for (int i = 0; i<m_ITerminals.GetSize(); i++)
      m_ITerminals[i]->Release();

    m_ITerminals.SetSize(size);
    for (int i = 0; i < size; i++)
      {
      IScdTerminal * p;
      ::CoCreateInstance(CLSID_ScdTerminal, NULL, CLSCTX_ALL, IID_IScdTerminal, (void**)&p);
      p->SetObjectInfo((DWORD)m_Terminals[i]);
      LPDISPATCH pDisp;
      SCD_TOF(p->QueryInterface(IID_IDispatch, (void**)&pDisp), "IDispatch on ScdTerminals")
        m_ITerminals[i]=pDisp;
      p->Release();
      }
    }
  //ASSERT(m_ITerminals.GetSize()==m_FlwNodes.GetSize());
  //return m_ITerminals.GetSize();
  }
