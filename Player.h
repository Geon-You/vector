#pragma once
#include "Object.h"

class Player :
    public Object
{
private:
    POINT originPoint[4]; // 원점(4개의 꼭짓점)의 정보
    POINT calPoint[4]; // 원점의 정보를 가지고 연산한 점들의 정보를 저장하는 배열

    int speed;
    // 탱크 몸체 회전 시, 사용할 각
    float bodyAngle;
    // 포신의 길이
    float gunBarrelDistance;
    // 포신 회전 시, 사용할 각
    float gunBarrelAngle;
    // 포신 위치
    float gunBarrelPosX;
    float gunBarrelPosY;
    // 키 입력으로 입력받은 이동량을 가질 벡터
    D3DXVECTOR3 moveDisplacement;

public:
    // Object을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC hdc) override;
    void Release() override;

public:
    void PrintInfo(HDC hdc);
    void PrintVector(HDC hdc, const wchar_t* text, D3DXVECTOR2 vec2, D3DXVECTOR2 textPt);

public:
    Player();
    virtual ~Player();
};

