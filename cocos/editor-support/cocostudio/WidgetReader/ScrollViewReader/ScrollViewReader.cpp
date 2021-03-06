

#include "ScrollViewReader.h"
#include "ui/UIScrollView.h"
#include "cocostudio/CocoLoader.h"
#include "cocostudio/CSParseBinary.pb.h"
#include "tinyxml2.h"

USING_NS_CC;
using namespace ui;

namespace cocostudio
{
    static const char* P_InnerWidth = "innerWidth";
    static const char* P_InnerHeight = "innerHeight";
    static const char* P_Direction = "direction";
    static const char* P_BounceEnable = "bounceEnable";
    
    static ScrollViewReader* instanceScrollViewReader = nullptr;
    
    IMPLEMENT_CLASS_WIDGET_READER_INFO(ScrollViewReader)
    
    ScrollViewReader::ScrollViewReader()
    {
        
    }
    
    ScrollViewReader::~ScrollViewReader()
    {
        
    }
    
    ScrollViewReader* ScrollViewReader::getInstance()
    {
        if (!instanceScrollViewReader)
        {
            instanceScrollViewReader = new (std::nothrow) ScrollViewReader();
        }
        return instanceScrollViewReader;
    }
    
    void ScrollViewReader::setPropsFromBinary(cocos2d::ui::Widget *widget, CocoLoader *cocoLoader, stExpCocoNode* cocoNode)
    {
        //TODO: need to refactor...
        LayoutReader::setPropsFromBinary(widget, cocoLoader, cocoNode);
        
        ScrollView* scrollView = static_cast<ScrollView*>(widget);
        
        stExpCocoNode *stChildArray = cocoNode->GetChildArray(cocoLoader);
        float innerWidth;
        float innerHeight;
        for (int i = 0; i < cocoNode->GetChildNum(); ++i) {
            std::string key = stChildArray[i].GetName(cocoLoader);
            std::string value = stChildArray[i].GetValue(cocoLoader);
            if (key == P_InnerWidth) {
                innerWidth = valueToFloat(value);
            }
            else if(key == P_InnerHeight){
                innerHeight = valueToFloat(value);
            }else if(key == P_Direction){
                scrollView->setDirection((ScrollView::Direction)valueToInt(value));
            }else if(key == P_BounceEnable){
                scrollView->setBounceEnabled(valueToBool(value));
            }
            
        } //end of for loop
        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

    }
    
    void ScrollViewReader::setPropsFromJsonDictionary(Widget *widget, const rapidjson::Value &options)
    {
        LayoutReader::setPropsFromJsonDictionary(widget, options);
        
        
        ScrollView* scrollView = static_cast<ScrollView*>(widget);
        float innerWidth = DICTOOL->getFloatValue_json(options, P_InnerWidth,200);
        float innerHeight = DICTOOL->getFloatValue_json(options, P_InnerHeight,200);
        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
        int direction = DICTOOL->getFloatValue_json(options, P_Direction,1);
        scrollView->setDirection((ScrollView::Direction)direction);
        scrollView->setBounceEnabled(DICTOOL->getBooleanValue_json(options, P_BounceEnable));
        
        
        LayoutReader::setColorPropsFromJsonDictionary(widget, options);
    }
    
