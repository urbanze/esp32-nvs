# ESP32 IDF NVS library
All functions have comments.\
**Attention:** .create() functions only create the key-pair if the key **not exist**. If it already exists with any value, the value will NOT be overwritten, the last value will remain.

## Simple example to create, read and write
```
NVS cfg;
cfg.init("nvs", "test"); //Init NVS partition.
cfg.create("wifi_sta_ssid", "1234567890"); //Create ONLY if key not exist in memory.

char password[32] = {0};
cfg.read("wifi_sta_ssid", password, 32); //Read nvs-string key up to 32B.

ESP_LOGI(__func__, "WiFi passowrd: [%s]", password);

//If password does not contain string "abc", rewrite new password.
if (strstr(password, "abc") == NULL)
{
    cfg.write("wifi_sta_ssid", "abc1234567890");
    esp_restart();
}
```
