#include "GUIUtils.h"
#include "../ImageUtils/ImageUtils.h"
#include <iostream>

Sprite *GUIUtils::createBackground(const std::string &background, Size &visibleSize)
{
    Sprite *bg = Sprite::create(background);
    if (bg == nullptr)
    {
        std::string err = "Can't create Background from " + background;
        throw std::invalid_argument(err);
    }

    float scaleY = visibleSize.height / bg->getContentSize().height;
    float scaleX = visibleSize.width  / bg->getContentSize().width;

    if (scaleX > scaleY) { bg->setScale(scaleX); }
    else { bg->setScale(scaleY); }

    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    return bg;
}

Label *GUIUtils::createLabel(const std::string &text, LabelStyle &labelStyle)
{
    Label *label = Label::createWithTTF(text, labelStyle.textStyle.fontFile, labelStyle.textStyle.fontSize);
    if (label == nullptr)
    {
        std::string err  = StringUtils::format("Can't create label:\ntext = %s\nfontFile = %s\nfontSize = %f\n",
                                               text.c_str(), labelStyle.textStyle.fontFile.c_str(),
                                               labelStyle.textStyle.fontSize);
        throw std::invalid_argument(err);
    }
    setStyleLabel(label, labelStyle);
    return label;
}

Label *::GUIUtils::createLabel(const std::string &text, const Vec2 &position, LabelStyle &labelStyle)
{
    Label *label = createLabel(text, labelStyle);
    label->setPosition(position);
    return label;
}

Menu *GUIUtils::createMenuLabel(const std::string &text, const Vec2 &position, LabelStyle &labelStyle,
                                const ccMenuCallback &callback)
{
    Label *label = createLabel(text, labelStyle);
    Menu *menu = createMenu(label, position, callback);
    return menu;
}

void GUIUtils::setStyleLabel(Label *label, LabelStyle &labelStyle)
{
    label->setTextColor(labelStyle.textStyle.color);
    label->enableShadow(labelStyle.shadowStyle.color,
                        labelStyle.shadowStyle.offset, labelStyle.shadowStyle.blurRadius);
    label->enableOutline(labelStyle.outlineStyle.color, labelStyle.outlineStyle.size);
}

Menu *GUIUtils::createMenu(Label *label, const Vec2 &position, const ccMenuCallback &callback)
{
    MenuItemLabel *menu_item = MenuItemLabel::create(label, callback);
    menu_item->setPosition(position);
    Menu* menu_label = Menu::create(menu_item, nullptr);
    menu_label->setPosition(Vec2::ZERO);
    return menu_label;
}

ui::Slider *GUIUtils::createSlider(const std::string &fileNameBar,      // Background of slider
                                   const std::string &fileNameBall,     // Ball of slider
                                   const std::string &fileNameLine,     // Line of slider
                                   int percent, const Vec2 &position,
                                   const ui::Slider::ccSliderCallback &callback,
                                   float scaleFactor)
{
    ui::Slider *slider = ui::Slider::create(fileNameBar, fileNameBall);
    if (slider == nullptr || ImageUtils::isExist(fileNameLine) == false)
    {
        std::string err = StringUtils::format("Can't create slider:\n"
                                                      "fileNameBar = %s\n"
                                                      "fileNameBall = %s\n"
                                                      "fileNameLine = %s\n",
                                              fileNameBar.c_str(),
                                              fileNameBall.c_str(),
                                              fileNameLine.c_str());
        throw std::invalid_argument(err);
    }
    slider->setPosition(position);
    slider->addEventListener(callback);

    slider->loadProgressBarTexture(fileNameLine);
    slider->setPercent(percent);
    slider->setScale(scaleFactor);
    return slider;
}

ui::Slider *GUIUtils::createSlider(SliderStyle &style, int percent, const Vec2 &position,
                                   const ui::Slider::ccSliderCallback &callback, float scaleFactor)
{
    return createSlider(style.fileBar, style.fileBall, style.fileLine,
                        percent, position, callback, scaleFactor);
}

