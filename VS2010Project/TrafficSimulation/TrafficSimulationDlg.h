
// TrafficSimulationDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"

#include <vector>
#include <set>
#include "ClassesDef.h"

// CTrafficSimulationDlg �Ի���
class CTrafficSimulationDlg : public CDialogEx
{
// ����
public:
	CTrafficSimulationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TRAFFICSIMULATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CRoad m_roadMain; // ��·����
	CPath m_paths; // ·�߶���
	std::vector<CCar> m_vecCars; // �������г��������ڱ������Ƴ�����
	std::set<CMyPoint> m_setLightPos; // �������к��̵ƣ����ڲ��Ҹõ��Ƿ��к��̵ƣ�

	bool m_bStop;

public:
	afx_msg void OnBnClickedButtonStartstopsimul();
	void MoveOneSec(CMyPoint& ptCarPos);
	void InitNewCar(CCar& carNew,  CMyPoint& pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_ctrlBtnStartStopSimul;
};
