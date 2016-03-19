// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.cpp v1.0
// Sprite based font.

#include "text.h"

//=============================================================================
// default constructor
//=============================================================================
Text::Text() : Image()
{
    file = NULL;                        // font texture
    graphics = NULL;                    // pointer to graphics
    color = graphicsNS::WHITE;          // default to white font
    backColor = TRANSCOLOR;             // default to transparent (no) fill
    align = textNS::LEFT;
    width = textNS::GRID_WIDTH - 3;     // -2 for transparent border and -1 for divider line
    height = textNS::GRID_HEIGHT - 3;
    fontHeight = textNS::FONT_HEIGHT;
    spriteData.width = 0;
    spriteData.height = 0;
    spriteData.rect.bottom = 1;         // rectangle to select parts of an image
    spriteData.rect.right = 1;
    for(int row=0; row<textNS::ROWS; row++)         // for each row of characters in font
    {
        for(int col=0; col<textNS::COLUMNS; col++)  // for each col of characters in font
        {
            fontData[row][col].left = MAXINT;
            fontData[row][col].right = 0;
        }
    }
    proportional = false;
    proportionalSpacing = textNS::PROPORTIONAL_SPACING;
    underline = false;
    bold = false;
    tabSize = textNS::TAB_SIZE;
}

//=============================================================================
// destructor
//=============================================================================
Text::~Text()
{
    Image::~Image();
}

//=============================================================================
// Text��������
// �t�H���g�摜���̊e�����̍��[�ƉE�[�����
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
// fontData�z��́A�e�����̍��[�ƉE�[���i�[
//=============================================================================
bool Text::initialize(Graphics *g, const char *file)
{
    try {
        graphics = g;           // graphics�I�u�W�F�N�g�ւ̃|�C���^

        //-------------------------------------------------------------
        // �t�H���g�e�N�X�`����ǂݍ���ŁA�e�����̐��m�Ȉʒu��
		// ���肷�邽�߂ɁA�s�N�Z���P�ʂŌ���
        //-------------------------------------------------------------

		// �t�H���g�e�N�X�`�����A���b�N�\�ȃV�X�e���������ɓǂݍ���
        UINT w,h;
        HRESULT result = graphics->loadTextureSystemMem(file, TRANSCOLOR, w, h, textureData);
        if (FAILED(result))
        {
            SAFE_RELEASE(textureData);
            return false;
        }

        // textureData.width��textureData.height�̓t�H���g�e�N�X�`��
		// �S�̂̃T�C�Y���i�[
		// �e������1�s�N�Z�����̘g������
		// ROWS * COLS�̕��������݂���
		// �i�s�N�Z���f�[�^�ɃA�N�Z�X���邽�߂ɕK�v�j
        D3DLOCKED_RECT rect;
        result = textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
        if(FAILED(result))                          // ���b�N�����s�����ꍇ
        {
            SAFE_RELEASE(textureData);
            return false;
        }

		// �t�H���g���̕����̊e�s������
        for(DWORD row=0; row<textNS::ROWS; row++)
        {
			// �t�H���g���̕����̊e�������
            for(DWORD col=0; col<textNS::COLUMNS; col++)
            {
                fontData[row][col].left = MAXINT;    // fontData��������
                fontData[row][col].right = 0;

                // ������1�s�N�Z��������
				// for y = ��[�s�N�Z��; y <= ���[�s�N�Z��; y++
                for(DWORD y=row*textNS::GRID_HEIGHT+1; y<(row+1)*textNS::GRID_HEIGHT-1; y++)
                {
					// �e�N�X�`�����̂��̃X�L�������C���̐擪�ւ̃|�C���^���擾
                    DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
                    // ���̍s�̃s�N�Z��������
                    for(DWORD x=col*textNS::GRID_WIDTH+1; x<(col+1)*textNS::GRID_WIDTH-1; x++)
                    {
                        // ���̃s�N�Z�����擾
                        DWORD dwPixel = pBits[x];

                        // �A���t�@�������łȂ��ꍇ
                        if((dwPixel & 0xff000000) != 0x00)
                        {
							// ���̃s�N�Z���̂ق������ɂ���ꍇ
                            if(x < fontData[row][col].left)
								// �����̍��[�Ƃ��ĕۑ�
                                fontData[row][col].left = x;
							// ���̃s�N�Z���̂ق����E�ɂ���ꍇ
                            if(x > fontData[row][col].right)
								// �����̉E�[�Ƃ��ĕۑ�
                                fontData[row][col].right = x;
                        }
                    }
                }
            }
        }

		// �e�N�X�`���̏���������������A���b�N������
        textureData->UnlockRect(0);
    
		// �P�Ƀt�H���g�̃X�y�[�X���擾���邽�߂ɕK�v�������̂ŁA
		// ���̃t�H���g�e�N�X�`�������
        SAFE_RELEASE(textureData);    

        //-------------------------------------------------------------
        // �t�H���g�摜���g�p���邽�߂Ƀe�N�X�`���}�l�[�W���ɓǂݍ���
        //-------------------------------------------------------------
        if (!fontTexture.initialize(graphics, file))
            return false;                   // �t�H���g�e�N�X�`���̓ǂݍ��݂��G���[�̏ꍇ
        // �t�H���g�摜������
        if (!Image::initialize(graphics,textNS::FONT_WIDTH,textNS::FONT_HEIGHT,0,&fontTexture))
            return false;                   // ���s�̏ꍇ
    }
    catch(...)
    {
        return false;
    }
    return true;                    // �����̏ꍇ
}

