
// TrafficSimulationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TrafficSimulation.h"
#include "TrafficSimulationDlg.h"
#include "afxdialogex.h"

#include <cstdlib>
#include <ctime>
#include <map>
#include <set>
#include <utility>
#include <vector>
using namespace std;

extern const int gc_nScalar; // ������������

// ȫ�ֱ��������ڿ��Ƴ�������Ƶ��
// EHW��EHE ���˵����Ƶ��Ϊ����·�˵�� 3 ��
const int gc_nCarGenFreqHoriz = 5; // 3 �����һ��
const int gc_nCarGenFreqVertical = gc_nCarGenFreqHoriz*3;
int g_nCarGenFreqHorizCnt = 1;
int g_nCarGenFreqVerticalCnt = 1;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTrafficSimulationDlg �Ի���




CTrafficSimulationDlg::CTrafficSimulationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrafficSimulationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bStop = false;
}

void CTrafficSimulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_STARTSTOPSIMUL, m_ctrlBtnStartStopSimul);
}

BEGIN_MESSAGE_MAP(CTrafficSimulationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTSTOPSIMUL, &CTrafficSimulationDlg::OnBnClickedButtonStartstopsimul)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTrafficSimulationDlg ��Ϣ�������

BOOL CTrafficSimulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��Ӻ��̵�
	m_roadMain.AddTrafficLight(CMyPoint(15,4), 120, 20);	// 1
	m_setLightPos.insert(CMyPoint(15,4));
	m_roadMain.AddTrafficLight(CMyPoint(20,11), 120, 20);	// 4
	m_setLightPos.insert(CMyPoint(20,11));
	m_roadMain.AddTrafficLight(CMyPoint(80,4), 120, 20);	// 5
	m_setLightPos.insert(CMyPoint(80,4));
	m_roadMain.AddTrafficLight(CMyPoint(85,11), 120, 20);	// 8
	m_setLightPos.insert(CMyPoint(85,11));
	m_roadMain.AddTrafficLight(CMyPoint(21,5), 20, 120);	// 2
	m_setLightPos.insert(CMyPoint(21,5));
	m_roadMain.AddTrafficLight(CMyPoint(14,10), 20, 120);	// 3
	m_setLightPos.insert(CMyPoint(14,10));
	m_roadMain.AddTrafficLight(CMyPoint(86,5), 20, 120);	// 6
	m_setLightPos.insert(CMyPoint(86,5));
	m_roadMain.AddTrafficLight(CMyPoint(79,10), 20, 120);	// 7
	m_setLightPos.insert(CMyPoint(79,10));
	// ��ʼ�����̵�
	m_roadMain.SetLightGreen(CMyPoint(21,5), 120-60);	// 2
	m_roadMain.SetLightGreen(CMyPoint(14,10), 120-60);	// 3
	m_roadMain.SetLightGreen(CMyPoint(86,5), 120);		// 6
	m_roadMain.SetLightGreen(CMyPoint(79,10), 120);		// 7
	m_roadMain.SetLightRed(CMyPoint(15,4), 120-60);		// 1
	m_roadMain.SetLightRed(CMyPoint(20,11), 120-60);	// 4
	m_roadMain.SetLightRed(CMyPoint(80,4), 120);		// 5
	m_roadMain.SetLightRed(CMyPoint(85,11), 120);		// 8

	m_roadMain.ResetAllPoints(); // ��ʼ����·�����е�

	// ��ʼ������·��
	// EHW->
	m_paths.AddTurningPoint(make_pair(EHW,BSN), CMyPoint(0,10));
	m_paths.AddTurningPoint(make_pair(EHW,BSN), CMyPoint(79,10));
	m_paths.AddTurningPoint(make_pair(EHW,BSN), CMyPoint(79,5));
	m_paths.AddTurningPoint(make_pair(EHW,BSN), CMyPoint(20,5));
	m_paths.AddTurningPoint(make_pair(EHW,BSN), CMyPoint(20,0));
	m_paths.AddTurningPoint(make_pair(EHW,BSS), CMyPoint(0,10));
	m_paths.AddTurningPoint(make_pair(EHW,BSS), CMyPoint(15,10));
	m_paths.AddTurningPoint(make_pair(EHW,BSS), CMyPoint(15,30));
	m_paths.AddTurningPoint(make_pair(EHW,TBN), CMyPoint(0,10));
	m_paths.AddTurningPoint(make_pair(EHW,TBN), CMyPoint(99,10));
	m_paths.AddTurningPoint(make_pair(EHW,TBN), CMyPoint(99,5));
	m_paths.AddTurningPoint(make_pair(EHW,TBN), CMyPoint(85,5));
	m_paths.AddTurningPoint(make_pair(EHW,TBN), CMyPoint(85,0));
	m_paths.AddTurningPoint(make_pair(EHW,TBS), CMyPoint(0,10));
	m_paths.AddTurningPoint(make_pair(EHW,TBS), CMyPoint(80,10));
	m_paths.AddTurningPoint(make_pair(EHW,TBS), CMyPoint(80,30));
	m_paths.AddTurningPoint(make_pair(EHW,EHE), CMyPoint(0,10));
	m_paths.AddTurningPoint(make_pair(EHW,EHE), CMyPoint(100,10));
	// BSN->
	m_paths.AddTurningPoint(make_pair(BSN,EHW), CMyPoint(15,0));
	m_paths.AddTurningPoint(make_pair(BSN,EHW), CMyPoint(15,5));
	m_paths.AddTurningPoint(make_pair(BSN,EHW), CMyPoint(0,5));
	m_paths.AddTurningPoint(make_pair(BSN,BSS), CMyPoint(15,0));
	m_paths.AddTurningPoint(make_pair(BSN,BSS), CMyPoint(15,30));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(15,0));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(15,5));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(1,5));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(1,10));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(99,10));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(99,5));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(85,5));
	m_paths.AddTurningPoint(make_pair(BSN,TBN), CMyPoint(85,0));
	m_paths.AddTurningPoint(make_pair(BSN,TBS), CMyPoint(15,0));
	m_paths.AddTurningPoint(make_pair(BSN,TBS), CMyPoint(15,5));
	m_paths.AddTurningPoint(make_pair(BSN,TBS), CMyPoint(1,5));
	m_paths.AddTurningPoint(make_pair(BSN,TBS), CMyPoint(1,10));
	m_paths.AddTurningPoint(make_pair(BSN,TBS), CMyPoint(80,10));
	m_paths.AddTurningPoint(make_pair(BSN,TBS), CMyPoint(80,30));
	m_paths.AddTurningPoint(make_pair(BSN,EHE), CMyPoint(15,0));
	m_paths.AddTurningPoint(make_pair(BSN,EHE), CMyPoint(15,5));
	m_paths.AddTurningPoint(make_pair(BSN,EHE), CMyPoint(1,5));
	m_paths.AddTurningPoint(make_pair(BSN,EHE), CMyPoint(1,10));
	m_paths.AddTurningPoint(make_pair(BSN,EHE), CMyPoint(100,10));
	// BSS->
	m_paths.AddTurningPoint(make_pair(BSS,EHW), CMyPoint(20,30));
	m_paths.AddTurningPoint(make_pair(BSS,EHW), CMyPoint(20,10));
	m_paths.AddTurningPoint(make_pair(BSS,EHW), CMyPoint(79,10));
	m_paths.AddTurningPoint(make_pair(BSS,EHW), CMyPoint(79,5));
	m_paths.AddTurningPoint(make_pair(BSS,EHW), CMyPoint(0,5));
	m_paths.AddTurningPoint(make_pair(BSS,BSN), CMyPoint(20,30));
	m_paths.AddTurningPoint(make_pair(BSS,BSN), CMyPoint(20,0));
	m_paths.AddTurningPoint(make_pair(BSS,TBN), CMyPoint(20,30));
	m_paths.AddTurningPoint(make_pair(BSS,TBN), CMyPoint(20,10));
	m_paths.AddTurningPoint(make_pair(BSS,TBN), CMyPoint(99,10));
	m_paths.AddTurningPoint(make_pair(BSS,TBN), CMyPoint(99,5));
	m_paths.AddTurningPoint(make_pair(BSS,TBN), CMyPoint(85,5));
	m_paths.AddTurningPoint(make_pair(BSS,TBN), CMyPoint(85,0));
	m_paths.AddTurningPoint(make_pair(BSS,TBS), CMyPoint(20,30));
	m_paths.AddTurningPoint(make_pair(BSS,TBS), CMyPoint(20,10));
	m_paths.AddTurningPoint(make_pair(BSS,TBS), CMyPoint(80,10));
	m_paths.AddTurningPoint(make_pair(BSS,TBS), CMyPoint(80,30));
	m_paths.AddTurningPoint(make_pair(BSS,EHE), CMyPoint(20,30));
	m_paths.AddTurningPoint(make_pair(BSS,EHE), CMyPoint(20,10));
	m_paths.AddTurningPoint(make_pair(BSS,EHE), CMyPoint(100,10));
	// TBN->
	m_paths.AddTurningPoint(make_pair(TBN,EHW), CMyPoint(80,0));
	m_paths.AddTurningPoint(make_pair(TBN,EHW), CMyPoint(80,5));
	m_paths.AddTurningPoint(make_pair(TBN,EHW), CMyPoint(0,5));
	m_paths.AddTurningPoint(make_pair(TBN,BSN), CMyPoint(80,0));
	m_paths.AddTurningPoint(make_pair(TBN,BSN), CMyPoint(80,5));
	m_paths.AddTurningPoint(make_pair(TBN,BSN), CMyPoint(20,5));
	m_paths.AddTurningPoint(make_pair(TBN,BSN), CMyPoint(20,0));
	m_paths.AddTurningPoint(make_pair(TBN,BSS), CMyPoint(80,0));
	m_paths.AddTurningPoint(make_pair(TBN,BSS), CMyPoint(80,5));
	m_paths.AddTurningPoint(make_pair(TBN,BSS), CMyPoint(1,5));
	m_paths.AddTurningPoint(make_pair(TBN,BSS), CMyPoint(1,10));
	m_paths.AddTurningPoint(make_pair(TBN,BSS), CMyPoint(15,10));
	m_paths.AddTurningPoint(make_pair(TBN,BSS), CMyPoint(15,30));
	m_paths.AddTurningPoint(make_pair(TBN,TBS), CMyPoint(80,0));
	m_paths.AddTurningPoint(make_pair(TBN,TBS), CMyPoint(80,30));
	m_paths.AddTurningPoint(make_pair(TBN,EHE), CMyPoint(80,0));
	m_paths.AddTurningPoint(make_pair(TBN,EHE), CMyPoint(80,5));
	m_paths.AddTurningPoint(make_pair(TBN,EHE), CMyPoint(21,5));
	m_paths.AddTurningPoint(make_pair(TBN,EHE), CMyPoint(21,10));
	m_paths.AddTurningPoint(make_pair(TBN,EHE), CMyPoint(100,10));
	// TBS->
	m_paths.AddTurningPoint(make_pair(TBS,EHW), CMyPoint(85,30));
	m_paths.AddTurningPoint(make_pair(TBS,EHW), CMyPoint(85,10));
	m_paths.AddTurningPoint(make_pair(TBS,EHW), CMyPoint(99,10));
	m_paths.AddTurningPoint(make_pair(TBS,EHW), CMyPoint(99,5));
	m_paths.AddTurningPoint(make_pair(TBS,EHW), CMyPoint(0,5));
	m_paths.AddTurningPoint(make_pair(TBS,BSN), CMyPoint(85,30));
	m_paths.AddTurningPoint(make_pair(TBS,BSN), CMyPoint(85,10));
	m_paths.AddTurningPoint(make_pair(TBS,BSN), CMyPoint(99,10));
	m_paths.AddTurningPoint(make_pair(TBS,BSN), CMyPoint(99,5));
	m_paths.AddTurningPoint(make_pair(TBS,BSN), CMyPoint(20,5));
	m_paths.AddTurningPoint(make_pair(TBS,BSN), CMyPoint(20,0));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(85,30));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(85,10));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(99,10));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(99,5));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(1,5));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(1,10));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(15,10));
	m_paths.AddTurningPoint(make_pair(TBS,BSS), CMyPoint(15,30));
	m_paths.AddTurningPoint(make_pair(TBS,TBN), CMyPoint(85,30));
	m_paths.AddTurningPoint(make_pair(TBS,TBN), CMyPoint(85,0));
	m_paths.AddTurningPoint(make_pair(TBS,EHE), CMyPoint(85,30));
	m_paths.AddTurningPoint(make_pair(TBS,EHE), CMyPoint(85,10));
	m_paths.AddTurningPoint(make_pair(TBS,EHE), CMyPoint(100,10));
	// EHE->
	m_paths.AddTurningPoint(make_pair(EHE,EHW), CMyPoint(100,5));
	m_paths.AddTurningPoint(make_pair(EHE,EHW), CMyPoint(0,5));
	m_paths.AddTurningPoint(make_pair(EHE,BSN), CMyPoint(100,5));
	m_paths.AddTurningPoint(make_pair(EHE,BSN), CMyPoint(20,5));
	m_paths.AddTurningPoint(make_pair(EHE,BSN), CMyPoint(20,0));
	m_paths.AddTurningPoint(make_pair(EHE,BSS), CMyPoint(100,5));
	m_paths.AddTurningPoint(make_pair(EHE,BSS), CMyPoint(1,5));
	m_paths.AddTurningPoint(make_pair(EHE,BSS), CMyPoint(1,10));
	m_paths.AddTurningPoint(make_pair(EHE,BSS), CMyPoint(15,10));
	m_paths.AddTurningPoint(make_pair(EHE,BSS), CMyPoint(15,30));
	m_paths.AddTurningPoint(make_pair(EHE,TBN), CMyPoint(100,5));
	m_paths.AddTurningPoint(make_pair(EHE,TBN), CMyPoint(85,5));
	m_paths.AddTurningPoint(make_pair(EHE,TBN), CMyPoint(85,0));
	m_paths.AddTurningPoint(make_pair(EHE,TBS), CMyPoint(100,5));
	m_paths.AddTurningPoint(make_pair(EHE,TBS), CMyPoint(21,5));
	m_paths.AddTurningPoint(make_pair(EHE,TBS), CMyPoint(21,10));
	m_paths.AddTurningPoint(make_pair(EHE,TBS), CMyPoint(80,10));
	m_paths.AddTurningPoint(make_pair(EHE,TBS), CMyPoint(80,30));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTrafficSimulationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTrafficSimulationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	CClientDC dcClient(this);

	// ========== ���Ƶ�·���� =============
	CMyPoint pt(0, 5-1);
	dcClient.MoveTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(15-1, 5-1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(15-1, 0);
	dcClient.LineTo(pt.GetX(), pt.GetY());

	pt = CMyPoint(20+1, 0);
	dcClient.MoveTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(20+1, 5-1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(80-1, 5-1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(80-1, 0);
	dcClient.LineTo(pt.GetX(), pt.GetY());

	pt = CMyPoint(85+1, 0);
	dcClient.MoveTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(85+1, 5-1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(100, 5-1);
	dcClient.LineTo(pt.GetX(), pt.GetY());

	pt = CMyPoint(100, 10+1);
	dcClient.MoveTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(85+1, 10+1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(85+1, 30);
	dcClient.LineTo(pt.GetX(), pt.GetY());

	pt = CMyPoint(80-1, 30);
	dcClient.MoveTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(80-1, 10+1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(20+1, 10+1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(20+1, 30);
	dcClient.LineTo(pt.GetX(), pt.GetY());

	pt = CMyPoint(15-1, 30);
	dcClient.MoveTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(15-1, 10+1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	pt = CMyPoint(0, 10+1);
	dcClient.LineTo(pt.GetX(), pt.GetY());
	// ===================================

	CRect rect;
	GetClientRect(rect);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTrafficSimulationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTrafficSimulationDlg::OnBnClickedButtonStartstopsimul()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bStop) {
		KillTimer(1);
		m_ctrlBtnStartStopSimul.SetWindowText("��������");
	}
	else {
		SetTimer(1, 1000, NULL);
		m_ctrlBtnStartStopSimul.SetWindowText("��ͣ����");
	}

	m_bStop = !m_bStop;
}

void CTrafficSimulationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CClientDC dcClient(this);

	Invalidate(); // ���ǰһ�λ��Ƶĳ���
	UpdateWindow(); // �����ػ�

	m_roadMain.UpdateLightState(); // ���º��̵�״̬

	// ============ ���ƺ��̵� =============
	map<CMyPoint, CTrafficLight>& mapLights
		= m_roadMain.GetTrafficLights();
	for (auto itMap = mapLights.begin(); itMap != mapLights.end(); ++itMap) {
		CBrush brush;
		if (itMap->second.IsGreen())
			brush.CreateSolidBrush(RGB(0,255,0));
		else
			brush.CreateSolidBrush(RGB(255,0,0));
		CBrush* pBrushOld = dcClient.SelectObject(&brush);
		dcClient.Ellipse(itMap->first.GetX()-8, itMap->first.GetY()-8,
			itMap->first.GetX()+8, itMap->first.GetY()+8);
		dcClient.SelectObject(pBrushOld);
	}
	// ===================================

	// ======== ���������·�ϸ���ĳ��� =============
	for (int i = 0; i <= 100; ++i) {
		CMyPoint pt(i, 5);
		if (m_roadMain.IsPointActivated(pt))
			MoveOneSec(pt);
	}
	for (int j = 100; j >= 0; --j) {
		CMyPoint pt(j, 10);
		if (m_roadMain.IsPointActivated(pt))
			MoveOneSec(pt);
	}
	for (int j = 30; j >= 0; --j) {
		if (j != 5 && j != 10) {
			CMyPoint pt(15, j);
			if (m_roadMain.IsPointActivated(pt))
				MoveOneSec(pt);
		}
	}
	for (int i = 0; i <= 30; ++i) {
		if (i != 5 && i != 10) {
			CMyPoint pt(20, i);
			if (m_roadMain.IsPointActivated(pt))
				MoveOneSec(pt);
		}
	}
	for (int j = 30; j >= 0; --j) {
		if (j != 5 && j != 10) {
			CMyPoint pt(80, j);
			if (m_roadMain.IsPointActivated(pt))
				MoveOneSec(pt);
		}
	}
	for (int i = 0; i <= 30; ++i) {
		if (i != 5 && i != 10) {
			CMyPoint pt(85, i);
			if (m_roadMain.IsPointActivated(pt))
				MoveOneSec(pt);
		}
	}
	for (int j = 9; j >= 6; --j) {
		CMyPoint pt(1, j);
		if (m_roadMain.IsPointActivated(pt))
			MoveOneSec(pt);
	}
	for (int j = 9; j >= 6; --j) {
		CMyPoint pt(21, j);
		if (m_roadMain.IsPointActivated(pt))
			MoveOneSec(pt);
	}
	for (int i = 6; i <= 9; ++i) {
		CMyPoint pt(79, i);
		if (m_roadMain.IsPointActivated(pt))
			MoveOneSec(pt);
	}
	for (int i = 6; i <= 9; ++i) {
		CMyPoint pt(99, i);
		if (m_roadMain.IsPointActivated(pt))
			MoveOneSec(pt);
	}
	// ========================================

	// ========== ����������� =============
	RoadEndPoint arrEntPt[] = { EHW, EHE, BSN, BSS, TBN, TBS };
	int nEndPtIdx;
	CMyPoint pt;
	CCar carNew;

	if (g_nCarGenFreqHorizCnt++ == gc_nCarGenFreqHoriz) {
		g_nCarGenFreqHorizCnt = 1;

		pt = CMyPoint(0, 10);
		if (!m_roadMain.IsPointActivated(pt)) {
			while (1) {
				nEndPtIdx = rand() % 6;
				if (arrEntPt[nEndPtIdx] != EHW)
					break;
			}
			carNew.SetPath(make_pair(EHW, arrEntPt[nEndPtIdx]));
			InitNewCar(carNew, pt);
		}

		pt = CMyPoint(100, 5);
		if (!m_roadMain.IsPointActivated(pt)) {
			while (1) {
				nEndPtIdx = rand() % 6;
				if (arrEntPt[nEndPtIdx] != EHE)
					break;
			}
			carNew.SetPath(make_pair(EHE, arrEntPt[nEndPtIdx]));
			InitNewCar(carNew, pt);
		}
	}

	if (g_nCarGenFreqVerticalCnt++ == gc_nCarGenFreqVertical) {
		g_nCarGenFreqVerticalCnt = 1;

		pt = CMyPoint(15, 0);
		if (!m_roadMain.IsPointActivated(pt)) {
			while (1) {
				nEndPtIdx = rand() % 6;
				if (arrEntPt[nEndPtIdx] != BSN)
					break;
			}
			carNew.SetPath(make_pair(BSN, arrEntPt[nEndPtIdx]));
			InitNewCar(carNew, pt);
		}

		pt = CMyPoint(20, 30);
		if (!m_roadMain.IsPointActivated(pt)) {
			while (1) {
				nEndPtIdx = rand() % 6;
				if (arrEntPt[nEndPtIdx] != BSS)
					break;
			}
			carNew.SetPath(make_pair(BSS, arrEntPt[nEndPtIdx]));
			InitNewCar(carNew, pt);
		}

		pt = CMyPoint(80, 0);
		if (!m_roadMain.IsPointActivated(pt)) {
			while (1) {
				nEndPtIdx = rand() % 6;
				if (arrEntPt[nEndPtIdx] != TBN)
					break;
			}
			carNew.SetPath(make_pair(TBN, arrEntPt[nEndPtIdx]));
			InitNewCar(carNew, pt);
		}

		pt = CMyPoint(85, 30);
		if (!m_roadMain.IsPointActivated(pt)) {
			while (1) {
				nEndPtIdx = rand() % 6;
				if (arrEntPt[nEndPtIdx] != TBS)
					break;
			}
			carNew.SetPath(make_pair(TBS, arrEntPt[nEndPtIdx]));
			InitNewCar(carNew, pt);	
		}
	}		
	// ===================================

	// ======== ���Ƴ��� ==============
	for (vector<CCar>::iterator itVec = m_vecCars.begin(); 
		itVec != m_vecCars.end(); ++itVec) {
			CMyPoint pt = itVec->GetPos();
			CRgn rgn;
			rgn.CreateRectRgn(pt.GetX()-4, pt.GetY()-4, pt.GetX()+4, pt.GetY()+4);
			CBrush brush;
			brush.CreateSolidBrush(itVec->m_rgb);
			dcClient.FillRgn(&rgn, &brush);
	}
	// ========================================

	CDialogEx::OnTimer(nIDEvent);
}

// �����н� 1 ��
void CTrafficSimulationDlg::MoveOneSec(CMyPoint& ptCarPos)
{
	for (auto it = m_vecCars.begin(); it != m_vecCars.end(); ++it) {
		if (it->GetPos() == ptCarPos) { // �ڳ����������ҵ��ó���
			pair<CMyPoint, CMyPoint> pairCurInterval = it->GetCurInterval();
			CMyPoint ptFrom = pairCurInterval.first;
			CMyPoint ptTo = pairCurInterval.second;

			if (ptTo.GetX() == ptCarPos.GetX()
				&& ptTo.GetY() == ptCarPos.GetY()) { // ��ʻ��һ��������յ�

					vector<CMyPoint>& vecTurningPoints
						= m_paths.GetTurningPoints(it->GetPath());

					if (it->m_nPtIdx < vecTurningPoints.size()) { // ��������
						ptFrom = ptTo;
						ptTo = vecTurningPoints.at(it->m_nPtIdx++);
						it->SetCurInterval(ptFrom, ptTo);
					}
					else { // ��ʻ��·�ߵ��յ㣬�ڳ���������ɾ���ó�
						m_vecCars.erase(it);
						m_roadMain.DeactivatePoint(ptCarPos);
						return ;
					}
			}

			CMyPoint ptNewCarPos;
			if (ptFrom.GetX() != ptTo.GetX()) { // ���� x ����ʻ
				int n = ptTo.GetX() - ptFrom.GetX() > 0 ? +1: -1;
				ptNewCarPos = CMyPoint(ptCarPos.GetX()/gc_nScalar+n, ptCarPos.GetY()/gc_nScalar);				
			}
			else { // ���� y ����ʻ
				int n = ptTo.GetY() - ptFrom.GetY() > 0 ? +1: -1;
				ptNewCarPos = CMyPoint(ptCarPos.GetX()/gc_nScalar, ptCarPos.GetY()/gc_nScalar+n);
			}

			bool bGreen = true;
			if ( ptNewCarPos != ptTo // �����ת�䣬��鿴���̵�
				&& m_setLightPos.find(ptCarPos) != m_setLightPos.end() ) // �������̵�
					bGreen = m_roadMain.IsGreen(ptCarPos);

			// �����н� 1 ��
			if ( bGreen && !m_roadMain.IsPointActivated(ptNewCarPos) ) {
				m_roadMain.DeactivatePoint(ptCarPos);
				m_roadMain.ActivatePoint(ptNewCarPos);
				it->SetPos(ptNewCarPos);
			}

			return ; // �������
		}
	}
}

// ��ʼ������
void CTrafficSimulationDlg::InitNewCar(CCar& carNew, CMyPoint& pt)
{
	// ȡ��������Ϊ��һ������
	vector<CMyPoint>& vecTurningPoints
		= m_paths.GetTurningPoints(carNew.GetPath());
	CMyPoint ptFrom(vecTurningPoints.at(carNew.m_nPtIdx++));
	CMyPoint ptTo(vecTurningPoints.at(carNew.m_nPtIdx++));
	carNew.SetCurInterval(ptFrom, ptTo);

	carNew.SetPos(pt); // ���ó���λ��
	m_roadMain.ActivatePoint(pt); // ����õ�

	carNew.m_rgb = RGB(rand()%256, rand()%256, rand()%256); // �������������ɫ

	m_vecCars.push_back(carNew); // ���������뼯��
}
