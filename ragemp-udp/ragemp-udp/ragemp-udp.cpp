#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>

// raknet
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"

#include "RakNetTypes.h"
#include "NativeFeatureIncludes.h"
#include "RakSleep.h"
#include "BitStream.h"
#include "SecureHandshake.h"

using namespace RakNet;

static void print_buf(const char *title, const unsigned char *buf, size_t buf_len)
{
	size_t i = 0;
	fprintf(stdout, "%s\n", title);
	for (i = 0; i < buf_len; ++i)
		fprintf(stdout, "%02X%s", buf[i],
				(i + 1) % 16 == 0 ? "\r\n" : " ");

	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::cerr << "usage: ragemp-udp IP PORT HASH" << std::endl;
		std::cerr << "example: ragemp-udp 127.0.0.1 22005 4e9d39bc0513d258" << std::endl;
		return 1;
	}

	std::string remote_addr(argv[1]);
	std::uint16_t remote_port = std::strtoul(argv[2], nullptr, 10);
	std::uint64_t replace_hash = std::strtoull(argv[3], nullptr, 16);

	std::uint16_t proxy_port = 22005;
	std::uint16_t proxy_http_port = 22006;

	auto remote = RakPeerInterface::GetInstance();
	auto proxy = RakPeerInterface::GetInstance();

	std::cout << "initialize a proxy server." << std::endl;

	SocketDescriptor local_socket(proxy_port, "0.0.0.0");

	// TODO: force enable encryption?

	// cat::EasyHandshake handshake;
	// char proxy_public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
	// char proxy_private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
	// handshake.GenerateServerKey(proxy_public_key, proxy_private_key);
	// proxy->InitializeSecurity(proxy_public_key, proxy_private_key, false);

	if (proxy->Startup(8, &local_socket, 1) != RAKNET_STARTED)
	{
		std::cerr << "couldn't start a proxy server" << std::endl;
		return 1;
	}

	proxy->SetMaximumIncomingConnections(8);

	local_socket.port = 0;

	PublicKey public_key;
	Packet *proxy_packet;
	Packet *remote_packet;

	public_key.publicKeyMode = PKM_ACCEPT_ANY_PUBLIC_KEY;

	// remote address
	SystemAddress remote_address(remote_addr.c_str(), remote_port);

	// proxy client addr
	SystemAddress client_address;

	std::cout << "the proxy server listen to " << proxy->GetMyBoundAddress().ToString(true, ':') << std::endl;
	std::cout << std::endl
			  << "settings:" << std::endl;
	std::cout << "remote host: " << remote_addr << ":" << remote_port << std::endl;

	printf("hash replacement: %llx\n", replace_hash);

	BitStream unsended;
	bool connected_to_remote = false;

	while (true)
	{
		// proxy
		proxy_packet = proxy->Receive();

		if (proxy_packet)
		{
			if ((int)proxy_packet->data[0] == ID_CONNECTION_LOST)
			{
				remote->Shutdown(0);
			}

			if ((int)proxy_packet->data[0] == ID_NEW_INCOMING_CONNECTION)
			{
				client_address = proxy_packet->systemAddress;
				remote->Startup(8, &local_socket, 1);
				remote->Connect(remote_addr.c_str(), remote_port, 0, 0, &public_key);
				remote->SetMaximumIncomingConnections(8);
			}
			else
			{
				BitStream out((unsigned char *)proxy_packet->data, proxy_packet->length, true);

				if (connected_to_remote)
				{
					remote->Send(&out, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);
				}
				else
				{
					unsended.Write(out);
				}
			}

			proxy->DeallocatePacket(proxy_packet);
		}

		// from remote
		remote_packet = remote->Receive();

		if (remote_packet)
		{
			if ((int)remote_packet->data[0] == ID_CONNECTION_LOST)
			{
				printf("remote timeout!\n");
			}

			if ((int)remote_packet->data[0] == ID_OUR_SYSTEM_REQUIRES_SECURITY)
			{
				std::cout << "remote server isn't secure, change secure policy" << std::endl;
				remote->CancelConnectionAttempt(remote_address);
				public_key.publicKeyMode = PKM_INSECURE_CONNECTION;
				auto result = remote->Connect(remote_addr.c_str(), remote_port, 0, 0, &public_key);

				std::cout << "Connect result " << result << std::endl;
			}

			if ((int)remote_packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			{
				std::cout << "connected to remote host" << std::endl;
				connected_to_remote = true;
				remote->Send(&unsended, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);
				std::cout << "send unseded packet" << std::endl;
			}
			else
			{
				BitStream out((unsigned char *)remote_packet->data, remote_packet->length, true);

				std::uint8_t packet_id;
				out.Read(packet_id);

				if ((int)packet_id == 0xD2)
				{
					std::uint16_t rpc_id;
					out.Read(rpc_id);

					// ответ от сервера
					// WriteRpcHeader
					if (rpc_id == 0x31) // 49 FUN_1400dd5b0
					{
						out.IgnoreBytes(8); // игнорируем хеш

						BitStream new_packet(out.GetNumberOfBytesUsed());
						new_packet.Write(packet_id);
						new_packet.Write(rpc_id);
						new_packet.Write(replace_hash);

						int bits_to_read = 0;
						bool custom = out.ReadBit();

						if (custom)
						{
							std::uint16_t f = 0;
							out.Read(f);
							bits_to_read = f << 3;
						}
						else
						{
							bits_to_read = 16;
						}

						// char *buffer = (char *)malloc(BITS_TO_BYTES(bits_to_read));

						// похоже на порт откуда качать говно
						// хост, если не указан fast-dl-host, то просто порт, иначе адрес
						// out.ReadBits((unsigned char *)buffer, bits_to_read);
						out.IgnoreBits(bits_to_read);

						std::uint16_t voice_sample_rate = 0;
						out.Read(voice_sample_rate);

						/*
						нам это не нужно, перенаправляем в любом случае на наш прокси сервер
						// fast-dl-host
						if (custom) {
							new_packet.Write1();
							new_packet.Write(BITS_TO_BYTES(bits_to_read));
						}
						else {
							new_packet.Write0();
						}
						*/

						new_packet.Write0(); // без fast-dl-host
						new_packet.Write(proxy_http_port);
						// voice-chat-sample-rate
						new_packet.Write(voice_sample_rate);

						proxy->Send(&new_packet, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
						remote->DeallocatePacket(remote_packet);

						continue;
					}
				}

				proxy->Send(&out, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
			}

			remote->DeallocatePacket(remote_packet);
		}

		RakSleep(0);
	}
}
