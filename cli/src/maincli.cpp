/*
 * cli.cpp
 *
 *  Created on: Mar 16, 2019
 *      Author: igor
 */

#include <iostream>

#include <boost/program_options.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "findcontiguousregion.h"

namespace po = boost::program_options;
using findcontiguousregion::ContiguousRegionFinder;

int main(int argc, char *argv[]) {

    po::options_description desc{"Options"};
    desc.add_options()
        ("help,h",                                              "Help")
        ("image,i", po::value<std::string>()->required(),       "image file")
        ("row,r",   po::value<uint16_t>()->required(),          "pixel row coordinate")
        ("col,c",   po::value<uint16_t>()->required(),          "pixel column coordinate")
        ("db",      po::value<uint8_t>()->default_value(16),    "delta for Blue component")
        ("dg",      po::value<uint8_t>()->default_value(16),    "delta for Green component")
        ("dr",      po::value<uint8_t>()->default_value(16),    "delta for Red component");


    po::positional_options_description posDesc;
    posDesc.add("image", 1);
    posDesc.add("row", 1);
    posDesc.add("col", 1);

    po::variables_map vm;

    try {
        po::command_line_parser parser{argc, argv};
        parser.options(desc).positional(posDesc).allow_unregistered();
        po::parsed_options parsedOptions = parser.run();

        store(parsedOptions, vm);
        notify(vm);

        if(vm.count("help")) {
          std::cout << desc << std::endl;
          return EXIT_SUCCESS;
        }
    }
    catch(po::required_option& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return EXIT_FAILURE;
    }
    catch(po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return EXIT_FAILURE;
    }

    std::string imageFilePath   = vm["image"].as<std::string>();
    uint16_t pixelRowCoordinate = vm["row"].as<uint16_t>();
    uint16_t pixelColCoordinate = vm["col"].as<uint16_t>();
    uint8_t  deltaBlue          = vm["db"].as<uint8_t>();
    uint8_t  deltaGreen         = vm["dg"].as<uint8_t>();
    uint8_t  deltaRed           = vm["dr"].as<uint8_t>();

    std::cout
        << "image " << imageFilePath        << std::endl
        << "row "   << pixelRowCoordinate   << std::endl
        << "col "   << pixelColCoordinate   << std::endl
        << "db "    << deltaBlue            << std::endl
        << "dg "    << deltaGreen           << std::endl
        << "dr "    << deltaRed             << std::endl;

    // B G R order
    // RGB could be not the best solution, but assume from the task that
    // program should work with it
    cv::Mat image = cv::imread(imageFilePath, CV_LOAD_IMAGE_COLOR);
    if(image.empty()) {
        std::cerr <<  "Could not open or find the image " << imageFilePath << std::endl;
        return EXIT_FAILURE;
    }

    ContiguousRegionFinder finder{std::move(image)};
    auto region = finder.find(
        pixelRowCoordinate,
        pixelColCoordinate,
        deltaBlue,
        deltaGreen,
        deltaRed
    );

    cv::namedWindow("Region", cv::WINDOW_AUTOSIZE);
    cv::imshow("Region", image);
    cv::waitKey(0);

    return EXIT_SUCCESS;
}


