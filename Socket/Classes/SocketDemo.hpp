//
//  SocketDemo.hpp
//  Socket
//
//  Created by student on 16/4/7.
//
//

#ifndef SocketDemo_hpp
#define SocketDemo_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "network/WebSocket.h"
using namespace cocos2d::network;
using namespace cocos2d;
class SocketLayer:public Layer,public network::WebSocket::Delegate{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(SocketLayer);
    void initUI();
    void onOpen(cocos2d::network::WebSocket* ws);
    void onMessage(cocos2d::network::WebSocket* ws,const network::WebSocket::Data& data);
    void onClose(cocos2d::network::WebSocket* ws);
    void onError(WebSocket *ws,const cocos2d::network::WebSocket::ErrorCode& error);
     //显示状态
    Label* sendTextStatus;
    cocos2d::network::WebSocket* _wsiSendText;
   
    void menuCloseCallback(Ref* sender);
    void keepAlive(float dt);//时间调度，向服务器发送数据并且计算时间差值
    long currentTime;
    //int sendTextTimes;
   // void menuCloseCallback(cocos2d::Ref* pSender);
    
    
};

#endif /* SocketDemo_hpp */
