#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "forecast_record.h"
#include "common_functions.h"

//#########################################################################################
void Convert_Readings_to_Imperial() {
  WxConditions[0].Pressure = hPa_to_inHg(WxConditions[0].Pressure);
  // WxForecast[1].Rainfall   = mm_to_inches(WxForecast[1].Rainfall);
  // WxForecast[1].Snowfall   = mm_to_inches(WxForecast[1].Snowfall);
}


struct OWMWeather {
  String main;         // OWM Main category (e.g., "Rain", "Clear")
  String description;  // Detailed description (e.g., "light rain")
  String icon;         // Icon filename (e.g., "10d")
};

OWMWeather GetOWMFromWMO(int wmoCode) {
  switch (wmoCode) {
    // Clear sky
    case 0:  return {"Clear", "clear sky", "01d"};

    // Mainly clear, partly cloudy, overcast
    case 1:  return {"Clouds", "mainly clear", "02d"};
    case 2:  return {"Clouds", "partly cloudy", "03d"};
    case 3:  return {"Clouds", "overcast clouds", "04d"};

    // Fog and depositing rime fog
    case 45: return {"Fog", "fog", "50d"};
    case 48: return {"Fog", "depositing rime fog", "50d"};

    // Drizzle
    case 51: return {"Drizzle", "light drizzle", "09d"};
    case 53: return {"Drizzle", "moderate drizzle", "09d"};
    case 55: return {"Drizzle", "dense drizzle", "09d"};

    // Freezing drizzle
    case 56: return {"Drizzle", "light freezing drizzle", "09d"};
    case 57: return {"Drizzle", "dense freezing drizzle", "09d"};

    // Rain
    case 61: return {"Rain", "slight rain", "10d"};
    case 63: return {"Rain", "moderate rain", "10d"};
    case 65: return {"Rain", "heavy rain", "10d"};

    // Freezing rain
    case 66: return {"Rain", "light freezing rain", "13d"};
    case 67: return {"Rain", "heavy freezing rain", "13d"};

    // Snowfall
    case 71: return {"Snow", "slight snowfall", "13d"};
    case 73: return {"Snow", "moderate snowfall", "13d"};
    case 75: return {"Snow", "heavy snowfall", "13d"};

    // Snow grains
    case 77: return {"Snow", "snow grains", "13d"};

    // Rain showers
    case 80: return {"Rain", "slight rain showers", "09d"};
    case 81: return {"Rain", "moderate rain showers", "09d"};
    case 82: return {"Rain", "violent rain showers", "09d"};

    // Snow showers
    case 85: return {"Snow", "slight snow showers", "13d"};
    case 86: return {"Snow", "heavy snow showers", "13d"};

    // Thunderstorm
    case 95: return {"Thunderstorm", "slight or moderate thunderstorm", "11d"};
    case 96: return {"Thunderstorm", "thunderstorm with slight hail", "11d"};
    case 99: return {"Thunderstorm", "thunderstorm with heavy hail", "11d"};

    // Unknown or not mapped
    default: return {"Unknown", "unknown weather condition", "nodata"};
  }
}

String GetOWMIconFromWMO(int wmoCode, bool isDay) {
  OWMWeather weather = GetOWMFromWMO(wmoCode);
  return isDay ? weather.icon : weather.icon.substring(0, 2) + "n";
}


