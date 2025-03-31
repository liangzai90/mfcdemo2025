#include "stdafx.h"
//#include "MainFrm.h"
#include "UPSView.h"
#include "Resource.h"
#include "UPS201501.h"
#include "DlgModifyNodeName.h"
#include "DlgAddUPS.h"
#include "ProgressDelDlg.h"

#include "SingleUPSInfo.h"

#include "TcpClient.h"
#import "msxml6.dll"
#define MAX_UPS_NUM 64
extern wchar_t g_cs_module_path[_MAX_PATH];////Dlg.cpp
extern UPS_DRAW_SATTE ups_State;
extern int i_global_ups_Cnt;
int i_whichUPS(-1);
int i_UPS_POW_LEVEL = 0;
CArray<UPS_NODE_CB, UPS_NODE_CB> m_ups_array;
int user_ReadUpsListConfig(const wchar_t *strFile);
int user_WriteUpsListConfig(const wchar_t *strFile);
CString str_Selected_UPSName = _T("");
std::vector<CString>  v_str_Para;
std::vector<list_ups> v_list_ups;
CString str_Root_Name = _T(" 请点击右键修改名称 ");
#define CALC_AVERAGE_CNT 20
float Group_Out_I[CALC_AVERAGE_CNT];
float P_Out[CALC_AVERAGE_CNT];
float P_In[CALC_AVERAGE_CNT];
float calc_average_value(float *sample);

UINT user_tcp_thread_function(LPVOID lParam)
{
	ASSERT(lParam != NULL);

	UPS_NODE_CB *pcb = (UPS_NODE_CB *)lParam;
	CTcpClient *pObj = new CTcpClient(pcb);
	
	if(pObj != NULL)
	{
		pObj->Initialize();
		while(pcb->run_flag > 0)
		{
			pObj->Run();
			Sleep(60);
		}
		CString cs;
		cs = pcb->tlcb.strIp;
		cs += L" 结束运行\n";
		OutputDebugString(cs);
		delete pObj;
	}
	return 0;
}


void _xml_showErrorMessage(MSXML2::IXMLDOMParseErrorPtr errPtr)
{
	CString strError, cs;
	strError.Format(L"Error code  =  0x%x\n", errPtr->errorCode);
	cs.Format(L"Source Line : %ld; Char : %ld\n", errPtr->line, errPtr->linepos);
	strError += cs;
	strError += L"Error Description : ";
	BSTR bstrErr = errPtr->reason;
	strError += bstrErr;
	strError += L"\n";
	AfxMessageBox(strError, MB_OK);
}

// 读 XML 文档
int user_ReadUpsListConfig(const wchar_t *strFile)
{
	CoInitialize(NULL);

	HRESULT   hr; 
	try 
	{
		MSXML2::IXMLDOMDocumentPtr   docPtr;

		hr = docPtr.CreateInstance("msxml2.domdocument.6.0");
		ASSERT(SUCCEEDED(hr));

		VARIANT varVal;
		CString strError, cs;

		if(docPtr->load(strFile) == FALSE)
		{
			MSXML2::IXMLDOMParseErrorPtr   pError = docPtr->GetparseError();
			_xml_showErrorMessage(pError);	
		} 
		else 
		{
			MSXML2::IXMLDOMElementPtr rootPtr;
			MSXML2::IXMLDOMNodePtr nodePtr, itemPtr;
			MSXML2::IXMLDOMNodeListPtr nodeListPtr;
			MSXML2::IXMLDOMNamedNodeMapPtr nodeMapPtr;
			rootPtr = docPtr->documentElement;
			nodeMapPtr = rootPtr->attributes;
			nodePtr = nodeMapPtr->getNamedItem("ROOT_NAME");
			nodePtr->get_nodeValue(&varVal);
			str_Root_Name = varVal.bstrVal;
			nodeListPtr = rootPtr->childNodes;

			UPS_NODE_CB ups_obj;
			for(int j=0; j<nodeListPtr->Getlength(); j++)
			{
				memset(ups_obj.Measure_ToHL,    0, sizeof(int)*UPS_MAX_PARA_NUM ); 
				memset(ups_obj.measure_0x67, 0, sizeof(int)*UPS_MAX_DATA_NUM);
				memset(ups_obj.measure_0x6F, 0, sizeof(int)*UPS_MAX_DATA_NUM);
				memset(ups_obj.dio_0x6F,     0, sizeof(int)*UPS_MAX_DATA_NUM);
				memset(ups_obj.old_dio_0x6F, 0, sizeof(int)*UPS_MAX_DATA_NUM);


				itemPtr = nodeListPtr->Getitem(j);
				nodeMapPtr = itemPtr->attributes;

				nodePtr = nodeMapPtr->getNamedItem("ip");
				nodePtr->get_nodeValue(&varVal);
				cs = varVal.bstrVal;
				size_t cnt = 0;
				wcstombs_s(&cnt, ups_obj.tlcb.strIp, cs.GetString(), 30);
				TRACE("UPS--IP : %s\n", ups_obj.tlcb.strIp);

				nodePtr = nodeMapPtr->getNamedItem("port");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.tlcb.port = _wtoi(varVal.bstrVal);

				nodePtr = nodeMapPtr->getNamedItem("name");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.tlcb.csName = varVal.bstrVal;

				nodePtr = nodeMapPtr->getNamedItem("id");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.Slave_addr = _wtoi(varVal.bstrVal);

				nodePtr = nodeMapPtr->getNamedItem("power_index");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.power_index = _wtoi(varVal.bstrVal);

				nodePtr = nodeMapPtr->getNamedItem("voltage_index");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.voltage_index = _wtoi(varVal.bstrVal);

				nodePtr = nodeMapPtr->getNamedItem("volume_index");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.volume_index = _wtoi(varVal.bstrVal);

				nodePtr = nodeMapPtr->getNamedItem("rfa");
				nodePtr->get_nodeValue(&varVal);
				ups_obj.rfa = _wtof(varVal.bstrVal);

				ups_obj.run_flag = 1;////上电就运行

				m_ups_array.Add(ups_obj);
			}
		}
	}
	catch   (_com_error   &e) 
	{ 
		_tprintf(_T( "Error:\n ")); 
		_tprintf(_T( "Code   =   %08lx\n "),   e.Error()); 
		_tprintf(_T( "Code   meaning   =   %s\n "),   (char*)   e.ErrorMessage()); 
		_tprintf(_T( "Source   =   %s\n "),   (char*)   e.Source()); 
		_tprintf(_T( "Error   Description   =   %s\n "),   (char*)   e.Description()); 
	} 
	catch(...) 
	{ 
		_tprintf(_T( "Unknown   error! ")); 
	} 
	CoUninitialize();
	return 0;
}

