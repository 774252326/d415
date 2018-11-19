#ifndef RSSETTING_HPP
#define RSSETTING_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <librealsense2/rs.hpp>

class RSSetting
{
protected:

    static std::string get_device_name(const rs2::device& dev)
    {
        // Each device provides some information on itself, such as name:
        std::string name = "Unknown Device";
        if (dev.supports(RS2_CAMERA_INFO_NAME))
            name = dev.get_info(RS2_CAMERA_INFO_NAME);

        // and the serial number of the device:
        std::string sn = "########";
        if (dev.supports(RS2_CAMERA_INFO_SERIAL_NUMBER))
            sn = std::string("#") + dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);

        return name + " " + sn;
    }

    static bool GetDevice(rs2::device &device, std::string id)
    {
        // First, create a rs2::context.
        // The context represents the current platform with respect to connected devices
        rs2::context ctx;

        // Using the context we can get all connected devices in a device list
        rs2::device_list devices = ctx.query_devices();

        // device_list is a "lazy" container of devices which allows
        //The device list provides 2 ways of iterating it
        //The first way is using an iterator (in this case hidden in the Range-based for loop)

        std::cout << "Found " << devices.size() << " devices:\n" << std::flush;

        int index = 0;
        for (rs2::device device1 : devices)
        {
            std::string deviceId=get_device_name(device1);
            std::cout << "found device " << index++ << " : " << deviceId << std::endl<<std::flush;
            if(deviceId.compare(id)==0)
            {
                std::cout << "device " << id << " connected\n"<<std::flush;
                device=device1;
                return true;
            }
        }
        std::cout << "device " << id << " not connected\n"<<std::flush;
        return false;
    }

    static void print_device_information(const rs2::device& dev)
    {
        // Each device provides some information on itself
        // The different types of available information are represented using the "RS2_CAMERA_INFO_*" enum

        std::cout << "Device information: " << std::endl;
        //The following code shows how to enumerate all of the RS2_CAMERA_INFO
        //Note that all enum types in the SDK start with the value of zero and end at the "*_COUNT" value
        for (int i = 0; i < static_cast<int>(RS2_CAMERA_INFO_COUNT); i++)
        {
            rs2_camera_info info_type = static_cast<rs2_camera_info>(i);
            //SDK enum types can be streamed to get a string that represents them
            std::cout << "  " << std::left << std::setw(20) << info_type << " : ";

            //A device might not support all types of RS2_CAMERA_INFO.
            //To prevent throwing exceptions from the "get_info" method we first check if the device supports this type of info
            if (dev.supports(info_type))
                std::cout << dev.get_info(info_type) << std::endl;
            else
                std::cout << "N/A" << std::endl;
        }
    }

    static std::string get_sensor_name(const rs2::sensor& sensor)
    {
        // Sensors support additional information, such as a human readable name
        if (sensor.supports(RS2_CAMERA_INFO_NAME))
            return sensor.get_info(RS2_CAMERA_INFO_NAME);
        else
            return "Unknown Sensor";
    }

    static bool GetSensor(rs2::sensor &sensor, const rs2::device& device, std::string id)
    {
        // A rs2::device is a container of rs2::sensors that have some correlation between them.
        // For example:
        //    * A device where all sensors are on a single board
        //    * A Robot with mounted sensors that share calibration information

        // Given a device, we can query its sensors using:
        std::vector<rs2::sensor> sensors = device.query_sensors();

        std::cout << "Device consists of " << sensors.size() << " sensors:\n" << std::flush;
        int index = 0;
        // We can now iterate the sensors and print their names
        for (rs2::sensor sensor1 : sensors)
        {
            std::string sensorId=get_sensor_name(sensor1);
            std::cout << "found sensor " << index++ << " : " << sensorId << std::endl;
            if(sensorId.compare(id)==0)
            {
                std::cout << "sensor " << id << " selected\n"<<std::flush;
                sensor=sensor1;
                return true;
            }
        }
        std::cout << "sensor " << id << " not found\n"<<std::flush;
        return false;
    }

    static void list_sensor_option(const rs2::sensor& sensor)
    {
        // Sensors usually have several options to control their properties
        //  such as Exposure, Brightness etc.

        std::cout << "Sensor supports the following options:\n" << std::endl;

        // The following loop shows how to iterate over all available options
        // Starting from 0 until RS2_OPTION_COUNT (exclusive)
        for (int i = 0; i < static_cast<int>(RS2_OPTION_COUNT); i++)
        {
            rs2_option option_type = static_cast<rs2_option>(i);
            //SDK enum types can be streamed to get a string that represents them
            std::cout << "  " << i << ": " << option_type;

            // To control an option, use the following api:

            // First, verify that the sensor actually supports this option
            if (sensor.supports(option_type))
            {
                std::cout << std::endl;

                // Get a human readable description of the option
                const char* description = sensor.get_option_description(option_type);
                std::cout << "       Description   : " << description << std::endl;

                // Get the current value of the option
                float current_value = sensor.get_option(option_type);
                std::cout << "       Current Value : " << current_value << std::endl;

                rs2::option_range range = sensor.get_option_range(option_type);
                float default_value = range.def;
                float maximum_supported_value = range.max;
                float minimum_supported_value = range.min;
                float difference_to_next_value = range.step;
                std::cout << "       Min Value     : " << minimum_supported_value << std::endl;
                std::cout << "       Max Value     : " << maximum_supported_value << std::endl;
                std::cout << "       Default Value : " << default_value << std::endl;
                std::cout << "       Step          : " << difference_to_next_value << std::endl;

                //To change the value of an option, please follow the change_sensor_option() function
            }
            else
            {
                std::cout << " is not supported" << std::endl;
            }
        }
    }

    static void list_device_option(const rs2::device &device)
    {
            print_device_information(device);
            // Given a device, we can query its sensors using:
            std::vector<rs2::sensor> sensors = device.query_sensors();
            std::cout << "Device consists of " << sensors.size() << " sensors:\n" << std::flush;
            int index = 0;
            // We can now iterate the sensors and print their names
            for (rs2::sensor sensor1 : sensors)
            {
                std::string sensorId=get_sensor_name(sensor1);
                std::cout << "\nfound sensor " << index++ << " : " << sensorId << std::endl;
                list_sensor_option(sensor1);
            }

    }