//=============================================================================
// setXY
// set x,y for text output
//=============================================================================
void Text::setXY(int x2, int y2)
{
    if (x2 < 0 || y2 < 0)
        return;
    spriteData.x = (float)x2;
    spriteData.y = (float)y2;
}

//=============================================================================
// print string at current x,y
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str)
{
    print(str, (int)spriteData.x, (int)spriteData.y);
}

//=============================================================================
// print with align at x,y
// text is aligned as specified from:
// LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y, textNS::Alignment al)
{
    align = al;             // save alignment
    print(str,x,y);
    align = textNS::LEFT;
}

//=============================================================================
// �������X�AY�ɏo��
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y)
{
    UCHAR ch = 0, chN = 0;
    std::string str2;
    width = textNS::FONT_WIDTH;
    int scaledWidth = static_cast<int>(textNS::FONT_WIDTH*spriteData.scale);
    float saveY=0;
    int tabX=0, tabW=0;
    
    spriteData.x = (float)x;
    spriteData.y = (float)y;
    doAlign(str);

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
		// �\���\�ȕ����̏ꍇ
        if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
        {
            chN = ch - textNS::MIN_CHAR;                // MIN_CHAR�̈ʒu���C���f�b�N�X0
            spriteData.rect.top = chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
            spriteData.rect.bottom = spriteData.rect.top + textNS::FONT_HEIGHT;
            if(proportional)
            {
                spriteData.rect.left = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].left;
                // DirectX�͉E�[+1��K�v�Ƃ���
                spriteData.rect.right = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
                width = spriteData.rect.right - spriteData.rect.left;
				// �S�����g�������̏ꍇ�A�X�y�[�X�͋󂯂Ȃ�
                if(width >= textNS::FONT_WIDTH)
                {
                    width = textNS::FONT_WIDTH;         // ���𐧌�
                    spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                    spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
                }
                else    // �S�����g�������łȂ��ꍇ�A�����ԂɃX�y�[�X���󂯂�
                    width += proportionalSpacing;
                scaledWidth = static_cast<int>(width*spriteData.scale);
                drawChar(ch);
            }
            else    // �Œ�s�b�`
            {
                width = textNS::FONT_WIDTH;
                spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
                drawChar(ch);
            }
            spriteData.x += scaledWidth;
        } 
        else    // �\���s�ȕ���
        {
            switch(ch)
            {
            case ' ':                            // �X�y�[�X
                if(proportional)
                {
                    width = textNS::FONT_WIDTH/2;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                }
                drawChar(' ');
                spriteData.x += scaledWidth;
                break;
            // �j���[���C���͉���1�s�i�݁A���[���A
			// ��ʂ̍��[�ł͂Ȃ��AX�̊J�n�ʒu�ɐݒ�
            case '\n':                            // �j���[���C��
                spriteData.x = (float)x;
                spriteData.y += static_cast<int>(height*spriteData.scale);
                saveY = spriteData.y;
                str2 = str.substr(i,str.length());
                doAlign(str2);
                spriteData.y = saveY;
                break;
            case '\r':                            // X�̊J�n�ʒu�ɖ߂�
                spriteData.x = (float)x;
                str2 = str.substr(i,str.length());
                doAlign(str2);
                break;
            case '\t':                            // �^�u
                width = textNS::FONT_WIDTH;
                scaledWidth = static_cast<int>(width*spriteData.scale);
                tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
                tabX = (tabX+1) * scaledWidth * tabSize;
                tabW = tabX - static_cast<int>(spriteData.x);
                while(tabW > 0)
                {
                    if(tabW >= scaledWidth)
                    {
                        drawChar(' ');
                        spriteData.x += scaledWidth;
                    }
                    else
                    {
						// �����̒[�������������ă^�u�ʒu�ɍ��킹��
                        width = tabW;
                        drawChar(' ');
                        spriteData.x += tabW;
                    }
                    tabW -= scaledWidth;
                }
                break;
            case '\b':                            // �o�b�N�X�y�[�X
                spriteData.x -= scaledWidth;
                if(spriteData.x < 0)
                    spriteData.x = 0;
                break;
            case '\v':                            // �����^�u
                spriteData.y += static_cast<int>(height*spriteData.scale);
                break;
            case 0x01:                            // �t�H���g�V�O�l�`������
                spriteData.rect.top = 1;
                spriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
                spriteData.rect.left = 1;
                spriteData.rect.right = 1 + textNS::FONT_WIDTH;
                draw(spriteData);
                spriteData.x += scaledWidth;
                break;
            }
        }
    }
    return;
}