// 写 XML 文档 
int user_WriteUpsListConfig(const wchar_t *strFile)
{
	int k;

	CoInitialize(NULL);

	MSXML2::IXMLDOMDocumentPtr   docPtr;

	HRESULT   hr; 
	try 
	{
		MSXML2::IXMLDOMElementPtr rootPtr, elemPtr;
		MSXML2::IXMLDOMAttributePtr attrPtr;
		MSXML2::IXMLDOMCommentPtr commentPtr;
		MSXML2::IXMLDOMProcessingInstructionPtr piPtr;
		hr = docPtr.CreateInstance("msxml2.domdocument.6.0");
		ASSERT(SUCCEEDED(hr));
		wchar_t strText[100];		
		piPtr = docPtr->createProcessingInstruction(L"xml",
			L"version='1.0' encoding='GB2312'");
		docPtr->appendChild(piPtr);
		commentPtr = docPtr->createComment(L"上海山源UPS设备配置文件, 请勿修改!!!");
		docPtr->appendChild(commentPtr);
		rootPtr = docPtr->createElement(L"shsy_ups_list");
		wsprintf(strText, L"%s", str_Root_Name);
		rootPtr->setAttribute(_T("ROOT_NAME"), strText);
		docPtr->appendChild(rootPtr);
		for(k=0; k<m_ups_array.GetCount(); k++)
		{
			elemPtr = docPtr->createElement(L"ups");
			rootPtr->appendChild(elemPtr);
			attrPtr = docPtr->createAttribute(L"name");
			wcscpy(strText, m_ups_array[k].tlcb.csName.GetString());
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
			attrPtr = docPtr->createAttribute(L"id");
			wsprintf(strText, L"%d", m_ups_array[k].Slave_addr);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
			attrPtr = docPtr->createAttribute(L"ip");
			mbstowcs(strText, m_ups_array[k].tlcb.strIp, 100);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
			attrPtr = docPtr->createAttribute(L"port");
			wsprintf(strText, L"%d", m_ups_array[k].tlcb.port);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);	
			attrPtr = docPtr->createAttribute(L"power_index");
			wsprintf(strText, L"%d", m_ups_array[k].power_index);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
			attrPtr = docPtr->createAttribute(L"voltage_index");
			wsprintf(strText, L"%d", m_ups_array[k].voltage_index);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
			attrPtr = docPtr->createAttribute(L"volume_index");
			wsprintf(strText, L"%d", m_ups_array[k].volume_index);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
			attrPtr = docPtr->createAttribute(L"rfa");
			swprintf(strText, L"%4.2f", m_ups_array[k].rfa);
			attrPtr->put_text(strText);
			elemPtr->setAttributeNode(attrPtr);
		}
		docPtr->save(strFile);
	}
	catch(...) 
	{
		MSXML2::IXMLDOMParseErrorPtr   pError = docPtr->GetparseError();
		_xml_showErrorMessage(pError);	
	}
	CoUninitialize();

	return 0;
}


/////////////////////////////////////////////////////////////////////
	// CUPSView 构造/析构
	//////////////////////////////////////////////////////////////////////

	CUPSView::CUPSView()
{

}

CUPSView::~CUPSView()
{
	// 析构函数
}

BEGIN_MESSAGE_MAP(CUPSView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ADD_NODE, OnAddNode)
	ON_COMMAND(ID_DEL_NODE, OnDeleNode)
	ON_COMMAND(ID_SET_NODE_IMAGE, OnSetNodeImage)
	ON_COMMAND(ID_MODIFY_NODE_ITEM_TEXT, OnModifyNodeItemText)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDS_UPS_TREE, &CUPSView::OnNMClickViewTree)
	ON_NOTIFY(NM_CUSTOMDRAW, IDS_UPS_TREE, &CUPSView::OnNMCustomdrawUPSTree)
	ON_NOTIFY(TVN_SELCHANGED, IDS_UPS_TREE, &CUPSView::OnTvnSelchangedUPSTree)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPSView 消息处理程序

