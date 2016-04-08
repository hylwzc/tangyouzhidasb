//
//  SocketDemo.cpp
//  Socket
//
//  Created by student on 16/4/7.
//
//

#include "SocketDemo.hpp"
#define TIMEOUT 11000
Scene*SocketLayer::createScene(){
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SocketLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;

}
bool SocketLayer:: init(){
    if ( !Layer::init() )
    {
        return false;
    }

    initUI();
    
    return true;
}
void SocketLayer::menuCloseCallback(Ref* pSender)
{
    _wsiSendText->send("Hello, I am wepsocket");
}
long getCurrentTime(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec *1000+tv.tv_usec/1000;
}
void SocketLayer:: keepAlive(float dt){
    long time=getCurrentTime();
    log("tiume=%ld",time);
    if (time-currentTime>TIMEOUT) {
        log("请求超时");
    }else{
        log("连接正常");
        _wsiSendText->send("timer");
    }
}

void SocketLayer::initUI(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png","CloseSelected.png",CC_CALLBACK_1(SocketLayer::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    sendTextStatus=Label::createWithTTF("aaaa","fonts/Marker Felt.ttf", 60);
    sendTextStatus->setPosition(Point(200,200));
    this->addChild(sendTextStatus,5);
    _wsiSendText=new WebSocket();
    //init 第一个参数是delegate,设置为this,第二个参数是服务器地址。
    //URL中的"ws://"标识是WepSocket协议，加密的WepSocket 为wss://".
    _wsiSendText->init(*this, "ws://echo.websocket.org");
    _wsiSendText->send("Hello,I am websocket");
}
void SocketLayer::onOpen(cocos2d::network::WebSocket* ws){
    log("onOpen");
    if (ws==_wsiSendText){
        sendTextStatus->setString("send Text Ws was opened");
        currentTime=getCurrentTime();
        this->schedule(schedule_selector(SocketLayer::keepAlive), 10);
        
    }
    
}
//network::Websocket::Data 对象存储客户端接收到的数据，
//isBinary 属性用来判断数据是二进制还是文本，len说明数据长度，bytes指向数据.
void SocketLayer::onMessage(cocos2d::network::WebSocket* ws,const network::WebSocket::Data& data){
    log("onMessage");
    if(!data.isBinary){
        std::string textStr=std::string(data.bytes);
        sendTextStatus->setString(textStr.c_str());
        log("text=%s",textStr.c_str());
        if (textStr=="timer") {
            //表示收到服务器发来的信息
            currentTime=getCurrentTime();
        }
    }
    
}
void SocketLayer::onClose(cocos2d::network::WebSocket* ws){
    log("onClose");
    this->unscheduleAllCallbacks();
    if (ws==_wsiSendText) {
        _wsiSendText=NULL;
    }
    CC_SAFE_DELETE(ws);
}
void SocketLayer::onError(WebSocket *ws,const cocos2d::network::WebSocket::ErrorCode& error){
    log("onError");
    if (ws==_wsiSendText) {
        char buf[100]={0};
        sprintf(buf, "an error was fired,code:%d",error);
        sendTextStatus->setString(buf);
    }
    
}