//=============================================================================
// ���݂̕�����Ɣz�u�̎w��ɍ��킹��spriteData.x�AspriteData.y��ݒ�
// �f�t�H���g�̔z�u��LEFT
//=============================================================================
void Text::doAlign(const std::string &str)
{
    if(spriteData.texture == NULL)  // �e�N�X�`�����Ȃ��ꍇ
        return;

    UINT w, h;
    switch(align) {
	// X�������ƂȂ�悤�ɒ��������AY����[�ƂȂ�悤�ɏ㑵��
    case textNS::CENTER:
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        break;
	// X�AY���E�[�ƂȂ�悤�ɉE����
    case textNS::RIGHT:
        getWidthHeight(str,w,h);
        spriteData.x -= w;
        break;
	// X�������ƂȂ�悤�ɒ��������AY���㉺�̐^�񒆂ƂȂ�悤�ɔz�u
    case textNS::CENTER_MIDDLE:
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        spriteData.y -= h/2;
        break;
	// X�������ƂȂ�悤�ɒ��������AY�����[�ƂȂ�悤�ɉ�����
    case textNS::CENTER_BOTTOM:
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        spriteData.y -= h;
        break;
	// X�����[�ƂȂ�悤�ɍ������AY�����[�ƂȂ�悤�ɉ�����
    case textNS::LEFT_BOTTOM:
        getWidthHeight(str,w,h);
        spriteData.y -= h;
        break;
	// X���E�[�ƂȂ�悤�ɉE�����AY�����[�ƂȂ�悤�ɉ�����
    case textNS::RIGHT_BOTTOM:
        getWidthHeight(str,w,h);
        spriteData.x -= w;
        spriteData.y -= h;
        break;
    }
}