int CUPSView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

 	
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();
 
 	// 创建视图:    // TVS_LINESATROOT
 	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_SHOWSELALWAYS;
 
 	if (!m_wndUPSView.Create(dwViewStyle, rectDummy, this, IDS_UPS_TREE))
 	{
 		TRACE0("未能创建 UPS 视图\n");
 		MessageBox(_T("未能创建 UPS 视图"), _T("Create 错误"));
 		return -1;      // 未能创建
 	}
 
 	hIcons[0] = theApp.LoadIcon(IDI_ICON_ROOT); // 根节点图标。。要么都有图片，要么都没有图片。
 	hIcons[1] = theApp.LoadIcon(IDI_ICON_UPSAC1);   // 旁路状态，绿色 【AC图标】
 	hIcons[2] = theApp.LoadIcon(IDI_ICON_UPSDC1);   // 逆变状态，绿色 【DC图标】
 
 	hIcons[3] = theApp.LoadIcon(IDI_ICON_UPSAC2);  // 旁路->断开，红色 【AC图标】
 	hIcons[4] = theApp.LoadIcon(IDI_ICON_UPSDC2);  // 逆变->断开，红色 【DC图标】  
 
 	hIcons[5] = theApp.LoadIcon(IDI_ICON_NO);  // UPS告警，那么显示告警色2  
 
 	hIcons[6] = theApp.LoadIcon(IDI_ICON_UPSAC3);   // 旁路->断开，绿色  ----闪烁用的
 	hIcons[7] = theApp.LoadIcon(IDI_ICON_UPSDC3);   // 逆变->断开，绿色  ----闪烁用的
 
 	m_UPSViewImages.Create(32,32,ILC_COLOR32, 8, 4);
 	for(int i=0; i<8; i++)
 	{
 		m_UPSViewImages.Add(hIcons[i]);
 	}
 	m_wndUPSView.SetImageList(&m_UPSViewImages, TVSIL_NORMAL);
 	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, ID_TOOLBAR);
 	m_wndToolBar.SetHeight(0);
 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
 	m_wndToolBar.SetOwner(this);
 	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
 	memset(&m_color_tab, 0, sizeof(m_color_tab));
 	for(int i=0; i<50; i++)
 	{
 		v_str_Para.push_back(_T(""));
 	}
 	b_ups_link_state = FALSE;
 	n_sample_ps = 0;
	str_Selected_UPSName = _T("");
	i_UPS_POW_LEVEL = 0;
 
 	// 读取 XML 文件
 	wchar_t file_name[_MAX_PATH];
 	wcscpy_s(file_name, g_cs_module_path);
 	wcscat_s(file_name, L"\\Shupslist.xml");
 	user_ReadUpsListConfig(file_name);
 	ArrayToVector();
 	FillUPSView();
  	HTREEITEM h_temp = m_wndUPSView.GetRootItem();
  	h_temp = m_wndUPSView.GetChildItem(h_temp);
  	m_wndUPSView.SelectItem(h_temp);
	InitAverageArr();
 	SetTimer(1, 2000, NULL);
 	SetTimer(2, 2000, NULL);
	AdjustLayout();
	return 0;
}

void CUPSView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CUPSView::FillUPSView()
{	
	HTREEITEM hRoot;
	HTREEITEM hCataItem;
	hRoot = m_wndUPSView.InsertItem(str_Root_Name,0,0); 
	m_wndUPSView.SetItemData(hRoot, 0);
	int i_ListNum = v_list_ups.size();
	for(int i=0; i<i_ListNum; i++)
	{
		hCataItem = m_wndUPSView.InsertItem(v_list_ups.at(i).str_ups_name, 5, 5, hRoot, TVI_LAST);
		m_wndUPSView.SetItemData(hCataItem, 5);
	}
	m_wndUPSView.Expand(hRoot, TVE_EXPAND);
}


void CUPSView::UpdateUPSTreeNodeImage()
{	
	HTREEITEM hRoot_temp = m_wndUPSView.GetRootItem();
	HTREEITEM hItemp1 = m_wndUPSView.GetChildItem(hRoot_temp);	
	int i_UPS_Cnt = m_wndUPSView.GetCount() - 1;
	for(int i=0; i<i_UPS_Cnt; i++)
	{
		int temp = v_list_ups.at(i).i_UPS_Data;
		switch(temp)
		{
		case 0:
			break;

		case 1:
			m_wndUPSView.SetItemData(hItemp1, 1);	
			break;

		case 2:
			m_wndUPSView.SetItemData(hItemp1, 2);
			break;

		case 5:
			if(1==m_wndUPSView.GetItemData(hItemp1))
			{
				m_wndUPSView.SetItemData(hItemp1, 3);

			}
			else if(2==m_wndUPSView.GetItemData(hItemp1))
			{
				m_wndUPSView.SetItemData(hItemp1, 4);
			}
			break;

		default:
			break;

		}

		hItemp1 = m_wndUPSView.GetNextItem(hItemp1,TVGN_NEXT);

	}
	m_wndUPSView.Expand(hRoot_temp, TVE_EXPAND);
}



void CUPSView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndUPSView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
		else
		{
			return ;
		}
	}

 	pWndTree->SetFocus();
 	CMenu menu, *pSubMenu;
 	menu.LoadMenu(IDR_MENU_UPSVIEW);
 	pSubMenu = menu.GetSubMenu(0);
 	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x, point.y, this);
}

void CUPSView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndUPSView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}