//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
bool DecodeWeather(WiFiClient& json) {
  Serial.print(F("\nCreating object...and "));
  // allocate the JsonDocument
  JsonDocument doc;
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  Serial.println(" Decoding data");
  // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
  // WxConditions[0].lon         = root["coord"]["lon"].as<float>();                    Serial.println(" Lon: "+String(WxConditions[0].lon));
  // WxConditions[0].lat         = root["coord"]["lat"].as<float>();                    Serial.println(" Lat: "+String(WxConditions[0].lat));
  // WxConditions[0].Main0       = root["weather"][0]["main"].as<const char*>();        Serial.println("Main: "+String(WxConditions[0].Main0));
  // WxConditions[0].Forecast0   = root["weather"][0]["description"].as<const char*>(); Serial.println("For0: "+String(WxConditions[0].Forecast0));
  // WxConditions[0].Forecast1   = root["weather"][1]["description"].as<const char*>(); Serial.println("For1: "+String(WxConditions[0].Forecast1));
  // WxConditions[0].Forecast2   = root["weather"][2]["description"].as<const char*>(); Serial.println("For2: "+String(WxConditions[0].Forecast2));
  // WxConditions[0].Icon        = root["weather"][0]["icon"].as<const char*>();        Serial.println("Icon: "+String(WxConditions[0].Icon));
  // WxConditions[0].Temperature = root["main"]["temp"].as<float>();                    Serial.println("Temp: "+String(WxConditions[0].Temperature));
  // WxConditions[0].Pressure    = root["main"]["pressure"].as<float>();                Serial.println("Pres: "+String(WxConditions[0].Pressure));
  // WxConditions[0].Humidity    = root["main"]["humidity"].as<float>();                Serial.println("Humi: "+String(WxConditions[0].Humidity));
  // WxConditions[0].Low         = root["main"]["temp_min"].as<float>();                Serial.println("TLow: "+String(WxConditions[0].Low));
  // WxConditions[0].High        = root["main"]["temp_max"].as<float>();                Serial.println("THig: "+String(WxConditions[0].High));
  // WxConditions[0].Windspeed   = root["wind"]["speed"].as<float>();                   Serial.println("WSpd: "+String(WxConditions[0].Windspeed));
  // WxConditions[0].Winddir     = root["wind"]["deg"].as<float>();                     Serial.println("WDir: "+String(WxConditions[0].Winddir));
  // WxConditions[0].Cloudcover  = root["clouds"]["all"].as<int>();                     Serial.println("CCov: "+String(WxConditions[0].Cloudcover)); // in % of cloud cover
  // WxConditions[0].Visibility  = root["visibility"].as<int>();                        Serial.println("Visi: "+String(WxConditions[0].Visibility)); // in metres
  // WxConditions[0].Rainfall    = root["rain"]["1h"].as<float>();                      Serial.println("Rain: "+String(WxConditions[0].Rainfall));
  // WxConditions[0].Snowfall    = root["snow"]["1h"].as<float>();                      Serial.println("Snow: "+String(WxConditions[0].Snowfall));
  // WxConditions[0].Country     = root["sys"]["country"].as<const char*>();            Serial.println("Ctry: "+String(WxConditions[0].Country));
  // WxConditions[0].Sunrise     = root["sys"]["sunrise"].as<int>();                    Serial.println("SRis: "+String(WxConditions[0].Sunrise));
  // WxConditions[0].Sunset      = root["sys"]["sunset"].as<int>();                     Serial.println("SSet: "+String(WxConditions[0].Sunset));
  // WxConditions[0].Timezone    = root["timezone"].as<int>();                          Serial.println("TZon: "+String(WxConditions[0].Timezone));
  WxConditions[0].Dt          = root["current"]["time"].as<int>();                    Serial.println("Time: "+String(WxConditions[0].Dt));
  WxConditions[0].lon         = root["longitude"].as<float>();                    Serial.println(" Lon: "+String(WxConditions[0].lon));
  WxConditions[0].lat         = root["latitude"].as<float>();                    Serial.println(" Lat: "+String(WxConditions[0].lat));
  // WxConditions[0].Main0       = root["current"]["weather_code"].as<int>();        Serial.println("Main: "+String(WxConditions[0].Main0));
  WxConditions[0].Main0       = GetOWMFromWMO(root["current"]["weather_code"].as<int>()).main; Serial.println("Main: "+String(WxConditions[0].Main0));
  WxConditions[0].Forecast0   = WxConditions[0].Main0; Serial.println("For0: "+String(WxConditions[0].Forecast0));
  WxConditions[0].Forecast1   = WxConditions[0].Main0; Serial.println("For1: "+String(WxConditions[0].Forecast1));
  WxConditions[0].Forecast2   = WxConditions[0].Main0; Serial.println("For2: "+String(WxConditions[0].Forecast2));
  //WxConditions[0].isDay       = root["current"]["is_day"].as<bool>();        Serial.println("Day : "+String(WxConditions[0].isDay));
  WxConditions[0].Icon        = GetOWMIconFromWMO(root["current"]["weather_code"].as<int>(), root["current"]["is_day"].as<bool>()); Serial.println("Icon: "+String(WxConditions[0].Icon));
  WxConditions[0].Temperature = root["current"]["temperature_2m"].as<float>();                    Serial.println("Temp: "+String(WxConditions[0].Temperature));
  WxConditions[0].Pressure    = root["current"]["pressure_msl"].as<float>();                Serial.println("Pres: "+String(WxConditions[0].Pressure));
  WxConditions[0].Humidity    = root["current"]["relative_humidity_2m"].as<float>();                Serial.println("Humi: "+String(WxConditions[0].Humidity));
  WxConditions[0].Low         = root["daily"]["temperature_2m_max"][0].as<float>();                Serial.println("TLow: "+String(WxConditions[0].Low));
  WxConditions[0].High        = root["daily"]["temperature_2m_min"][0].as<float>();                Serial.println("THig: "+String(WxConditions[0].High));
  WxConditions[0].Windspeed   = root["current"]["wind_speed_10m"].as<float>();                   Serial.println("WSpd: "+String(WxConditions[0].Windspeed));
  WxConditions[0].Winddir     = root["current"]["wind_direction_10m"].as<float>();                     Serial.println("WDir: "+String(WxConditions[0].Winddir));
  WxConditions[0].Cloudcover  = root["current"]["cloud_cover"].as<int>();                     Serial.println("CCov: "+String(WxConditions[0].Cloudcover)); // in % of cloud cover
  //Todo: get current timestamp to get visibility from hourly data
  WxConditions[0].Visibility  = 0;                        Serial.println("Visi: "+String(WxConditions[0].Visibility)); // in metres
  WxConditions[0].Rainfall    = root["current"]["rain"].as<float>();                      Serial.println("Rain: "+String(WxConditions[0].Rainfall));
  WxConditions[0].Snowfall    = root["current"]["snowfall"].as<float>();                      Serial.println("Snow: "+String(WxConditions[0].Snowfall));
  WxConditions[0].Country     = Country;            Serial.println("Ctry: "+String(WxConditions[0].Country));
  WxConditions[0].Sunrise     = root["daily"]["sunrise"][0].as<int>();                    Serial.println("SRis: "+String(WxConditions[0].Sunrise));
  WxConditions[0].Sunset      = root["daily"]["sunset"][0].as<int>();                     Serial.println("SSet: "+String(WxConditions[0].Sunset));
  //in utc_offset_seconds
  WxConditions[0].Timezone    = root["utc_offset_seconds"].as<int>();                          Serial.println("TZon: "+String(WxConditions[0].Timezone));
  
  
  //Serial.println(json);
  Serial.print(F("\nReceiving Forecast period - ")); //------------------------------------------------
  //JsonArray list                    = root["list"];
  for (byte r = 0; r < max_readings; r++) {
    Serial.println("\nPeriod-" + String(r) + "--------------");
    // WxForecast[r].Dt                = list[r]["dt"].as<int>();                                Serial.println("DTim: "+String(WxForecast[r].Dt));
    // WxForecast[r].Temperature       = list[r]["main"]["temp"].as<float>();                    Serial.println("Temp: "+String(WxForecast[r].Temperature));
    // WxForecast[r].Low               = list[r]["main"]["temp_min"].as<float>();                Serial.println("TLow: "+String(WxForecast[r].Low));
    // WxForecast[r].High              = list[r]["main"]["temp_max"].as<float>();                Serial.println("THig: "+String(WxForecast[r].High));
    // WxForecast[r].Pressure          = list[r]["main"]["pressure"].as<float>();                Serial.println("Pres: "+String(WxForecast[r].Pressure));
    // WxForecast[r].Humidity          = list[r]["main"]["humidity"].as<float>();                Serial.println("Humi: "+String(WxForecast[r].Humidity));
    // WxForecast[r].Forecast0         = list[r]["weather"][0]["main"].as<const char*>();        Serial.println("For0: "+String(WxForecast[r].Forecast0));
    // WxForecast[r].Forecast1         = list[r]["weather"][1]["main"].as<const char*>();        Serial.println("For1: "+String(WxForecast[r].Forecast1));
    // WxForecast[r].Forecast2         = list[r]["weather"][2]["main"].as<const char*>();        Serial.println("For2: "+String(WxForecast[r].Forecast2));
    // WxForecast[r].Icon              = list[r]["weather"][0]["icon"].as<const char*>();        Serial.println("Icon: "+String(WxForecast[r].Icon));
    // WxForecast[r].Description       = list[r]["weather"][0]["description"].as<const char*>(); Serial.println("Desc: "+String(WxForecast[r].Description));
    // WxForecast[r].Cloudcover        = list[r]["clouds"]["all"].as<int>();                     Serial.println("CCov: "+String(WxForecast[r].Cloudcover)); // in % of cloud cover
    // WxForecast[r].Windspeed         = list[r]["wind"]["speed"].as<float>();                   Serial.println("WSpd: "+String(WxForecast[r].Windspeed));
    // WxForecast[r].Winddir           = list[r]["wind"]["deg"].as<float>();                     Serial.println("WDir: "+String(WxForecast[r].Winddir));
    // WxForecast[r].Rainfall          = list[r]["rain"]["3h"].as<float>();                      Serial.println("Rain: "+String(WxForecast[r].Rainfall));
    // WxForecast[r].Snowfall          = list[r]["snow"]["3h"].as<float>();                      Serial.println("Snow: "+String(WxForecast[r].Snowfall));
    // WxForecast[r].Pop               = list[r]["pop"].as<float>();                             Serial.println("Pop:  "+String(WxForecast[r].Pop));
    // WxForecast[r].Period            = list[r]["dt_txt"].as<const char*>();                    Serial.println("Peri: "+String(WxForecast[r].Period));
    WxForecast[r].Dt                = root["hourly"]["time"][r].as<int>();                                Serial.println("DTim: "+String(WxForecast[r].Dt));
    WxForecast[r].Temperature       = root["hourly"]["temperature_2m"][r].as<float>();                    Serial.println("Temp: "+String(WxForecast[r].Temperature));
    WxForecast[r].Low               = root["hourly"]["temperature_2m"][r].as<float>();               Serial.println("TLow: "+String(WxForecast[r].Low));
    WxForecast[r].High              = root["hourly"]["temperature_2m"][r].as<float>();               Serial.println("THig: "+String(WxForecast[r].High));
    WxForecast[r].Pressure          = root["hourly"]["pressure_msl"][r].as<float>();               Serial.println("Pres: "+String(WxForecast[r].Pressure));
    WxForecast[r].Humidity          = root["hourly"]["relative_humidity_2m"][r].as<float>();               Serial.println("Humi: "+String(WxForecast[r].Humidity));
    // WxForecast[r].Forecast0         = root["hourly"]["weather_code"][r].as<int>();        Serial.println("For0: "+String(WxForecast[r].Forecast0));
    WxForecast[r].Forecast0         = GetOWMFromWMO(root["hourly"]["weather_code"][r].as<int>()).main; Serial.println("For0: "+String(WxForecast[r].Forecast0));
    WxForecast[r].Forecast1         = GetOWMFromWMO(root["hourly"]["weather_code"][r].as<int>()).main;   Serial.println("For1: "+String(WxForecast[r].Forecast1));
    WxForecast[r].Forecast2         = GetOWMFromWMO(root["hourly"]["weather_code"][r].as<int>()).main;        Serial.println("For2: "+String(WxForecast[r].Forecast2));
    WxForecast[r].Icon              = GetOWMIconFromWMO(root["hourly"]["weather_code"][r].as<int>(), root["hourly"]["is_day"][r].as<bool>());        Serial.println("Icon: "+String(WxForecast[r].Icon));
    WxForecast[r].Description       = GetOWMFromWMO(root["hourly"]["weather_code"][r].as<int>()).description; Serial.println("Desc: "+String(WxForecast[r].Description));
    WxForecast[r].Cloudcover        = root["hourly"]["cloud_cover"][r].as<int>();                     Serial.println("CCov: "+String(WxForecast[r].Cloudcover)); // in % of cloud cover
    WxForecast[r].Windspeed         = root["hourly"]["wind_speed_10m"][r].as<float>();                   Serial.println("WSpd: "+String(WxForecast[r].Windspeed));
    WxForecast[r].Winddir           = root["hourly"]["wind_direction_10m"][r].as<float>();                     Serial.println("WDir: "+String(WxForecast[r].Winddir));
    WxForecast[r].Rainfall          = root["hourly"]["precipitation"][r].as<float>();                      Serial.println("Rain: "+String(WxForecast[r].Rainfall));
    WxForecast[r].Snowfall          = root["hourly"]["snowfall"][r].as<float>();                      Serial.println("Snow: "+String(WxForecast[r].Snowfall));
    WxForecast[r].Pop               = (root["hourly"]["precipitation_probability"][r].as<float>())/100.0f;                             Serial.println("Pop:  "+String(WxForecast[r].Pop));
    WxForecast[r].Period            = root["hourly"]["time"][r].as<const char*>();                    Serial.println("Peri: "+String(WxForecast[r].Period));
  }
  //------------------------------------------
  float pressure_trend = WxForecast[2].Pressure - WxForecast[0].Pressure; // Measure pressure slope between ~now and later
  pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
  WxConditions[0].Trend = "0";
  if (pressure_trend > 0)  WxConditions[0].Trend = "+";
  if (pressure_trend < 0)  WxConditions[0].Trend = "-";
  if (pressure_trend == 0) WxConditions[0].Trend = "0";

  if (Units == "I") Convert_Readings_to_Imperial();
  
  return true;
}


