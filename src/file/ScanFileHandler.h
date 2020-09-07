//
// Created by Sean ng pack on 7/8/20.
//

#ifndef SWAG_SCANNER_SCANFILEHANDLER_H
#define SWAG_SCANNER_SCANFILEHANDLER_H

#include "IFileHandler.h"
#include <nlohmann/json.hpp>

namespace file {
    /**
     * Represents a file handler for scanning related processes.
     */
    class ScanFileHandler : public IFileHandler {
    public:

        /**
         * Default constructor makes a ScanFileHandler. Searches for SwagScanner in the /applications path
         * and creates a new SwagScanner directory if it doesn't exist. Will also create a folder "1" under
         * the /data directory and set it as the current scan folder. * Then it will update the settings.json on the
         * latest scan. If SwagScanner exists, then it will use the current scan folder according to the
         * settings.json file.
         */
        ScanFileHandler();

        /**
         * Constructor takes in a flag and determines whether to create a new scan folder
         * or not. If the flag is set to true then it will create a new scan folder numerically.
         * Then it will update the settings.json on the latest scan.
         * @param auto_create_flag
         */
        ScanFileHandler(bool auto_create_flag);

        /**
         * Crates a scan folder with the given scan name. If the scan is already there,
         * then just set the current scan folder to it and do not touch anything inside the directory.
         *
         * @param scan_name name of the scan.
         */
        ScanFileHandler(const char *scan_name);

        /**
         * Scan folder will be created and name will be be assigned based on previous scans in
         * alphanumeric order.
         */
        void auto_create_new_scan();

        /**
         * Point to the given scan. If the scan does not exist then create a new one.
         *
         * @param scan name of the scan.
         */
        void set_scan(const std::string &scan_name);

        void save_cloud(const std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> &cloud,
                        const std::string &cloud_name,
                        const CloudType::Type &cloud_type) override;

        std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>> load_cloud(const std::string &cloud_name,
                                                                   const CloudType::Type &cloud_type) override;

        std::vector<std::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>> load_clouds(
                const CloudType::Type &cloud_type) override;

        std::string get_scan_name();



        /**
         * Get the latest calibration json file.
         * Finds the latest calibration file via info.json.
         * @return the calibration json.
         */
        nlohmann::json get_calibration_json();

        /**
         * Get the info.json file.
         * @return json file.
         */
        nlohmann::json get_info_json();

        /**
         * Update the json file in the latest scan with the given info.
         * @param date current date and time.
         * @param angle angle intervals of the scan.
         * @param cal calibration path.
         */
        void update_info_json(const std::string &date,
                              int angle,
                              const std::string &cal = "None");

    private:
        /**
         * Checks to see if a /SwagScanner folder exists in Library/Application Support.
         * If the folder does not exist, then create one and load in default configuration.
         * Otherwise, continue.
         *
         * TODO: I should probably move this out of this derived class. Might want to make it
         * higher level, or apply to all file handlers.
         * @returns true if the program folder is already there. False if it isn't.
         */
        bool check_program_folder();

        /**
         * Create the sub folders defined in CloudTypes in the scan_folder_path if they
         * don't exist.
         * Also creates a /calibration folder with a calibration_info.txt file
         */
        void create_sub_folders();

        /**
         * Finds the last scan folder using the settings.json file in the /settings directory.
         * TODO: Might wanna move this to IFilehandler. Also might wanna not depend on the json file to accomplish this.
         *
         * @return path to the latest scan.
         */
        boost::filesystem::path find_latest_scan();


        /**
         * Update the settings.json file "latest_scan" field.
         */
        void set_settings_latest_scan(const boost::filesystem::path &folder_path);

    };
}

#endif //SWAG_SCANNER_SCANFILEHANDLER_H