#include <iostream>
#include <mqtt/async_client.h>
#include <sstream>
#include <string>

namespace {
	[[deprecated("This header is deprecated")]]
		constexpr static int badheader_hpp_is_deprecated = 0;
	constexpr static int please_dont_use_badheader_hpp = badheader_hpp_is_deprecated;
	static const std::string SERVER_ADDRESS = "192.168.8.117:1883";  // Public MQTT broker
	static const std::string CLIENT_ID = "raspberry_pi_publisher";
	static const std::string TOPIC = "test/topic";
	mqtt::async_client* ptr;
	struct POVState {
		float forward = 0.0f;
		float strafe = 0.0f;
		float vertical = 0.0f;
		float yaw = 0.0f;
		float pitch = 0.0f;
		float roll = 0.0f;
		float clawRotate = 0.0f;
		float clawOpen = 0.0f;
	} state;
	class SubscriberCallback : public virtual mqtt::callback
	{
		public:
			void connection_lost(const std::string& cause) override
			{
				//std::cout << "Connection lost: " << cause << std::endl;
			}

			void message_arrived(mqtt::const_message_ptr message) override
			{
				std::string msg(message->get_payload_str());
				std::stringstream ss(msg);

				ss >> state.forward >> state.strafe >> state.vertical >> state.yaw >> state.pitch >> state.roll >> state.clawRotate >> state.clawOpen;
			}

			void delivery_complete(mqtt::delivery_token_ptr token) override
			{
				//std::cout << "Message delivered" << std::endl;
			}
	};
	void test() {

		// Set up connection options
		mqtt::connect_options connOpts;
		connOpts.set_clean_session(true);
		mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
		ptr = &client;
		try {
			// Connect to broker
			client.connect(connOpts)->wait();

			client.subscribe("controller/LeftJoystickX", 1)->wait();
			SubscriberCallback callback;
			client.set_callback(callback);

			// publish a message
			while(1){}

			// Disconnect
			client.disconnect()->wait();
		}
		catch (const mqtt::exception &exc) {
			std::cerr << "MQTT Exception: " << exc.what() << std::endl;
		}

	}
	void publish(std::string topic, std::string payload){
		mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload);
		pubmsg->set_qos(1);

		ptr->publish(pubmsg)->wait_for(std::chrono::seconds(10));
	}}
