
#pragma once

#include <map>
#include <utility>
#include <vector>

const int gc_nScalar = 10; // ������������

// ====== �Զ������꣨�㣩�࣬���ڱ�ʾλ�� ========
class CMyPoint {
public:
	CMyPoint(int nX = 0, int nY = 0) {
		m_nX = nX * gc_nScalar;
		m_nY = nY * gc_nScalar;
	}
	CMyPoint(const CMyPoint& pt) { // �������캯��
		m_nX = pt.m_nX;
		m_nY = pt.m_nY;
	}
	CMyPoint& operator=(const CMyPoint& pt) { // ��ֵ���캯��
		m_nX = pt.m_nX;
		m_nY = pt.m_nY;
		return *this;
	}
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	// ����С�ڱȽ���������� map��set ģ�����
	friend bool operator<(const CMyPoint& ptLhs, const CMyPoint& ptRhs) {
		if (ptLhs.m_nX < ptRhs.m_nX)
			return true;
		else if (ptLhs.m_nX == ptRhs.m_nX
			&& ptLhs.m_nY < ptRhs.m_nY)
			return true;
		else
			return false;
	}
	// ���ص��ڱȽ������������ȷ���Ƿ�Ϊͬһλ��
	friend bool operator==(const CMyPoint& ptLhs, const CMyPoint& ptRhs) {
		if (ptLhs.m_nX == ptRhs.m_nX
			&& ptLhs.m_nY == ptRhs.m_nY)
			return true;
		else
			return false;
	}
	friend bool operator!=(const CMyPoint& ptLhs, const CMyPoint& ptRhs) {
		return !(ptLhs == ptRhs);
	}

private:
	int m_nX;
	int m_nY;
};
// =====================================================


// ��·�˵�
enum RoadEndPoint { EHW = 0, EHE, BSN, BSS, TBN, TBS };

// ============ ��·���� ==========================
class CTrafficLight {
	enum LightType { RED, GREEN };
public:
	CTrafficLight(int nRedLastsTimeSec = 0, int nGreenLastsTimeSec = 0);
	void SetRed(int nStartTimeSec); // ��ʼ��Ϊ��ƣ��������䵹��ʱʱ��
	void SetGreen(int nStartTimeSec);
	void DecOneSec(); // ���º��̵�״̬(��һ��)
	bool IsGreen();

private:
	int m_nRedLastsTimeSec; // ��Ƴ���ʱ��
	int m_nGreenLastsTimeSec; // �̵Ƴ���ʱ��
	LightType m_eLightType; // ��ǰ�Ƶ���ɫ(���/�̵�)
	int m_nTimeSecCur; // ��ǰ����ʱʱ��
};
// ===================================

// ============= ��·�� ==============
// ��Ӻ�·�ƣ����º��̵�״̬��
// ��ʼ����·��ÿ���㣬ָʾ�õ��Ƿ��г���
class CRoad {
public:
	void AddTrafficLight(CMyPoint& ptLightPos, 
		int nRedLastsTimeSec, int nGreenLastsTimeSec); // ���һ�����̵�
	std::map<CMyPoint, CTrafficLight>& 
		GetTrafficLights() { return m_mapLights; }
	void SetLightRed(CMyPoint& ptLightPos, int nStartTimeSec); // ��ʼ��ĳһ����̵�Ϊ���
	void SetLightGreen(CMyPoint& ptLightPos, int nStartTimeSec);
	void UpdateLightState(); // ���µ�·�ϵ����к��̵�
	bool IsGreen(CMyPoint& ptLightPos); // �õ㴦�ĺ��̵��Ƿ�Ϊ�̵�(����ͨ��)
	void ResetAllPoints(); // ��ʼ����·�ϵ����е�Ϊδ����(û�г���)
	void ActivatePoint(CMyPoint& ptCarPos); // ���øõ㼤��(�г���)
	bool IsPointActivated(CMyPoint& pt); // �õ��Ƿ��г���
	void DeactivatePoint(CMyPoint& pt); // ���øõ�δ����(�����뿪)

private:
	std::map<CMyPoint, CTrafficLight> m_mapLights; // ���̵Ƽ���λ��
	std::map<CMyPoint, bool> m_mapRoadPoints; // ��·�ϵ����е�(λ��)
};
// ==============================

// =========== ·���� =============
// ����ÿ��·�ߵ���ʼ���֮����۵�
// ���ڵ���������ʻ
class CPath {
public:
	void AddTurningPoint(
		std::pair<RoadEndPoint, RoadEndPoint> pairEndpts,
		CMyPoint ptTurning); // Ϊĳһ·������۵�
	std::vector<CMyPoint>& GetTurningPoints(
		std::pair<RoadEndPoint, RoadEndPoint> pairEndpts);

private:
	std::map<std::pair<RoadEndPoint, RoadEndPoint>, 
		std::vector<CMyPoint> > m_mapTurningPoints; // ·�߼����Ӧ�����۵�
};
// =====================================

// =========== ������ ==============
// �����������ʱ������·�ߣ�
// ���δ�·����ժȡֱ�߶Σ����䣩ָ������ǰ��
class CCar {
public:
	void SetPath(std::pair<RoadEndPoint, RoadEndPoint> pairEndpts) { // ���øó�·��
			m_pairEndpts = pairEndpts;
			m_nPtIdx = 0;
	}
	std::pair<RoadEndPoint, RoadEndPoint>& GetPath() {
		return m_pairEndpts;
	}
	void SetCurInterval(CMyPoint& ptFrom, CMyPoint& ptTo) { // ���õ�ǰ��ʻ����
		m_pairCurInterval = std::make_pair(ptFrom, ptTo);
	}
	std::pair<CMyPoint, CMyPoint>& GetCurInterval() {
		return m_pairCurInterval;
	}
	void SetPos(CMyPoint& ptPos) { m_ptPos = ptPos; } // ���õ�ǰλ��
	CMyPoint GetPos() { return m_ptPos; }

private:
	CMyPoint m_ptPos; // ������ǰλ��
	std::pair<RoadEndPoint, RoadEndPoint> m_pairEndpts; // �ó�·�ߵ���ʼ�˵�
	std::pair<CMyPoint, CMyPoint> m_pairCurInterval; // ��ǰ�����ڵ�·������
public:
	unsigned m_nPtIdx; // ·���۵�����(����ȡ����һ���۵㹹����һ������)
	COLORREF m_rgb; // ������ɫ
};
// =================================