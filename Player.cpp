#include "Player.h"

// ���ӿ��� ����ϰ� ����ϴ� ��ǥ���� ��ȯ �۾����� ���� �ִ���?
// ũ�� (���� ��ȯ), ȸ�� (���� ��ȯ), �̵� (���� ��ȯ�� �ƴ�)
// ���� ��ȯ - ������ �ٲ��� �ʴ� ��ȯ
// ���� ��ȯ�� �����ϴٸ� ��ķ� ��ȯ�� ����������, ��ķ� ��ȯ�� ���������ٸ� ������ ��ȯ�� �� ��Ŀ� �����Ͽ� ��ȯ�� �����ϴ�.
// ���� �ӵ��� �޸� ���� ���鿡�� �̵��� ì��

void Player::Initialize()
{
	speed = 2.f;
	moveDisplacement = D3DXVECTOR3(0, 0, 0);
	info.pos = D3DXVECTOR3(WIN_SIZE_X / 2.f, WIN_SIZE_Y / 2.f, 0);
	info.dir = D3DXVECTOR3(0, 0, 0);
	info.look = D3DXVECTOR3(1.f, 0, 0);

	bodyAngle = 0;
	gunBarrelAngle = 0;

	// ��, ����
	originPoint[0].x = info.pos.x - 50.f;
	originPoint[0].y = info.pos.y - 50.f;

	// ��, ����
	originPoint[1].x = info.pos.x + 50.f;
	originPoint[1].y = info.pos.y - 50.f;

	// ��, �Ʒ���
	originPoint[2].x = info.pos.x + 50.f;
	originPoint[2].y = info.pos.y + 50.f;

	// ��, �Ʒ���
	originPoint[3].x = info.pos.x - 50.f;
	originPoint[3].y = info.pos.y + 50.f;

	gunBarrelDistance = 100.f;
}

void Player::Update()
{
	if (GetAsyncKeyState('A'))
	{
		bodyAngle -= D3DXToRadian(5.f);
	}

	if (GetAsyncKeyState('D'))
	{
		bodyAngle += D3DXToRadian(5.f);
	}

	if (GetAsyncKeyState('W'))
	{
		D3DXVECTOR3 temp;
		temp.x = info.look.x * cosf(bodyAngle) - info.look.y * sinf(bodyAngle);
		temp.y = info.look.x * sinf(bodyAngle) + info.look.y * cosf(bodyAngle);

		moveDisplacement += temp;
	}

	// Ŭ���̾�Ʈ ���콺 ��ǥ �ޱ�
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	// ����Ʈ ����ü�� �� ���콺 ��ǥ�� ���͵���� ������
	// ���ϰ� �ϱ� ����, ���콺 ��ǥ Ÿ���� ���ͷ� ��ȯ (����ü ������ ���)
	D3DXVECTOR3 mousePos(mouse.x, mouse.y, 0);

	// �÷��̾ ���콺�� �ٶ󺸴� ���� ���͸� ���Ѵ�.
	info.dir = mousePos - info.pos;
	// �� ����(x, y, z)�� ������ ũ���� ������ ��
	float length = sqrt(info.dir.x * info.dir.x + info.dir.y * info.dir.y);
	info.dir.x /= length;
	info.dir.y /= length;

	// look ���ʹ� ���� ���� (���� 2d x��)���� ���� ���� ũ�Ⱑ 1�� ����
	// �ش� ������ ���а� ĳ���Ͱ� �ٶ󺸴� ���� ���г��� ���� ��,
	// ������� ������ ��, ��������� x�� ���� ����
	// �� ��, dir�� ũ�Ⱑ 1�� ���� �����̹Ƿ� ���� ���� �����Ѵٰ� �� �� ����
	// ���� ���� �����Ѵٸ�, x�� ���� cos ��Ÿ�� �ǹ���
	// cos ��Ÿ�� �غ� / �����̰�, �ش� ���� ���Լ��� ���� ��, ��������� ���ϴ� ��Ÿ ���� ���� �� ����
	gunBarrelAngle = acosf(info.dir.x * info.look.x + info.dir.y * info.look.y);

	// ��ũ �ڻ����� 180������ �ۿ� ǥ���� ���ϴ� ���� ó��
	if (mousePos.y > info.pos.y)
	{
		gunBarrelAngle = 2 * D3DX_PI - gunBarrelAngle;
	}

	// ���� ��ġ ����
	gunBarrelPosX = cosf(gunBarrelAngle) * gunBarrelDistance + info.pos.x + moveDisplacement.x;
	gunBarrelPosY = -sinf(gunBarrelAngle) * gunBarrelDistance + info.pos.y + moveDisplacement.y;

	// ����Ʈ ����ü ���� ��� ��Ƶ� ����
	POINT temp;
	// ĳ������ �� ���� ���� �����̴� ����
	for (int i = 0; i < 4; ++i)
	{
		// ���� ���� ��������
		calPoint[i] = originPoint[i];

		// �÷��̾��� ��ġ�� ������ ���� ��ǥ�� ���ϱ�
		calPoint[i].x -= info.pos.x;
		calPoint[i].y -= info.pos.y;

		// ���� ��ǥ�� �̿��� ȸ��
		temp = calPoint[i];
		// �ﰢ�Լ��� ��������
		// cosf(x�� ȸ���� ��ǥ) = cosf * cosf - sinf * sinf
		// sinf(y�� ȸ���� ��ǥ) = cosf * sinf + sinf * cosf
		// -> ������ �İ� ������ ����
		calPoint[i].x = temp.x * cosf(bodyAngle) + (-temp.y * sinf(bodyAngle));
		calPoint[i].y = temp.x * sinf(bodyAngle) + temp.y * cosf(bodyAngle);

		// ���� ȸ���� �������Ƿ�, �ٽ� �÷��̾��� ��ġ�� ���� ���������� ȸ���� ��ġ�� ��Ÿ��
		//���� ��ǥ�� ���� ��ǥ�� �ٲ�
		calPoint[i].x += info.pos.x;
		calPoint[i].y += info.pos.y;

		// ���� ��ǥ�� ��ġ���� �̵����� ���Ѵ�.
		calPoint[i].x += moveDisplacement.x;
		calPoint[i].y += moveDisplacement.y;
	}
}

