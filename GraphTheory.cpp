
/*
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 - 2021 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.
	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class GraphTheory : public olc::PixelGameEngine
{
public:
	GraphTheory()
	{
		// Name your application
		sAppName = "Mathematics in Window";
	}

private:
	//-----------------------------------------------------------------------------------------------First Create type and structs and ect
	enum EFFID {
		DrawBackground, //Slow color change
		SetupTest, //For testing
		ProcessTest,
		DrawTest,
		ProcessMouse, //for mouse stuff
		SetupScene1, //Main menu
		ProcessScene1,
		ProcessMouseScene1, 
		SetupScene2, //Intro
		ProcessScene2,
		ProcessMouseScene2, //Useful in general
		SetupScene3, //More intro

		ProcessMouseScene3, 
		SetupScene4, //Degrees
		ProcessScene4,
		ProcessMouseScene4,
		SetupScene5, //Degree sequences
		ProcessScene5,

		SetupScene6, //Degree sequence example
		ProcessScene6,
		ProcessMouseScene6,
		SetupScene7, //Deg Seq in reverse (Use mouse 2)
		ProcessScene7,

		SetupScene8, //Deg Seq in reverse (Use mouse 2)
		ProcessScene8,

		SetupScene9, //Uses mouse 2 and proc 2


		SetupScene10, //Explain HH example

		ProcessMouseScene10,
		SetupScene11, //More HH Example
		ProcessScene11,
		ProcessMouseScene11,
		SetupScene12, //Step by step
		ProcessScene12,

		SetupScene13, //example over


		SetupScene14, //Removing highest degree
		ProcessScene14,
		ProcessMouseScene14,
		SetupScene15, //Removing highest degree
		ProcessScene15,
		ProcessMouseScene15,
		SetupScene16,


		SetupScene17,


		Quit
	};
	typedef bool (GraphTheory::* EveryFrameFunction)(float);
	typedef std::pair<EFFID, EveryFrameFunction> FrameFunc;
	struct Node {
		float Rad = 1.0f;
		olc::vf2d Pos = { 0.0f,0.0f };
		olc::vf2d PotPos;
		olc::vf2d Vel = { 0.0f, 0.0f };
		//olc::vf2d Speed = { 0.0f, 0.0f };
		olc::vf2d Accel = { 0.0f, 0.0f };
		olc::Pixel Color = olc::DARK_BLUE;
		olc::Pixel BorderColor = olc::WHITE;
		bool Active = true;
	};
	struct Edge {
		float Length = 1.0f;
		uint8_t Node1 = 0; //This is an index
		uint8_t Node2 = 0;
		olc::Pixel Color = olc::WHITE;
	};
	struct Graph {
		std::vector<Node> N;
		std::vector<Edge> E;
	};
	struct MouseThing {
		int node = -1;
		//Maybe more?
	};
	struct AdvStr {
		std::string Origin;
		std::string Text;
		float Pcnt = 0.0f;
		bool Done = false;
		olc::vf2d Pos = { 0.0f, 0.0f };
	};
	struct ScButt {
		bool Visible = false;
		bool Hover = false;
		olc::vf2d Pos = { 100.0f, 75.0f };
		olc::vf2d Size = { 100.0f, 75.0f };
		olc::Pixel Color = olc::Pixel(128, 128, 128, 50);
		std::string Text = "Hi";
	};
	//-------------------------------------------------------------------------------------------------Then initialize variables
	std::vector<FrameFunc> TheList;
	std::vector<FrameFunc> TheCopiedList;
	bool TheListChanged = false;

	Graph BigG;
	AdvStr MainText;
	AdvStr SubText;
	ScButt NextB;
	ScButt PrevB;
	//Scene stuff
	bool SceneFinished = false;
	int SubScene = 0;
	int CurrScene = 0;
	//olc_Terminate();
	std::array<FrameFunc, 18> SceneList = {
		FrameFunc(SetupScene1, &GraphTheory::SetupScene1F),
		FrameFunc(SetupScene2, &GraphTheory::SetupScene2F),
		FrameFunc(SetupScene3, &GraphTheory::SetupScene3F),
		FrameFunc(SetupScene4, &GraphTheory::SetupScene4F),
		FrameFunc(SetupScene5, &GraphTheory::SetupScene5F),
		FrameFunc(SetupScene6, &GraphTheory::SetupScene6F),
		FrameFunc(SetupScene7, &GraphTheory::SetupScene7F),
		FrameFunc(SetupScene8, &GraphTheory::SetupScene8F),
		FrameFunc(SetupScene9, &GraphTheory::SetupScene9F),
		FrameFunc(SetupScene10, &GraphTheory::SetupScene10F),
		FrameFunc(SetupScene11, &GraphTheory::SetupScene11F),
		FrameFunc(SetupScene12, &GraphTheory::SetupScene12F),
		FrameFunc(SetupScene13, &GraphTheory::SetupScene13F),
		FrameFunc(SetupScene14, &GraphTheory::SetupScene14F),
		FrameFunc(SetupScene15, &GraphTheory::SetupScene15F),
		FrameFunc(SetupScene16, &GraphTheory::SetupScene16F),
		FrameFunc(SetupScene17, &GraphTheory::SetupScene17F),
		FrameFunc(Quit, &GraphTheory::QuitF)
	};
	//-------------------------------------------------------------------------------------------------And now the Useful functions
	void RemoveFuncByName(EFFID d) {
		for (int i = 0; i < TheList.size(); i++) {
			if (TheList[i].first == d) {
				TheListChanged = true;
				TheList.erase(TheList.begin() + i); //A bit weird but that's expected
				return;
			}
		}
		std::cout << "\nCouldn't find specified Func " << d;
	}
	void AddFuncByPos(FrameFunc f, int index) {
		if (index > TheList.size() - 1) { std::cout << "\nAddFunc Failed"; return; }
		TheListChanged = true;
		TheList.insert(TheList.begin() + index, f);
	}
	void AddFuncAfterName(FrameFunc f, EFFID d) {
		for (int i = 0; i < TheList.size(); i++) {
			if (TheList[i].first == d) {
				TheListChanged = true;
				TheList.insert(TheList.begin() + i, f);
				return;
			}
		}
		std::cout << "\nCouldn't find specified Func " << d;
	}
	void AddFuncBeforeName(FrameFunc f, EFFID d) {
		for (int i = 0; i < TheList.size(); i++) {
			if (TheList[i].first == d) {
				TheListChanged = true;
				TheList.insert(TheList.begin() + i - 1, f);
				return;
			}
		}
		std::cout << "\nCouldn't find specified Func " << d;
	}
	void ResetTheList() {
		TheListChanged = true;
		TheList.clear();
		TheList.push_back(FrameFunc(DrawBackground, &GraphTheory::DrawBackgroundF));
	}
	void DrawNode(Node n) {
		FillCircle(olc::vi2d(n.Pos), (int)n.Rad, n.Color);
		DrawCircle(olc::vi2d(n.Pos), (int)n.Rad, n.BorderColor);
		//DrawStringDecal(n.Pos + olc::vf2d(n.Rad + 5.0f, 0.0f), std::to_string(n.Vel.x), olc::WHITE, { 1.0f, 1.0f });
	}
	void DrawNodeWithText(Node n, std::string s) {
		FillCircle(olc::vi2d(n.Pos), (int)n.Rad, n.Color);
		DrawCircle(olc::vi2d(n.Pos), (int)n.Rad, n.BorderColor);
		DrawStringDecal(n.Pos + olc::vf2d(n.Rad + 5.0f, 0.0f), s, olc::WHITE, { 1.25f, 1.3f });
	}
	void DrawEdge(Edge e) {
		olc::vi2d b = olc::vi2d(BigG.N[e.Node1].Pos);
		olc::vi2d a = olc::vi2d(BigG.N[e.Node2].Pos);

		DrawLine(b, a, e.Color);
		DrawLine(b - olc::vi2d(1, 0), a, e.Color); //Make it thicc
		DrawLine(b, a + olc::vi2d(0, 1), e.Color);
		DrawLine(b - olc::vi2d(2, 0), a, e.Color); 
		DrawLine(b, a + olc::vi2d(0, 2), e.Color);
	}
	float AbsMin(float Num, float Num2) {
		if (std::abs(Num) > Num2) {
			return copysignf(Num2, Num);
		}
		return Num;
	}
	float Reduce(float Num, float by) {
		float S = copysignf(1.0f, Num);
		Num -= by;
		if (copysignf(1.0f, Num) != S) {
			return 0.0f;
		}
		return Num;
	}
	void ShowText(AdvStr& T, float S, float Sc, olc::Pixel C = olc::WHITE) {
		T.Pcnt = std::min(T.Pcnt + S, 1.0f);
		if (T.Pcnt >= 1.0f) { T.Done = true; }
		T.Text = T.Origin.substr(0, T.Pcnt * T.Origin.length());
		DrawStringDecal(T.Pos, T.Text, C, { Sc, Sc*1.25f });
	}
	void DrawButton(ScButt B) {
		if (!B.Visible) { return; }
		if (B.Hover) { B.Color.a = 200; }
		FillRectDecal(B.Pos, B.Size, B.Color);
		DrawStringDecal(B.Pos + olc::vf2d(15.0f, 30.0f), B.Text, olc::Pixel(0,0,0,B.Color.a), {2.3f, 2.3f});
	}
	bool MouseInside(olc::vf2d Pos, olc::vf2d Size) {
		if ((GetMouseX() >= Pos.x && GetMouseX() <= Pos.x + Size.x)
			&& (GetMouseY() >= Pos.y && GetMouseY() <= Pos.y + Size.y)) {
			return true;
		}
		return false;
	}
	void NodePhysics(float f) {

		//First would be update potential position
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Pos = BigG.N[i].PotPos;
		}

		//Then ensure edge lengths and reset accels
		for (int i = 0; i < BigG.E.size(); i++) {
			BigG.E[i].Length = (BigG.N[BigG.E[i].Node1].Pos - BigG.N[BigG.E[i].Node2].Pos).mag();
		}
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Accel = { 0.0f, 0.0f };
		}

		//Then calculate accels
		for (int i = 0; i < BigG.E.size(); i++) {
			Edge& E = BigG.E[i];
			Node& One = BigG.N[E.Node1];
			Node& Two = BigG.N[E.Node2];
			One.Accel.x = AbsMin(One.Accel.x +
				(Two.Pos.x - One.Pos.x - copysignf(100.0f, (Two.Pos.x - One.Pos.x))) * 0.25f, 5.0f);
			One.Accel.y = AbsMin(One.Accel.y +
				(Two.Pos.y - One.Pos.y - copysignf(100.0f, (Two.Pos.y - One.Pos.y))) * 0.25f, 5.0f);
			Two.Accel.x = AbsMin(Two.Accel.x +
				(One.Pos.x - Two.Pos.x - copysignf(100.0f, (One.Pos.x - Two.Pos.x))) * 0.25f, 5.0f);
			Two.Accel.y = AbsMin(Two.Accel.y +
				(One.Pos.y - Two.Pos.y - copysignf(100.0f, (One.Pos.y - Two.Pos.y))) * 0.25f, 5.0f);
		}

		//Adjust velocities
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Vel += BigG.N[i].Accel;
		}

		//Check for ultra small values/friction
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Vel.x -= copysignf(3.5f, BigG.N[i].Vel.x);
			BigG.N[i].Vel.y -= copysignf(3.5f, BigG.N[i].Vel.y);
			if (std::abs(BigG.N[i].Vel.x) > 0.0f && std::abs(BigG.N[i].Vel.x) < 1.5f) {
				BigG.N[i].Vel.x = 0.0f;
			}
			if (std::abs(BigG.N[i].Vel.y) > 0.0f && std::abs(BigG.N[i].Vel.y) < 1.5f) {
				BigG.N[i].Vel.y = 0.0f;
			}
		}

		//Adjust potential position
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].PotPos += (BigG.N[i].Vel * f);
		}

		//Basic collision
		for (int i = 0; i < BigG.N.size(); i++) {
			if (BigG.N[i].PotPos.x <= (0.0f + BigG.N[i].Rad)) {
				BigG.N[i].PotPos.x = 0.0f + BigG.N[i].Rad;
				BigG.N[i].Vel.x *= -1.0f;
			}
			if (BigG.N[i].PotPos.x >= (ScreenWidth() - BigG.N[i].Rad)) {
				BigG.N[i].PotPos.x = ScreenWidth() - BigG.N[i].Rad;
				BigG.N[i].Vel.x *= -1.0f;
			}
			if (BigG.N[i].PotPos.y <= (0.0f + BigG.N[i].Rad)) {
				BigG.N[i].PotPos.y = 0.0f + BigG.N[i].Rad;
				BigG.N[i].Vel.y *= -1.0f;
			}
			if (BigG.N[i].PotPos.y >= (ScreenHeight() - BigG.N[i].Rad)) {
				BigG.N[i].PotPos.y = ScreenHeight() - BigG.N[i].Rad;
				BigG.N[i].Vel.y *= -1.0f;
			}
		}


	}
	//-----------------------------------------------------------------------------------------------------These are organizational functions, they don't have to exist, but it makes code more tidy

	//-------------------------------------------------------------------------------------------------And here are the FrameFunctions (Make sure they have F at the end)
	bool DrawBackgroundF(float f) {
		static float Count = 0.0f;
		SetDrawTarget(1);
		SetPixelMode(olc::Pixel::ALPHA);
		Clear(olc::BLANK);
		Clear(olc::Pixel(
			(int8_t)(80.0f * (0.5f * std::cos(Count + 0.785f) + 0.5f)),
			(int8_t)(80.0f * (0.5f * std::sin(Count) + 0.5f)),
			(int8_t)(80.0f * (0.5f * std::cos(Count) + 0.5f))
		));
		FillRectDecal({ 5.0f,5.0f }, {(float) ScreenWidth() - 10.0f, (float)ScreenHeight() - 10.0f },
			olc::Pixel((int8_t)(80.0f * (0.5f * std::sin(Count + 0.785f) + 0.5f) + 60.0f),
				(int8_t)(80.0f * (0.5f * std::cos(Count) + 0.5f) + 60.0f),
				(int8_t)(80.0f * (0.5f * std::sin(Count) + 0.5f) + 60.0f)
			));

		Count += 6.28f / 7200.0f; //Demoninator is how many frames for a full cycle
		if (Count > 6.28f) { Count -= 6.28f; }

		SetPixelMode(olc::Pixel::NORMAL);
		SetDrawTarget(nullptr);
		return true;
	}
	bool SetupTestF(float f) {
		
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.3f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.45f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.75f, ScreenHeight() * 0.75f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.8f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		Edge E;
		E.Node1 = 0;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 3;
		BigG.E.push_back(E);

		MainText.Origin = "This is a test String.\nBlah blah bleh...";
		MainText.Pos = { 20.0f, ScreenHeight() - 200.0f };
		MainText.Pcnt = 0.0f;
		MainText.Done = false;

		//Buttons
		NextB.Visible = false;
		PrevB.Visible = true;
		SceneFinished = false;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessTest, &GraphTheory::ProcessTestF), SetupTest);
		AddFuncAfterName(FrameFunc(ProcessMouse, &GraphTheory::ProcessMouseF), ProcessTest);
		AddFuncAfterName(FrameFunc(DrawTest, &GraphTheory::DrawTestF), ProcessTest);
		RemoveFuncByName(SetupTest);
		return true;
	}
	bool ProcessTestF(float f) {
		//First would be update potential position
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Pos = BigG.N[i].PotPos;
		}

		//Then ensure edge lengths and reset accels
		for (int i = 0; i < BigG.E.size(); i++) {
			BigG.E[i].Length = (BigG.N[BigG.E[i].Node1].Pos - BigG.N[BigG.E[i].Node2].Pos).mag();
		}
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Accel = { 0.0f, 0.0f };
		}

		//Then calculate accels
		for (int i = 0; i < BigG.E.size(); i++) {
			Edge& E = BigG.E[i];
			Node& One = BigG.N[E.Node1];
			Node& Two = BigG.N[E.Node2];
			One.Accel.x = AbsMin(One.Accel.x + 
				(Two.Pos.x - One.Pos.x - copysignf(100.0f, (Two.Pos.x - One.Pos.x))) * 0.25f, 5.0f);
			One.Accel.y = AbsMin(One.Accel.y + 
				(Two.Pos.y - One.Pos.y - copysignf(100.0f, (Two.Pos.y - One.Pos.y))) * 0.25f, 5.0f);
			Two.Accel.x = AbsMin(Two.Accel.x + 
				(One.Pos.x - Two.Pos.x - copysignf(100.0f, (One.Pos.x - Two.Pos.x))) * 0.25f, 5.0f);
			Two.Accel.y = AbsMin(Two.Accel.y + 
				(One.Pos.y - Two.Pos.y - copysignf(100.0f, (One.Pos.y - Two.Pos.y))) * 0.25f, 5.0f);
		}

		//Adjust velocities
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Vel += BigG.N[i].Accel;
		}

		//Check for ultra small values/friction
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].Vel.x -= copysignf(3.5f, BigG.N[i].Vel.x);
			BigG.N[i].Vel.y -= copysignf(3.5f, BigG.N[i].Vel.y);
			if (std::abs(BigG.N[i].Vel.x) > 0.0f && std::abs(BigG.N[i].Vel.x) < 1.5f) {
				BigG.N[i].Vel.x = 0.0f;
			}
			if (std::abs(BigG.N[i].Vel.y) > 0.0f && std::abs(BigG.N[i].Vel.y) < 1.5f) {
				BigG.N[i].Vel.y = 0.0f;
			}
		}

		//Adjust potential position
		for (int i = 0; i < BigG.N.size(); i++) {
			BigG.N[i].PotPos += (BigG.N[i].Vel * f);
		}

		//Basic collision
		for (int i = 0; i < BigG.N.size(); i++) {
			if (BigG.N[i].PotPos.x <= (0.0f + BigG.N[i].Rad)) {
				BigG.N[i].PotPos.x = 0.0f + BigG.N[i].Rad;
				BigG.N[i].Vel.x *= -1.0f;
			}
			if (BigG.N[i].PotPos.x >= (ScreenWidth() - BigG.N[i].Rad)) {
				BigG.N[i].PotPos.x = ScreenWidth() - BigG.N[i].Rad;
				BigG.N[i].Vel.x *= -1.0f;
			}
			if (BigG.N[i].PotPos.y <= (0.0f + BigG.N[i].Rad)) {
				BigG.N[i].PotPos.y = 0.0f + BigG.N[i].Rad;
				BigG.N[i].Vel.y *= -1.0f;
			}
			if (BigG.N[i].PotPos.y >= (ScreenHeight() - BigG.N[i].Rad)) {
				BigG.N[i].PotPos.y = ScreenHeight() - BigG.N[i].Rad;
				BigG.N[i].Vel.y *= -1.0f;
			}
		}


		
		return true;
	}
	bool DrawTestF(float f) {
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}

		for (int i = 0; i < BigG.N.size(); i++) {
			DrawNode(BigG.N[i]);
		}

		//String stuff
		ShowText(MainText, 1.0f * f, 4.0f);

		if (MainText.Done) { SceneFinished = true; }
		//Buttons
		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	bool ProcessMouseF(float f) {
		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					ResetTheList();
					CurrScene--;
					TheList.push_back(SceneList[CurrScene]);
				}
			}
		}
		//if (MouseInside(PrevB.Pos, PrevB.Size)) {
		//	PrevB.Hover = true;
		//}
		return true;
	}
	bool ProcessMouseScene1F(float f) {
		NextB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					ResetTheList();
					CurrScene++;
					TheList.push_back(SceneList[CurrScene]);
				}
			}
		}
		//if (MouseInside(PrevB.Pos, PrevB.Size)) {
		//	PrevB.Hover = true;
		//}

		
		return true;
	}
	bool SetupScene1F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.3f, ScreenHeight() * 0.4f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.5f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.4f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		Edge E;
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);

		SceneFinished = false;

		MainText.Origin = "The Havel Hakimi Theorem";
		MainText.Pcnt = 0.0f;
		MainText.Pos = { ScreenWidth() * 0.05f, 200.0f };
		MainText.Done = false;

		//Buttons
		NextB.Pos = { ScreenWidth() - NextB.Size.x, ScreenHeight() - NextB.Size.y };
		PrevB.Pos = { 0.0f, ScreenHeight() - PrevB.Size.y };
		NextB.Text = "Next";
		PrevB.Text = "Back";
		NextB.Visible = true;
		PrevB.Visible = false;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene1, &GraphTheory::ProcessMouseScene1F), SetupScene1);
		AddFuncAfterName(FrameFunc(ProcessScene1, &GraphTheory::ProcessScene1F), SetupScene1);
		//AddFuncAfterName(FrameFunc(DrawTest, &GraphTheory::DrawTestF), ProcessTest);
		RemoveFuncByName(SetupScene1);
		return true;
	}
	bool ProcessScene1F(float f) {
		static AdvStr SubTitle = { "        Made by WowTwentyFive\n\n\nFor the Summer of Math Exposition #1", "", 0.0f, false, {ScreenWidth() * 0.30f, 450.0f} };
		static AdvStr CN = { "               Click next to begin!", "", 0.0f, false, {ScreenWidth() * 0.50f, ScreenHeight() - 77.0f} };


		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}

		for (int i = 0; i < BigG.N.size(); i++) {
			DrawNode(BigG.N[i]);
		}

		//String stuff
		ShowText(MainText, 1.0f * f, 6.0f);
		ShowText(SubTitle, 0.5f * f, 2.0f);
		ShowText(CN, 0.4f * f, 1.75f);

		if (CN.Done) { SceneFinished = true; }
		//Buttons
		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	//-----Scene2
	bool SetupScene2F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.8f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);

		SceneFinished = false;

		SubText.Origin = 
			"This interactive application showcases Graph Theory, and a fun\n"
			"little theorem that has a nice visual understanding to it. This\n"
			"should (hopefully) require almost zero technical knowledge on\n"
			"the subject, but I may gloss over things to ensure that it is \nunderstandable."
			"\n\n\n\n\n\n\n"
			"This program only uses the mouse! You can click the Next and\n"
			"Back buttons (When they appear) to to move through the lecture.\n"
			"You can also move the nodes (blue circles) around by clicking\n"
			"and holding the mouse button. Try it now!";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Goal and Purpose:\n\n\n\n\n\n\nInstructions:", "", 0.0f, false, { ScreenWidth() * 0.075f, 150.0f } };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene2);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene2);
		RemoveFuncByName(SetupScene2);
		return true;
	}
	bool ProcessScene2F(float f) {
		

		NodePhysics(f);


		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}

		for (int i = 0; i < BigG.N.size(); i++) {
			DrawNode(BigG.N[i]);
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 3.5f);

		if (MainText.Done) { SceneFinished = true; }
		//Buttons
		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	bool ProcessMouseScene2F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					ResetTheList();
					CurrScene++;
					TheList.push_back(SceneList[CurrScene]);
				}
			}
		}
		
		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					ResetTheList();
					CurrScene--;
					TheList.push_back(SceneList[CurrScene]);
				}
			}
		}


		return true;
	}
	//----------------Scene3
	bool SetupScene3F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.75f, ScreenHeight() * 0.75f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.8f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.6f, ScreenHeight() * 0.7f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		Edge E;
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 2;
		E.Node2 = 3;
		BigG.E.push_back(E);
		E.Node1 = 3;
		E.Node2 = 4;
		BigG.E.push_back(E);

		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"Graph Theory is all about two things: Nodes and Edges. Take a \n"
			"look at the graph below. It has 5 nodes (blue circles), and 4\n"
			"edges (white lines).";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "What is Graph Theory?:", "", 0.0f, false, { ScreenWidth() * 0.075f, 150.0f } };


		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene3, &GraphTheory::ProcessMouseScene3F), SetupScene3);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene3);
		RemoveFuncByName(SetupScene3);
		return true;
	}

	bool ProcessMouseScene3F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
						SubText.Origin = "Now it has 6 nodes and 6 edges!";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						{
							Node N;
							N.Rad = 15.0f;
							N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.6f };
							N.PotPos = N.Pos;
							N.Color = olc::BLUE;
							BigG.N.push_back(N);
							Edge E;
							E.Node1 = 4;
							E.Node2 = 5;
							E.Color = olc::YELLOW;
							BigG.E.push_back(E);
							E.Node1 = 5;
							E.Node2 = 1;
							BigG.E.push_back(E);
						}
						break;
					case 1:
						SubScene = 2;
						SubText.Origin = "And now there's 6 nodes and 0 edges! This still\ncounts as one graph, by the way.";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;

						BigG.E.clear();

						break;
					case 2:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
					
				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 1:
						SubScene = 0;
						SubText.Origin = "Graph Theory is all about two things: Nodes and Edges. Take a \n"
							"look at the graph below. It has 5 nodes (blue circles), and 4\n"
							"edges (white lines).";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;

						BigG.N.erase(BigG.N.end() - 1);
						BigG.E.erase(BigG.E.end() - 1);
						BigG.E.erase(BigG.E.end() - 1);
						
						break;
					case 2:
						SubScene = 1;
						SubText.Origin = "Now it has 6 nodes and 6 edges!";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;

						{
							Edge E;
							E.Node1 = 0;
							E.Node2 = 1;
							BigG.E.push_back(E);
							E.Node1 = 1;
							E.Node2 = 2;
							BigG.E.push_back(E);
							E.Node1 = 2;
							E.Node2 = 3;
							BigG.E.push_back(E);
							E.Node1 = 3;
							E.Node2 = 4;
							BigG.E.push_back(E);
							E.Color = olc::YELLOW;
							E.Node1 = 4;
							E.Node2 = 5;
							BigG.E.push_back(E);
							E.Node1 = 5;
							E.Node2 = 1;
							BigG.E.push_back(E);
						}
						break;
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	//----------------Scene4
	bool SetupScene4F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.3f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.3f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.625f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.625f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.675f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.6f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		Edge E;
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 2;
		E.Node2 = 0;
		BigG.E.push_back(E);
		E.Node1 = 3;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 4;
		E.Node2 = 5;
		BigG.E.push_back(E);
		E.Node1 = 5;
		E.Node2 = 3;
		BigG.E.push_back(E);
		E.Node1 = 2;
		E.Node2 = 5;
		BigG.E.push_back(E);

		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"These Graphs don't really look like math, do they? After all,\n"
			"there's not even any numbers! Rest assured, mathematicians \n"
			"found a way.\n"
			"Let's observe the graph below, and count how many edges each\n"
			"node is attached to. Click next and we'll label them.";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Degree and Degree Sequences:", "", 0.0f, false, {ScreenWidth() * 0.075f, 100.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene4, &GraphTheory::ProcessMouseScene4F), SetupScene4);
		AddFuncAfterName(FrameFunc(ProcessScene4, &GraphTheory::ProcessScene4F), SetupScene4);
		RemoveFuncByName(SetupScene4);
		return true;
	}
	bool ProcessScene4F(float f) {

		NodePhysics(f);


		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}
		static std::array<int, 6> DL = { 2,2,3,2,2,3 };
		if (SubScene == 0) {
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNode(BigG.N[i]);
			}
		}
		else {
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(DL[i]));
			}
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);

		if (MainText.Done) { SceneFinished = true; }
		//Buttons
		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	bool ProcessMouseScene4F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
		  SubText.Origin = "Mathematicians call this number the Degree, and we can do\n"
							"tons of stuff with it. Like, for example, putting them in\n"
							"a list!\n"
							"{3, 3, 2, 2, 2, 2}\n"
							"Is the list we come up with.";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;

						break;
					case 1:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}

				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 1:
						SubScene = 0;
						SubText.Origin = "These Graphs don't really look like math, do they? After all,\n"
							"there's not even any numbers! Rest assured, mathematicians \n"
							"found a way.\n"
							"Let's observe the graph below, and count how many edges each\n"
							"node is attached to. Click next and we'll label them.";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						break;
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	//----------------Scene5
	bool SetupScene5F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.2f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.25f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.2f, ScreenHeight() * 0.625f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.8f, ScreenHeight() * 0.55f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.83f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.78f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.675f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);

		Edge E;
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 2;
		E.Node2 = 0;
		BigG.E.push_back(E);
		E.Node1 = 3;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 4;
		E.Node2 = 5;
		BigG.E.push_back(E);
		E.Node1 = 5;
		E.Node2 = 6;

		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"These lists actually have a special name: Degree Sequences.\n"
			"They're a way of analyzing a graph numerically, and they\n"
			"are the focus of this lecture. The two graphs below show \n"
			"different kinds of degree sequences.\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;

		MainText = { "Degree and Degree Sequences:", "", 0.0f, false, {ScreenWidth() * 0.075f, 100.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene5);
		AddFuncAfterName(FrameFunc(ProcessScene5, &GraphTheory::ProcessScene5F), SetupScene5);
		RemoveFuncByName(SetupScene5);
		return true;
	}
	bool ProcessScene5F(float f) {
		static AdvStr Seq1 = { "{2, 2, 2}", "", 0.0f, false, {ScreenWidth() * 0.075f, ScreenHeight() - 200.0f} };
		static AdvStr Seq2 = { "{2, 1, 1, 0}", "", 0.0f, false, {ScreenWidth() * 0.625f, ScreenHeight() - 200.0f} };

		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}
		static std::array<int, 7> s = { 2,2,2,1,2,1,0 };
		
		for (int i = 0; i < BigG.N.size(); i++) {
			DrawNodeWithText(BigG.N[i], std::to_string(s[i]));
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);
		ShowText(Seq1, 3.0f * f, 2.5f);
		ShowText(Seq2, 3.0f * f, 2.5f);

		if (MainText.Done) { SceneFinished = true; }
		//Buttons
		
		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	//----------------Scene6
	bool SetupScene6F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.2f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.25f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.2f, ScreenHeight() * 0.625f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.8f, ScreenHeight() * 0.55f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.83f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.78f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.675f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.55f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.45f, ScreenHeight() * 0.55f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);

		Edge E;
		E.Node1 = 3;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 3;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 3;
		E.Node2 = 7;
		BigG.E.push_back(E);
		E.Node1 = 3;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 5;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 8;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 9;
		BigG.E.push_back(E);
		E.Node1 = 5;
		E.Node2 = 6;
		BigG.E.push_back(E);
		E.Node1 = 5;
		E.Node2 = 7;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 2;
		BigG.E.push_back(E);

		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"As an exercise, try and figure out the Degree Sequence\n"
			"of the graph below! Press next to see the answer.\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Degree and Degree Sequences:", "", 0.0f, false, {ScreenWidth() * 0.075f, 100.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene6, &GraphTheory::ProcessMouseScene6F), SetupScene6);
		AddFuncAfterName(FrameFunc(ProcessScene6, &GraphTheory::ProcessScene6F), SetupScene6);
		RemoveFuncByName(SetupScene6);
		return true;
	}
	bool ProcessScene6F(float f) {
		static AdvStr Seq1 = { "{4, 4, 3, 3, 3, 1, 1, 1, 1}", "", 0.0f, false, {ScreenWidth() * 0.075f, ScreenHeight() - 200.0f} };
		

		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}
		static std::array<int, 10> s = { 1,3,3,4,1,3,1, 4 ,1 ,1 };

		if (SubScene == 0) {
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNode(BigG.N[i]);
			}
		}
		else {
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s[i]));
			}
			ShowText(Seq1, 3.0f * f, 2.5f);
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);
		

		if (MainText.Done) { SceneFinished = true; }
		//Buttons

		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	bool ProcessMouseScene6F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
						break;
					case 1:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}

				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 1:
						SubScene = 0;
						break;
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	bool SetupScene7F(float f) {
		BigG.N.clear();
		BigG.E.clear();


		SceneFinished = false;

		SubText.Origin =
			"What about if we are given a Sequence first, and have to find\n"
			"the graph? Let's say the Degree Sequence is {2, 2, 1}, what\n"
			"would that graph look like?\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "The Question:", "", 0.0f, false, {ScreenWidth() * 0.075f, 150.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene7);
		AddFuncAfterName(FrameFunc(ProcessScene7, &GraphTheory::ProcessScene7F), SetupScene7);
		RemoveFuncByName(SetupScene7);
		return true;
	}
	bool ProcessScene7F(float f) {

		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}

		for (int i = 0; i < BigG.N.size(); i++) {
			DrawNode(BigG.N[i]);
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);

		if (MainText.Done) { SceneFinished = true; }
		//Buttons
		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	
	//----------Scene8
	bool SetupScene8F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.8f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.6f, ScreenHeight() * 0.7f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.8f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);

		Edge E;
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 2;
		BigG.E.push_back(E);

		SceneFinished = false;

		SubText.Origin =
			"The truth is there is no graph! We can get pretty close,\n"
			"but there's just no room for that last edge. Most\n"
			"Degree Sequences (actually most of them) cannot be made\n"
			"into a graph. So the big question is: how do we check if\n"
			"a Degree Sequence is valid or not?\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "The Question:", "", 0.0f, false, {ScreenWidth() * 0.075f, 150.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene8);
		AddFuncAfterName(FrameFunc(ProcessScene8, &GraphTheory::ProcessScene8F), SetupScene8);
		RemoveFuncByName(SetupScene8);
		return true;
	}
	bool ProcessScene8F(float f) {

		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}
		static std::array<int, 10> s = { 1,2,1 };

		for (int i = 0; i < BigG.N.size(); i++) {
			DrawNodeWithText(BigG.N[i], std::to_string(s[i]) + "?");
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);


		if (MainText.Done) { SceneFinished = true; }
		//Buttons

		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	//-------Scene 9
	bool SetupScene9F(float f) {
		BigG.N.clear();
		BigG.E.clear();


		SceneFinished = false;

		SubText.Origin =
			"Lucky for us, two Mathematicians found a somewhat unexpected\n"
			"answer. Vaclav Havel (in 1955) and Seifollah Hakimi (in 1962)\n"
			"both discovered an intersting property about Degree Sequences.\n"
			"It doesn't answer the question right away, but it makes it a\n"
			"lot easier!"
			"\n\n"
			"We'll start with an example, since the theorem is pretty\n"
			"intuitive to follow.";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "The Answer:", "", 0.0f, false, {ScreenWidth() * 0.075f, 150.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene9);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene9);
		RemoveFuncByName(SetupScene9);
		return true;
	}
	//--------Scene 10
	bool SetupScene10F(float f) {
		BigG.N.clear();
		BigG.E.clear();


		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"Let's say we get the following Sequence:\n"
			"{6, 4, 3, 2, 2, 2, 1, 0}\n"
			"and want to know if there is a graph associated with it,\n"
			"the process goes like this:\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "", "", 0.0f, false, {ScreenWidth() * 0.075f, 150.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene10, &GraphTheory::ProcessMouseScene10F), SetupScene10);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene10);
		RemoveFuncByName(SetupScene10);
		return true;
	}

	bool ProcessMouseScene10F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
						SubText.Origin = SubText.Origin +
							"\n\n"
							"Look at the largest number in the list, which is 6. Then,\n"
							"subtract 1 from the next 6 nodes. And then remove the 6 from\n"
							"the sequence.";
						SubText.Done = false;
						SubText.Pcnt = 0.5f;

						break;
					case 1:
						SubScene = 2;
						SubText.Origin = SubText.Origin + 
							"\n\n"
							"Our new Sequence is {3, 2, 1, 1, 1, 0, 0}\n"
							"And that's it! The Havel-Hakimi theorem tells us that: if this\n"
							"simpler sequence we've made corresponds to a graph, so does\n"
							"the first sequence! It works the other way, too. If the new\n"
							"sequence is NOT a valid Degree Sequence, neither is the first.";
						SubText.Done = false;
						SubText.Pcnt = 0.66f;
						break;
					case 2:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}

				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 1:
						SubScene = 0;
						SubText.Origin = "Let's say we get the following Sequence:\n"
							"{6, 4, 3, 2, 2, 2, 1, 0}\n"
							"and want to know if there is a graph associated with it,\n"
							"the process goes like this:\n";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						break;
					case 2:
						SubScene = 1;
						SubText.Origin = "Let's say we get the following Sequence:\n"
							"{6, 4, 3, 2, 2, 2, 1, 0}\n"
							"and want to know if there is a graph associated with it,\n"
							"the process goes like this:\n"
							"\n\n"
							"Look at the largest number in the list, which is 6. Then,\n"
							"subtract 1 from the next 6 nodes. And then remove the 6 from\n"
							"the sequence.";
						SubText.Done = false;
						SubText.Pcnt = 0.5f;
						break;
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	//--------Scene 11
	bool SetupScene11F(float f) {
		BigG.N.clear();
		BigG.E.clear();


		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"Well... That hasn't really answered our question, has it?\n"
			"It's not obvious if the new sequence makes a valid graph\n"
			"(At least, not to me). So what comes next?\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 100.0f };
		SubText.Done = false;
		MainText = { "", "", 0.0f, false, {ScreenWidth() * 0.075f, 150.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene11, &GraphTheory::ProcessMouseScene11F), SetupScene11);
		AddFuncAfterName(FrameFunc(ProcessScene11, &GraphTheory::ProcessScene11F), SetupScene11);
		RemoveFuncByName(SetupScene11);
		return true;
	}

	bool ProcessScene11F(float f) {
		//Just drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);

		if (SubText.Done) { SceneFinished = true; }
		//Buttons

		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}

	bool ProcessMouseScene11F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
						SubText.Origin = SubText.Origin +
							"\n\n"
							"We just do it again! This theorem can be used over and over\n"
							"until we get an answer. Let's see this step-by-step:\n";
						SubText.Done = false;
						SubText.Pcnt = 0.5f;

						break;
					case 1:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}

				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 1:
						SubScene = 0;
						SubText.Origin = "Well... That hasn't really answered our question, has it?\n"
							"It's not obvious if the new sequence makes a valid graph\n"
							"(At least, not to me). So what comes next?\n";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						break;
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	//--------Scene 12
	bool SetupScene12F(float f) {
		BigG.N.clear();
		BigG.E.clear();


		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"At this point, the Havel-Hakimi Theorem stops doing anything,\n"
			"but now we can answer our question! Is this bunch of Zeros\n"
			"a valid graph?\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.075f, ScreenHeight() * 0.8f };
		SubText.Done = false;
		MainText = { "", "", 0.0f, false, {ScreenWidth() * 0.075f, ScreenHeight() * 0.8f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene12);
		AddFuncAfterName(FrameFunc(ProcessScene12, &GraphTheory::ProcessScene12F), SetupScene12);
		RemoveFuncByName(SetupScene12);
		return true;
	}

	bool ProcessScene12F(float f) {
		static AdvStr Seq1 = { "{6, 4, 3, 2, 2, 2, 1, 0}\n|\nv\n{6, 4, 3, 2, 2, 2, 1, 0} <- Use the H-H Theorem", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.1f} };
		static AdvStr Seq2 = { "{3, 2, 1, 1, 1, 0, 0}\n|\nv\n{3, 2, 1, 1, 1, 0, 0} <- Use the H-H Theorem", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Seq3 = { "{1, 1, 0, 0, 0, 0}\n|\nv\n{1, 1, 0, 0, 0, 0} <- Use the H-H Theorem", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.5f} };
		static AdvStr Seq4 = { "{0, 0, 0, 0, 0} <- Stop!", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.7f} };
		static AdvStr Marks1 = { "\n\n\n /\n   -1 -1 -1 -1 -1 -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.1f} };
		static AdvStr Marks2 = { "\n\n\n /\n   -1 -1 -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Marks3 = { "\n\n\n /\n   -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.5f} };


		//Just drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);

			ShowText(Seq1, 3.0f * f, 2.0f);
			ShowText(Seq2, 3.0f * f, 2.0f);
			ShowText(Seq3, 3.0f * f, 2.0f);
			ShowText(Seq4, 3.0f * f, 2.0f);
			ShowText(Marks1, 2.0f * f, 2.0f, olc::Pixel(255, 100, 100));
			ShowText(Marks2, 2.0f * f, 2.0f, olc::Pixel(255, 100, 100));
			ShowText(Marks3, 2.0f * f, 2.0f, olc::Pixel(255, 100, 100));

		if (SubText.Done) { SceneFinished = true; }
		//Buttons

		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	//----------------Scene13
	bool SetupScene13F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 18.0f;
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.45f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.625f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.6f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);


		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"{0, 0, 0, 0, 0}\n"
			"This is just 5 nodes with no edges between them.\n"
			"And thanks to the Theorem, this means ALL of the\n"
			"Degrees Sequences we saw are valid graphs, too!\n"
			"\n\n\n\n"
			"Now wait, the math might be simple, but why are\n"
			"removing nodes and subtracting? What is this theorem\n"
			"actually doing to the graphs?\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "It sure is!", "", 0.0f, false, {ScreenWidth() * 0.075f, 100.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene13);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene13);
		RemoveFuncByName(SetupScene13);
		return true;
	}
	//----------------Scene14
	bool SetupScene14F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 18.0f;
		N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.6f, ScreenHeight() * 0.7f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.75f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.45f, ScreenHeight() * 0.75f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.7f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.45f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.7f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N); //Deg 6 node

		Edge E;
		E.Node1 = 7;
		E.Node2 = 0;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 3;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 5;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 3;
		BigG.E.push_back(E);

		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"Below is the graph for the Degree Sequence we saw\n"
			"before: {6, 4, 3, 2, 2, 2, 1, 0}.\n"
			"We've arranged it in such a way that the node with\n"
			"the highest degree is attached to nodes with the\n"
			"next highest degrees. But watch this:";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Back to graphs", "", 0.0f, false, {ScreenWidth() * 0.075f, 100.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene14, &GraphTheory::ProcessMouseScene14F), SetupScene14);
		AddFuncAfterName(FrameFunc(ProcessScene14, &GraphTheory::ProcessScene14F), SetupScene14);
		RemoveFuncByName(SetupScene14);
		return true;
	}
	bool ProcessScene14F(float f) {
		static AdvStr Seq1 = { "{6, 4, 3, 2, 2, 2, 1, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() - 75.0f} };
		static AdvStr Seq2 = { "{6, 4, 3, 2, 2, 2, 1, 0}  ->  {3, 2, 1, 1, 1, 0, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() - 75.0f} };
		static AdvStr Marks1 = { " /\n   -1 -1 -1 -1 -1 -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() - 75.0f} };

		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}
		static std::vector<int> s = { 4, 3, 2, 2, 2, 1, 0, 6 };
		static std::vector<int> s2 = { 3, 2, 1, 1, 1, 0, 0 };

		if (SubScene < 2) {
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s[i]));
				ShowText(Seq1, 3.0f * f, 2.0f);
			}
		}
		else {
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s2[i]));
				ShowText(Seq2, 3.0f * f, 2.0f);
				ShowText(Marks1, 3.0f * f, 2.0f, olc::Pixel(255, 100, 100));
			}
			
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);


		if (SubText.Done) { SceneFinished = true; }
		//Buttons

		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	bool ProcessMouseScene14F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
						SubText.Origin = "Let's get rid of the highest-degree node, and all its\n"
							"edges. I've marked them a different color.\n";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;

						BigG.N[7].Color = olc::DARK_RED;
						for (int i = 0; i < 6; i++) {
							BigG.E[i].Color = olc::Pixel(255, 100, 100);
						}
						break;
					case 1:
						SubScene = 2;
						SubText.Origin = "Does that look familiar? This is what the Havel-\n"
										 "Hakimi Theorem is actually doing to the graph. It\n"
										 "removes the largest node and takes away an edge from\n"
										 "any nodes that were connected to it!\n";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;

						BigG.N.erase(BigG.N.begin() + 7);
						for (int i = 0; i < 6; i++) {
							BigG.E.erase(BigG.E.begin());
						}
						break;
					case 2:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}

				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 1:
						SubScene = 0;
			SubText.Origin = "Below is the graph for the Degree Sequence we saw\n"
							"before: {6, 4, 3, 2, 2, 2, 1, 0}.\n"
							"We've arranged it in such a way that the node with\n"
							"the highest degree is attached to nodes with the\n"
							"next highest degrees. But watch this:";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						break;
					case 2:
						SubScene = 1;

						SubText.Origin = "Let's get rid of the highest-degree node, and all its\n"
							"edges. I've marked them a different color.\n";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						{
							Node N;
							N.Rad = 18.0f;
							N.Color = olc::DARK_RED;
							N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.7f };
							N.PotPos = N.Pos;
							BigG.N.push_back(N);

							Edge E;
							E.Color = olc::Pixel(255, 100, 100);
							E.Node1 = 7;
							E.Node2 = 0;
							BigG.E.push_back(E);
							E.Node1 = 7;
							E.Node2 = 1;
							BigG.E.push_back(E);
							E.Node1 = 7;
							E.Node2 = 2;
							BigG.E.push_back(E);
							E.Node1 = 7;
							E.Node2 = 3;
							BigG.E.push_back(E);
							E.Node1 = 7;
							E.Node2 = 4;
							BigG.E.push_back(E);
							E.Node1 = 7;
							E.Node2 = 5;
							BigG.E.push_back(E);
						}
						break;
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	//----------------Scene14
	bool SetupScene15F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 18.0f;
		N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.55f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.6f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.45f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.4f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.45f, ScreenHeight() * 0.65f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);
		N.Pos = { ScreenWidth() * 0.5f, ScreenHeight() * 0.6f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N); //Deg 6 node

		Edge E;
		E.Node1 = 7;
		E.Node2 = 0;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 3;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 7;
		E.Node2 = 5;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 1;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 2;
		BigG.E.push_back(E);
		E.Node1 = 0;
		E.Node2 = 4;
		BigG.E.push_back(E);
		E.Node1 = 1;
		E.Node2 = 3;
		BigG.E.push_back(E);

		SceneFinished = false;
		SubScene = 0;

		SubText.Origin =
			"Let's see this process, with the graphs, from start to\n"
			"finish.\n";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Final showing", "", 0.0f, false, {ScreenWidth() * 0.075f, 100.0f} };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene15, &GraphTheory::ProcessMouseScene15F), SetupScene15);
		AddFuncAfterName(FrameFunc(ProcessScene15, &GraphTheory::ProcessScene15F), SetupScene15);
		RemoveFuncByName(SetupScene15);
		return true;
	}
	bool ProcessScene15F(float f) {
		static AdvStr Seq1 = { "{6, 4, 3, 2, 2, 2, 1, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Seq2 = { "{6, 4, 3, 2, 2, 2, 1, 0}  ->  {3, 2, 1, 1, 1, 0, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Marks2 = { " /\n   -1 -1 -1 -1 -1 -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Seq3 = { "{3, 2, 1, 1, 1, 0, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Seq31 = { "{3, 2, 1, 1, 1, 0, 0}  ->  {1, 1, 0, 0, 0, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Marks3 = { " /\n   -1 -1 -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Seq4 = { "{1, 1, 0, 0, 0, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Seq41 = { "{1, 1, 0, 0, 0, 0}  ->  {0, 0, 0, 0, 0}", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };
		static AdvStr Marks4 = { " /\n   -1 -1 -1", "", 0.0f, false, {ScreenWidth() * 0.2f, ScreenHeight() * 0.3f} };

		NodePhysics(f);

		//Now drawing
		SetDrawTarget(nullptr); //One is for nodes
		Clear(olc::BLANK);
		for (int i = 0; i < BigG.E.size(); i++) {
			DrawEdge(BigG.E[i]);
		}
		static std::vector<int> s = { 4, 3, 2, 2, 2, 1, 0, 6 };
		static std::vector<int> s2 = { 3, 2, 1, 1, 1, 0, 0 };
		static std::vector<int> s3 = { 1, 0, 0, 1, 0, 0 };
		static std::vector<int> s4 = { 0, 0, 0, 0, 0 };

		switch (SubScene) {
		case 0:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s[i]));
			}
			break;
		case 1:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s[i]));
				ShowText(Seq1, 3.0f * f, 2.0f);
			}
			break;
		case 2:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s2[i]));
				ShowText(Seq2, 3.0f * f, 2.0f);
				ShowText(Marks2, 3.0f * f, 2.0f, olc::Pixel(255, 100, 100));
			}
			break;
		case 3:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s2[i]));
				ShowText(Seq3, 3.0f * f, 2.0f);
			}
			break;
		case 4:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s3[i]));
				ShowText(Seq31, 3.0f * f, 2.0f);
				ShowText(Marks3, 3.0f * f, 2.0f, olc::Pixel(255, 100, 100));
			}
			break;
		case 5:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s3[i]));
				ShowText(Seq4, 3.0f * f, 2.0f);
			}
			break;
		case 6:
			for (int i = 0; i < BigG.N.size(); i++) {
				DrawNodeWithText(BigG.N[i], std::to_string(s4[i]));
				ShowText(Seq41, 3.0f * f, 2.0f);
				ShowText(Marks4, 3.0f * f, 2.0f, olc::Pixel(255, 100, 100));
			}
			break;
		}

		//String stuff
		ShowText(SubText, 1.2f * f, 2.3f);
		ShowText(MainText, 2.5f * f, 2.5f);


		if (SubText.Done) { SceneFinished = true; }
		//Buttons

		if (SceneFinished) {
			DrawButton(NextB);
			DrawButton(PrevB);
		}
		return true;
	}
	bool ProcessMouseScene15F(float f) {

		static MouseThing M;

		//Check for press
		if (GetMouse(0).bPressed) {
			for (int i = 0; i < BigG.N.size(); i++) {
				if ((BigG.N[i].Pos - olc::vf2d(GetMouseX(), GetMouseY())).mag() < BigG.N[i].Rad + 4.0f) {
					M.node = i;
					break;
				}
			}
		}
		if (GetMouse(0).bReleased) {
			M.node = -1;
		}

		//Check for hold
		if (M.node != -1) {
			Node& One = BigG.N[M.node];
			One.Vel.x = ((float)GetMouseX() - One.Pos.x) * 5.0f;
			One.Vel.y = ((float)GetMouseY() - One.Pos.y) * 5.0f;
		}

		NextB.Hover = false;
		PrevB.Hover = false;
		if (SceneFinished) {
			if (MouseInside(NextB.Pos, NextB.Size)) {
				NextB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 0:
						SubScene = 1;
						SubText.Origin = "";
						SubText.Done = true;
						SubText.Pcnt = 0.0f;

						BigG.N[7].Color = olc::DARK_RED;
						for (int i = 0; i < 6; i++) {
							BigG.E[i].Color = olc::Pixel(255, 100, 100);
						}
						break;
					case 1:
						SubScene = 2;

						BigG.N.erase(BigG.N.begin() + 7);
						for (int i = 0; i < 6; i++) {
							BigG.E.erase(BigG.E.begin());
						}
						break;
					case 2:
						SubScene = 3;
						BigG.N[0].Color = olc::DARK_RED;
						for (int i = 0; i < 3; i++) {
							BigG.E[i].Color = olc::Pixel(255, 100, 100);
						}
						break;
					case 3:
						SubScene = 4;
						BigG.N.erase(BigG.N.begin());
						BigG.E.clear();
						{
							Edge E;
							E.Node1 = 0;
							E.Node2 = 3;
							BigG.E.push_back(E);
						}
						break;
					case 4:
						SubScene = 5;
						BigG.N[0].Color = olc::DARK_RED;
						BigG.E[0].Color = olc::Pixel(255, 100, 100);
						break;
					case 5:
						SubScene = 6;
						BigG.N.erase(BigG.N.begin());
						BigG.E.clear();
						break;
					case 6:
						ResetTheList();
						CurrScene++;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}

				}
			}
		}

		if (SceneFinished) {
			if (MouseInside(PrevB.Pos, PrevB.Size)) {
				PrevB.Hover = true;
				//Check for press
				if (GetMouse(0).bPressed) {
					switch (SubScene) {
					case 6:
						SubScene = 5;
						{
							Node N;
							N.Rad = 18.0f;
							N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.55f };
							N.PotPos = N.Pos;
							N.Color = olc::DARK_RED;
							BigG.N.insert(BigG.N.begin(), N);

							Edge E;
							E.Node1 = 0;
							E.Node2 = 3;
							E.Color = olc::Pixel(255, 100, 100);
							BigG.E.push_back(E);
						}
						break;

					case 5:
						SubScene = 4;
						BigG.N[0].Color = olc::DARK_BLUE;
						BigG.E[0].Color = olc::WHITE;
						break;
					case 4:
						SubScene = 3;
						{
							Node N;
							N.Rad = 18.0f;
							N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.55f };
							N.PotPos = N.Pos;
							N.Color = olc::DARK_RED;
							BigG.N.insert(BigG.N.begin(), N);

							BigG.E.clear();
							Edge E;
							E.Color = olc::Pixel(255, 100, 100);
							E.Node1 = 0;
							E.Node2 = 1;
							BigG.E.push_back(E);
							E.Node1 = 0;
							E.Node2 = 2;
							BigG.E.push_back(E);
							E.Node1 = 0;
							E.Node2 = 4;
							BigG.E.push_back(E);
							E.Color = olc::WHITE;
							E.Node1 = 1;
							E.Node2 = 3;
							BigG.E.push_back(E);
							
						}
						break;
					case 3:
						SubScene = 2;
						BigG.N[0].Color = olc::DARK_BLUE;
						BigG.E[0].Color = olc::WHITE;
						BigG.E[1].Color = olc::WHITE;
						BigG.E[2].Color = olc::WHITE;
						break;
					case 2:
						SubScene = 1;
						{
							Node N;
							N.Rad = 18.0f;
							N.Pos = { ScreenWidth() * 0.55f, ScreenHeight() * 0.55f };
							N.PotPos = N.Pos;
							N.Color = olc::DARK_RED;
							BigG.N.insert(BigG.N.begin() + 7, N);

							Edge E;
							E.Color = olc::Pixel(255, 100, 100);
							E.Node1 = 7;
							E.Node2 = 5;
							BigG.E.insert(BigG.E.begin(), E);
							E.Node1 = 7;
							E.Node2 = 4;
							BigG.E.insert(BigG.E.begin(), E);
							E.Node1 = 7;
							E.Node2 = 3;
							BigG.E.insert(BigG.E.begin(), E);
							E.Node1 = 7;
							E.Node2 = 2;
							BigG.E.insert(BigG.E.begin(), E);
							E.Node1 = 7;
							E.Node2 = 1;
							BigG.E.insert(BigG.E.begin(), E);
							E.Node1 = 7;
							E.Node2 = 0;
							BigG.E.insert(BigG.E.begin(), E);
						}
						break;
					case 1:
						SubScene = 0;
						SubText.Origin = "Let's see this process, with the graphs, from start to\n"
							"finish.\n";
						SubText.Done = false;
						SubText.Pcnt = 0.0f;
						BigG.N[7].Color = olc::DARK_BLUE;
						for (int i = 0; i < 6; i++) {
							BigG.E[i].Color = olc::WHITE;
						}
						break;
					
					case 0:
						ResetTheList();
						CurrScene--;
						TheList.push_back(SceneList[CurrScene]);
						break;
					default:
						break;
					}
				}
			}
		}


		return true;
	}
	//--------Scene 16
	bool SetupScene16F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.8f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);

		SceneFinished = false;

		SubText.Origin =
			"The Havel-Hakimi Theorem is very simple process (from a\n"
			"practical perspective), but shows how connected all kinds of\n"
			"graphs really are. Graph Theory is a very fascinating subject,\n"
			"and this theorem is just a tiny part."
			"\n\n\n\n\n\n\n"
			"I don't consider myself very good at teaching, as this app may\n"
			"or may not have proven. But I hope I can at least get someone\n"
			"a little more interested in this subject that I enjoy so much.\n"
			"\nThank you for your time.";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Final Thoughts:\n\n\n\n\n\n\nConclusion:", "", 0.0f, false, { ScreenWidth() * 0.075f, 150.0f } };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene16);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene16);
		RemoveFuncByName(SetupScene16);
		return true;
	}
	//--------Scene 17
	bool SetupScene17F(float f) {
		BigG.N.clear();
		BigG.E.clear();

		Node N;
		N.Rad = 15.0f;
		N.Pos = { ScreenWidth() * 0.7f, ScreenHeight() * 0.8f };
		N.PotPos = N.Pos;
		BigG.N.push_back(N);

		SceneFinished = false;

		SubText.Origin =
			"-Made with the OLC PixelGameEngine, in C++\n\n"
			"-All other programming done by me\n\n"
			"-Thanks to my Math professors, and 3Blue1Brown\n\n"
			"Press Next or the X to close the program.";
		SubText.Pcnt = 0.0f;
		SubText.Pos = { ScreenWidth() * 0.08f, 200.0f };
		SubText.Done = false;
		MainText = { "Credits:", "", 0.0f, false, { ScreenWidth() * 0.075f, 150.0f } };

		//Buttons
		NextB.Visible = true;
		PrevB.Visible = true;

		//Then remove
		AddFuncAfterName(FrameFunc(ProcessMouseScene2, &GraphTheory::ProcessMouseScene2F), SetupScene17);
		AddFuncAfterName(FrameFunc(ProcessScene2, &GraphTheory::ProcessScene2F), SetupScene17);
		RemoveFuncByName(SetupScene17);
		return true;
	}
	bool QuitF(float f) {
		olc_Terminate();
		return true;
	}
	//------------------------------------------------------------------------------And now the main stuff
public:
	bool OnUserCreate() override
	{
		Clear(olc::BLANK);
		int o = CreateLayer();
		EnableLayer(o, true);
		//--We should set the function list for main menu here, same with copied list
		FrameFunc BG = { DrawBackground, &GraphTheory::DrawBackgroundF };
		TheList.push_back(BG);
		BG = { SetupScene1, &GraphTheory::SetupScene1F };
		TheList.push_back(BG);
		TheCopiedList = TheList;
		return true;
	}


	bool OnUserUpdate(float fElapsedTime) override
	{
		if (!IsFocused()) { return true; }
		if (TheList.size() == 0) { std::cout << "\n\n!THE LIST IS EMPTY!"; return true; }
		for (int i = 0; i < TheCopiedList.size(); i++) {
			(this->*(TheCopiedList[i].second))(fElapsedTime);
		}
		//Then update copied list if necessary
		if (TheListChanged) {
			TheCopiedList = TheList;
			TheListChanged = false;
		}

		return true;
	}
};

int main()
{
	GraphTheory demo;
	if (demo.Construct(1280, 720, 1, 1, false, true))
		demo.Start();
	return 0;
}

/*Todo:
--Make slow color changing background, dont forget framerate DONE
--Set up fade in/fade out text DONE I GUESS
--Draw nodes without physics DONE
--Process nodes' physics DONE
--Do Draggable nodes DONE
--Setup scene logic DONE
--Make two scenes DONE
--Next and prev buttons DONE
--Scene switching DONE
--Make the thing I guess


*/

//--A bit of scene logic: All that we need to store for the SceneList is the Setup func for each thing, since that can add in the process and draw funcs.
//You might need to make a new func that 'resets' the TheList var to a simple state, before changing scenes