void CUPSView::OnAddNode()
{
	CDlgAddUPS DlgAddUps;
	int cnt = m_ups_array.GetCount();
	if (cnt < MAX_UPS_NUM)
	{
		if(IDOK == DlgAddUps.DoModal())
		{
			UPS_NODE_CB obj;
			memset(obj.Measure_ToHL, 0, sizeof(int)*UPS_MAX_PARA_NUM);
			memset(obj.measure_0x67, 0, sizeof(int)*UPS_MAX_DATA_NUM);
			memset(obj.measure_0x6F, 0, sizeof(int)*UPS_MAX_DATA_NUM);
			memset(obj.dio_0x6F, 0, sizeof(int)*UPS_MAX_DATA_NUM);
			memset(obj.old_dio_0x6F, 0, sizeof(int)*UPS_MAX_DATA_NUM);
			obj.tlcb.csName = DlgAddUps.m_str_upsName;
			obj.tlcb.link_state = 0;
			obj.tlcb.port = DlgAddUps.m_TCPport;

			obj.Slave_addr = 17;
			obj.run_flag = 1;
			obj.online = 0;
			obj.rfa = DlgAddUps.m_fRsa;
			obj.power_index = DlgAddUps.m_nPowerIndex;
			obj.voltage_index = DlgAddUps.m_nVoltageIndex;
			obj.volume_index = DlgAddUps.m_nVolumeIndex;

			sprintf_s(obj.tlcb.strIp, 30, "%d.%d.%d.%d", DlgAddUps.cip0, DlgAddUps.cip1, DlgAddUps.cip2, DlgAddUps.cip3);
			int ps = m_ups_array.Add(obj);

			Sleep(500);

			// TODO: 启动线程
			AfxBeginThread(user_tcp_thread_function, (LPVOID)&m_ups_array[ps]);
			wchar_t file_name[MAX_PATH];
			wcscpy_s(file_name, g_cs_module_path);
			wcscat_s(file_name, L"\\Shupslist.xml");
			user_WriteUpsListConfig(file_name);
 			HTREEITEM hRoot_temp = m_wndUPSView.GetRootItem();
 			HTREEITEM hItem_insert ;

 			list_ups one_ups;
 			one_ups.str_ups_name = DlgAddUps.m_str_upsName;
 			CString str_temp = _T(""); 
 			str_temp.Format(_T("%d.%d.%d.%d"), DlgAddUps.cip0, DlgAddUps.cip1, DlgAddUps.cip2, DlgAddUps.cip3);
 			one_ups.str_ups_IP = str_temp;
 			one_ups.i_UPS_Data = 5;

			// 端口号。。。
			str_temp.Format(_T("%d"), DlgAddUps.m_TCPport);
			one_ups.str_ups_port = str_temp;

 			hItem_insert = m_wndUPSView.InsertItem(one_ups.str_ups_name, 5, 5,  hRoot_temp, TVI_LAST);
 			m_wndUPSView.SetItemData(hItem_insert, 5);  
 			v_list_ups.push_back(one_ups);
			m_wndUPSView.Expand(hRoot_temp, TVE_EXPAND);
			m_wndUPSView.SetFocus();
		}
	}
	else
	{
		MessageBox(_T("您的UPS个数超过了最大数目！！！   "), _T("请联系程序开发人员    "), MB_OK);
	}


}

// 2.删除节点
void CUPSView::OnDeleNode()
{
	HTREEITEM hItem = m_wndUPSView.GetSelectedItem();
	if (hItem == m_wndUPSView.GetRootItem())
	{
		MessageBox(_T("删除根节点？？？"), _T("NO   NO  NO ."), MB_OK);
	}
	else
	{
		int Res = MessageBox(_T("请再次确认！  \r\n    删除，点击是；      \r\n    不删除点击否 "), _T("删除确认"), MB_OKCANCEL);
		if(IDOK == Res)
		{
			int I_index = GetUPSIndex();
			m_ups_array[I_index].run_flag = 0;
			CProgressDelDlg  tempDlg;
			tempDlg.DoModal();
			m_ups_array.RemoveAt(I_index);
			wchar_t file_name[MAX_PATH];
			wcscpy_s(file_name, g_cs_module_path);
			wcscat_s(file_name, L"\\Shupslist.xml");
			user_WriteUpsListConfig(file_name);
			v_list_ups.erase(v_list_ups.begin()+I_index);
			m_wndUPSView.DeleteItem(m_wndUPSView.GetSelectedItem());
		}
		else
		{
			return ;
		}

	}
	HTREEITEM hItem_root = m_wndUPSView.GetRootItem();
	HTREEITEM hItem_Child = m_wndUPSView.GetChildItem(hItem_root);
	m_wndUPSView.SelectItem(hItem_Child);
}

// 3.修改节点名称。节点的IP、端口号不支持修改
void CUPSView::OnModifyNodeItemText()
{
	CDlgModifyNodeName dlg_NodeName;
	int i_index = GetUPSIndex();
	CString strName = _T("");
	if(i_index >= 0)
	{
		CString strIP = _T("");
		dlg_NodeName.m_str_nodeIP = v_list_ups.at(i_index).str_ups_IP;

		dlg_NodeName.m_upsPort = v_list_ups.at(i_index).str_ups_port;


		if(IDOK == dlg_NodeName.DoModal())
		{
			UpdateData(TRUE);
			strName = dlg_NodeName.m_str_nodename;
			v_list_ups.at(i_index).str_ups_name = strName;
			HTREEITEM hTemp = m_wndUPSView.GetSelectedItem();
			m_wndUPSView.SetItemText(hTemp, strName);
			m_ups_array[i_index].tlcb.csName = strName;
			wchar_t file_name[MAX_PATH];
			wcscpy_s(file_name, g_cs_module_path);
			wcscat_s(file_name, L"\\Shupslist.xml");
			user_WriteUpsListConfig(file_name);	
		}	
	}
	else 
	{
		if(IDOK == dlg_NodeName.DoModal())
		{
			UpdateData(TRUE);
			strName = dlg_NodeName.m_str_nodename;
			HTREEITEM hRoot = m_wndUPSView.GetRootItem();
			m_wndUPSView.SetItemText(hRoot, strName);
			str_Root_Name = strName ;
			wchar_t file_name[MAX_PATH];
			wcscpy_s(file_name, g_cs_module_path);
			wcscat_s(file_name, L"\\Shupslist.xml");
			user_WriteUpsListConfig(file_name);	
		}
	}
}


