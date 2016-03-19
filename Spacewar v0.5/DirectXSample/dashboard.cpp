// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// dashboard.cpp v1.0

#include "dashboard.h"

//=============================================================================
// Seven Segment constructor
//=============================================================================
SevenSegment::SevenSegment()
{
    digits = 1;
    number = 0;
}

//=============================================================================
// �Z�u���Z�O�����g�\���̏�����
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   digits = ����
//		   color = �����̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool SevenSegment::initialize(Graphics *graphics, TextureManager *textureM,
                   int left, int top, float scale, UINT digs, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SEGMENT_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
        if(digs < 1)
            digs = 1;
        digits = digs;

        decimal.initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE,
                           dashboardNS::TEXTURE_COLS, textureM);
        decimal.setCurrentFrame(dashboardNS::DECIMAL_FRAME);
        decimal.setColorFilter(color);
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// �Z�u���Z�O�����g�\���ɕ\�����鐔�l��ݒ�
//=============================================================================
void SevenSegment::set(double value)
{
    number = value;
}

//=============================================================================
// �Z�u���Z�O�����g�̐����u0�v�`�u9�v�Ɓu-�v��\��
//         A
//       -----
//     F|     |B
//      |  G  |
//       -----
//     E|     |C
//      |  D  |
//       -----  
//=============================================================================
void SevenSegment::drawDigit(char n, COLOR_ARGB color)
{
    float lowerY = spriteData.y + spriteData.height * spriteData.scale * 0.75f;
    float saveY = spriteData.y;

	// �Z�O�����gA
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(0);
        Image::draw(color);
    }
	// �Z�O�����gB
    if(n=='0' || n=='1' || n=='2' || n=='3' || n=='4' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
	// �Z�O�����gG
    if(n=='-' || n=='2' || n=='3' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// �Z�O�����gF�̏ꍇ
    if(n=='0' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(270);
        Image::draw(color);
    }

    spriteData.y = lowerY;  // �����̉������p��Y��ݒ�
	
	// �Z�O�����gE�̏ꍇ
    if(n=='0' || n=='2' || n=='6' || n=='8')
    {
        setDegrees(270);
        Image::draw(color);
    }
	// �Z�O�����gD�̏ꍇ
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// �Z�O�����gC�̏ꍇ
    if(n=='0' || n=='1' || n=='3' || n=='4' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
    spriteData.y = saveY;
}

//=============================================================================
// �����_��`��
//=============================================================================
void SevenSegment::drawDecimal(COLOR_ARGB color)
{
    float saveX = spriteData.x;
    float saveY = spriteData.y;

    setDegrees(0);
    spriteData.x -= spriteData.width * spriteData.scale * 0.25f;
    spriteData.y += spriteData.height * spriteData.scale * 0.80f;

    decimal.draw(spriteData, color);

    spriteData.x = saveX;
    spriteData.y = saveY;
}

//=============================================================================
// �Z�u���Z�O�����g�\����`��
// number�ϐ��ɂ́A�\�����镂�������_���l���i�[����Ă���
//=============================================================================
void SevenSegment::draw(COLOR_ARGB color)
{
    float saveX = spriteData.x;
    float saveY = spriteData.y;
    char ch;

    if(digits == 0)
        return;

	// ���l�𕶎���ɕϊ�
    std::stringstream strstm;
    strstm.precision(digits);
    strstm.flags(std::ios_base::fixed);
    strstm << number;           // ���l�𕶎���ɕϊ�
    std::string str = strstm.str();

    UINT digitN = str.length(); // ������̌������擾
	// ������̌������A�Z�u���Z�O�����g�\���̌������������ꍇ
    if (digitN > digits)
        digitN = digits;

	// �ł����Ɉʒu���錅��x�ʒu
    spriteData.x += spriteData.width * spriteData.scale * 1.2f * (digits-digitN);

    UINT n=0;
    ch = str.at(n++);           // number�̍ŏ��̌����擾
    while(digitN > 0)           // �\�����錅���c���Ă���Ԃ͌p��
    {
        if(ch == '.')           // �����_�̏ꍇ
            drawDecimal(color);
        else
        {
            drawDigit(ch, color);   // ������\��
			// ���̌��̉�ʏ�̈ʒu
            spriteData.x += spriteData.width * spriteData.scale * 1.2f;
        }
        if(n < str.length())
            ch = str.at(n++);   // ���̌����擾
        else
            ch = '0';           // �����񂪁u.�v�ŏI����Ă���ꍇ�A����0�Ŗ��߂�
        if(ch != '.')           // �����_�łȂ��ꍇ
            digitN--;           // ����������
    }
    spriteData.x = saveX;
}

//=============================================================================
// Bar��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   color = �o�[�̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool Bar::initialize(Graphics *graphics, TextureManager *textureM, int left,
                     int top, float scale, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
    }
    catch(...)
    {
        return false;
    }
    // OK��߂�
    return true;
}

