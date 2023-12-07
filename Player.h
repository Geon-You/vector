#pragma once
#include "Object.h"

class Player :
    public Object
{
private:
    POINT originPoint[4]; // ����(4���� ������)�� ����
    POINT calPoint[4]; // ������ ������ ������ ������ ������ ������ �����ϴ� �迭

    int speed;
    // ��ũ ��ü ȸ�� ��, ����� ��
    float bodyAngle;
    // ������ ����
    float gunBarrelDistance;
    // ���� ȸ�� ��, ����� ��
    float gunBarrelAngle;
    // ���� ��ġ
    float gunBarrelPosX;
    float gunBarrelPosY;
    // Ű �Է����� �Է¹��� �̵����� ���� ����
    D3DXVECTOR3 moveDisplacement;

public:
    // Object��(��) ���� ��ӵ�
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

