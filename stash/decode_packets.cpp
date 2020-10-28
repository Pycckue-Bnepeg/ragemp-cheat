// if (rpc_id >= 0x02 && rpc_id <= 0x05)
// {
// 	// std::cout << "incoming RPC 0x" << rpc_id << std::endl;
// 	std::cout << "packet length bytes " << remote_packet->length << " bits " << remote_packet->bitSize << std::endl;

// 	// if (rpc_id == 0x02 || rpc_id == 0x03)
// 	// {
// 	// 	std::uint16_t id = 0xFFFF;
// 	// 	out.Read(id);

// 	// 	std::cout << "RPC " << rpc_id << " ID: " << id << std::endl;

// 	// 	out.ResetReadPointer();
// 	// }

// 	if (rpc_id == 0x03)
// 	{
// 		std::cout << "RPC 0x03" << std::hex << std::endl;

// 		std::uint16_t id = 0xFFFF;
// 		std::uint16_t size = 0;
// 		std::uint32_t unk_0 = 0;

// 		out.PrintHex();

// 		out.Read(id);
// 		out.IgnoreBytes(3); //
// 		out.Read(size);
// 		out.IgnoreBytes(size); // some string
// 		out.Read(unk_0);	   // uint32_t (some size?)

// 		if (unk_0 != 0)
// 		{
// 			out.Read(size);
// 			unsigned char *unk = (unsigned char *)malloc(size + 1);
// 			out.ReadBits(unk, size << 3);
// 			unk[size] = 0;

// 			std::cout << "unk string: " << unk << std::endl;
// 			std::cout << "remaining bits: " << out.GetNumberOfUnreadBits() << std::endl;
// 		}
// 		else
// 		{
// 			std::uint8_t unk_3 = 0;

// 			out.Read(unk_0);
// 			std::cout << "unk_0: " << unk_0 << std::endl;
// 			out.Read(size);
// 			std::cout << "unk_1: " << size << std::endl;
// 			out.Read(size);
// 			std::cout << "unk_2: " << size << std::endl;
// 			out.Read(unk_3);
// 			std::cout << "unk_3: " << unk_3 << std::endl;
// 		}
// 	}

// 	if (rpc_id == 0x04 || rpc_id == 0x05)
// 	{
// 		std::cout << "ignore 0x04 packet be interesting ..." << std::endl;

// 		std::uint16_t unk_0 = 0;
// 		unsigned char *unk_1 = (unsigned char *)malloc((0x58 >> 3) + 1);
// 		std::uint16_t unk_2 = 0;
// 		std::uint32_t unk_4 = 0;

// 		// block 0
// 		std::cout << "block 0" << std::endl
// 				  << std::endl;

// 		out.Read(unk_0);
// 		// out.IgnoreBits(0x58);
// 		out.ReadBits(unk_1, 0x58);
// 		out.Read(unk_2);

// 		unk_1[0x58 >> 3] = 0;

// 		std::cout << "unk_0: " << unk_0 << std::endl;
// 		std::cout << "unk_1: " << unk_1 << " hex " << std::hex << unk_1 << std::dec << std::endl;
// 		std::cout << "unk_2: " << unk_2 << std::endl;

// 		unsigned char *unk_3 = (unsigned char *)malloc(unk_2 + 1);
// 		out.ReadBits(unk_3, unk_2 << 3);

// 		unk_3[unk_2] = 0;

// 		std::cout << "unk_3: " << unk_3 << " hex " << std::hex << unk_3 << std::dec << std::endl;

// 		std::cout << "block 1" << std::endl
// 				  << std::endl;

// 		out.Read(unk_4);

// 		std::cout << "unk_4: " << unk_4 << std::endl;

// 		if ((unk_4 & 0xFF000000) == 0)
// 		{
// 			std::cout << "block 2" << std::endl
// 					  << std::endl;

// 			// first loop
// 			std::cout << "first loop count ";
// 			for (std::uint32_t idx = 0; idx < 0xC; idx++)
// 			{
// 				if (out.ReadBit())
// 				{
// 					std::cout << "1 ";
// 					out.IgnoreBits(0x18);
// 				}
// 			}
// 			std::cout << std::endl;

// 			// second loop
// 			std::cout << "second loop count ";
// 			for (std::uint32_t idx = 0; idx < 0x8; idx++)
// 			{
// 				if (out.ReadBit())
// 				{
// 					std::cout << "1 ";
// 					out.IgnoreBits(0x10);
// 				}
// 			}
// 			std::cout << std::endl;

// 			if (out.ReadBit())
// 			{
// 				// block 3
// 				std::cout << "another gg ..." << std::endl;
// 			}
// 			else
// 			{
// 				std::cout << "thanks god ..." << std::endl;

// 				// BitStream fixed((unsigned char *)remote_packet->data, out.GetReadOffset() >> 3, true);

// 				// proxy->Send(&fixed, HIGH_PRIORITY, RELIABLE, 0, client_address, false);
// 				// remote->DeallocatePacket(remote_packet);

// 				// std::cout << "sent fixed " << rpc_id << " packet. new size " << (out.GetReadOffset() >> 3) << std::endl;

// 				// continue;
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "gg" << std::endl;
// 		}

// 		out.ResetReadPointer();

// 		// continue;
// 	}

// 	// out.PrintHex();
// 	// out.PrintBits();
// 	// std::cout << std::endl;
// }

// // if (rpc_id == 0x2E) // 46 FUN_1400e1580
// // {
// // 	std::cout << "0x2E packet " << remote_packet->length << " " << remote_packet->bitSize << std::endl;
// // 	out.PrintHex();
// // }

// if (rpc_id <= 0x12)
// {
// 	std::cout << "incoming RPC 0x" << rpc_id << " length " << remote_packet->length << std::endl;

// 	// if (rpc_id != 0x04 && rpc_id != 0x05 && rpc_id != 0x07 && rpc_id != 0x06)
// 	// {
// 	// 	out.PrintHex();
// 	// 	std::cout << std::endl;
// 	// }
// }

// // if (remote_packet->length > 10000)
// // {
// // 	continue;
// // }

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
