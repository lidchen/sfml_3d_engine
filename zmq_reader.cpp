#include <iostream>
#include <zmq.hpp>

int zmq_read() {
  std::cout << "start recving";
  std::flush(std::cout);
  zmq::context_t ctx;
  zmq::socket_t sock(ctx, ZMQ_SUB);
  sock.connect("tcp://127.0.0.1:5555");
  sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  while (true) {
    zmq::message_t msg;
    sock.recv(&msg);
    float* xyz = (float*)(msg.data());
    for (int i = 0; i < 3; i++) {
      std::cout << xyz[i] << " ";
    } 
    // std::cout << msg.to_string();
    std::cout << '\n';
    // engine.setEyePos(xyz[0], xyz[1], xyz[2]);
  }
  return 0;
}