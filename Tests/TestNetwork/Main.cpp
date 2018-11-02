#include <iostream>
#include <Engine/Log.hpp>
#include <Network/Http/Http.hpp>
#include <Network/Udp/UdpSocket.hpp>
#include <Network/Packet.hpp>

using namespace acid;

int main(int argc, char **argv)
{
	Http http = Http("http://equilibrium.games/");

	HttpRequest request;
	request.SetMethod(HTTP_METHOD_POST);
	request.SetUri("/devblog/");
	request.SetHttpVersion(1, 1); // HTTP 1.1
	request.SetField("From", "me");
	request.SetField("Content-Type", "application/x-www-form-urlencoded");
	request.SetBody("para1=value1&param2=value2");

	HttpResponse response = http.SendRequest(request);
	Log::Out("Status: %i\n", response.GetStatus());
	Log::Out("HTTP version: %i.%i\n", response.GetMajorHttpVersion(), response.GetMinorHttpVersion());
	Log::Out("Content-Type header: %s\n", response.GetField("Content-Type").c_str());
	Log::Out("Body: %s\n", response.GetBody().c_str());

	/*UdpSocket socket = UdpSocket();

	std::string name = "Hello world";
	uint32_t a = 420;
	float b = 6.9f;
	Vector3 c = Vector3(8.1f, -9.11f, 2.083f);

	Packet packet = Packet();
	packet << name << a << b << c; // Sending data.
	packet >> name >> a >> b >> c; // Receiving data.

	IpAddress recipient = IpAddress("172.168.0.9");
	unsigned short port = 54000;
	if (socket.Send(packet.GetData(), packet.GetDataSize(), recipient, port) != SOCKET_STATUS_DONE)
	{
		Log::Error("Error OH NO\n");
		// error...
	}*/

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
}
