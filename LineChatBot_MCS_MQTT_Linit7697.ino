#include <SPI.h>
#include <LWiFi.h>
#include <PubSubClient.h>
#define USER_NAME "your user name"             //裝置認證名稱
#define USER_PASS "your user password"        //裝置認證密碼
#define CLIENT_ID  "your client id"                     //裝置認證ID      
/***************/
// Update these with values suitable for your network.
IPAddress server(52,220,130,27);       //MCS Server
char data[80];
char message1[256];

#define LED_PIN 7

char ssid[] = "Xperia XZ_9fcf";              //網路名稱
char pass[] = "77777777";    //網路密碼
int keyIndex = 0; 
int status = WL_IDLE_STATUS;
WiFiClient client;
PubSubClient upload(client);
void callback(char* topic, byte* payload, unsigned int length) {
                                                            //從QIoT訂閱資料  處理資料回傳動作
  Serial.print(z"Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    message1[i]=payload[i];
  }
  Serial.println(message1[length-1]);
  if(message1[length-1]=='1'){
    Serial.println("Turn on the Light");
    digitalWrite(LED_PIN,HIGH);
  }else if(message1[length-1]=='0'){
    Serial.println("Turn off the Light");
    digitalWrite(LED_PIN,LOW);
     }
}
void reconnect() {  //重新連線
  // Loop until we're reconnected
  while (!upload.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (upload.connect(CLIENT_ID, USER_NAME, USER_PASS )) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(upload.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{  Serial.begin(9600);
    pinMode(LED_PIN,OUTPUT);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();
  upload.setServer("mqtt.mcs.mediatek.com", 1883);    //設定與server的連線port
  upload.setCallback(callback);        //設定執行callback 副程式
  delay(1500);
}
void loop()
{
  if (!upload.connected()) {
    reconnect();
  }
  else{     //從QIoT訂閱資料 而topic會在驗證後resource 得到
    upload.subscribe("mcs/DAIiPdZ0/c1acrs2ohoiNouvW/LED_Control");    // 訂閱qiot 主題 
    delay(100);
  }
  upload.loop();
  delay(1000);

}
void printWifiStatus() {  // wifi 連線資訊顯示
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

