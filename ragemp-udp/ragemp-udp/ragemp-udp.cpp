#include <iostream>
#include <cstdlib>
#include <chrono>

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

	std::vector<BitStream> unsended;
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
				// print_buf("from proxy client", (unsigned char*)proxy_packet->data, proxy_packet->length);
				BitStream out((unsigned char *)proxy_packet->data, proxy_packet->length, true);

				if (connected_to_remote)
				{
					remote->Send(&out, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);
				}
				else
				{
					BitStream bs;
					bs.Write(out);
					unsended.push_back(bs);
					// unsended.Write(out);
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
				remote->Connect(remote_addr.c_str(), remote_port, 0, 0, &public_key);
			}

			if ((int)remote_packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			{
				std::cout << "connected to remote host" << std::endl;
				connected_to_remote = true;
				// remote->Send(&unsended, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);

				while (unsended.size() > 0)
				{
					auto last = unsended.back();
					remote->Send(&last, HIGH_PRIORITY, RELIABLE, 0, remote_address, false);
					unsended.pop_back();

					std::cout << "send unseded packet" << std::endl;
				}
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

						char *buffer = (char *)malloc(BITS_TO_BYTES(bits_to_read));

						// похоже на порт откуда качать говно
						// хост, если не указан fast-dl-host, то просто порт, иначе адрес
						out.ReadBits((unsigned char *)buffer, bits_to_read);

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

					if (rpc_id >= 0x02 && rpc_id <= 0x05)
					{
						// std::cout << "incoming RPC 0x" << rpc_id << std::endl;
						std::cout << "packet length bytes " << remote_packet->length << " bits " << remote_packet->bitSize << std::endl;

						// if (rpc_id == 0x02 || rpc_id == 0x03)
						// {
						// 	std::uint16_t id = 0xFFFF;
						// 	out.Read(id);

						// 	std::cout << "RPC " << rpc_id << " ID: " << id << std::endl;

						// 	out.ResetReadPointer();
						// }

						if (rpc_id == 0x03)
						{
							std::cout << "RPC 0x03" << std::hex << std::endl;

							std::uint16_t id = 0xFFFF;
							std::uint16_t size = 0;
							std::uint32_t unk_0 = 0;

							out.PrintHex();

							out.Read(id);
							out.IgnoreBytes(3); //
							out.Read(size);
							out.IgnoreBytes(size); // some string
							out.Read(unk_0);	   // uint32_t (some size?)

							if (unk_0 != 0)
							{
								out.Read(size);
								unsigned char *unk = (unsigned char *)malloc(size + 1);
								out.ReadBits(unk, size << 3);
								unk[size] = 0;

								std::cout << "unk string: " << unk << std::endl;
								std::cout << "remaining bits: " << out.GetNumberOfUnreadBits() << std::endl;
							}
							else
							{
								std::uint8_t unk_3 = 0;

								out.Read(unk_0);
								std::cout << "unk_0: " << unk_0 << std::endl;
								out.Read(size);
								std::cout << "unk_1: " << size << std::endl;
								out.Read(size);
								std::cout << "unk_2: " << size << std::endl;
								out.Read(unk_3);
								std::cout << "unk_3: " << unk_3 << std::endl;
							}
						}

						if (rpc_id == 0x04 || rpc_id == 0x05)
						{
							std::cout << "ignore 0x04 packet be interesting ..." << std::endl;

							std::uint16_t unk_0 = 0;
							unsigned char *unk_1 = (unsigned char *)malloc((0x58 >> 3) + 1);
							std::uint16_t unk_2 = 0;
							std::uint32_t unk_4 = 0;

							// block 0
							std::cout << "block 0" << std::endl
									  << std::endl;

							out.Read(unk_0);
							// out.IgnoreBits(0x58);
							out.ReadBits(unk_1, 0x58);
							out.Read(unk_2);

							unk_1[0x58 >> 3] = 0;

							std::cout << "unk_0: " << unk_0 << std::endl;
							std::cout << "unk_1: " << unk_1 << " hex " << std::hex << unk_1 << std::dec << std::endl;
							std::cout << "unk_2: " << unk_2 << std::endl;

							unsigned char *unk_3 = (unsigned char *)malloc(unk_2 + 1);
							out.ReadBits(unk_3, unk_2 << 3);

							unk_3[unk_2] = 0;

							std::cout << "unk_3: " << unk_3 << " hex " << std::hex << unk_3 << std::dec << std::endl;

							std::cout << "block 1" << std::endl
									  << std::endl;

							out.Read(unk_4);

							std::cout << "unk_4: " << unk_4 << std::endl;

							if ((unk_4 & 0xFF000000) == 0)
							{
								std::cout << "block 2" << std::endl
										  << std::endl;

								// first loop
								std::cout << "first loop count ";
								for (std::uint32_t idx = 0; idx < 0xC; idx++)
								{
									if (out.ReadBit())
									{
										std::cout << "1 ";
										out.IgnoreBits(0x18);
									}
								}
								std::cout << std::endl;

								// second loop
								std::cout << "second loop count ";
								for (std::uint32_t idx = 0; idx < 0x8; idx++)
								{
									if (out.ReadBit())
									{
										std::cout << "1 ";
										out.IgnoreBits(0x10);
									}
								}
								std::cout << std::endl;

								if (out.ReadBit())
								{
									// block 3
									std::cout << "another gg ..." << std::endl;
								}
								else
								{
									std::cout << "thanks god ..." << std::endl;

									// BitStream fixed((unsigned char *)remote_packet->data, out.GetReadOffset() >> 3, true);

									// proxy->Send(&fixed, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
									// remote->DeallocatePacket(remote_packet);

									// std::cout << "sent fixed " << rpc_id << " packet. new size " << (out.GetReadOffset() >> 3) << std::endl;

									// continue;
								}
							}
							else
							{
								std::cout << "gg" << std::endl;
							}

							out.ResetReadPointer();

							// continue;
						}

						// out.PrintHex();
						// out.PrintBits();
						// std::cout << std::endl;
					}

					// if (rpc_id == 0x2E) // 46 FUN_1400e1580
					// {
					// 	std::cout << "0x2E packet " << remote_packet->length << " " << remote_packet->bitSize << std::endl;
					// 	out.PrintHex();
					// }

					if (rpc_id <= 0x12)
					{
						std::cout << "incoming RPC 0x" << rpc_id << " length " << remote_packet->length << std::endl;

						// if (rpc_id != 0x04 && rpc_id != 0x05 && rpc_id != 0x07 && rpc_id != 0x06)
						// {
						// 	out.PrintHex();
						// 	std::cout << std::endl;
						// }
					}

					// if (remote_packet->length > 10000)
					// {
					// 	continue;
					// }
				}

				proxy->Send(&out, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
			}

			remote->DeallocatePacket(remote_packet);
		}

		RakSleep(5);
	}
}

/*

Packet handlers:
	- FUN_1400dad90 (called by FUN_1400dc490) possible 0x1E RPC packet
	- FUN_1400da8e0 (called by FUN_1400dc390) possible 0x12 RPC packet

Sends RPCs:
	- FUN_1400daaf0 (called by FUN_1400dc410) 0x33 RPC packet (also handles 0x33 incoming RPC)
	- FUN_1400d99c0 (called by FUN_1400dc710) 0x02 0x03 0x04 RPC packets (step by step) (also handles 0x32)
	- FUN_14008b850 (called by above) 0x04

C -> RPC_RequestJoin (0x03)		FUN_1400dd5b0
S -> RPC_ServerSettings (0x31)	FUN_1400dd5b0
C -> RPC_Join (0x32)			FUN_1400dc710

*/

// make clean && make && bin/ragemp-udp 80.66.82.88 22005 3268b9f243006c9e
// cargo +nightly run --release --package ragemp-http -- rage2.grand-rp.su:22006
