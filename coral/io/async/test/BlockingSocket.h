/*
 * Copyright 2015 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <coral/Optional.h>
#include <coral/io/async/SSLContext.h>
#include <coral/io/async/AsyncSocket.h>
#include <coral/io/async/AsyncSSLSocket.h>

class BlockingSocket : public coral::AsyncSocket::ConnectCallback,
                       public coral::AsyncTransportWrapper::ReadCallback,
                       public coral::AsyncTransportWrapper::WriteCallback
{
 public:
  explicit BlockingSocket(int fd)
    : sock_(new coral::AsyncSocket(&eventBase_, fd)) {
  }

  BlockingSocket(coral::SocketAddress address,
                 std::shared_ptr<coral::SSLContext> sslContext)
    : sock_(sslContext ? new coral::AsyncSSLSocket(sslContext, &eventBase_) :
            new coral::AsyncSocket(&eventBase_)),
    address_(address) {}

  void open() {
    sock_->connect(this, address_);
    eventBase_.loop();
    if (err_.hasValue()) {
      throw err_.value();
    }
  }
  void close() {
    sock_->close();
  }

  int32_t write(uint8_t const* buf, size_t len) {
    sock_->write(this, buf, len);
    eventBase_.loop();
    if (err_.hasValue()) {
      throw err_.value();
    }
    return len;
  }

  void flush() {}

  int32_t readAll(uint8_t *buf, size_t len) {
    return readHelper(buf, len, true);
  }

  int32_t read(uint8_t *buf, size_t len) {
    return readHelper(buf, len, false);
  }

  int getSocketFD() const {
    return sock_->getFd();
  }

 private:
  coral::EventBase eventBase_;
  coral::AsyncSocket::UniquePtr sock_;
  coral::Optional<coral::AsyncSocketException> err_;
  uint8_t *readBuf_{nullptr};
  size_t readLen_{0};
  coral::SocketAddress address_;

  void connectSuccess() noexcept override {}
  void connectErr(const coral::AsyncSocketException& ex) noexcept override {
    err_ = ex;
  }
  void getReadBuffer(void** bufReturn, size_t* lenReturn) override {
    *bufReturn = readBuf_;
    *lenReturn = readLen_;
  }
  void readDataAvailable(size_t len) noexcept override {
    readBuf_ += len;
    readLen_ -= len;
    if (readLen_ == 0) {
      sock_->setReadCB(nullptr);
    }
  }
  void readEOF() noexcept override {
  }
  void readErr(const coral::AsyncSocketException& ex) noexcept override {
    err_ = ex;
  }
  void writeSuccess() noexcept override {}
  void writeErr(size_t bytesWritten,
                const coral::AsyncSocketException& ex) noexcept override {
    err_ = ex;
  }

  int32_t readHelper(uint8_t *buf, size_t len, bool all) {
    readBuf_ = buf;
    readLen_ = len;
    sock_->setReadCB(this);
    while (!err_ && sock_->good() && readLen_ > 0) {
      eventBase_.loop();
      if (!all) {
        break;
      }
    }
    sock_->setReadCB(nullptr);
    if (err_.hasValue()) {
      throw err_.value();
    }
    if (all && readLen_ > 0) {
      throw coral::AsyncSocketException(coral::AsyncSocketException::UNKNOWN,
                                        "eof");
    }
    return len - readLen_;
  }
};