//=============================================================================
// �o�[�̃T�C�Y��ݒ�
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    spriteData.rect.right = spriteData.rect.left + (LONG)(spriteData.width*p/100);
}

//=============================================================================
// DialGauge��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   type = �~�Ղ̎��
//		   zeroAngle = �~�Ղ̃[���̈ʒu
//		   dialColor = �~�Ղ̐F
//		   pointColor = �����̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool DialGauge::initialize(Graphics *graphics, TextureManager *textureM, int left,
                    int top, float scale, dashboardNS::DialType type,
                    float zeroAngle, COLOR_ARGB dialColor, COLOR_ARGB pointerColor)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        dialType = type;
        switch(type)
        {
        case dashboardNS::DIAL360:
            setCurrentFrame(dashboardNS::DIAL360_FRAME); break;
        case dashboardNS::DIAL270:
            setCurrentFrame(dashboardNS::DIAL270_FRAME); break;
        case dashboardNS::DIAL180:
            setCurrentFrame(dashboardNS::DIAL180_FRAME); break;
        }
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = dialColor;
        setDegrees(zeroAngle);

        pointer.initialize(graphics, dashboardNS::IMAGE_SIZE,
                           dashboardNS::IMAGE_SIZE, dashboardNS::TEXTURE_COLS,
                           textureM);
        pointer.setCurrentFrame(dashboardNS::POINTER_FRAME);
        pointer.setColorFilter(pointerColor);
    }
    catch(...)
    {
        return false;
    }
    // OK��߂�
    return true;
}

//=============================================================================
// DialGauge�̎w�j��ݒ�
//=============================================================================
void DialGauge::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    switch(dialType)
    {
    case dashboardNS::DIAL360:
        pointer.setDegrees(p * 360/100.0f + getDegrees()); break;
    case dashboardNS::DIAL270:
        pointer.setDegrees(p * 270/100.0f + getDegrees() - 135); break;
    case dashboardNS::DIAL180:
        pointer.setDegrees(p * 180/100.0f + getDegrees() - 90); break;
    }
}

//=============================================================================
// DialGauge�Ǝw�j��`��
//=============================================================================
void DialGauge::draw(COLOR_ARGB color)
{
    Image::draw(color);
    float dialAngle = spriteData.angle;
    spriteData.angle = pointer.getRadians();
    pointer.draw(spriteData, graphicsNS::FILTER);
    spriteData.angle = dialAngle;
}

//=============================================================================
// Light constructor
//=============================================================================
Light::Light()
{
    flashDelay = 0;
    flashTimer = 0;
    onColor = graphicsNS::RED;
    offColor = SETCOLOR_ARGB(255,32,0,0);   // dark RED
}

//=============================================================================
// Light��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   flashRate = �_��/����/�_�ł̑���
//		   �i<0�̏ꍇ�͓_���A=0�̏ꍇ�͏����A>0�̏ꍇ��1�b������̓_�ł̎��ԁj
//		   colorOn = �_�����̃��C�g�̐F
//		   colorOff = �������̃��C�g�̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool Light::initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                float scale, float flashRate, COLOR_ARGB colorOn, COLOR_ARGB colorOff)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::LIGHT_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = colorOff;
        offColor = colorOff;
        onColor = colorOn;
        if(flashRate > 0)
            flashDelay = flashRate/2.0f;    // �f���[�e�B�[���50%
        else
            flashDelay = flashRate;
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// flashRate��ݒ�F<0�̏ꍇ�͓_���A =0�̏ꍇ�͏����A>0�̏ꍇ�͓_�ł̎���
//=============================================================================
void Light::set(float flashRate)
{
    if(flashRate > 0)
        flashDelay = flashRate/2.0f;    // �f���[�e�B�[���50%
    else
        flashDelay = flashRate;
}

//=============================================================================
// ���C�g���X�V
//=============================================================================
void Light::update(float frameTime)
{
    if(flashDelay > 0)  // >0�̏ꍇ�A���C�g��_�ł�����
    {
        flashTimer += frameTime;
        if(flashTimer >= flashDelay)
        {
            flashTimer -= flashDelay;
            if(colorFilter == onColor)
                colorFilter = offColor;
            else
                colorFilter = onColor;
        }
    }
}


//=============================================================================
//  ToggleSwitch Constructor
//=============================================================================
ToggleSwitch::ToggleSwitch()
{
    switchOn = false;
    mouseClick = true;
}