// 4.改变节点前面图片的颜色
void CUPSView::OnSetNodeImage()
{
	HTREEITEM hItem = m_wndUPSView.GetSelectedItem();
	if (hItem == m_wndUPSView.GetRootItem())
	{
		MessageBox(_T("这是根节点....."), _T("NO   NO  NO ."), MB_OK);
	}
	else
	{

		// 显示 单个 UPS 详细信息
		CSingleUPSInfo upsDlg;
		upsDlg.DoModal();
	}
}



void CUPSView::OnPaint()
{
	CPaintDC dc(this); 
	CRect rectTree;
	m_wndUPSView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);
	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CUPSView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndUPSView.SetFocus();
}

int CUPSView::GetUPSIndex(void)
{
	HTREEITEM hRoot_temp = m_wndUPSView.GetRootItem();
	HTREEITEM hItemp1 = m_wndUPSView.GetChildItem(hRoot_temp);	
	HTREEITEM hItem2 = m_wndUPSView.GetSelectedItem();
	int i_index = -1; 
	if(m_wndUPSView.GetCount() < 1)
		return -3;
	if(m_wndUPSView.GetCount() < 2)
		return -2;
	for(int i=2; i<= m_wndUPSView.GetCount()+1 ; i++)
	{
		if(hItem2 != hItemp1)
		{
			++i_index;
			hItemp1 = m_wndUPSView.GetNextItem(hItemp1,TVGN_NEXT);
		}
		else
		{
			++i_index;
			break;
		}
	}
	if (i_index >= (m_wndUPSView.GetCount()-1) )
	{
		if(hItem2 == hRoot_temp)
		{
			i_index = -1;
		}
		else
		{
			TRACE("\r\n  --  --   存在吗 ？---\r\n");
			i_index = -4;
		}
	}
	return  i_index;
}


void CUPSView::ArrayToVector()
{
	v_list_ups.clear();
	int i_cnt = m_ups_array.GetCount();
	int i_temp = 0;
	float f_temp = 0.0f;
	CString str_temp = _T("");

	for(int i=0; i<i_cnt; i++)
	{

		if( (m_ups_array[i].online > 0) && (m_ups_array[i].tlcb.link_state > 0) )
		{
			m_color_tab[i][1] = 0;
			if((m_ups_array[i].dio_0x6F[26] & 0x40) > 0)
				m_color_tab[i][1] = 2;
			else
				m_color_tab[i][1] = 0;
		}
		else
		{
			m_color_tab[i][1] = 1;
		}

		//******************************************************
		list_ups one_ups;
		// 名称
		one_ups.str_ups_name =m_ups_array[i].tlcb.csName;
		// IP地址
		one_ups.str_ups_IP = m_ups_array[i].tlcb.strIp;		
		if(m_ups_array[i].Measure_ToHL[27] == 0)
			one_ups.str_ups_workmode = _T("旁路状态");
		else
			one_ups.str_ups_workmode = _T("逆变状态");
		if(m_ups_array[i].tlcb.link_state > 0)
		{
			m_color_tab[i][3] = 0;
			one_ups.str_ups_linkstate  = _T("连接");
		}
		else
		{
			m_color_tab[i][3] = 1;
			one_ups.str_ups_linkstate  = _T("中断");
		}
		if(m_ups_array[i].online > 0)
		{
			m_color_tab[i][4] = 0;
			one_ups.str_ups_online = _T("正常");
		}
		else
		{
			m_color_tab[i][4] = 1;
			one_ups.str_ups_online = _T("中断");

			InitAverageArr();
		}
		i_temp = m_ups_array[i].Measure_ToHL[24];
		f_temp = i_temp * 1.0f;
		str_temp.Format(_T("%0.0f%%"), f_temp);
		one_ups.str_ups_battery_C = str_temp;		
		// 整组电池电压
		i_temp = m_ups_array[i].Measure_ToHL[26];
		f_temp = i_temp * 0.001;
		str_temp.Format(_T("%0.2fV"), f_temp);
		one_ups.str_ups_battery_V = str_temp;
		// 电池组温度.第1节电池温度
		i_temp = m_ups_array[i].Measure_ToHL[12];
		f_temp = i_temp * 0.1;
		str_temp.Format(_T("%0.1f℃"), f_temp);
		one_ups.str_ups_battery_T = str_temp;
		// 如果网络中断，容量就清空吧
		if(m_ups_array[i].online < 1)
		{
			one_ups.str_ups_battery_C = _T(" --");
			one_ups.str_ups_battery_V = _T(" --");
			one_ups.str_ups_battery_T = _T(" --");
		}
		if((m_ups_array[i].online < 1) || (m_ups_array[i].tlcb.link_state  < 1))
		{   
			one_ups.i_UPS_Data = 5;  
		}
		else if(m_ups_array[i].Measure_ToHL[27] == 0)
		{
			one_ups.i_UPS_Data = 1;  
		}
		else
		{    
			one_ups.i_UPS_Data = 2;  
		}

		// 端口号
		CString str_port_temp = _T("");
		str_port_temp.Format(_T("%d"), m_ups_array[i].tlcb.port);
		one_ups.str_ups_port =  str_port_temp;

		v_list_ups.push_back(one_ups);
	} //for

}