//#########################################################################################
String ConvertUnixTime(int unix_time) {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unix_time;
  struct tm *now_tm = gmtime(&tm);
  char output[40];
  if (Units == "M") {
    strftime(output, sizeof(output), "%H:%M %d/%m/%y", now_tm);
  }
  else {
    strftime(output, sizeof(output), "%I:%M%P %m/%d/%y", now_tm);
  }
  return output;
}
//#########################################################################################
//WiFiClient client; // wifi client object

bool obtain_wx_data(WiFiClient& client, const String& RequestType) {
  if(RequestType != "weather")
  {
    // only "weather" needed as open-meteo API does both weather and forecast in one call
    return true;
  }
  // for imperial units: &temperature_unit=fahrenheit&wind_speed_unit=mph&precipitation_unit=inch
  const String units = (Units == "M" ? "&wind_speed_unit=ms" : "&temperature_unit=fahrenheit&wind_speed_unit=mph&precipitation_unit=inch");
  client.stop(); // close connection before sending a new request
  HTTPClient http;    
  // Update for API 3.0 June '24
  // String uri = "/data/3.0/onecall?lat=" + LAT + "&lon=" + LON + "&appid=" + apikey + "&mode=json&units=" + units + "&lang=" + Language;
  // owmn: String uri = "/data/2.5/" + RequestType + "?lat=" + LAT + "&lon=" + LON + "&appid=" + apikey + "&mode=json&units=" + units + "&lang=" + Language;
  String uri = "/v1/forecast?latitude=" + LAT + "&longitude=" + LON + units + "&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,precipitation,weather_code,cloud_cover,pressure_msl,wind_speed_10m,wind_direction_10m,wind_gusts_10m&hourly=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation_probability,precipitation,weather_code,pressure_msl,cloud_cover,visibility,wind_speed_10m,wind_direction_10m,wind_gusts_10m,uv_index,is_day,sunshine_duration&daily=weather_code,temperature_2m_max,temperature_2m_min,sunrise,sunset,sunshine_duration,uv_index_max,precipitation_sum,precipitation_hours,precipitation_probability_max,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant&timeformat=unixtime&forecast_days=3";
  
  //http.begin(uri,test_root_ca); //HTTPS example connection
  
  http.useHTTP10(true); // see: https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/#solution-1-use-http-10-recommended 
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (!DecodeWeather(http.getStream())) return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}
#endif /* ifndef COMMON_H_ */
