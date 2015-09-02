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

#include <coral/io/async/AsyncUDPSocket.h>
#include <coral/io/async/AsyncUDPServerSocket.h>
#include <coral/io/async/AsyncTimeout.h>
#include <coral/io/async/EventBase.h>
#include <coral/SocketAddress.h>

#include <boost/thread/barrier.hpp>

#include <coral/io/IOBuf.h>

#include <thread>

#include <gtest/gtest.h>

using coral::AsyncUDPSocket;
using coral::AsyncUDPServerSocket;
using coral::AsyncTimeout;
using coral::EventBase;
using coral::SocketAddress;
using coral::IOBuf;

class UDPAcceptor
    : public AsyncUDPServerSocket::Callback {
 public:
  UDPAcceptor(EventBase* evb, int n): evb_(evb), n_(n) {
  }

  void onListenStarted() noexcept override {}

  void onListenStopped() noexcept override {}

  void onDataAvailable(std::shared_ptr<coral::AsyncUDPSocket> socket,
                       const coral::SocketAddress& client,
                       std::unique_ptr<coral::IOBuf> data,
                       bool truncated) noexcept override {

    lastClient_ = client;
    lastMsg_ = data->moveToFbString().toStdString();

    auto len = data->computeChainDataLength();
    VLOG(4) << "Worker " << n_ << " read " << len << " bytes "
            << "(trun:" << truncated << ") from " << client.describe()
            << " - " << lastMsg_;

    sendPong();
  }

  void sendPong() noexcept {
    try {
      AsyncUDPSocket socket(evb_);
      socket.bind(coral::SocketAddress("127.0.0.1", 0));
      socket.write(lastClient_, coral::IOBuf::copyBuffer(lastMsg_));
    } catch (const std::exception& ex) {
      VLOG(4) << "Failed to send PONG " << ex.what();
    }
  }

 private:
  EventBase* const evb_{nullptr};
  const int n_{-1};

  coral::SocketAddress lastClient_;
  std::string lastMsg_;
};

class UDPServer {
 public:
  UDPServer(EventBase* evb, coral::SocketAddress addr, int n)
      : evb_(evb), addr_(addr), evbs_(n) {
  }

  void start() {
    CHECK(evb_->isInEventBaseThread());

    socket_ = coral::make_unique<AsyncUDPServerSocket>(
        evb_,
        1500);

    try {
      socket_->bind(addr_);
      VLOG(4) << "Server listening on " << socket_->address().describe();
    } catch (const std::exception& ex) {
      LOG(FATAL) << ex.what();
    }

    acceptors_.reserve(evbs_.size());
    threads_.reserve(evbs_.size());

    // Add numWorkers thread
    int i = 0;
    for (auto& evb: evbs_) {
      acceptors_.emplace_back(&evb, i);

      std::thread t([&] () {
        evb.loopForever();
      });

      auto r = std::make_shared<boost::barrier>(2);
      evb.runInEventBaseThread([r] () {
        r->wait();
      });
      r->wait();

      socket_->addListener(&evb, &acceptors_[i]);
      threads_.emplace_back(std::move(t));
      ++i;
    }

    socket_->listen();
  }

  coral::SocketAddress address() const {
    return socket_->address();
  }

  void shutdown() {
    CHECK(evb_->isInEventBaseThread());
    socket_->close();
    socket_.reset();

    for (auto& evb: evbs_) {
      evb.terminateLoopSoon();
    }

    for (auto& t: threads_) {
      t.join();
    }
  }

 private:
  EventBase* const evb_{nullptr};
  const coral::SocketAddress addr_;

  std::unique_ptr<AsyncUDPServerSocket> socket_;
  std::vector<std::thread> threads_;
  std::vector<coral::EventBase> evbs_;
  std::vector<UDPAcceptor> acceptors_;
};

class UDPClient
    : private AsyncUDPSocket::ReadCallback,
      private AsyncTimeout {
 public:
  explicit UDPClient(EventBase* evb)
      : AsyncTimeout(evb),
        evb_(evb) {
  }

  void start(const coral::SocketAddress& server, int n) {
    CHECK(evb_->isInEventBaseThread());

    server_ = server;
    socket_ = coral::make_unique<AsyncUDPSocket>(evb_);

    try {
      socket_->bind(coral::SocketAddress("127.0.0.1", 0));
      VLOG(4) << "Client bound to " << socket_->address().describe();
    } catch (const std::exception& ex) {
      LOG(FATAL) << ex.what();
    }

    socket_->resumeRead(this);

    n_ = n;

    // Start playing ping pong
    sendPing();
  }

  void shutdown() {
    CHECK(evb_->isInEventBaseThread());
    socket_->pauseRead();
    socket_->close();
    socket_.reset();
    evb_->terminateLoopSoon();
  }

  void sendPing() {
    if (n_ == 0) {
      shutdown();
      return;
    }

    --n_;
    scheduleTimeout(5);
    socket_->write(
        server_,
        coral::IOBuf::copyBuffer(coral::to<std::string>("PING ", n_)));
  }

  void getReadBuffer(void** buf, size_t* len) noexcept override {
    *buf = buf_;
    *len = 1024;
  }

  void onDataAvailable(const coral::SocketAddress& client,
                       size_t len,
                       bool truncated) noexcept override {
    VLOG(4) << "Read " << len << " bytes (trun:" << truncated << ") from "
              << client.describe() << " - " << std::string(buf_, len);
    VLOG(4) << n_ << " left";

    ++pongRecvd_;

    sendPing();
  }

  void onReadError(const coral::AsyncSocketException& ex) noexcept override {
    VLOG(4) << ex.what();

    // Start listening for next PONG
    socket_->resumeRead(this);
  }

  void onReadClosed() noexcept override {
    CHECK(false) << "We unregister reads before closing";
  }

  void timeoutExpired() noexcept override {
    VLOG(4) << "Timeout expired";
    sendPing();
  }

  int pongRecvd() const {
    return pongRecvd_;
  }

 private:
  EventBase* const evb_{nullptr};

  coral::SocketAddress server_;
  std::unique_ptr<AsyncUDPSocket> socket_;

  int pongRecvd_{0};

  int n_{0};
  char buf_[1024];
};

TEST(AsyncSocketTest, PingPong) {
  coral::EventBase sevb;
  UDPServer server(&sevb, coral::SocketAddress("127.0.0.1", 0), 4);
  boost::barrier barrier(2);

  // Start event loop in a separate thread
  auto serverThread = std::thread([&sevb] () {
    sevb.loopForever();
  });

  // Wait for event loop to start
  sevb.runInEventBaseThread([&] () { barrier.wait(); });
  barrier.wait();

  // Start the server
  sevb.runInEventBaseThread([&] () { server.start(); barrier.wait(); });
  barrier.wait();

  coral::EventBase cevb;
  UDPClient client(&cevb);

  // Start event loop in a separate thread
  auto clientThread = std::thread([&cevb] () {
    cevb.loopForever();
  });

  // Wait for event loop to start
  cevb.runInEventBaseThread([&] () { barrier.wait(); });
  barrier.wait();

  // Send ping
  cevb.runInEventBaseThread([&] () { client.start(server.address(), 1000); });

  // Wait for client to finish
  clientThread.join();

  // Check that some PING/PONGS were exchanged. Out of 1000 transactions
  // at least 1 should succeed
  CHECK_GT(client.pongRecvd(), 0);

  // Shutdown server
  sevb.runInEventBaseThread([&] () {
    server.shutdown();
    sevb.terminateLoopSoon();
  });

  // Wait for server thread to joib
  serverThread.join();
}