ui::CheckBox * GUIUtils::createCheckBox(const std::string& backGround, const std::string& cross,
                                        const Vec2 &position, bool is_active,
                                        const ui::CheckBox::ccCheckBoxCallback &callback,
                                        float scaleFactor)
{
    ui::CheckBox *check_box = ui::CheckBox::create(backGround, cross);
    if (check_box == nullptr)
    {
        std::string err = StringUtils::format("Can't create CheckBox:\nbackGround = %s\ncross = %s\n",
                                              backGround.c_str(), cross.c_str());
        throw std::invalid_argument(err);
    }
    check_box->setPosition(position);
    check_box->setSelected(is_active);
    check_box->addEventListener(callback);
    check_box->setScale(scaleFactor);

    return check_box;
}


ui::CheckBox *GUIUtils::createCheckBox(CheckBoxStyle &style,
                                       const Vec2 &position, bool is_active,
                                       const ui::CheckBox::ccCheckBoxCallback &callback,
                                       float scaleFactor)
{
    return createCheckBox(style.fileBackGround, style.fileCross,
                          position, is_active, callback, scaleFactor);
}

void GUIUtils::setupButton(ui::Button *button, const Vec2 &position,
                           const ui::Widget::ccWidgetTouchCallback &callback,
                           float scaleFactor)
{
    button->setPosition(position);
    button->addTouchEventListener(callback);
    button->setScale(scaleFactor);
}

ui::Button *GUIUtils::createButton(const std::string &button, const Vec2 &position,
                                   const ui::Widget::ccWidgetTouchCallback &callback)
{
    ui::Button* btn = ui::Button::create(button);
    if (btn == nullptr)
    {
        std::string err = "Can't create Button from " + button;
        throw std::invalid_argument(err);
    }
    setupButton(btn, position, callback);
    return btn;
}

ui::Button *GUIUtils::createButtonWithTitle(const std::string &button, const std::string &tilte, const Vec2 &position,
                                            TitletStyle &titleStyle, const ui::Widget::ccWidgetTouchCallback &callback)
{
    ui::Button* btn = createButton(button, position, callback);
    if (btn == nullptr)
    {
        std::string err = "Can't create Button from " + button;
        throw std::invalid_argument(err);
    }

    btn->setTitleText(tilte);
    btn->setTitleColor(titleStyle.color);
    btn->setTitleFontName(titleStyle.fontName);
    btn->setTitleFontSize(titleStyle.size);

    return btn;
}

TextStyle::TextStyle(float pFontSize, const std::string &pFontFile, const Color4B &pTextColor):
        fontSize(pFontSize),
        fontFile(pFontFile),
        color(pTextColor)
{}

LabelShadowStyle::LabelShadowStyle(const Color4B &pShadowColor, Size pShadowOffset, int pBlurRadius):
        color(pShadowColor),
        offset(pShadowOffset),
        blurRadius(pBlurRadius)
{}

LabelOutlineStyle::LabelOutlineStyle(const Color4B &pOutlineColor, int pOutlineSize):
        color(pOutlineColor),
        size(pOutlineSize)
{}

LabelStyle::LabelStyle(TextStyle &pTextStyle, LabelShadowStyle &pLabelShadowStyle,
                       LabelOutlineStyle &pLabelOutlineStyle):
        textStyle(pTextStyle),
        shadowStyle(pLabelShadowStyle),
        outlineStyle(pLabelOutlineStyle)
{}

TitletStyle::TitletStyle(float pSize, const std::string &pfontName, const Color3B &pColor):
        size(pSize),
        fontName(pfontName),
        color(pColor)
{}

SliderStyle::SliderStyle(const std::string &pFileBar,
                         const std::string &pFileBall,
                         const std::string &pFileLine):
        fileBar(pFileBar),
        fileBall(pFileBall),
        fileLine(pFileLine)
{}

CheckBoxStyle::CheckBoxStyle(const std::string &pFileBackGround,
                             const std::string &pFileCross):
        fileBackGround(pFileBackGround),
        fileCross(pFileCross)
{}