    void ScrollViewReader::setPropsFromProtocolBuffers(ui::Widget *widget, const protocolbuffers::NodeTree &nodeTree)
    {
        WidgetReader::setPropsFromProtocolBuffers(widget, nodeTree);
        
        
        ScrollView* scrollView = static_cast<ScrollView*>(widget);
		const protocolbuffers::ScrollViewOptions& options = nodeTree.scrollviewoptions();

		std::string protocolBuffersPath = GUIReader::getInstance()->getFilePath();
        
        scrollView->setClippingEnabled(options.clipable());
        
        bool backGroundScale9Enable = options.backgroundscale9enable();
        scrollView->setBackGroundImageScale9Enabled(backGroundScale9Enable);
        
        
        int cr;
        int cg;
        int cb;
        int scr;
        int scg;
        int scb;
        int ecr;
        int ecg;
        int ecb;
        
        
        
        cr = options.has_bgcolorr() ? options.bgcolorr() : 255;
        cg = options.has_bgcolorg() ? options.bgcolorg() : 150;
        cb = options.has_bgcolorb() ? options.bgcolorb() : 100;
        
        scr = options.has_bgstartcolorr() ? options.bgstartcolorr() : 255;
        scg = options.has_bgstartcolorg() ? options.bgstartcolorg() : 255;
        scb = options.has_bgstartcolorb() ? options.bgstartcolorb() : 255;
        
        ecr = options.has_bgendcolorr() ? options.bgendcolorr() : 255;
        ecg = options.has_bgendcolorg() ? options.bgendcolorg() : 150;
        ecb = options.has_bgendcolorb() ? options.bgendcolorb() : 100;
        
		float bgcv1 = 0.0f;
        float bgcv2 = -0.5f;
		if(options.has_vectorx())
		{
			bgcv1 = options.vectorx();
		}
		if(options.has_vectory())
		{
			bgcv2 = options.vectory();
		}
        scrollView->setBackGroundColorVector(Vec2(bgcv1, bgcv2));
        
        int co = options.has_bgcoloropacity() ? options.bgcoloropacity() : 100;
        
        int colorType = options.has_colortype() ? options.colortype() : 1;
        scrollView->setBackGroundColorType(Layout::BackGroundColorType(colorType));
        
        scrollView->setBackGroundColor(Color3B(scr, scg, scb),Color3B(ecr, ecg, ecb));
        scrollView->setBackGroundColor(Color3B(cr, cg, cb));
        scrollView->setBackGroundColorOpacity(co);
        
        
		const protocolbuffers::ResourceData& imageFileNameDic = options.backgroundimagedata();
        int imageFileNameType = imageFileNameDic.resourcetype();
        std::string imageFileName = this->getResourcePath(imageFileNameDic.path(), (Widget::TextureResType)imageFileNameType);
        scrollView->setBackGroundImage(imageFileName, (Widget::TextureResType)imageFileNameType);
        
        
        if (backGroundScale9Enable)
        {
            float cx = options.capinsetsx();
            float cy = options.capinsetsy();
            float cw = options.has_capinsetswidth() ? options.capinsetswidth() : 1;
            float ch = options.has_capinsetsheight() ? options.capinsetsheight() : 1;
            scrollView->setBackGroundImageCapInsets(Rect(cx, cy, cw, ch));
            bool sw = options.has_scale9width();
            bool sh = options.has_scale9height();
            if (sw && sh)
            {
                float swf = options.scale9width();
                float shf = options.scale9height();
                scrollView->setContentSize(Size(swf, shf));
            }
        }
        
        scrollView->setLayoutType((Layout::Type)options.layouttype());
        
        const protocolbuffers::WidgetOptions& widgetOptions = nodeTree.widgetoptions();
        
        int red = widgetOptions.has_colorr() ? widgetOptions.colorr() : 255;
        int green = widgetOptions.has_colorg() ? widgetOptions.colorg() : 255;
        int blue = widgetOptions.has_colorb() ? widgetOptions.colorb() : 255;
        scrollView->setColor(Color3B(red, green, blue));
        
        int opacity = widgetOptions.has_alpha() ? widgetOptions.alpha() : 255;
        scrollView->setOpacity(opacity);
        
        
        
        float innerWidth = options.has_innerwidth() ? options.innerwidth() : 200;
        float innerHeight = options.has_innerheight() ? options.innerheight() : 200;
        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
        int direction = options.has_direction() ? options.direction() : 1;
        scrollView->setDirection((ScrollView::Direction)direction);
        scrollView->setBounceEnabled(options.bounceenable());
        
        
        // other commonly protperties
        setAnchorPointForWidget(widget, nodeTree);
        
        bool flipX = widgetOptions.flipx();
        bool flipY = widgetOptions.flipy();
        if (flipX)
        {
            widget->setFlippedX(flipX);
        }
        if (flipY)
        {
            widget->setFlippedY(flipY);
        }
    }
    
