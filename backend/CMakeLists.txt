cmake_minimum_required(VERSION 3.15)
project(ICNARI_Conference_Backend VERSION 1.0.0)

find_package(Drogon REQUIRED)
find_package(AWSSDK CONFIG REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(libxcrypt REQUIRED)

add_executable(ICNARI_Conference_Backend
    # config
    ./src/config/database.cc 

    # controllers
    ./src/controllers/health/health.cc
    ./src/controllers/user/api_v1_users.cc
    ./src/controllers/auth/api_v1_auth.cc
    ./src/controllers/notifications/api_v1_notifications.cc
    ./src/controllers/gallery/api_v1_gallery.cc
    ./src/controllers/committee/api_v1_committee.cc
    ./src/controllers/contact/api_v1_contact.cc
    ./src/controllers/speaker/api_v1_speaker.cc

    # # models
    ./src/models/Users.cc
    ./src/models/Notifications.cc
    ./src/models/Gallery.cc
    ./src/models/Committee.cc
    ./src/models/Contacts.cc
    ./src/models/Speakers.cc

    # filters
    ./src/filters/AuthFilter.cc

    # services
    ./src/services/S3Service.cc

    # utils
    ./src/utils/token.cc

    # repositories
    ./src/repositories/user/user.repository.cc
    ./src/repositories/notifications/notification.repository.cc
    ./src/repositories/gallery/gallery.repository.cc
    ./src/repositories/contact/contact.repository.cc
    ./src/repositories/committee/committee.repository.cc
    ./src/repositories/speakers/speaker.repository.cc

    # main
    ./src/main.cc)

add_executable(createAdmin
./src/utils/token.cc
./scripts/createAdmin.cc)

target_link_libraries(createAdmin PRIVATE
    jwt-cpp::jwt-cpp
    Drogon::Drogon
    libxcrypt::libxcrypt
)

target_link_libraries(ICNARI_Conference_Backend PRIVATE
    Drogon::Drogon
    AWS::aws-sdk-cpp-s3
    AWS::aws-sdk-cpp-core
    jwt-cpp::jwt-cpp
    libxcrypt::libxcrypt
)