void CUPSView::VectorToArray(void)
{

}



void CUPSView::OnTvnSelchangedUPSTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	InitAverageArr();
}




void CUPSView::OnNMClickViewTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD   dwpos = GetMessagePos();   
	TVHITTESTINFO ht = {0};   

	ht.pt.x = GET_X_LPARAM(dwpos);
	ht.pt.y = GET_Y_LPARAM(dwpos);
	::MapWindowPoints(HWND_DESKTOP,pNMHDR->hwndFrom,&ht.pt,1); 
	TreeView_HitTest(pNMHDR->hwndFrom,&ht);  
 	HTREEITEM hRoot_temp = m_wndUPSView.GetRootItem();
 	HTREEITEM hItemp1 = m_wndUPSView.GetChildItem(hRoot_temp);	
	HTREEITEM hItem2 = ht.hItem;
 	int temp_index = GetUPSIndex();
	InitAverageArr();
 	if(temp_index < -2)
 	{
 		return ;
 	}
 	else
 	{
         SetUPSState();
 		if(temp_index>=0)
 		{
			InitAverageArr();
 			ReadPara(temp_index);
 		}
 	}
 }
   
       
void CUPSView::ReadPara(int index)
{
	int i_ups_index = index;
	CStringA str;
	str.Format(("\r\n----UPSView.cpp   ReadPrar-----------\r\n  当前UPS的索引是：%d  \r\n"), i_ups_index);
	TRACE(str.GetString());

	if(index<0)
		return ;

	float f_temp_P_out = 0.0f;
	float f_temp_U_out = 0.0f;
	float f_temp_I_out = 0.0f;
	float f_temp_P_in  = 0.0f;  // 40伏 乘以 输入电流（充电电流）得到输入功率。充电电流不需要求平均值
	float f_temp_I_in  = 0.0f;
	// 先读取那 35 个参数。
	CString str_temp = _T("");
	int i_temp = 0;
	float f_temp = 0.0f;
	// 12节电池单体电压
	for(int i=0; i<12; i++)
	{
		i_temp = m_ups_array[i_ups_index].Measure_ToHL[i];
		f_temp = i_temp * 0.001;
		str_temp.Format(_T("%0.3fV"), f_temp);
		v_str_Para.at(i) = str_temp;
	}
	i_temp = m_ups_array[i_ups_index].Measure_ToHL[26];
	f_temp = i_temp * 0.001;
	str_temp.Format(_T("%0.2fV"), f_temp);
	v_str_Para.at(12) = str_temp;




	if( (m_ups_array[i_ups_index].dio_0x6F[26] & 0x40) > 0)
	{
		f_temp = 0.0f;
		str_temp.Format(_T("%0.2fA"), f_temp);
		v_str_Para.at(13) = str_temp;
	}
	else
	{
		InitAverageArr();
		i_temp = m_ups_array[i_ups_index].Measure_ToHL[28];
		f_temp = i_temp * 0.001;
		str_temp.Format(_T("%0.2fA"), f_temp);
		v_str_Para.at(13) = str_temp;
	}
	f_temp_I_out = 0.001f * m_ups_array[i_ups_index].Measure_ToHL[27];
	// TODO5: 放电电流跟踪测试
	TRACE("\r\n=====实时放电电流是： %0.2f A   ", f_temp_I_out);

	if( (m_ups_array[i_ups_index].dio_0x6F[26] & 0x40) > 0)
	{
		Group_Out_I[n_sample_ps] = 0.001f * m_ups_array[i_ups_index].Measure_ToHL[27];
		f_temp_I_out = calc_average_value(Group_Out_I);
		str_temp.Format(_T("%0.2fA"), f_temp_I_out);
		v_str_Para.at(14) = str_temp;
		
		// 条件为假，则中断；
		// 条件正常，什么也不做。
		// 如果电流小于20.0f，则正常，否则触发Assert，程序中断
		ASSERT(f_temp_I_out < 20.0f);

		TRACE(" 【计算后】的电流是： %0.2f A ==========\r\n", f_temp_I_out);

	}
	else
	{
		f_temp_I_out = 0.0f; 
		str_temp.Format(_T("%0.2fA"), f_temp_I_out);
		v_str_Para.at(14) = str_temp;
	}


	for(int j=15; j<27; j++)
	{
		i_temp = m_ups_array[i_ups_index].Measure_ToHL[j-3];
		f_temp = i_temp * 0.1;
		str_temp.Format(_T("%0.1f℃"), f_temp);
		v_str_Para.at(j) = str_temp;
	}
	i_temp = m_ups_array[i_ups_index].Measure_ToHL[24];
	f_temp = i_temp * 1.0;
	str_temp.Format(_T("%0.0f%%"), f_temp);
	v_str_Para.at(27) = str_temp;
	if (m_ups_array[i_ups_index].Measure_ToHL[24] < 0)
		i_UPS_POW_LEVEL = -1;
	else
	{
		i_UPS_POW_LEVEL = m_ups_array[i_ups_index].Measure_ToHL[24];
	}
	i_temp = 0;
	f_temp = i_temp * 0.001;
	str_temp.Format(_T("%0.3fV"), f_temp);
	v_str_Para.at(28) = str_temp;

	i_temp = 0;
	f_temp = i_temp * 0.001;
	str_temp.Format(_T("%0.3fV"), f_temp);
	v_str_Para.at(29) = str_temp;

	// 充电过流保护值
	i_temp = 0;
	f_temp = i_temp * 0.001;
	str_temp.Format(_T("%0.3fA"), f_temp);
	v_str_Para.at(30) = str_temp;

	// 放电过流保护值
	i_temp = 0;
	f_temp = i_temp * 0.001;
	str_temp.Format(_T("%0.3fA"), f_temp);
	v_str_Para.at(31) = str_temp;

	// 温度保护值
	i_temp = 0;
	f_temp = i_temp * 0.1;
	str_temp.Format(_T("%0.1f℃"), f_temp);
	v_str_Para.at(32) = str_temp;

	// 电池组实际容量
	i_temp = m_ups_array[i_ups_index].Measure_ToHL[25];
	str_temp.Format(_T("%dmAH"), i_temp);
	v_str_Para.at(33) = str_temp;

	// 电池组最低可用容量
	i_temp = 0;
	str_temp.Format(_T("%dmAH"), i_temp);
	v_str_Para.at(34) = str_temp;

	// 网络状态  35
	int i_net = m_color_tab[i_ups_index][3];
	if(0 == i_net)
		v_str_Para.at(35) = _T("连接");
	if(1 == i_net)
		v_str_Para.at(35) = _T("中断");

	// 通信状态  36
	int i_connect = m_color_tab[i_ups_index][4];
	if(0 == i_connect)
	{
		v_str_Para.at(36) = _T("正常");
		b_ups_link_state = TRUE;
	}
	if(1 == i_connect)
	{
		v_str_Para.at(36) = _T("中断");
		b_ups_link_state = FALSE;
	}
	if((m_ups_array[i_ups_index].dio_0x6F[26] & 0x40) > 0)
		v_str_Para.at(37) = _T("交流失电");
	else
		v_str_Para.at(37) = _T("交流正常");

	if((m_ups_array[i_ups_index].dio_0x6F[26] & 0x40) > 0)
		v_str_Para.at(38) = _T("逆变状态");
	else
		v_str_Para.at(38) = _T("旁路状态");

	// 交流输出过流  39. 
	if(1)
		v_str_Para.at(39) = _T("输出正常");
	else
		v_str_Para.at(39) = _T("输出过流");

	// 逆变电源故障  40. 
	if(1)
		v_str_Para.at(40) = _T("逆变正常");
	else
		v_str_Para.at(40) = _T("逆变故障");

	// 输出功率.41.   用电流*电压，得到功率值，功率累计100个，然后求平均值。刷新。
	// 电流在：measure[14]，单位:mA ；   电压在：measure[12]，单位:mV  ；
	f_temp_I_out = ( m_ups_array[i_ups_index].Measure_ToHL[27] + 0.0 ) * 0.001;  // 电流值。把 mA 变成 A
	f_temp_U_out = ( m_ups_array[i_ups_index].Measure_ToHL[26] + 0.0 ) * 0.001;  // 电压值。把 mV 编程 V
	f_temp_P_out = f_temp_I_out * f_temp_U_out ;
	if((m_ups_array[i_ups_index].dio_0x6F[26] & 0x40) > 0)
	{
		// 加工处理 f_temp_P_out 
		P_Out[n_sample_ps] = f_temp_P_out;
		// 函数处理后的  输出功率
		f_temp_P_out = calc_average_value(P_Out);
		str_temp.Format(_T("%0.2fW"), f_temp_P_out);
		v_str_Para.at(41) = str_temp;
	}
	else
	{
		f_temp_P_out = 0.0f; 
		str_temp.Format(_T("%0.2fW"), f_temp_P_out);
		v_str_Para.at(41) = str_temp;
	}
	// 输入功率 42
	// 输入电压，马工说是40V。输入电流，就是充电电流（13,单位:mA）
	if((m_ups_array[i_ups_index].dio_0x6F[26] & 0x40) > 0)
	{
		f_temp_P_in = 0.0f;
		str_temp.Format(_T("%0.2fW"), f_temp_P_in);
		v_str_Para.at(42) = str_temp;
	}
	else
	{
		f_temp_I_in = ( m_ups_array[i_ups_index].Measure_ToHL[28] + 0.0) * 0.001;
		f_temp_P_in = 40.0 * f_temp_I_in;
		P_In[n_sample_ps] = f_temp_P_in;
		// 函数处理后的，输入功率
		f_temp_P_in = calc_average_value(P_In);
		str_temp.Format(_T("%0.2fW"), f_temp_P_in);
		v_str_Para.at(42) = str_temp;
	}
	v_str_Para.at(43) = m_ups_array[i_ups_index].tlcb.csName;
	v_str_Para.at(44) = m_ups_array[i_ups_index].tlcb.strIp;
	if(!b_ups_link_state)
	{
		InitAverageArr();
		for(int i=0; i<43; i++)
		{
			if( (i>=35) && (i<=38) )
				continue;
			v_str_Para.at(i) = _T("");
		}
	}
}



 void CUPSView::OnTimer(UINT_PTR nIDEvent)
 {
	 int temp_index = -2;
	i_global_ups_Cnt = v_list_ups.size();
	 switch(nIDEvent)
	 {
	 case 1:
		 ArrayToVector();
		 UpdateUPSTreeNodeImage();
		 break;

	 case 2: 
		 temp_index= GetUPSIndex();
		 SetUPSState();
		 if(temp_index<0)
		 {
		 }
		 else
		 { 
			 ReadPara(temp_index);
			 str_Selected_UPSName = v_list_ups.at(temp_index).str_ups_name;
			 n_sample_ps = (n_sample_ps + 1) % ( CALC_AVERAGE_CNT );
		 }
		 break;
	 default:
		 break;
	 }
 }



 void CUPSView::SetUPSState()
 {
	 int temp_index = GetUPSIndex();
	 if(temp_index < -2)
	 {
		 ups_State = DRAW_NULL_NULL;
	 }
	 else if(-2 == temp_index)
	 { 
		 ups_State = DRAW__ROOT_NULL;
	 }
	 else if(-1 == temp_index)
	 {
		 if(v_list_ups.size()>0)
		 {
			 ups_State = DRAW__ROOT_ROOT;
		 }
		 else
		 {
			 ups_State = DRAW__ROOT_NULL;
		 }
	 }
	 else
	 {
		 int i_ups_state = v_list_ups.at(temp_index).i_UPS_Data;
		 switch(i_ups_state)
		 {
			 // 旁路状态
		 case 1:
			 ups_State = DRAW__NODE_OK_OK_127;
			 break;

			 // 逆变
		 case 2:
			 ups_State = DRAW__NODE_OK_OK_0;
			 break;

		 case 5:
			 ups_State = DRAW__NODE_NO_NO;
			 break;

		 default:
			 ups_State = DRAW__NODE_NO_NO;
			 break;
		 }
	 }

 }


 void CUPSView::OnNMCustomdrawUPSTree(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	 *pResult = 0;
	 LPNMHDR pNmhdr = (LPNMHDR)pNMHDR;
	  //this->GetTreeCtrl() 或者  m_wndUPSView.m_hWnd
 	 if(m_wndUPSView.m_hWnd != pNmhdr->hwndFrom)
 		 return;
	 LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	 switch (pCustomDraw->nmcd.dwDrawStage)
	 {
	 case CDDS_PREPAINT:
		 *pResult = CDRF_NOTIFYITEMDRAW;
		 return;

	 case CDDS_ITEMPREPAINT:
		 if(this->IsWindowEnabled()==1)
		 {
			 if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0
				 &&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
			 { 
				 pCustomDraw->clrTextBk=RGB(255, 255, 255);
				 pCustomDraw->clrText = RGB(0, 0, 0);
			 }
			 *pResult = CDRF_NOTIFYPOSTPAINT;
			 return;
		 }
		 else{
			 *pResult = CDRF_DODEFAULT ;
			 return;
		 }
	 case CDDS_ITEMPOSTPAINT:
		 if(this->IsWindowEnabled()==1)
		 {
			 if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0
				 &&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
			 {
				 CRect   rcText; 
				 HTREEITEM hItem=(HTREEITEM) pCustomDraw->nmcd.dwItemSpec;
				 CString txt=m_wndUPSView.GetItemText(hItem);
				 m_wndUPSView.GetItemRect(hItem,   &rcText,   true);
				 CPen penBlue(PS_SOLID ,4,RGB(0, 0, 255));
				 CDC* pDC=CDC::FromHandle(pCustomDraw->nmcd.hdc);
				 CBrush* pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				 CBrush* pOldBrush = pDC->SelectObject(pBrush);
				 CPen* pOldPen = pDC->SelectObject(&penBlue);
				 CBrush* myBrush;   
				 myBrush = CBrush::FromHandle((HBRUSH)CreateSolidBrush(RGB(49, 106, 197) ));
				 pDC->FillRect(&rcText, myBrush);
				 pDC->SetBkMode(TRANSPARENT);
				 pDC->SetTextColor(RGB(255,255,255));
				 pDC->TextOutW(rcText.left+2,rcText.top+7, txt);
				 pDC->SelectObject(pOldBrush);
				 pDC->SelectObject(pOldPen);
			 }
			 *pResult = CDRF_SKIPDEFAULT;
			 return;
		 }
		 else{
			 *pResult = CDRF_DODEFAULT ;
			 return;
		 }

	 }

 }


 float calc_average_value(float sample[])
 {
	int j = 0;
	int Num = 0;
	int iMax = 0;
	int iMin = 0;
	float fMax, fMin;
	fMax = 0.0f;
	fMin = 0.0f;
 	float fsum = 0.0f;

	// 	for(j=0; j<CALC_AVERAGE_CNT; j++)
// 	{
// 		fsum += sample[j];
// 	}
// 	fsum = fsum / CALC_AVERAGE_CNT;

	// 先判断是否存在0值
	bool b_haveZero = FALSE;
	int dataCnt = 0;
	for(int i=0; i<CALC_AVERAGE_CNT; i++)
	{
		if(sample[i] < 1.0f)
			b_haveZero = TRUE;
		else
			dataCnt++;
	}

	// 有0值，非0值数据有dataCnt个。
	if(b_haveZero)
	{
		if(0 == dataCnt)
		{
			fsum = 0.0f;
			return fsum;
		}
	}

	for(int i=0; i<CALC_AVERAGE_CNT; i++)
	{
		fsum += sample[i];
	}
	fsum = fsum/dataCnt;


	 TRACE("/ Ave : %f\n", fsum);
	 return fsum;
 }


 void CUPSView::InitAverageArr(void)
 {
	 for(int i=0; i<CALC_AVERAGE_CNT; i++)
	 {
		 Group_Out_I[i] = 0.0f;
		 P_Out[i] = 0.0f;
		 P_In[i] = 0.0f;
	 }
 }


