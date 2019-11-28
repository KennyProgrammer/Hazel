#include "hzpch.h"
#include "Hazel/Renderer/OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		float dTranslationSpeed = m_CameraTranslationSpeed * ts;
		float dRotationRot      = m_CameraRotationSpeed * ts;
		float theta = m_CameraRotation;
		float dX = cos(glm::radians(theta));
		float dY = sin(glm::radians(theta));
		
		if (Input::IsKeyPressed(HZ_KEY_A)) decreasePosition(dX * m_CameraTranslationSpeed, dY * m_CameraTranslationSpeed);
		if (Input::IsKeyPressed(HZ_KEY_D)) increasePosition(dX * m_CameraTranslationSpeed, dY * m_CameraTranslationSpeed);
		if (Input::IsKeyPressed(HZ_KEY_W)) increasePosition(-dY * m_CameraTranslationSpeed, dX * m_CameraTranslationSpeed);
		if (Input::IsKeyPressed(HZ_KEY_S)) decreasePosition(-dY * m_CameraTranslationSpeed, dX * m_CameraTranslationSpeed);

		if (rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q)) decreaseRotation(dRotationRot);
			if (Input::IsKeyPressed(HZ_KEY_E)) increaseRotation(dRotationRot);

			if (m_CameraRotation >= 360)
				m_CameraRotation = 0;
			else if (m_CameraRotation <= -360)
				m_CameraRotation = 0;
			
			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
