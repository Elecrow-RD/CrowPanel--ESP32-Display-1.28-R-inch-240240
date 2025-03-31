#define LGFX_USE_V1
#include "Arduino.h"
#include <lvgl.h>
#include "demos/lv_demos.h"
#include <LovyanGFX.hpp>
#include "CST816D.h"
#include <WiFi.h>
#include <AsyncMqttClient.h>

#define WIFI_SSID "yanfa_software"
#define WIFI_PASSWORD "yanfa-123456"

// Raspberry Pi Mosquitto MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 50, 177)

// For a cloud MQTT broker, type the domain name
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883

// Temperature MQTT Topics
#define MQTT_PUB_Vibration_Motor_S  "esp32/VibrationMotor/state"
#define MQTT_PUB_Vibration_Motor_C "esp32/VibrationMotor/command"
#define mqtt_username  "sea"
#define mqtt_password  "lisihai525"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// I2C pins
#define I2C_SDA 4
#define I2C_SCL 5
// Touch panel interrupt and reset pins
#define TP_INT 0
#define TP_RST -1

// I2C address for the I/O extender
#define PI4IO_I2C_ADDR 0x43

// Buffer size definition
#define buf_size 120

char Vibration_Motor_flag = 0;
char Vibration_Motor_flag_Lock = 0;
//char Change_Color_flag_Lock = 0;
// LGFX class definition for display device interface
class LGFX : public lgfx::LGFX_Device
{
    // Internal components of the LGFX class
    lgfx::Panel_GC9A01 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
  public:
    LGFX(void)
    {
      {
        // Configuration for the SPI bus and panel
        auto cfg = _bus_instance.config();
        cfg.spi_host = SPI2_HOST;
        cfg.spi_mode = 0;
        cfg.freq_write = 80000000;
        cfg.freq_read = 20000000;
        cfg.spi_3wire = true;
        cfg.use_lock = true;
        cfg.dma_channel = SPI_DMA_CH_AUTO;
        cfg.pin_sclk = 6;
        cfg.pin_mosi = 7;
        cfg.pin_miso = -1;
        cfg.pin_dc = 2;
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }
      {
        auto cfg = _panel_instance.config();
        cfg.pin_cs = 10;
        cfg.pin_rst = -1;
        cfg.pin_busy = -1;
        cfg.memory_width = 240;
        cfg.memory_height = 240;
        cfg.panel_width = 240;
        cfg.panel_height = 240;
        cfg.offset_x = 0;
        cfg.offset_y = 0;
        cfg.offset_rotation = 0;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits = 1;
        cfg.readable = false;
        cfg.invert = true;
        cfg.rgb_order = false;
        cfg.dlen_16bit = false;
        cfg.bus_shared = false;
        _panel_instance.config(cfg);
      }
      setPanel(&_panel_instance);
    }
};

// Global instances for the display and touch screen
LGFX tft;
CST816D touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

/*Change to your screen resolution*/
static const uint32_t screenWidth = 240;
static const uint32_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][screenWidth * buf_size];

#if LV_USE_LOG != 0
// Custom print function for LVGL debugging
void my_print(lv_log_level_t level, const char *file, uint32_t line, const char *fn_name, const char *dsc)
{
  Serial.printf("%s(%s)@%d->%s\r\n", file, fn_name, line, dsc);
  Serial.flush();
}
#endif

// Function to handle flushing of display buffer to the screen
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  if (tft.getStartCount() == 0)
  {
    tft.endWrite();
  }

  tft.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::swap565_t *)&color_p->full);

  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

// Function to read touchpad input
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  bool touched;
  uint8_t gesture;
  uint16_t touchX, touchY;
  touched = touch.getTouch(&touchX, &touchY, &gesture);
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

// Function to initialize I/O extender
void init_IO_extender() {
  Wire.beginTransmission(PI4IO_I2C_ADDR);
  Wire.write(0x01); // test register
  Wire.endTransmission();
  Wire.requestFrom(PI4IO_I2C_ADDR, 1);
  uint8_t rxdata = Wire.read();
  Serial.print("Device ID: ");
  Serial.println(rxdata, HEX);

  Wire.beginTransmission(PI4IO_I2C_ADDR);
  Wire.write(0x03); // IO direction register
  Wire.write((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4)); // set pins 0, 1, 2 as outputs
  Wire.endTransmission();

  Wire.beginTransmission(PI4IO_I2C_ADDR);
  Wire.write(0x07); // Output Hi-Z register
  Wire.write(~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4))); // set pins 0, 1, 2 low
  Wire.endTransmission();
}

