#pragma once

#include <iostream>
#include <string>

#include <hiredis/hiredis.h>

class Redis {
 public:
  Redis() {}
  ~Redis() {
    connect_ = nullptr;
    reply_ = nullptr;
  }

  bool connect(std::string host, int port) {
    connect_ = redisConnect(host.c_str(), port);
    if (connect_ != nullptr && connect_->err) {
      printf("connect err: %s\n", connect_->errstr);
      return 0;
    }
    return 1;
  }

  std::string get(std::string key) {
    reply_ = (redisReply*)redisCommand(connect_, "GET %s", key.c_str());
    std::string str = reply_->str;
    freeReplyObject(reply_);
    return str;
  }

  void set(std::string key, std::string value) {
    redisCommand(connect_, "SET %s %s", key.c_str(), value.c_str());
  }

  // 如果redis设置了密码，则需要密码登录
  void auth(std::string password) {
    redisCommand(connect_, "auth %s", password.c_str());
  }

 private:
  redisContext* connect_;
  redisReply* reply_;
};