//=============================================================================
// �؂�ւ��X�C�b�`�iToggleSwitch�j��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   *in = Input�I�u�W�F�N�g�ւ̃|�C���^
//         hwnd = �E�B���h�E�ւ̃n���h��
//		   left�Atop = ��ʈʒu
//		   scale = �{���i�Y�[���j
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool ToggleSwitch::initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND h,
                    int left, int top, float scale)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // �E�B���h�E�ւ̃n���h��
        input = in;                     // ���̓I�u�W�F�N�g
        switchRect.left = left;
        switchRect.top  = top;
        switchRect.right = (long)(left + (dashboardNS::SWITCH_WIDTH * spriteData.scale));
        switchRect.bottom = (long)(top + (spriteData.height * spriteData.scale));
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// �X�C�b�`��ł̃}�E�X�N���b�N���`�F�b�N
//=============================================================================
void ToggleSwitch::update(float frameTime)
{
    if (!initialized || !visible)
        return;

    // �E�B���h�E���T�C�Y�ύX����Ă���ꍇ���l�����āA��ʂ̔䗦���v�Z
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButton())       // �}�E�X�̍��{�^���̏ꍇ
    {
		// �}�E�X�N���b�N���X�C�b�`�̓����Ŕ��������ꍇ
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
        {
            if(mouseClick)
            {
                mouseClick = false;
                switchOn = !switchOn;       // �X�C�b�`��؂�ւ���
                if(switchOn)
                    setCurrentFrame(dashboardNS::SWITCH_ON_FRAME);
                else
                    setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
            }
        }
    } else
        mouseClick = true;
}

//=============================================================================
// Constructor
//=============================================================================
PushButton::PushButton()
{
    switchOn = false;
    mouseClick = true;
    momentary = true;
}

//=============================================================================
// �v�b�V���{�^���iPushButton�j��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   *in = Input�I�u�W�F�N�g�ւ̃|�C���^
//         hwnd = �E�B���h�E�ւ̃n���h��
//		   left�Atop = ��ʈʒu
//		   scale = �{���i�Y�[���j
//		   type = true�̏ꍇ�̓��[�����^���[�Afalse�̏ꍇ�̓I���^�l�[�g
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool PushButton::initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND h,
                    int left, int top, float scale, bool type)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // �E�B���h�E�ւ̃n���h��
        input = in;                     // ���̓I�u�W�F�N�g
        switchRect.left = left;
        switchRect.top  = top;
        switchRect.right = (long)(left + spriteData.width * spriteData.scale);
        switchRect.bottom = (long)(top + spriteData.height * spriteData.scale);
        momentary = type;
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// �v�b�V���{�^����ł̃}�E�X�N���b�N���`�F�b�N
//=============================================================================
void PushButton::update(float frameTime)
{
    if (!initialized || !visible)
        return;

	// �E�B���h�E���T�C�Y�ύX����Ă���ꍇ���l�����āA��ʂ̔䗦���v�Z
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButton())           // �}�E�X�̍��{�^���̏ꍇ
    {
        // if mouse clicked inside switch
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
        {
            if(mouseClick)
            {
                mouseClick = false;
                if(momentary)               // ���[�����^���[�X�C�b�`�̏ꍇ
                    switchOn = true;
                else
                    switchOn = !switchOn;   // �X�C�b�`��؂�ւ���
                if(switchOn)
                    setCurrentFrame(dashboardNS::BUTTON_DOWN_FRAME);
                else
                    setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
            }
        }
    } 
    else
    {
        mouseClick = true;
        if(momentary)
        {
            switchOn = false;
            setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        }
    }
}

//=============================================================================
//  Constructor
//=============================================================================
BarGraph::BarGraph()
{
    maxBars = 1;
    barsOn = 0;
}

//=============================================================================
// BarGraph��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//		   scale = �{���i�Y�[���j
//		   bars = ���[�^�[���̃o�[�̖{��
//		   color = �o�[�̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool BarGraph::initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                    float scale, UINT bars, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_GRAPH_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
        if(maxBars > 0)
            maxBars = bars;
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// barsOn�̒l���A�\������o�[�̖{���ɐݒ�
// �p�����[�^p�͊����i0�`100�j
//=============================================================================
void BarGraph::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    barsOn = (int)(p * 0.01f * maxBars + 0.5);
}

//=============================================================================
// BarGraph��`��
// barsOn�́A�\������o�[�̖{�����i�[
//=============================================================================
void BarGraph::draw(COLOR_ARGB color)
{
    float saveX = spriteData.x;

    for (int i=barsOn; i > 0; i--)
    {
		// ���̃o�[�̉�ʏ�̈ʒu
        spriteData.x += dashboardNS::BAR_GRAPH_WIDTH * spriteData.scale;
        Image::draw(color);
    }
    spriteData.x = saveX;
}