void Player::Render(HDC hdc)
{
	// ȭ�� ����� �뵵
	Rectangle(hdc, 0, 0, WIN_SIZE_X, WIN_SIZE_Y);

	PrintInfo(hdc);

	// �÷��̾� �׸���
	// ������
	MoveToEx(hdc, calPoint[0].x, calPoint[0].y, NULL);
	// �� �׸���
	LineTo(hdc, calPoint[1].x, calPoint[1].y);
	LineTo(hdc, calPoint[2].x, calPoint[2].y);
	LineTo(hdc, calPoint[3].x, calPoint[3].y);
	LineTo(hdc, calPoint[0].x, calPoint[0].y);

	// ���� �׸���
	MoveToEx(hdc, info.pos.x + moveDisplacement.x, info.pos.y + moveDisplacement.y, NULL);
	LineTo(hdc, gunBarrelPosX, gunBarrelPosY);
}

void Player::Release()
{
}

void Player::PrintInfo(HDC hdc)
{
	// ���콺 ��ǥ
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);
	PrintVector(hdc, L"���콺", D3DXVECTOR2(mouse.x, mouse.y), D3DXVECTOR2(10, 10));

	// ���⺤�� (�÷��̾ ���콺���� �ٶ󺸴� ����) (�������α׷��� ��ǥ�� �Ʒ��� ������ ���� ��ǥ)
	PrintVector(hdc, L"���� ����", D3DXVECTOR2(info.dir.x, info.dir.y), D3DXVECTOR2(10, 30));

	// ��ü�� ȸ�� ���� (�÷��̾ ���콺���� �ٶ󺸴� ����)
	PrintVector(hdc, L"��ü ȸ�� ����", D3DXVECTOR2(bodyAngle, D3DXToDegree(bodyAngle)), D3DXVECTOR2(10, 50));

	// ������ ���� (����)
	PrintVector(hdc, L"���Ű� (r, d)", D3DXVECTOR2(gunBarrelAngle, D3DXToDegree(gunBarrelAngle)), D3DXVECTOR2(10, 70));

	// ���� ��ġ
	PrintVector(hdc, L"���� ��ġ", D3DXVECTOR2(gunBarrelPosX, gunBarrelPosY), D3DXVECTOR2(10, 90));

	// ��ü�� ȸ���� ����� �� ���� ��ġ
	for (int i = 0; i < 4; ++i)
	{
		wchar_t str[32];
		swprintf_s(str, L"CalPoint %d", i);
		PrintVector(hdc, str, D3DXVECTOR2(calPoint[i].x, calPoint[i].y), D3DXVECTOR2(10, 110 + i * 20));
	}
}

void Player::PrintVector(HDC hdc, const wchar_t* text, D3DXVECTOR2 vec2, D3DXVECTOR2 textPt)
{
	wchar_t temp[128];

	swprintf_s(temp, L"%s : %.2f, %.2f", text, vec2.x, vec2.y);

	TextOut(hdc, textPt.x, textPt.y, temp, wcslen(temp));
}

Player::Player()
{
}

Player::~Player()
{
}
