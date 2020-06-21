/*
 Name:		TestStdIR.ino
 Created:	3/15/2020 11:32:23 PM
 Author:	MCP
*/

#include "NYG.h"
#include "StdIR.h"

using namespace NYG;

int RECV_PIN = 2;        // Infrared receiving pin
IRrecv receiver(RECV_PIN); // Create a class object used to receive class

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200); 
	Serial.println("UNO is ready!!!");
	receiver.enableIRIn();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	StdIR::Key key;
	if (StdIR::Recv(receiver, key))
		Serial.println(StdIR::GetName(key));
}
