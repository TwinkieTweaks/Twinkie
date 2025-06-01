#pragma once

#include "../../IModule.h"

const char* DashboardStyleNames[];

// thank you omar very cool
struct Vec2
{
	float x;
	float y;

	Vec2 operator+(Vec2 B)
	{
		return Vec2(this->x + B.x, this->y + B.y);
	}

	Vec2 operator-(Vec2 B)
	{
		return Vec2(this->x - B.x, this->y - B.y);
	}

	Vec2 operator*(float F)
	{
		return Vec2(this->x * F, this->y * F);
	}

	operator ImVec2()
	{
		return ImVec2(this->x, this->y);
	}
};

ImVec2 Lerp(ImVec2 A, ImVec2 B, float T);

class DashboardInputsModule : public IModule
{
public:
	bool IsDebug = false;

	ImVec4 ColorSteer = ImVec4(0.976f, 0.737f, 0.008f, 1.f);
	ImVec4 ColorSteerI = ImVec4(1.f, 1.f, 1.f, 0.5f);
	ImVec4 ColorAccel = ImVec4(0.f, 0.94f, 0.f, 1.f);
	ImVec4 ColorAccelI = ImVec4(1.f, 1.f, 1.f, 0.5f);
	ImVec4 ColorBrake = ImVec4(0.94f, 0.f, 0.f, 1.f);
	ImVec4 ColorBrakeI = ImVec4(1.f, 1.f, 1.f, 0.5f);
	ImVec4 ColorBackground = ImVec4(0.f, 0.f, 0.f, 0.f);

	std::string StyleName = "Keyboard";
	int StyleIdx = 1;

	bool ForceTMVizDimensions = false;

	DashboardInputsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "Dashboard";
		this->FancyName = "Input display";
	}

	virtual void Render() {}
	virtual void RenderAnyways();

	virtual void RenderSettings();
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);

	virtual void SettingsSave(SettingMgr& Settings);
};
