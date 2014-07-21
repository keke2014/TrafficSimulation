
#include "stdafx.h"
#include "ClassesDef.h"

// ========== ���̵����Ա�������� =============
CTrafficLight::CTrafficLight(int nRedLastsTimeSec, int nGreenLastsTimeSec)
{
	m_nRedLastsTimeSec = nRedLastsTimeSec;
	m_nGreenLastsTimeSec = nGreenLastsTimeSec;
}

void CTrafficLight::SetRed(int nStartTimeSec)
{
	m_eLightType = RED;
	m_nTimeSecCur = nStartTimeSec;
}

void CTrafficLight::SetGreen(int nStartTimeSec)
{
	m_eLightType = GREEN;
	m_nTimeSecCur = nStartTimeSec;
}

void CTrafficLight::DecOneSec()
{
	--m_nTimeSecCur;
	if (m_nTimeSecCur == 0) { // ʱ���Ϊ0����ת���̵�
		if (m_eLightType == RED) {
			m_eLightType = GREEN;
			m_nTimeSecCur = m_nGreenLastsTimeSec;
		}
		else {
			m_eLightType = RED;
			m_nTimeSecCur = m_nRedLastsTimeSec;
		}
	}
}

bool CTrafficLight::IsGreen()
{
	return m_eLightType == GREEN;
}
// =================================

// ========== ��·���Ա�������� =============
void CRoad::AddTrafficLight(CMyPoint& ptLightPos, 
	int nRedLastsTimeSec, int nGreenLastsTimeSec)
{
	m_mapLights.insert(std::make_pair(ptLightPos, 
		CTrafficLight(nRedLastsTimeSec, nGreenLastsTimeSec)));
}

void CRoad::SetLightRed(CMyPoint& ptLightPos, int nStartTimeSec)
{
	m_mapLights[ptLightPos].SetRed(nStartTimeSec);
}

void CRoad::SetLightGreen(CMyPoint& ptLightPos, int nStartTimeSec)
{
	m_mapLights[ptLightPos].SetGreen(nStartTimeSec);
}

void CRoad::UpdateLightState()
{ // �������µ�·�ϵ����к��̵�
	for (auto it = m_mapLights.begin(); it != m_mapLights.end(); ++it) {
		it->second.DecOneSec();
	}
}

bool CRoad::IsGreen(CMyPoint& ptLightPos)
{
	return m_mapLights[ptLightPos].IsGreen();
}

void CRoad::ResetAllPoints()
{ // ��ʼ����·�ϵ����е�Ϊδ����޳�����
	for (int i = 0; i <= 100; ++i) {
		m_mapRoadPoints[CMyPoint(i, 5)] = false;
		m_mapRoadPoints[CMyPoint(i, 10)] = false;
	}

	for (int i = 0; i <= 30; ++i) {
		m_mapRoadPoints[CMyPoint(15, i)] = false;
		m_mapRoadPoints[CMyPoint(20, i)] = false;
		m_mapRoadPoints[CMyPoint(80, i)] = false;
		m_mapRoadPoints[CMyPoint(85, i)] = false;
	}

	for (int i = 5; i <= 10; ++i) {
		m_mapRoadPoints[CMyPoint(1, i)] = false;
		m_mapRoadPoints[CMyPoint(21, i)] = false;
		m_mapRoadPoints[CMyPoint(79, i)] = false;
		m_mapRoadPoints[CMyPoint(99, i)] = false;
	}
}

void CRoad::ActivatePoint(CMyPoint& ptCarPos)
{
	m_mapRoadPoints[ptCarPos] = true;
}

bool CRoad::IsPointActivated(CMyPoint& pt)
{
	return m_mapRoadPoints[pt];
}

void CRoad::DeactivatePoint(CMyPoint& pt)
{
	m_mapRoadPoints[pt] = false;
}
// =================================

// ========== ·�����Ա�������� =============
void CPath::AddTurningPoint(
	std::pair<RoadEndPoint, RoadEndPoint> pairEndpts, 
	CMyPoint ptTurning)
{
	m_mapTurningPoints[pairEndpts].push_back(ptTurning);
}

std::vector<CMyPoint>& CPath::GetTurningPoints(
	std::pair<RoadEndPoint, RoadEndPoint> pairEndpts)
{
	return m_mapTurningPoints[pairEndpts];
}
// =================================