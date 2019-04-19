#include <iostream>
#include <cstdlib>

// raknet
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"

#include "RakNetTypes.h"
#include "NativeFeatureIncludes.h"
#include "RakSleep.h"
#include "BitStream.h"
#include "SecureHandshake.h"

using namespace RakNet;

static void print_buf(const char* title, const unsigned char* buf, size_t buf_len)
{
	size_t i = 0;
	fprintf(stdout, "%s\n", title);
	for (i = 0; i < buf_len; ++i)
		fprintf(stdout, "%02X%s", buf[i],
		(i + 1) % 16 == 0 ? "\r\n" : " ");

	printf("\n");
}

int main()
{
	// здесь целевой сервер
	std::string remote_addr("145.239.149.95");
	// std::string remote_addr("127.0.0.1");
	std::uint16_t remote_port = 22005;
	std::uint16_t proxy_port = 22005;

	auto remote = RakPeerInterface::GetInstance();
	auto proxy = RakPeerInterface::GetInstance();

	std::cout << "initialize a proxy server." << std::endl;

	SocketDescriptor local_socket(proxy_port, 0);

	if (proxy->Startup(8, &local_socket, 1) != RAKNET_STARTED) {
		std::cout << "couldn't start a proxy server" << std::endl;
		exit(1);
	}

	proxy->SetMaximumIncomingConnections(8);

	local_socket.port = 0;

	PublicKey public_key;
	Packet* proxy_packet;
	Packet* remote_packet;

	// без шифрования
	// public_key.publicKeyMode = PKM_INSECURE_CONNECTION;
	
	// с шифрованием
	public_key.publicKeyMode = PKM_ACCEPT_ANY_PUBLIC_KEY;

	// remote address
	SystemAddress remote_address(remote_addr.c_str(), remote_port);

	// proxy client addr
	SystemAddress client_address;

	std::cout << "the proxy server listen to " << proxy->GetMyBoundAddress().ToString(true, ':') << std::endl;

	BitStream unsended;
	bool connected_to_remote = false;

	while (true) {
		// proxy
		proxy_packet = proxy->Receive();

		if (proxy_packet) {
			if ((int)proxy_packet->data[0] == ID_CONNECTION_LOST) {
				remote->Shutdown(0);
			}

			if ((int)proxy_packet->data[0] == ID_NEW_INCOMING_CONNECTION) {
				client_address = proxy_packet->systemAddress;
				remote->Startup(8, &local_socket, 1);
				remote->Connect(remote_addr.c_str(), remote_port, 0, 0, &public_key);
				remote->SetMaximumIncomingConnections(8);
			}
			else {
				// print_buf("from proxy client", (unsigned char*)proxy_packet->data, proxy_packet->length);
				BitStream out((unsigned char*)proxy_packet->data, proxy_packet->length, true);

				if (connected_to_remote) {
					remote->Send(&out, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);
				}
				else {
					unsended.Write(out);
				}
			}

			proxy->DeallocatePacket(proxy_packet);
		}

		// from remote
		remote_packet = remote->Receive();

		if (remote_packet) {
			if ((int)remote_packet->data[0] == ID_CONNECTION_LOST) {
				printf("remote timeout!\n");
			}

			if ((int)remote_packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED) {
				connected_to_remote = true;
				remote->Send(&unsended, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);
				
			} else {
				BitStream out((unsigned char*)remote_packet->data, remote_packet->length, true);

				std::uint8_t packet_id;
				out.Read(packet_id);

				if ((int)packet_id == 0xD2) {
					std::uint16_t rpc_id;
					out.Read(rpc_id);
					
					// ответ от сервера
					if (rpc_id == 0x31) {
						print_buf("old packet", remote_packet->data, remote_packet->length);

						std::uint64_t new_hash = 0x5f9415cf68c0bb3f; // это хеш, взятый из ragemp-http
						char* buffer = (char*)alloca(5); // хуй проссышь что это
						out.IgnoreBytes(8); // игнорируем хеш
						out.Read(buffer, 5);

						BitStream new_packet(out.GetNumberOfBytesUsed());
						new_packet.Write(packet_id);
						new_packet.Write(rpc_id);
						new_packet.Write(new_hash);
						new_packet.WriteBits((unsigned char*)buffer, BYTES_TO_BITS(5));

						
						print_buf("new packet", new_packet.GetData(), new_packet.GetNumberOfBytesUsed());
						proxy->Send(&new_packet, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
						remote->DeallocatePacket(remote_packet);

						continue;
					}
				}

				//out.ResetReadPointer();
				//out.ResetWritePointer();

				proxy->Send(&out, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
			}

			remote->DeallocatePacket(remote_packet);
		}

		RakSleep(5);
	}
}
