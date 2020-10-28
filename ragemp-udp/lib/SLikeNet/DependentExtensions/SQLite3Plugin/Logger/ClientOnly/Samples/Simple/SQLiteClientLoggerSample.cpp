/*
 * This file was taken from RakNet 4.082.
 * Please see licenses/RakNet license.txt for the underlying license and related copyright.
 *
 * Modified work: Copyright (c) 2016-2018, SLikeSoft UG (haftungsbeschränkt)
 *
 * This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 * license found in the license.txt file in the root directory of this source tree.
 */

#include "slikenet/peerinterface.h"
#include "SQLiteClientLoggerPlugin.h"
#include "slikenet/BitStream.h"
#include "slikenet/sleep.h"

#include "slikenet/Kbhit.h"
#include "slikenet/GetTime.h"
#include "slikenet/PacketizedTCP.h"
#include "slikenet/types.h"
#include "slikenet/rand.h"
#define M_PI 3.14159265358979323846

int main(void)
{
	printf("Demonstration of SQLiteClientLoggerPlugin.\n");

	SLNet::PacketizedTCP packetizedTCP;
	SLNet::SQLiteClientLoggerPlugin loggerPlugin;
	packetizedTCP.AttachPlugin(&loggerPlugin);
	packetizedTCP.Start(0,0);
	printf("Connecting.\n");
	SLNet::SystemAddress serverAddress = packetizedTCP.Connect("127.0.0.1", 38123, true);
	printf("Connected.\n");


	/*
	int *heap1=1234, *heap2=2000;
	loggerPlugin.SetServerParameters(serverAddress, "memoryReport.sqlite");
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("Heaps/Heap1", "new", 1234, "Heap.cpp", heap1, 15000));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("Heaps/Heap2", "new", 1234, "Heap.cpp", heap2, 10000));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("General/Graphics/3D", "new", 1234, "3DRenderer.cpp", heap1, 500));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("General/Graphics/3D", "new", 1235, "3DRenderer.cpp", heap1+500, 500));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("General/Graphics/2D", "new", 1234, "3DRenderer.cpp", heap1+500*2, 500));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("General/Graphics/2D", "new", 666, "2DRenderer.cpp", heap2, 1000));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("General/Graphics/3D", "new", 668, "2DRenderer.cpp", heap2+1000, 1000));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("Uncategorized", "realloc", 50, "Main.cpp", heap1, -200));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("Uncategorized", "free", 50, "Main.cpp", heap2, -1000));
	rakSqlLog("memoryReport", "Category,Operation,Line,File,Address,Amount", ("General/Graphics/3D", "free", 800, "3DRenderer.cpp", heap2+1000, -1000));
	*/


	loggerPlugin.SetServerParameters(serverAddress, "functionLog.sqlite");

	SLNet::SQLLogResult res;
	int x=1;
	unsigned short y=2;
	float c=3;
	double d=4;
	char *e="HI";
	res = rakFnLog("My func", (x,y,c,d,e,&loggerPlugin));
	RakAssert(res== SLNet::SQLLR_OK);
	res = rakSqlLog("sqlLog", "handle, mapName, positionX, positionY, positionZ, gameMode, connectedPlayers", ("handle1", "mapname1", 1,2,3,"",4));
	RakAssert(res== SLNet::SQLLR_OK);
	res = rakSqlLog("sqlLog", "handle, mapName, positionX, positionY, positionZ, gameMode, connectedPlayers", ("handle2", "mapname2", 5,6,7,"gameMode2",8));
	RakAssert(res== SLNet::SQLLR_OK);
	res = rakSqlLog("sqlLog", "x", (999));
	RakAssert(res== SLNet::SQLLR_OK);
	res = rakFnLog("My func2", ("cat", "carrot", ""));
	RakAssert(res== SLNet::SQLLR_OK);
	loggerPlugin.IncrementAutoTickCount();

	loggerPlugin.SetServerParameters(serverAddress, "scatterPlot.sqlite");

	for (int i=0; i < 1000; i++)
	{
		res = rakSqlLog("ScatterPlot", "x, y, z, Color, Intensity", (i, (cosf((float)i/30.0f)+1.0f)*500.0f, (sinf((float)i/30.0f)+1.0f)*500.0f, (i%2)==0 ? "red" : "blue", frandomMT()));
		RakAssert(res== SLNet::SQLLR_OK);
		RakSleep(1);
		// Calling Receive() is something you should do anyway, and increments autotick count
		loggerPlugin.IncrementAutoTickCount();
	}

	loggerPlugin.IncrementAutoTickCount();
	loggerPlugin.SetServerParameters(serverAddress, "gradient.sqlite");

	double s;
	unsigned int *bytes = new unsigned int[256*256*256];
	for (int i=0; i < 4096; i++)
	{
		for (int j=0; j < 4096; j++)
		{
			int r,g,b;
			float intensity = 1.0f - (float)i/4096.0f;
			s = 2.0 * sin(((double)j/4096.0)*(2.0*M_PI));
			if (s>0.0)
			{
				if (s>1.0)
					s=1.0;
				r=static_cast<int>(255.0*s);
			//	r=static_cast<int>(pow(r/255.0,.5));
			}
			else
				r=0;
			s = 2.0 * sin(((double)j/4096.0)*(2.0*M_PI)+2.0*M_PI/3.0);
			if (s>0.0)
			{
				if (s>1.0)
					s=1.0;
				g=static_cast<int>(255.0*s);
			//	g=static_cast<int>(pow(g/255.0,.5));
			}
			else
				g=0;
			s = 2.0 * sin(((double)j/4096.0)*(2.0*M_PI)+4.0*M_PI/3.0);
			if (s>0.0)
			{
				if (s>1.0)
					s=1.0;
				b=static_cast<int>(255.0*s);
			//	b=static_cast<int>(pow(b/255.0,.5));
			}
			else
				b=0;

			if (intensity>.5)
			{
				r=static_cast<int>(255.0-((255.0-(double)r)*(255-(2*(intensity-.5))*255.0)) /256.0);
				g=static_cast<int>(255.0-((255.0-(double)g)*(255-(2*(intensity-.5))*255.0)) /256.0);
				b=static_cast<int>(255.0-((255.0-(double)b)*(255-(2*(intensity-.5))*255.0)) /256.0);
			}
			else
			{
				r=static_cast<int>((intensity*(double)r*2.0));
				g=static_cast<int>((intensity*(double)g*2.0));
				b=static_cast<int>((intensity*(double)b*2.0));
			}
			bytes[i*4096+j]=(r)|(g<<8)|(b<<16);
		}
	}

	SLNet::RGBImageBlob imageblob(bytes, 4096, 4096, 4096 * 4, 4);
	// #med - SLNet::SQLiteClientLoggerPlugin::ParameterListHelper requires proper const pointer handling
	rakSqlLog("gradient", "gradientImage", (&imageblob));
	delete [] bytes;

	RakSleep(5000);

	return 1;
}
