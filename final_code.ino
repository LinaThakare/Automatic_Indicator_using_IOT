#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ESP8266WiFi.h>
 
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
int LED1 =D6; //green
int LED2= D5; //red
int buzzer = D3;
int a=0;
int b=1;

const char* ssid = "Galaxy";
const char* password = "lina1920";

WiFiServer server(80);

void displaySensorDetails(void)
{
 sensor_t sensor;
 accel.getSensor(&sensor);
 Serial.println("------------------------------------");
 Serial.print ("Sensor: "); Serial.println(sensor.name);
 Serial.print ("Driver Ver: "); Serial.println(sensor.version);
 Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id);
 Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
 Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
 Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println(" m/s^2"); 
 Serial.println("------------------------------------");
 Serial.println("");
 delay(500);
}
 
void displayDataRate(void)
{
 Serial.print ("Data Rate: "); 
 
 switch(accel.getDataRate())
 {
 case ADXL345_DATARATE_3200_HZ:
 Serial.print ("3200 "); 
 break;
 case ADXL345_DATARATE_1600_HZ:
 Serial.print ("1600 "); 
 break;
 case ADXL345_DATARATE_800_HZ:
 Serial.print ("800 "); 
 break;
 case ADXL345_DATARATE_400_HZ:
 Serial.print ("400 "); 
 break;
 case ADXL345_DATARATE_200_HZ:
 Serial.print ("200 "); 
 break;
 case ADXL345_DATARATE_100_HZ:
 Serial.print ("100 "); 
 break;
 case ADXL345_DATARATE_50_HZ:
 Serial.print ("50 "); 
 break;
 case ADXL345_DATARATE_25_HZ:
 Serial.print ("25 "); 
 break;
 case ADXL345_DATARATE_12_5_HZ:
 Serial.print ("12.5 "); 
 break;
 case ADXL345_DATARATE_6_25HZ:
 Serial.print ("6.25 "); 
 break;
 case ADXL345_DATARATE_3_13_HZ:
 Serial.print ("3.13 "); 
 break;
 case ADXL345_DATARATE_1_56_HZ:
 Serial.print ("1.56 "); 
 break;
 case ADXL345_DATARATE_0_78_HZ:
 Serial.print ("0.78 "); 
 break;
 case ADXL345_DATARATE_0_39_HZ:
 Serial.print ("0.39 "); 
 break;
 case ADXL345_DATARATE_0_20_HZ:
 Serial.print ("0.20 "); 
 break;
 case ADXL345_DATARATE_0_10_HZ:
 Serial.print ("0.10 "); 
 break;
 default:
 Serial.print ("???? "); 
 break;
 } 
 Serial.println(" Hz"); 
}
 
void displayRange(void)
{
 Serial.print ("Range: +/- "); 
 
 switch(accel.getRange())
 {
 case ADXL345_RANGE_16_G:
 Serial.print ("16 "); 
 break;
 case ADXL345_RANGE_8_G:
 Serial.print ("8 "); 
 break;
 case ADXL345_RANGE_4_G:
 Serial.print ("4 "); 
 break;
 case ADXL345_RANGE_2_G:
 Serial.print ("2 "); 
 break;
 default:
 Serial.print ("?? "); 
 break;
 } 
 Serial.println(" g"); 
}
 
void setup(void) 
{
 Serial.begin(9600);
 Serial.print("Connecting to ");
 Serial.println(ssid);
 
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

 Serial.println("");
 Serial.println("NodeMCU is connected to WiFi");
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 server.begin();
 delay(3000);
  
 Serial.println("Accelerometer Test"); Serial.println("");
 
 /* Initialise the sensor */
 if(!accel.begin())
 {
 /* There was a problem detecting the ADXL345 ... check your connections */
 Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
 while(1);
 }
 
 /* Set the range to whatever is appropriate for your project */
 accel.setRange(ADXL345_RANGE_16_G);
 // displaySetRange(ADXL345_RANGE_8_G);
 // displaySetRange(ADXL345_RANGE_4_G);
 // displaySetRange(ADXL345_RANGE_2_G);
 
 /* Display some basic information on this sensor */
 displaySensorDetails();
 
 /* Display additional settings (outside the scope of sensor_t) */
 displayDataRate();
 displayRange();
 Serial.println("");
 pinMode(LED1, OUTPUT);
 pinMode(LED2, OUTPUT);
 pinMode(buzzer, OUTPUT);
}
 
void loop(void) 
{
  WiFiClient client;
  client = server.available();
  
 /* Get a new sensor event */ 
 sensors_event_t event; 
 accel.getEvent(&event);

 if (client == 1) {
    String request = client.readStringUntil('\n');
    client.flush();
    Serial.println(request);
    if (request.indexOf("wifion")!=-1){
      WiFi.begin(ssid, password);
      Serial.println("WiFi is ON");
    }
    if (request.indexOf("wifioff")!=-1){
      WiFi.disconnect();
      Serial.println("WiFi is OFF");
      Serial.println("Client disonnected");
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); 
  
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title> WIFI System </title>");
    client.println("<script type=\"text/javascript\">");
    client.println("function showMessageon()");
    client.println("{");
    client.println("alert(\"WIFI IS CONNECTED.\");");
    client.println("}");
    client.println("function showMessageoff()") ;
    client.println("{");
    client.println("alert(\"WIFI IS DISCONNECTED.\");");
    client.println("}");
    client.println("</script>");
    client.println("</head>");
    client.println("<body align= \"center\" >");
    client.println("<h1> <font size = \"25\" face = \"Cooper\" >  NodeMCU and Wifi Network  </font> </h1> ");
    client.println(" <a href=\"/wifion\">  <button style=\"height:60px; background-color: yellow; width:200px; cursor: pointer\" id=\"btnShowMsg\" onClick=\"showMessageon()\"> ON  </button> <a> </font> ");
    client.println(" <a href=\"/wifioff\"> <button style=\"height:60px; background-color: yellow; width:200px; cursor: pointer\" id=\"btnShowMsg\" onClick=\"showMessageoff()\"> OFF </button> <a> </font> ");
    client.println("</body>");
    client.println("</html>");

    Serial.println("------------------");
    Serial.println("                  ");
 }

 /* Display the results (acceleration is measured in m/s^2) */
 if (event.acceleration.x>1.00 and event.acceleration.x>-1.00){
 digitalWrite(LED1, HIGH);// turn the LED on
 digitalWrite(LED2, LOW);// turn the LED off
 delay(100); // wait for a second
 digitalWrite(LED1, LOW); // turn the LED off
 delay(10); // wait for a second
 if (a==0)
    {tone(buzzer,20000,500);
    }
 }

 if (event.acceleration.y>-1.00 and event.acceleration.y>1.00){
 digitalWrite(LED2, HIGH); // turn the LED on
 digitalWrite(LED1, LOW); // turn the LED off
 delay(100); // wait for a second
 digitalWrite(LED2, LOW); // turn the LED off
 delay(10); // wait for a second
 if (b==1)
    {tone(buzzer,20000,500);
    }
 }
 
 Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" ");
 Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
 Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");Serial.println("m/s^2 ");
 delay(500);
}
