#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FloodingMesh.h>
#include <TypeConversionFunctions.h>

#include "data.hpp"

bool received_callback(String &msg, FloodingMesh &meshInstance);

// A custom encryption key is required when using encrypted ESP-NOW transmissions. There is always a default Kok set, but it can be replaced if desired.
// All ESP-NOW keys below must match in an encrypted connection pair for encrypted communication to be possible.
// Note that it is also possible to use Strings as key seeds instead of arrays.
uint8_t espnow_encrypted_connection_key[16] = { 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44,  // This is the key for encrypting transmissions of encrypted connections.
																						 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x32, 0x11 };
uint8_t espnow_hash_key[16] = { 0xEF, 0x44, 0x33, 0x0C, 0x33, 0x44, 0xFE, 0x44,  // This is the secret key used for HMAC during encrypted connection requests.
															0x33, 0x44, 0x33, 0xB0, 0x33, 0x44, 0x32, 0xAD };

FloodingMesh mesh = FloodingMesh(received_callback, FPSTR(password), espnow_encrypted_connection_key,
								espnow_hash_key, FPSTR(ssid), MeshTypeConversionFunctions::uint64ToString(ESP.getChipId()), true);
String chip_id;                                
void setup() {
	WiFi.persistent(false);
	chip_id = String(ESP.getChipId());

	mesh.begin();
	mesh.activateAP();
}

void loop() {
	floodingMeshDelay(1);
}

bool received_callback(String &msg, FloodingMesh &meshInstance) {
    mesh.broadcast(chip_id + String('\t') + msg);
    return true;
}
