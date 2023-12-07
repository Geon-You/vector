#include "Player.h"

// 게임에서 빈번하게 사용하는 대표적인 변환 작업들이 뭐가 있는지?
// 크기 (선형 변환), 회전 (선형 변환), 이동 (선형 변환이 아님)
// 선형 변환 - 원점이 바뀌지 않는 변환
// 선형 변환이 가능하다면 행렬로 변환이 가능해지고, 행렬로 변환이 가능해진다면 복수의 변환을 그 행렬에 누적하여 변환이 가능하다.
// 연산 속도와 메모리 유지 측면에서 이득을 챙김

void Player::Initialize()
{
	speed = 2.f;
	moveDisplacement = D3DXVECTOR3(0, 0, 0);
	info.pos = D3DXVECTOR3(WIN_SIZE_X / 2.f, WIN_SIZE_Y / 2.f, 0);
	info.dir = D3DXVECTOR3(0, 0, 0);
	info.look = D3DXVECTOR3(1.f, 0, 0);

	bodyAngle = 0;
	gunBarrelAngle = 0;

	// 왼, 위점
	originPoint[0].x = info.pos.x - 50.f;
	originPoint[0].y = info.pos.y - 50.f;

	// 오, 위점
	originPoint[1].x = info.pos.x + 50.f;
	originPoint[1].y = info.pos.y - 50.f;

	// 오, 아래점
	originPoint[2].x = info.pos.x + 50.f;
	originPoint[2].y = info.pos.y + 50.f;

	// 왼, 아래점
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

	// 클라이언트 마우스 좌표 받기
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	// 포인트 구조체로 된 마우스 좌표를 벡터들과의 연산을
	// 편하게 하기 위해, 마우스 좌표 타입을 벡터로 변환 (구조체 생성자 사용)
	D3DXVECTOR3 mousePos(mouse.x, mouse.y, 0);

	// 플레이어가 마우스를 바라보는 방향 벡터를 구한다.
	info.dir = mousePos - info.pos;
	// 각 성분(x, y, z)를 벡터의 크리고 나누면 됨
	float length = sqrt(info.dir.x * info.dir.x + info.dir.y * info.dir.y);
	info.dir.x /= length;
	info.dir.y /= length;

	// look 벡터는 기저 방향 (현재 2d x축)으로 값만 갖는 크기가 1인 벡터
	// 해당 벡터의 성분과 캐릭터가 바라보는 방향 성분끼리 곱한 후,
	// 결과들을 더했을 때, 결과적으로 x의 값만 남음
	// 이 때, dir은 크기가 1인 단위 벡터이므로 단위 원에 존재한다고 볼 수 있음
	// 단위 원에 존재한다면, x의 값은 cos 세타를 의미함
	// cos 세타는 밑변 / 빗변이고, 해당 값을 역함수에 넣을 시, 결과적으로 원하는 세타 값을 구할 수 있음
	gunBarrelAngle = acosf(info.dir.x * info.look.x + info.dir.y * info.look.y);

	// 아크 코사인은 180도까지 밖에 표현을 못하니 예외 처리
	if (mousePos.y > info.pos.y)
	{
		gunBarrelAngle = 2 * D3DX_PI - gunBarrelAngle;
	}

	// 포신 위치 설정
	gunBarrelPosX = cosf(gunBarrelAngle) * gunBarrelDistance + info.pos.x + moveDisplacement.x;
	gunBarrelPosY = -sinf(gunBarrelAngle) * gunBarrelDistance + info.pos.y + moveDisplacement.y;

	// 포인트 구조체 값을 잠시 담아둘 변수
	POINT temp;
	// 캐릭터의 네 개의 점을 움직이는 연산
	for (int i = 0; i < 4; ++i)
	{
		// 원점 정보 가져오기
		calPoint[i] = originPoint[i];

		// 플레이어의 위치를 제거한 로컬 좌표를 구하기
		calPoint[i].x -= info.pos.x;
		calPoint[i].y -= info.pos.y;

		// 로컬 좌표를 이용한 회전
		temp = calPoint[i];
		// 삼각함수의 덧셈정리
		// cosf(x를 회전한 좌표) = cosf * cosf - sinf * sinf
		// sinf(y를 회전한 좌표) = cosf * sinf + sinf * cosf
		// -> 내적의 식과 동일한 형태
		calPoint[i].x = temp.x * cosf(bodyAngle) + (-temp.y * sinf(bodyAngle));
		calPoint[i].y = temp.x * sinf(bodyAngle) + temp.y * cosf(bodyAngle);

		// 로컬 회전이 끝났으므로, 다시 플레이어의 위치를 더해 최종적으로 회전된 위치를 나타냄
		//로컬 좌표를 월드 좌표로 바꿈
		calPoint[i].x += info.pos.x;
		calPoint[i].y += info.pos.y;

		// 월드 좌표의 위치에서 이동량을 더한다.
		calPoint[i].x += moveDisplacement.x;
		calPoint[i].y += moveDisplacement.y;
	}
}

void Player::Render(HDC hdc)
{
	// 화면 지우기 용도
	Rectangle(hdc, 0, 0, WIN_SIZE_X, WIN_SIZE_Y);

	PrintInfo(hdc);

	// 플레이어 그리기
	// 시작점
	MoveToEx(hdc, calPoint[0].x, calPoint[0].y, NULL);
	// 선 그리기
	LineTo(hdc, calPoint[1].x, calPoint[1].y);
	LineTo(hdc, calPoint[2].x, calPoint[2].y);
	LineTo(hdc, calPoint[3].x, calPoint[3].y);
	LineTo(hdc, calPoint[0].x, calPoint[0].y);

	// 포신 그리기
	MoveToEx(hdc, info.pos.x + moveDisplacement.x, info.pos.y + moveDisplacement.y, NULL);
	LineTo(hdc, gunBarrelPosX, gunBarrelPosY);
}

void Player::Release()
{
}

void Player::PrintInfo(HDC hdc)
{
	// 마우스 좌표
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);
	PrintVector(hdc, L"마우스", D3DXVECTOR2(mouse.x, mouse.y), D3DXVECTOR2(10, 10));

	// 방향벡터 (플레이어가 마우스쪽을 바라보는 방향) (응용프로그램상 좌표는 아래로 갈수록 양의 좌표)
	PrintVector(hdc, L"방향 벡터", D3DXVECTOR2(info.dir.x, info.dir.y), D3DXVECTOR2(10, 30));

	// 몸체의 회전 각도 (플레이어가 마우스쪽을 바라보는 방향)
	PrintVector(hdc, L"몸체 회전 각도", D3DXVECTOR2(bodyAngle, D3DXToDegree(bodyAngle)), D3DXVECTOR2(10, 50));

	// 포신의 각도 (라디안)
	PrintVector(hdc, L"포신각 (r, d)", D3DXVECTOR2(gunBarrelAngle, D3DXToDegree(gunBarrelAngle)), D3DXVECTOR2(10, 70));

	// 포신 위치
	PrintVector(hdc, L"포신 위치", D3DXVECTOR2(gunBarrelPosX, gunBarrelPosY), D3DXVECTOR2(10, 90));

	// 몸체의 회전이 적용된 네 점의 위치
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