// Function to set I/O pin state on the extender
void set_pin_io(uint8_t pin_number, bool value) {

  Wire.beginTransmission(PI4IO_I2C_ADDR);
  Wire.write(0x05); // test register
  Wire.endTransmission();
  Wire.requestFrom(PI4IO_I2C_ADDR, 1);
  uint8_t rxdata = Wire.read();
  //  Serial.print("Before the change: ");
  //  Serial.println(rxdata, HEX);

  Wire.beginTransmission(PI4IO_I2C_ADDR);
  Wire.write(0x05); // Output register

  if (!value)
    Wire.write((~(1 << pin_number)) & rxdata); // set pin low
  else
    Wire.write((1 << pin_number) | rxdata); // set pin high
  Wire.endTransmission();

  Wire.beginTransmission(PI4IO_I2C_ADDR);
  Wire.write(0x05); // test register
  Wire.endTransmission();
  Wire.requestFrom(PI4IO_I2C_ADDR, 1);
  rxdata = Wire.read();
  //  Serial.print("after the change: ");
  //  Serial.println(rxdata, HEX);
}


void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe(MQTT_PUB_Vibration_Motor_C, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

lv_obj_t * sw1;

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  Serial.println("Publish received.");
  for (int i = 0; i < len; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println("");
  if (strncmp(payload, "ON", 2) == 0) {
    Vibration_Motor_flag_Lock = 1;
    Vibration_Motor_flag = 1;
    mqttClient.publish(MQTT_PUB_Vibration_Motor_S, 0, true, "ON");
    lv_obj_add_state(sw1, LV_STATE_CHECKED);
  }
  if (strncmp(payload, "OFF", 3) == 0) {
    Vibration_Motor_flag_Lock = 1;
    Vibration_Motor_flag = 0;
    mqttClient.publish(MQTT_PUB_Vibration_Motor_S, 0, true, "OFF");
    lv_obj_clear_state(sw1, LV_STATE_CHECKED);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup()
{
  // Initialization of serial communication, I2C, display, touch screen, and LVGL
  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.println("I am LVGL_Arduino");
  Wire.begin(4, 5);
  init_IO_extender();
  delay(100);
  set_pin_io(3, true);
  set_pin_io(4, true);
  set_pin_io(2, true);

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  WiFi.onEvent(WiFiEvent);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  mqttClient.setCredentials(mqtt_username, mqtt_password);

  connectToWifi();

  tft.init();
  tft.initDMA();
  tft.startWrite();
  tft.setColor(0, 0, 0);

  tft.fillScreen(TFT_BLACK);
  delay(200);
  touch.begin();
  lv_init();
#if LV_USE_LOG != 0
  //lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  lv_disp_draw_buf_init(&draw_buf, buf[0], buf[1], screenWidth * buf_size);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);


#if 0
  /* Create simple label */
  lv_obj_t *label = lv_label_create( lv_scr_act() );
  lv_label_set_text( label, "Hello Arduino! (V8.0.X)" );
  lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else
  // uncomment one of these demos
  //lv_demo_widgets(); // OK
  lvgl_switch_event_test();
#endif
  Serial.println("Setup done");
}


void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  if (Vibration_Motor_flag_Lock == 1)
  {
    if (Vibration_Motor_flag == 1)
    {
      Vibration_Motor_flag_Lock = 0;
      set_pin_io(0, true);
      mqttClient.publish(MQTT_PUB_Vibration_Motor_S, 0, true, "ON");
    }
    else
    {
      Vibration_Motor_flag_Lock = 0;
      set_pin_io(0, false);
      mqttClient.publish(MQTT_PUB_Vibration_Motor_S, 0, true, "OFF");
    }
  }
}

static void switch_event_test_handler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    //Serial.println("State: %s\n", lv_switch_get_state(obj) ? "On" : "Off");
    Vibration_Motor_flag = !Vibration_Motor_flag;
    Vibration_Motor_flag_Lock = 1;
  }
}

void lvgl_switch_event_test(void)
{
  lv_obj_t * label;
  /*Create a switch and apply the styles*/
  sw1 = lv_switch_create(lv_scr_act());
  lv_obj_set_width( sw1, 150);
  lv_obj_set_height( sw1, 25);
  lv_obj_add_event_cb(sw1, switch_event_test_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(sw1, LV_ALIGN_CENTER, 0, 0);
  label = lv_label_create(sw1);
  lv_label_set_text(label, "Vibration_Motor");
  lv_obj_center(label);
}
