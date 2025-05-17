#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <sstream>

using boost::asio::ip::udp;

struct POVState{
	float forward = 0.0f;
	float strafe = 0.0f;
	float vertical = 0.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;
	float clawRotate = 0.0f;
	float clawOpen = 0.0f;

} state;
void server() {
    try {
        boost::asio::io_context io_context;

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));
        std::cout << "UDP server is running on port 12345...\n";

        for (;;) {
            char data[1024];
            udp::endpoint remote_endpoint;
            boost::system::error_code error;

            std::size_t length = socket.receive_from(boost::asio::buffer(data), remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            //std::cout << "Received from " << remote_endpoint.address().to_string()         << ": " << std::string(data, length) << std::endl;
		std::string msg(data, length);
		std::stringstream ss(msg);
			ss >> state.forward >> state.strafe >> state.vertical >> state.yaw >> state.pitch >> state.roll >> state.clawRotate >> state.clawOpen;
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }

}


