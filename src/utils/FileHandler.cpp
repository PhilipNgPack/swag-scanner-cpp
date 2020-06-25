#include "FileHandler.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace boost::filesystem;

file::FileHandler::FileHandler() {
    bool exists = check_program_folder();
    if (!exists) {
        scan_folder_path = find_latest_scan_folder_numeric(swag_scanner_path + "/scans");
        create_directory(scan_folder_path);
        create_sub_folders();
        update_settings_latest_scan(scan_folder_path);
    } else {
        scan_folder_path = find_latest_scan_folder();
    }

}

file::FileHandler::FileHandler(bool auto_create_flag) {
    scan_folder_path = find_latest_scan_folder_numeric(swag_scanner_path + "/scans");
    std::cout << scan_folder_path << std::endl;
    if (auto_create_flag) {
        create_directory(scan_folder_path);
        create_sub_folders();
        update_settings_latest_scan(scan_folder_path);
    }
}

file::FileHandler::FileHandler(const char *scan_name) {
    if (check_folder_input(scan_name)) {
        scan_folder_path = swag_scanner_path + "/scans/" + scan_name;
    } else {
        scan_folder_path = swag_scanner_path + "/scans/" + scan_name;
        create_directory(scan_folder_path);
        create_sub_folders();
        update_settings_latest_scan(scan_folder_path);
    }
}


void file::FileHandler::set_scan_folder_path(const std::string &path) {
    if (check_folder_input(path)) {
        this->scan_folder_path = path;
        create_sub_folders();
    } else {
        throw std::invalid_argument("cannot set path, folder does not exist");
    }
}

std::string file::FileHandler::get_scan_folder_path() {
    return this->scan_folder_path;
}

void file::FileHandler::save_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud,
                                   const std::string &cloud_name,
                                   CloudType::Type cloud_type) {

    std::cout << "saving file to ";
    std::string out_path = scan_folder_path
                           + "/"
                           + CloudType::String(cloud_type)
                           + "/" + cloud_name + ".pcd";
    std::cout << out_path << std::endl;

    pcl::io::savePCDFileASCII(out_path, *cloud);
}

void file::FileHandler::load_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                                   const std::string &cloud_name,
                                   CloudType::Type cloud_type) {
    check_file_input(cloud_name);
    std::string open_path = scan_folder_path
                            + "/"
                            + CloudType::String(cloud_type)
                            + "/" + cloud_name;
    if (pcl::io::loadPCDFile<pcl::PointXYZ>(open_path, *cloud) == -1) {
        PCL_ERROR ("Couldn't read file \n");
    }
}

void file::FileHandler::load_clouds(
        std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr, Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr>> &cloud_vector,
        CloudType::Type cloud_type,
        const std::string &folder_path) {
    std::string load_path;
    if (folder_path.empty()) {
        load_path = scan_folder_path + "/" + CloudType::String(cloud_type);;

    } else {
        check_folder_input(folder_path);
        load_path = folder_path + "/" + CloudType::String(cloud_type);
    }

    std::vector<path> cloud_paths;

    // load paths into cloud_paths vector
    for (auto &p : boost::filesystem::directory_iterator(load_path)) {
        // extension must be .pcd and must have number in the filename
        if (p.path().extension() == ".pcd" && p.path().string().find_first_of("0123456789") != std::string::npos) {
            cloud_paths.push_back(p.path());
        }
    }

    // sort the paths numerically
    std::sort(cloud_paths.begin(), cloud_paths.end(), path_sort);

    // finally we load the clouds into the cloud_vector

    for (auto &p : cloud_paths) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        if (pcl::io::loadPCDFile<pcl::PointXYZ>(p.string(), *cloud) == -1) {
            PCL_ERROR ("Couldn't read file \n");
        }
        std::cout << "loading " << p.string() << std::endl;
        cloud_vector.push_back(cloud);
    }
    std::cout << "finished loading clouds" << std::endl;
}

std::string file::FileHandler::find_latest_scan_folder() {
    std::ifstream settings(swag_scanner_path + "/settings/settings.json");
    json settings_json;
    settings >> settings_json;
    std::string latest = settings_json["latest_scan"];
    std::cout << "found latest scan in settings.json file to be " << latest << std::endl;
    return latest;
}

std::string file::FileHandler::find_latest_scan_folder_numeric(const std::string &folder) {
    if (!check_folder_input(folder)) {
        throw std::invalid_argument("This shouldn't happen");
    }

    // if folder is empty let's start at 1.
    if (is_empty(folder)) {
        std::string name = folder + "/1";
        return name;
    }

    // make a vector to hold paths of all FOLDERS in the path
    std::vector<path> v;
    // don't include directories with '.' or without numbers in them.
    for (auto &&x : directory_iterator(folder)) {
        if (x.path().string().find('.') == std::string::npos &&
            x.path().string().find_first_of("0123456789") != std::string::npos) {
            v.push_back(x.path());
        }
    }

    // sort them using custom lambda function to order.
    std::sort(v.begin(), v.end(), path_sort);

    // get the last item in list, convert to string, convert to int, then add 1
    int name_count = std::stoi(v.back().filename().string()) + 1;
    std::string name_count_str = std::to_string(name_count);

    std::string last_path = v.back().string();
    std::string to_replace = v.back().filename().string();
    std::string name = last_path.replace(last_path.find(to_replace),
                                         sizeof(name_count_str) - 1,
                                         name_count_str);
    return name;
}

bool file::FileHandler::check_program_folder() {
    if (!exists(swag_scanner_path)) {
        std::cout << "No SwagScanner application folder detected, creating one at: " + swag_scanner_path << std::endl;
        create_directory(swag_scanner_path);
        create_directory(swag_scanner_path + "/settings");
        create_directory(swag_scanner_path + "/scans");
        create_directory(swag_scanner_path + "/calibration");
        std::ofstream settings(swag_scanner_path + "/settings/settings.json"); // create json file
        json settings_json = {
                {"version",     .1},
                {"latest_scan", "none"}
        };
        settings << std::setw(4) << settings_json << std::endl; // write to file
        std::ofstream calibration(swag_scanner_path + "/calibration/default_calibration.json"); // create json file
        json calibration_json = {
                {"origin point",   {-0.0002, 0.0344,  0.4294}},
                {"axis direction", {-0.0158, -0.8661, -0.4996}}
        };
        calibration << std::setw(4) << calibration_json << std::endl; // write to file
        return false;
    }
    return true;
}

void file::FileHandler::create_sub_folders() {
    for (const auto element : CloudType::All) {
        std::string p = scan_folder_path + "/" + CloudType::String(element);
        if (!exists(p)) {
            create_directory(p);
            std::cout << "Creating folder " + p << std::endl;
        }
    }
}

void file::FileHandler::update_settings_latest_scan(std::string &folder_path) {
    std::ifstream settings(swag_scanner_path + "/settings/settings.json");
    json settings_json;
    settings >> settings_json;
    settings_json["latest_scan"] = folder_path;

    std::ofstream updated_file(swag_scanner_path + "/settings/settings.json");
    updated_file << settings_json;
}

bool file::FileHandler::check_folder_input(const std::string &folder) {
    return is_directory(folder);
}

bool file::FileHandler::check_file_input(const std::string &file_path) {
    if (!exists(file_path)) {
        throw std::invalid_argument("File path error, " + file_path + " does not exist");
    }
    return true;
}


