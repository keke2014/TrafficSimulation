
// TrafficSimulationDlg.h : 头文件
//

#pragma once

#include "afxwin.h"

#include <vector>
#include <set>
#include "ClassesDef.h"

// CTrafficSimulationDlg 对话框
class CTrafficSimulationDlg : public CDialogEx
{
// 构造
public:
	CTrafficSimulationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRAFFICSIMULATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CRoad m_roadMain; // 道路对象
	CPath m_paths; // 路线对象
	std::vector<CCar> m_vecCars; // 保存所有车辆（用于遍历绘制车辆）
	std::set<CMyPoint> m_setLightPos; // 保存所有红绿灯（用于查找该点是否有红绿灯）

	bool m_bStop;

public:
	afx_msg void OnBnClickedButtonStartstopsimul();
	void MoveOneSec(CMyPoint& ptCarPos);
	void InitNewCar(CCar& carNew,  CMyPoint& pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_ctrlBtnStartStopSimul;
};
