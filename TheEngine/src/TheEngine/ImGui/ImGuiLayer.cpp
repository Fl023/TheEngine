#include "tepch.h"
#include "ImGuiLayer.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "TheEngine/Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// #include "../../Platform/Windows/WindowsWindow.h"

namespace TheEngine {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		m_keyMap = {
		{GLFW_KEY_TAB, ImGuiKey_Tab}, {GLFW_KEY_LEFT, ImGuiKey_LeftArrow}, {GLFW_KEY_RIGHT, ImGuiKey_RightArrow},
		{GLFW_KEY_UP, ImGuiKey_UpArrow}, {GLFW_KEY_DOWN, ImGuiKey_DownArrow}, {GLFW_KEY_PAGE_UP, ImGuiKey_PageUp},
		{GLFW_KEY_PAGE_DOWN, ImGuiKey_PageDown}, {GLFW_KEY_HOME, ImGuiKey_Home}, {GLFW_KEY_END, ImGuiKey_End},
		{GLFW_KEY_INSERT, ImGuiKey_Insert}, {GLFW_KEY_DELETE, ImGuiKey_Delete}, {GLFW_KEY_BACKSPACE, ImGuiKey_Backspace},
		{GLFW_KEY_SPACE, ImGuiKey_Space}, {GLFW_KEY_ENTER, ImGuiKey_Enter}, {GLFW_KEY_ESCAPE, ImGuiKey_Escape},
		{GLFW_KEY_APOSTROPHE, ImGuiKey_Apostrophe}, {GLFW_KEY_COMMA, ImGuiKey_Comma}, {GLFW_KEY_MINUS, ImGuiKey_Minus},
		{GLFW_KEY_PERIOD, ImGuiKey_Period}, {GLFW_KEY_SLASH, ImGuiKey_Slash}, {GLFW_KEY_SEMICOLON, ImGuiKey_Semicolon},
		{GLFW_KEY_EQUAL, ImGuiKey_Equal}, {GLFW_KEY_LEFT_BRACKET, ImGuiKey_LeftBracket}, {GLFW_KEY_BACKSLASH, ImGuiKey_Backslash},
		{GLFW_KEY_RIGHT_BRACKET, ImGuiKey_RightBracket}, {GLFW_KEY_GRAVE_ACCENT, ImGuiKey_GraveAccent},
		{GLFW_KEY_CAPS_LOCK, ImGuiKey_CapsLock}, {GLFW_KEY_SCROLL_LOCK, ImGuiKey_ScrollLock}, {GLFW_KEY_NUM_LOCK, ImGuiKey_NumLock},
		{GLFW_KEY_PRINT_SCREEN, ImGuiKey_PrintScreen}, {GLFW_KEY_PAUSE, ImGuiKey_Pause}, {GLFW_KEY_KP_0, ImGuiKey_Keypad0},
		{GLFW_KEY_KP_1, ImGuiKey_Keypad1}, {GLFW_KEY_KP_2, ImGuiKey_Keypad2}, {GLFW_KEY_KP_3, ImGuiKey_Keypad3},
		{GLFW_KEY_KP_4, ImGuiKey_Keypad4}, {GLFW_KEY_KP_5, ImGuiKey_Keypad5}, {GLFW_KEY_KP_6, ImGuiKey_Keypad6},
		{GLFW_KEY_KP_7, ImGuiKey_Keypad7}, {GLFW_KEY_KP_8, ImGuiKey_Keypad8}, {GLFW_KEY_KP_9, ImGuiKey_Keypad9},
		{GLFW_KEY_KP_DECIMAL, ImGuiKey_KeypadDecimal}, {GLFW_KEY_KP_DIVIDE, ImGuiKey_KeypadDivide},
		{GLFW_KEY_KP_MULTIPLY, ImGuiKey_KeypadMultiply}, {GLFW_KEY_KP_SUBTRACT, ImGuiKey_KeypadSubtract},
		{GLFW_KEY_KP_ADD, ImGuiKey_KeypadAdd}, {GLFW_KEY_KP_ENTER, ImGuiKey_KeypadEnter}, {GLFW_KEY_KP_EQUAL, ImGuiKey_KeypadEqual},
		{GLFW_KEY_LEFT_SHIFT, ImGuiKey_LeftShift}, {GLFW_KEY_LEFT_CONTROL, ImGuiKey_LeftCtrl}, {GLFW_KEY_LEFT_ALT, ImGuiKey_LeftAlt},
		{GLFW_KEY_LEFT_SUPER, ImGuiKey_LeftSuper}, {GLFW_KEY_RIGHT_SHIFT, ImGuiKey_RightShift}, {GLFW_KEY_RIGHT_CONTROL, ImGuiKey_RightCtrl},
		{GLFW_KEY_RIGHT_ALT, ImGuiKey_RightAlt}, {GLFW_KEY_RIGHT_SUPER, ImGuiKey_RightSuper}, {GLFW_KEY_MENU, ImGuiKey_Menu},
		{GLFW_KEY_0, ImGuiKey_0}, {GLFW_KEY_1, ImGuiKey_1}, {GLFW_KEY_2, ImGuiKey_2}, {GLFW_KEY_3, ImGuiKey_3},
		{GLFW_KEY_4, ImGuiKey_4}, {GLFW_KEY_5, ImGuiKey_5}, {GLFW_KEY_6, ImGuiKey_6}, {GLFW_KEY_7, ImGuiKey_7},
		{GLFW_KEY_8, ImGuiKey_8}, {GLFW_KEY_9, ImGuiKey_9}, {GLFW_KEY_A, ImGuiKey_A}, {GLFW_KEY_B, ImGuiKey_B},
		{GLFW_KEY_C, ImGuiKey_C}, {GLFW_KEY_D, ImGuiKey_D}, {GLFW_KEY_E, ImGuiKey_E}, {GLFW_KEY_F, ImGuiKey_F},
		{GLFW_KEY_G, ImGuiKey_G}, {GLFW_KEY_H, ImGuiKey_H}, {GLFW_KEY_I, ImGuiKey_I}, {GLFW_KEY_J, ImGuiKey_J},
		{GLFW_KEY_K, ImGuiKey_K}, {GLFW_KEY_L, ImGuiKey_L}, {GLFW_KEY_M, ImGuiKey_M}, {GLFW_KEY_N, ImGuiKey_N},
		{GLFW_KEY_O, ImGuiKey_O}, {GLFW_KEY_P, ImGuiKey_P}, {GLFW_KEY_Q, ImGuiKey_Q}, {GLFW_KEY_R, ImGuiKey_R},
		{GLFW_KEY_S, ImGuiKey_S}, {GLFW_KEY_T, ImGuiKey_T}, {GLFW_KEY_U, ImGuiKey_U}, {GLFW_KEY_V, ImGuiKey_V},
		{GLFW_KEY_W, ImGuiKey_W}, {GLFW_KEY_X, ImGuiKey_X}, {GLFW_KEY_Y, ImGuiKey_Y}, {GLFW_KEY_Z, ImGuiKey_Z},
		{GLFW_KEY_F1, ImGuiKey_F1}, {GLFW_KEY_F2, ImGuiKey_F2}, {GLFW_KEY_F3, ImGuiKey_F3}, {GLFW_KEY_F4, ImGuiKey_F4},
		{GLFW_KEY_F5, ImGuiKey_F5}, {GLFW_KEY_F6, ImGuiKey_F6}, {GLFW_KEY_F7, ImGuiKey_F7}, {GLFW_KEY_F8, ImGuiKey_F8},
		{GLFW_KEY_F9, ImGuiKey_F9}, {GLFW_KEY_F10, ImGuiKey_F10}, {GLFW_KEY_F11, ImGuiKey_F11}, {GLFW_KEY_F12, ImGuiKey_F12},
		{GLFW_KEY_F13, ImGuiKey_F13}, {GLFW_KEY_F14, ImGuiKey_F14}, {GLFW_KEY_F15, ImGuiKey_F15}, {GLFW_KEY_F16, ImGuiKey_F16},
		{GLFW_KEY_F17, ImGuiKey_F17}, {GLFW_KEY_F18, ImGuiKey_F18}, {GLFW_KEY_F19, ImGuiKey_F19}, {GLFW_KEY_F20, ImGuiKey_F20},
		{GLFW_KEY_F21, ImGuiKey_F21}, {GLFW_KEY_F22, ImGuiKey_F22}, {GLFW_KEY_F23, ImGuiKey_F23}, {GLFW_KEY_F24, ImGuiKey_F24}
		};


		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(TE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));

	}

	ImGuiKey ImGuiLayer::MapGLFWKeyToImGuiKey(int keycode)
	{
		auto it = m_keyMap.find(keycode);
		return it != m_keyMap.end() ? it->second : ImGuiKey_None;
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();
		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey imguiKey = MapGLFWKeyToImGuiKey(e.GetKeyCode());
		if (imguiKey != ImGuiKey_None)
		{
			io.AddKeyEvent(imguiKey, true);
			io.KeyCtrl = io.KeysData[GLFW_KEY_LEFT_CONTROL].Down || io.KeysData[GLFW_KEY_RIGHT_CONTROL].Down;
			io.KeyShift = io.KeysData[GLFW_KEY_LEFT_SHIFT].Down || io.KeysData[GLFW_KEY_RIGHT_SHIFT].Down;
			io.KeyAlt = io.KeysData[GLFW_KEY_LEFT_ALT].Down || io.KeysData[GLFW_KEY_RIGHT_ALT].Down;
			io.KeySuper = io.KeysData[GLFW_KEY_LEFT_SUPER].Down || io.KeysData[GLFW_KEY_RIGHT_SUPER].Down;
			/*WindowsWindow& win = static_cast<WindowsWindow&>(Application::Get().GetWindow());
			GLFWwindow* window = static_cast<GLFWwindow*>(win.GetNativeWindow());
			io.AddKeyEvent(ImGuiMod_Ctrl, (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS));
			io.AddKeyEvent(ImGuiMod_Shift, (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS));
			io.AddKeyEvent(ImGuiMod_Alt, (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS));
			io.AddKeyEvent(ImGuiMod_Super, (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS));
*/

		}
		return false;
	}



	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey imguiKey = MapGLFWKeyToImGuiKey(e.GetKeyCode());
		if (imguiKey != ImGuiKey_None)
		{
			io.AddKeyEvent(imguiKey, false);
		}
		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(static_cast<unsigned int>(e.GetKeyCode()));
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}


}