    void ScrollViewReader::setPropsFromXML(cocos2d::ui::Widget *widget, const tinyxml2::XMLElement *objectData)
    {
        WidgetReader::setPropsFromXML(widget, objectData);
        
        ScrollView* scrollView = static_cast<ScrollView*>(widget);
        
        std::string xmlPath = GUIReader::getInstance()->getFilePath();
        
        bool scale9Enabled = false;
        float width = 0.0f, height = 0.0f;
        float cx = 0.0f, cy = 0.0f, cw = 0.0f, ch = 0.0f;
        
        Layout::BackGroundColorType colorType = Layout::BackGroundColorType::NONE;
        int color_opacity = 255, bgimg_opacity = 255, opacity = 255;
        int red = 255, green = 255, blue = 255;
        int bgimg_red = 255, bgimg_green = 255, bgimg_blue = 255;
        int singleRed = 255, singleGreen = 255, singleBlue = 255;
        int start_red = 255, start_green = 255, start_blue = 255;
        int end_red = 255, end_green = 255, end_blue = 255;
        float vector_color_x = 0.0f, vector_color_y = -0.5f;
        
        int direction = 1;
        
        int resourceType = 0;
        std::string path = "", plistFile = "";
        
        // attributes
        const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
        while (attribute)
        {
            std::string name = attribute->Name();
            std::string value = attribute->Value();
            
            if (name == "ClipAble")
            {
                scrollView->setClippingEnabled((value == "True") ? true : false);
            }
            else if (name == "ComboBoxIndex")
            {
                colorType = (Layout::BackGroundColorType)atoi(value.c_str());
            }
            else if (name == "BackColorAlpha")
            {
                color_opacity = atoi(value.c_str());
            }
            else if (name == "Alpha")
            {
                opacity = atoi(value.c_str());
                bgimg_opacity = atoi(value.c_str());
            }
            else if (name == "Scale9Enable")
            {
                scale9Enabled = (value == "True") ? true : false;
            }
            else if (name == "Scale9OriginX")
            {
                cx = atof(value.c_str());
            }
            else if (name == "Scale9OriginY")
            {
                cy = atof(value.c_str());
            }
            else if (name == "Scale9Width")
            {
                cw = atof(value.c_str());
            }
            else if (name == "Scale9Height")
            {
                ch = atof(value.c_str());
            }
            else if (name == "ScrollDirectionType")
            {
                if (value == "Vertical")
                {
                    direction = 1;
                }
                else if (value == "Horizontal")
                {
                    direction = 2;
                }
                else if (value == "Vertical_Horizontal")
                {
                    direction = 3;
                }
            }
            else if (name == "IsBounceEnabled")
            {
                scrollView->setBounceEnabled((value == "True") ? true : false);
            }
            
            attribute = attribute->Next();
        }
        
        // child elements
        const tinyxml2::XMLElement* child = objectData->FirstChildElement();
        while (child)
        {
            std::string name = child->Name();
            
            if (name == "InnerNodeSize")
            {
                attribute = child->FirstAttribute();
                width = 0.0f; height = 0.0f;
                
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "Width")
                    {
                        width = atof(value.c_str());
                    }
                    else if (name == "Height")
                    {
                        height = atof(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
                
                scrollView->setInnerContainerSize(Size(width, height));
            }
            else if (name == "Size")
            {
                attribute = child->FirstAttribute();
                
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "X")
                    {
                        width = atof(value.c_str());
                    }
                    else if (name == "Y")
                    {
                        height = atof(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
            }
            else if (name == "CColor")
            {
                attribute = child->FirstAttribute();
                
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "R")
                    {
                        red = atoi(value.c_str());
                        bgimg_red = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        green = atoi(value.c_str());
                        bgimg_green = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        blue = atoi(value.c_str());
                        bgimg_blue = atoi(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
            }
            else if (name == "SingleColor")
            {
                attribute = child->FirstAttribute();
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "R")
                    {
                        singleRed = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        singleGreen = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        singleBlue = atoi(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
            }
            else if (name == "EndColor")
            {
                attribute = child->FirstAttribute();
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "R")
                    {
                        end_red = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        end_green = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        end_blue = atoi(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
            }
            else if (name == "FirstColor")
            {
                attribute = child->FirstAttribute();
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "R")
                    {
                        start_red = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        start_green = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        start_blue = atoi(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
            }
            else if (name == "ColorVector")
            {
                attribute = child->FirstAttribute();
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "ScaleX")
                    {
                        vector_color_x = atof(value.c_str());
                    }
                    else if (name == "ScaleY")
                    {
                        vector_color_y = atof(value.c_str());
                    }
                    
                    attribute = attribute->Next();
                }
            }
            else if (name == "FileData")
            {
                attribute = child->FirstAttribute();
                
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "Path")
                    {
                        path = value;
                    }
                    else if (name == "Type")
                    {
                        resourceType = (value == "Normal" || value == "Default" || value == "MarkedSubImage") ? 0 : 1;
                    }
                    else if (name == "Plist")
                    {
                        plistFile = value;
                    }
                    
                    attribute = attribute->Next();
                }
            }
            
            child = child->NextSiblingElement();
        }
        
        scrollView->setColor(Color3B(red, green, blue));
        scrollView->setOpacity(opacity);
        
        scrollView->setBackGroundColorType(colorType);
        switch (colorType)
        {
            case Layout::BackGroundColorType::SOLID:
                scrollView->setBackGroundColor(Color3B(singleRed, singleGreen, singleBlue));
                break;
                
            case Layout::BackGroundColorType::GRADIENT:
                scrollView->setBackGroundColor(Color3B(start_red, start_green, start_blue),
                                               Color3B(end_red, end_green, end_blue));
                scrollView->setBackGroundColorVector(Vec2(vector_color_x, vector_color_y));
                break;
                
            default:
                break;
        }
        
        scrollView->setBackGroundColorOpacity(color_opacity);
        
        switch (resourceType)
        {
            case 0:
            {
                scrollView->setBackGroundImage(xmlPath + path, Widget::TextureResType::LOCAL);
                break;
            }
                
            case 1:
            {
                SpriteFrameCache::getInstance()->addSpriteFramesWithFile(xmlPath + plistFile);
                scrollView->setBackGroundImage(path, Widget::TextureResType::PLIST);
                break;
            }
                
            default:
                break;
        }
        
        if (path != "")
        {
            if (scale9Enabled)
            {
                scrollView->setBackGroundImageScale9Enabled(scale9Enabled);
                scrollView->setBackGroundImageCapInsets(Rect(cx, cy, cw, ch));
                scrollView->setContentSize(Size(width, height));
            }
        }
        
        scrollView->setDirection((ScrollView::Direction)direction);
        
    }
    
}
