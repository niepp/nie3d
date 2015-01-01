#pragma once

#if PLATFORM == PLATFORM_WIN32

class CameraCtrl
{
public:
	
	CameraCtrl();

	void InitCamera(const Vector3 &pos,
				const Vector3 &forward,
				const Vector3 &up);

	void MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	const Quaternion& GetCameraRot() const { return m_rot; }
	const Vector3& GetCameraPos() const { return m_pos; }

protected:
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnRButtonDown(int x, int y);
	void OnRButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseWheel(short zDelta);

	void RotateByAxis(const Vector3 &axis, float angle);
	void MouseRotateCamera(int x, int y);

	Vector3 m_pos;
	Quaternion m_rot;
	bool m_lbtn_down;
	bool m_rbtn_down;
	Float m_mouse_pos_down_x, m_mouse_pos_down_y;

	Vector3 m_cam_pos_down;
	Quaternion m_cam_rot_down;
	Vector3 m_cam_to_target_down;
	Vector3 m_cam_to_target;

};

#endif
