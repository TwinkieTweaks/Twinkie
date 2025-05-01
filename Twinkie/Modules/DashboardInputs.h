#pragma once

#include "../IModule.h"

const char* DashboardStyleNames[] = { "Pad", "Keyboard" };

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

ImVec2 Lerp(ImVec2 A, ImVec2 B, float T)
{
	Vec2 A2 = Vec2(A.x, A.y);
	Vec2 B2 = Vec2(B.x, B.y);

	return A2 + ((B2 - A2) * T);
}

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

	DashboardInputsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "Dashboard";
		this->FancyName = "Input display";
	}

	virtual void Render() {}
	virtual void RenderAnyways() 
	{
		using namespace ImGui;
		if (Twinkie->IsPlaying())
		{
			static bool IsPrevHovered = false;
			VehicleInputs InputInfo = Twinkie->GetInputInfo();
			
		    PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
		    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		
		    int DashboardWindowFlags = ImGuiWindowFlags_NoTitleBar;
			if (!*UiRenderEnabled) DashboardWindowFlags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

		    Begin("Dashboard##Inputs", nullptr, DashboardWindowFlags);
		
		    PopStyleColor();
		    PopStyleVar();

			if (StyleName == "Pad")
			{
				auto UIDrawList = GetWindowDrawList();
				auto CursorPos = GetCursorScreenPos();

				float WindowWidth = GetWindowWidth() / 3.f;
				float WindowHeight = GetWindowHeight();

				float Width = WindowWidth * -InputInfo.Steer;

				float OffsettedWidth = abs(WindowWidth - Width);

				auto UpperL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y);
				auto LowerL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight);
				auto TipL = ImVec2(CursorPos.x, CursorPos.y + WindowHeight / 2.f);

				auto UpperLSteer = Lerp(UpperL, TipL, -InputInfo.Steer);
				auto LowerLSteer = Lerp(LowerL, TipL, -InputInfo.Steer);

				auto UpperR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y);
				auto LowerR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight);
				auto TipR = ImVec2(CursorPos.x + WindowWidth * 3, CursorPos.y + WindowHeight / 2.f);

				auto UpperRSteer = Lerp(UpperR, TipR, InputInfo.Steer);
				auto LowerRSteer = Lerp(LowerR, TipR, InputInfo.Steer);

				UIDrawList->AddTriangleFilled(UpperL, LowerL, TipL, ColorConvertFloat4ToU32(ColorSteerI));
				// HACK: dawg what is this...
				UIDrawList->AddQuadFilled(UpperR, ImVec2(TipR.x, TipR.y - 1.f), ImVec2(TipR.x, TipR.y + 1.f), LowerR, ColorConvertFloat4ToU32(ColorSteerI));

				if (InputInfo.Steer > 0)
				{
					UIDrawList->AddQuadFilled(UpperRSteer, LowerRSteer, LowerR, UpperR, ColorConvertFloat4ToU32(ColorSteer));
				}
				else if (InputInfo.Steer < 0)
				{
					UIDrawList->AddQuadFilled(UpperL, LowerL, LowerLSteer, UpperLSteer, ColorConvertFloat4ToU32(ColorSteer));
				}

				auto BottomCornerGas = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight / 2.f);
				auto TopCornerBrake = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight / 2.f);

				UIDrawList->AddRectFilled(ImVec2(UpperL.x + 6.f, UpperL.y), ImVec2(BottomCornerGas.x - 6.f, BottomCornerGas.y - 3.f), InputInfo.get_Gas() ? ColorConvertFloat4ToU32(ColorAccel) : ColorConvertFloat4ToU32(ColorAccelI));
				UIDrawList->AddRectFilled(ImVec2(TopCornerBrake.x + 6.f, TopCornerBrake.y + 3.f), ImVec2(LowerR.x - 6.f, LowerR.y), InputInfo.get_Brake() ? ColorConvertFloat4ToU32(ColorBrake) : ColorConvertFloat4ToU32(ColorBrakeI));
			}
			else if (StyleName == "Keyboard")
			{
				auto UIDrawList = GetWindowDrawList();
				auto CursorPos = GetCursorScreenPos();

				float WindowWidth = GetWindowWidth() / 3.f;
				float WindowHeight = GetWindowHeight();

				float Width = WindowWidth * -InputInfo.Steer;

				float OffsettedWidth = abs(WindowWidth - Width);

				auto UpperL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y);
				auto LowerL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight);
				auto UpperTipL = ImVec2(CursorPos.x + 13.f, CursorPos.y + 3.f + WindowHeight / 2.f);
				auto LowerTipL = ImVec2(CursorPos.x + 6.f, CursorPos.y + WindowHeight / 2.f);

				auto UpperR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y);
				auto LowerR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight);
				auto UpperTipR = ImVec2(CursorPos.x + WindowWidth * 3 - 13.f, CursorPos.y + 3.f + WindowHeight / 2.f);
				auto LowerTipR = ImVec2(CursorPos.x + WindowWidth * 3 - 6.f, CursorPos.y + WindowHeight / 2.f);

				UIDrawList->AddRectFilled(LowerL, UpperTipL, ColorConvertFloat4ToU32(ColorSteerI));
				UIDrawList->AddRectFilled(LowerR, UpperTipR, ColorConvertFloat4ToU32(ColorSteerI));

				if (InputInfo.Steer > 0)
				{
					auto LerpedUpperR = Lerp(ImVec2(UpperR.x, UpperTipR.y), UpperTipR, InputInfo.Steer);
					auto LerpedLowerR = Lerp(ImVec2(LowerR.x, LowerTipR.y), LowerTipR, InputInfo.Steer);

					UIDrawList->AddRectFilled(LowerR, LerpedUpperR, ColorConvertFloat4ToU32(ColorSteer));
				}
				else if (InputInfo.Steer < 0)
				{
					auto LerpedUpperL = Lerp(ImVec2(UpperL.x, UpperTipL.y), UpperTipL, abs(InputInfo.Steer));
					auto LerpedLowerL = Lerp(ImVec2(LowerL.x, LowerTipL.y), LowerTipL, abs(InputInfo.Steer));

					UIDrawList->AddRectFilled(LowerL, LerpedUpperL, ColorConvertFloat4ToU32(ColorSteer));
				}

				auto BottomCornerGas = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight / 2.f);
				auto TopCornerBrake = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight / 2.f);

				UIDrawList->AddRectFilled(ImVec2(UpperL.x + 6.f, UpperL.y), ImVec2(BottomCornerGas.x - 6.f, BottomCornerGas.y - 3.f), InputInfo.get_Gas() ? ColorConvertFloat4ToU32(ColorAccel) : ColorConvertFloat4ToU32(ColorAccelI));
				UIDrawList->AddRectFilled(ImVec2(TopCornerBrake.x + 6.f, TopCornerBrake.y + 3.f), ImVec2(LowerR.x - 6.f, LowerR.y), InputInfo.get_Brake() ? ColorConvertFloat4ToU32(ColorBrake) : ColorConvertFloat4ToU32(ColorBrakeI));
			}
		    End();
		}
	}

	virtual void RenderSettings() 
	{
		using namespace ImGui;
		if (BeginTabItem(FancyName.c_str()))
		{
			ColorEdit4("Steering", &ColorSteer.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Acceleration", &ColorAccel.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Brake", &ColorBrake.x, ImGuiColorEditFlags_NoInputs);

			Separator();

			ColorEdit4("Steering (inactive)", &ColorSteerI.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Acceleration (inactive)", &ColorAccelI.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Brake (inactive)", &ColorBrakeI.x, ImGuiColorEditFlags_NoInputs);

			Separator();

			ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);

			Separator();

			Combo("Style", &StyleIdx, DashboardStyleNames, IM_ARRAYSIZE(DashboardStyleNames));
			StyleName = DashboardStyleNames[StyleIdx];
			
			EndTabItem();
		}
	}
	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem("Input display", "Dashboard", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual void SettingsInit(SettingMgr& Settings) 
	{
		Settings["Dashboard"]["Steer color"].GetAsVec4(&ColorSteer);
		Settings["Dashboard"]["Brake color"].GetAsVec4(&ColorBrake);
		Settings["Dashboard"]["Acceleration color"].GetAsVec4(&ColorAccel);

		Settings["Dashboard"]["Steer color (inactive)"].GetAsVec4(&ColorSteerI);
		Settings["Dashboard"]["Brake color (inactive)"].GetAsVec4(&ColorBrakeI);
		Settings["Dashboard"]["Acceleration color (inactive)"].GetAsVec4(&ColorAccelI);

		Settings["Dashboard"]["Input display background color"].GetAsVec4(&ColorBackground);

		Settings["Dashboard"]["Enable input display"].GetAsBool(&Enabled);

		Settings["Dashboard"]["Input display style"].GetAsString(&StyleName);
		StyleIdx = std::string(DashboardStyleNames[0]) == StyleName ? 0 : 1;
	}

	virtual void SettingsSave(SettingMgr& Settings) 
	{
		Settings["Dashboard"]["Steer color"].Set(ColorSteer);
		Settings["Dashboard"]["Brake color"].Set(ColorBrake);
		Settings["Dashboard"]["Acceleration color"].Set(ColorAccel);

		Settings["Dashboard"]["Steer color (inactive)"].Set(ColorSteerI);
		Settings["Dashboard"]["Brake color (inactive)"].Set(ColorBrakeI);
		Settings["Dashboard"]["Acceleration color (inactive)"].Set(ColorAccelI);

		Settings["Dashboard"]["Input display background color"].Set(ColorBackground);

		Settings["Dashboard"]["Enable input display"].Set(Enabled);

		Settings["Dashboard"]["Input display style"].Set(StyleName);
	}
};