//=============================================================================
// getWidthHeight
// ���݂̃t�H���g�T�C�Y�ł̕�����̕��ƍ����i�s�N�Z���P�ʁj���擾
// ������͕\�����Ȃ�
//=============================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{
    if (spriteData.texture == NULL)         // �e�N�X�`�����Ȃ��ꍇ
        return;

    UCHAR ch = 0, chN = 0;
    width = textNS::FONT_WIDTH;
    int scaledWidth = static_cast<int>(width*spriteData.scale);
    int strW = 0;
    h = 0;
    int stringWidth = 0;

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
		// �\���\�ȕ����̏ꍇ
        if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
        {
            chN = ch - textNS::MIN_CHAR;    // MIN_CHAR�̈ʒu���C���f�b�N�X0
            if(proportional)
            {
                spriteData.rect.left = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].left;
                // DirectX�X�v���C�g���̂���+1
                spriteData.rect.right = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
                width = spriteData.rect.right - spriteData.rect.left + proportionalSpacing;
                scaledWidth = static_cast<int>(width*spriteData.scale);
            }
            else    // �Œ�s�b�`
            {
                width = textNS::FONT_WIDTH;
                spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
            }
            stringWidth += scaledWidth;
        } 
        else    // �܂��́A�\���s�\�ȕ����̏ꍇ
        {
            switch(ch)
            {
            case ' ':   // �X�y�[�X
                if(proportional)
                {
                    width = (textNS::FONT_WIDTH)/2;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                }
                stringWidth += scaledWidth;
                break;
            case '\n':  // �j���[���C��
                if(strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                h += static_cast<int>(height*spriteData.scale);
                break;
            case '\r':  // ���^�[��
                if(strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                break;
            case '\t':  // �^�u
                {
                    width = textNS::FONT_WIDTH;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                    int tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
                    tabX = (tabX+1) * scaledWidth * tabSize;
                    int tabW = tabX - static_cast<int>(spriteData.x);
                    while(tabW > 0)
                    {
                        if(tabW >= scaledWidth)
                            stringWidth += scaledWidth;
                        else
                        {
							// �����̒[��������������
							// �^�u�ʒu�ɍ��킹��
                            width = tabW;
                            stringWidth += tabW;
                        }
                        tabW -= scaledWidth;
                    }
                }
                break;
            case '\b':      // �o�b�N�X�y�[�X
                stringWidth -= scaledWidth;
                if(stringWidth < 0)
                    stringWidth = 0;
                break;
            case 0x01:      // ���ʂȃP�[�X
                stringWidth += scaledWidth;
                break;
            }
        }
    }
    if(strW==0)
        strW = stringWidth;
    w = strW;
    return;
}

//=============================================================================
// drawChar
// spriteData�ɂ���ċL�q����镶���X�v���C�g��F�Ɠh��Ԃ����g���ĕ\��
// �����ƕ�����\������
//=============================================================================
void Text::drawChar(UCHAR ch)
{
    SpriteData sd2 = spriteData;    // �X�v���C�g�f�[�^���R�s�[

    // display backColor color
    if(backColor != TRANSCOLOR)     // backColor�������łȂ��ꍇ
    {
        spriteData.rect.top = (textNS::SOLID-textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
        spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
        spriteData.rect.left = (textNS::SOLID-textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
        spriteData.rect.right = spriteData.rect.left + width;
        draw(backColor);        // backColor��`��
        spriteData.rect = sd2.rect;     // �����̋�`�𕜌�
    }

    // ������\��
    if(underline)
    {
        spriteData.rect.top = (textNS::UNDERLINE-textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
        spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
        spriteData.rect.left = (textNS::UNDERLINE-textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
        spriteData.rect.right = spriteData.rect.left + width;
        draw(color);
        spriteData.rect = sd2.rect;     // �����̋�`�𕜌�
    }

    // ������\��
    if(ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // �\���\�ȕ����̏ꍇ
    {
        draw(spriteData, color);
        if (bold)   // �����́A�I�t�Z�b�gX���g���ĕ�����2��\�����邱�Ƃɂ���Ď���
        {
            spriteData.x += textNS::BOLD_SIZE*spriteData.scale; 
            draw(spriteData, color);
            spriteData.x = sd2.x;
        }
    }
}


//=============================================================================
// called when graphics device is lost
//=============================================================================
void Text::onLostDevice()
{
    try{
        if (!initialized)
            return;
        fontTexture.onLostDevice();
    } catch(...){}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Text::onResetDevice()
{
    try{
        if (!initialized)
            return;
        fontTexture.onResetDevice();
    } catch(...){}
}