public:
     static bool Set(rs2_option option=RS2_OPTION_ENABLE_AUTO_EXPOSURE, float value=1, std::string deviceId="Intel RealSense 435 #819112071701", std::string sensorId="RGB Camera")
     {
         rs2::device dev;
         if(GetDevice(dev, deviceId))
         {
//             print_device_information(dev);


             rs2::sensor sensor;
             if(GetSensor(sensor, dev, sensorId))
             {
//                 list_sensor_option(sensor);
                 // To set an option to a different value, we can call set_option with a new value
                 try
                 {
                     sensor.set_option(option, value);
//                     sensor.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0);
//                     sensor.set_option(RS2_OPTION_EXPOSURE, 200);
                     return true;
                 }
                 catch (const rs2::error& e)
                 {
                     // Some options can only be set while the camera is streaming,
                     // and generally the hardware might fail so it is good practice to catch exceptions from set_option
                     std::cerr << "Failed to set option. (" << e.what() << ")" << std::endl;
                 }
                 list_sensor_option(sensor);
             }
         }

         return false;
     }

     static void Status()
     {
         // First, create a rs2::context.
         // The context represents the current platform with respect to connected devices
         rs2::context ctx;

         // Using the context we can get all connected devices in a device list
         rs2::device_list devices = ctx.query_devices();

         // device_list is a "lazy" container of devices which allows
         //The device list provides 2 ways of iterating it
         //The first way is using an iterator (in this case hidden in the Range-based for loop)

         std::cout << "Found " << devices.size() << " devices:\n" << std::flush;
         int index = 0;
         for (rs2::device device1 : devices)
         {
             std::string deviceId=get_device_name(device1);
             std::cout << "\nfound device " << index++ << " : " << deviceId << std::endl<<std::flush;
             list_device_option(device1);
         }
     }

};

#endif // RSSETTING_HPP

