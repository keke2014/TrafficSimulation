
#pragma once

#include <map>
#include <utility>
#include <vector>

const int gc_nScalar = 10; // 坐标缩放因子

// ====== 自定义坐标（点）类，用于表示位置 ========
class CMyPoint {
public:
	CMyPoint(int nX = 0, int nY = 0) {
		m_nX = nX * gc_nScalar;
		m_nY = nY * gc_nScalar;
	}
	CMyPoint(const CMyPoint& pt) { // 拷贝构造函数
		m_nX = pt.m_nX;
		m_nY = pt.m_nY;
	}
	CMyPoint& operator=(const CMyPoint& pt) { // 赋值构造函数
		m_nX = pt.m_nX;
		m_nY = pt.m_nY;
		return *this;
	}
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	// 重载小于比较运算符，供 map、set 模板调用
	friend bool operator<(const CMyPoint& ptLhs, const CMyPoint& ptRhs) {
		if (ptLhs.m_nX < ptRhs.m_nX)
			return true;
		else if (ptLhs.m_nX == ptRhs.m_nX
			&& ptLhs.m_nY < ptRhs.m_nY)
			return true;
		else
			return false;
	}
	// 重载等于比较运算符，用于确定是否为同一位置
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


// 道路端点
enum RoadEndPoint { EHW = 0, EHE, BSN, BSS, TBN, TBS };

// ============ 红路灯类 ==========================
class CTrafficLight {
	enum LightType { RED, GREEN };
public:
	CTrafficLight(int nRedLastsTimeSec = 0, int nGreenLastsTimeSec = 0);
	void SetRed(int nStartTimeSec); // 初始化为红灯，并设置其倒计时时间
	void SetGreen(int nStartTimeSec);
	void DecOneSec(); // 更新红绿灯状态(减一秒)
	bool IsGreen();

private:
	int m_nRedLastsTimeSec; // 红灯持续时间
	int m_nGreenLastsTimeSec; // 绿灯持续时间
	LightType m_eLightType; // 当前灯的颜色(红灯/绿灯)
	int m_nTimeSecCur; // 当前倒计时时间
};
// ===================================

// ============= 道路类 ==============
// 添加红路灯，更新红绿灯状态；
// 初始化道路上每个点，指示该店是否有车；
class CRoad {
public:
	void AddTrafficLight(CMyPoint& ptLightPos, 
		int nRedLastsTimeSec, int nGreenLastsTimeSec); // 添加一个红绿灯
	std::map<CMyPoint, CTrafficLight>& 
		GetTrafficLights() { return m_mapLights; }
	void SetLightRed(CMyPoint& ptLightPos, int nStartTimeSec); // 初始化某一点红绿灯为红灯
	void SetLightGreen(CMyPoint& ptLightPos, int nStartTimeSec);
	void UpdateLightState(); // 更新道路上的所有红绿灯
	bool IsGreen(CMyPoint& ptLightPos); // 该点处的红绿灯是否为绿灯(可以通行)
	void ResetAllPoints(); // 初始化道路上的所有点为未激活(没有车辆)
	void ActivatePoint(CMyPoint& ptCarPos); // 设置该点激活(有车辆)
	bool IsPointActivated(CMyPoint& pt); // 该点是否有车？
	void DeactivatePoint(CMyPoint& pt); // 设置该点未激活(车辆离开)

private:
	std::map<CMyPoint, CTrafficLight> m_mapLights; // 红绿灯及其位置
	std::map<CMyPoint, bool> m_mapRoadPoints; // 道路上的所有点(位置)
};
// ==============================

// =========== 路线类 =============
// 保存每条路线的起始点和之间的折点
// 用于导航车辆行驶
class CPath {
public:
	void AddTurningPoint(
		std::pair<RoadEndPoint, RoadEndPoint> pairEndpts,
		CMyPoint ptTurning); // 为某一路线添加折点
	std::vector<CMyPoint>& GetTurningPoints(
		std::pair<RoadEndPoint, RoadEndPoint> pairEndpts);

private:
	std::map<std::pair<RoadEndPoint, RoadEndPoint>, 
		std::vector<CMyPoint> > m_mapTurningPoints; // 路线及其对应所有折点
};
// =====================================

// =========== 车辆类 ==============
// 随机产生车辆时设置其路线，
// 依次从路线中摘取直线段（区间）指导车辆前行
class CCar {
public:
	void SetPath(std::pair<RoadEndPoint, RoadEndPoint> pairEndpts) { // 设置该车路线
			m_pairEndpts = pairEndpts;
			m_nPtIdx = 0;
	}
	std::pair<RoadEndPoint, RoadEndPoint>& GetPath() {
		return m_pairEndpts;
	}
	void SetCurInterval(CMyPoint& ptFrom, CMyPoint& ptTo) { // 设置当前行驶区间
		m_pairCurInterval = std::make_pair(ptFrom, ptTo);
	}
	std::pair<CMyPoint, CMyPoint>& GetCurInterval() {
		return m_pairCurInterval;
	}
	void SetPos(CMyPoint& ptPos) { m_ptPos = ptPos; } // 设置当前位置
	CMyPoint GetPos() { return m_ptPos; }

private:
	CMyPoint m_ptPos; // 车辆当前位置
	std::pair<RoadEndPoint, RoadEndPoint> m_pairEndpts; // 该车路线的起始端点
	std::pair<CMyPoint, CMyPoint> m_pairCurInterval; // 当前车所在的路线区间
public:
	unsigned m_nPtIdx; // 路线折点索引(用于取出下一个折点构造下一个区间)
	COLORREF m_rgb; // 车辆颜色
};
// =================================