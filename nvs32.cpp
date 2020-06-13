#include "nvs32.h"


int8_t NVS::init(const char *partition, const char *name, int8_t debug=1)
{
    esp_err_t err;
    _debug = debug;

    err = nvs_flash_init_partition(partition);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to init partition [0x%x]", err);
        return 0;
    }

    err = nvs_open_from_partition(partition, name, NVS_READWRITE, &_handler);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to open namespace [0x%x]", err);
        return 0;
    }

    return 1;
}

int8_t NVS::erase_all()
{
    esp_err_t err = nvs_erase_all(_handler);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to erase all keys [0x%x]", err);
        return 0;
    }

    nvs_commit(_handler);
    return 1;
}

int8_t NVS::erase_key(const char *key)
{
    esp_err_t err = nvs_erase_key(_handler, key);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to erase key ['%s'] [0x%x]", key, err);
        return 0;
    }

    nvs_commit(_handler);
    return 1;
}



int8_t NVS::create(const char *key, const char *value)
{
    esp_err_t err;
    size_t sz;

    err = nvs_get_str(_handler, key, NULL, &sz);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        err = nvs_set_str(_handler, key, value);
        if (err != ESP_OK)
        {
            ESP_LOGE(tag, "Fail to create key ['%s'] [0x%x]", key, err);
            return 0;
        }

        nvs_commit(_handler);
        if (_debug)
        {
            ESP_LOGI(tag, "Key ['%s'] created", key);
        }

        return 1;
    }

    return 0;
}

int8_t NVS::create(const char *key, int32_t value)
{
    esp_err_t err;
    int32_t bff;

    err = nvs_get_i32(_handler, key, &bff);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        err = nvs_set_i32(_handler, key, value);
        if (err != ESP_OK)
        {
            ESP_LOGE(tag, "Fail to create key ['%s'] [0x%x]", key, err);
            return 0;
        }

        nvs_commit(_handler);
        if (_debug)
        {
            ESP_LOGI(tag, "Key ['%s'] created", key);
        }

        return 1;
    }

    return 0;
}



int8_t NVS::write(const char *key, const char *value)
{
    esp_err_t err = nvs_set_str(_handler, key, value);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to write key ['%s'] [0x%x]", key, err);
        return 0;
    }

    nvs_commit(_handler);
    if (_debug)
    {
        ESP_LOGI(tag, "['%s']: ['%s']", key, value);
    }

    return 1;
}

int8_t NVS::write(const char *key, int32_t value)
{
    esp_err_t err = nvs_set_i32(_handler, key, value);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to write key ['%s'] [0x%x]", key, err);
        return 0;
    }

    nvs_commit(_handler);
    if (_debug)
    {
        ESP_LOGI(tag, "['%s']: [%d]", key, value);
    }

    return 1;
}




int8_t NVS::read(const char *key, char *dst, uint16_t size)
{
    size_t sz; //Size of string allocated in nvs key.
    esp_err_t err;

    err = nvs_get_str(_handler, key, NULL, &sz);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to read key ['%s'] [0x%x]", key, err);
        return 0;
    }

    if (sz > size)
    {
        ESP_LOGE(tag, "Your buffer [%d] is smaller than necessary [%d]", size, sz);
        return 0;
    }

    err = nvs_get_str(_handler, key, dst, &sz);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail2 to read key ['%s'] [0x%x]", key, err);
        return 0;
    }

    return 1;
}

int8_t NVS::read(const char *key, int32_t *dst)
{
    esp_err_t err = nvs_get_i32(_handler, key, dst);
    if (err != ESP_OK)
    {
        ESP_LOGE(tag, "Fail to read key ['%s'] [0x%x]", key, err);
        return 0;
    }

    return 1;
}
