#if PLATFORM == PLATFORM_WIN32

#include <windows.h>

#include "core/core.h"
#include "engine/engine.h"
#include "cameractrl.h"

CameraCtrl::CameraCtrl() : m_lbtn_down(false)
, m_rbtn_down(false)
, m_pos(0, 0, 0)
, m_rot(0, 0, 0, 1)
, m_cam_rot_down(0, 0, 0, 1)
, m_cam_pos_down(0, 0, 0)
, m_cam_to_target_down(0, 0, 0)
, m_cam_to_target(0, 0, 0)
{
}

void CameraCtrl::InitCamera(const Vector3 &pos, const Vector3 &forward, const Vector3 &up)
{
	m_pos = pos;
	Matrix mat;
	OrientToMatrix(forward, up, mat); 
	MatrixToQuaternion(mat, m_rot);
	m_cam_to_target = -m_pos;
}

void CameraCtrl::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(HIWORD(wParam));
		break;
	default:
		break;
	}
}

void CameraCtrl::OnLButtonDown(int x, int y)
{
	m_lbtn_down = true;
	m_mouse_pos_down_x = (float)x;
	m_mouse_pos_down_y = (float)y;
	m_cam_pos_down = m_pos;
}

void CameraCtrl::OnLButtonUp(int x, int y)
{
	m_lbtn_down = false;
}

void CameraCtrl::OnRButtonDown(int x, int y)
{
	m_rbtn_down = true;
	m_mouse_pos_down_x = (float)x;
	m_mouse_pos_down_y = (float)y;
	m_cam_to_target_down = -m_pos;
	m_cam_rot_down = m_rot;
}

void CameraCtrl::OnRButtonUp(int x, int y)
{
	m_rbtn_down = false;
}

void CameraCtrl::OnMouseMove(int x, int y)
{
	if (m_rbtn_down)
	{
		MouseRotateCamera(x, y);
	}

	if (m_lbtn_down)
	{
		Vector3 p0(m_mouse_pos_down_x, m_mouse_pos_down_y, 0.0f);
		Vector3 p1((float)x, (float)y, 0.0f);
		p1 -= p0;
		if (p1.Length() < 0.0001f) {
			return;
		}

		Vector3 right, up;
		m_rot.GetRight(right);
		right.Normalize();
		m_rot.GetUp(up);
		up.Normalize();

		m_pos = m_cam_pos_down + (right * p1.x + up * p1.y) * 0.02f;
		m_cam_to_target = -m_pos;
	}
}

void CameraCtrl::OnMouseWheel(short zDelta)
{
	if (zDelta != 0)
	{
		m_cam_to_target *= 1.0f - 0.0016f * zDelta;
		m_pos = -m_cam_to_target;
		m_cam_to_target = -m_pos;
	}
}

void CameraCtrl::MouseRotateCamera(int x, int y)
{
	Vector3 p0(m_mouse_pos_down_x, m_mouse_pos_down_y, 0.0f);
	Vector3 p1((float)x, (float)y, 0.0f);
	p1 -= p0;
	if (p1.Length() < 0.0001f) {
		return;
	}

	m_rot = m_cam_rot_down;
	m_pos = -m_cam_to_target_down;

	Vector3 right;
	m_rot.GetRight(right);
	right.Normalize();
	
	RotateByAxis(right, p1.y * 0.012f);
	right.Set(0.0f, 0.0f, 1.0f);
	RotateByAxis(right, -p1.x * 0.012f);

	m_cam_to_target = -m_pos;

}

void CameraCtrl::RotateByAxis(const Vector3 &axis, float angle)
{
	Vector3 dir(axis);
	dir.Normalize();
	Quaternion quat(dir, angle);
	m_rot = quat * m_rot;
	quat.RotateVector(m_pos, m_pos);
}

#endif