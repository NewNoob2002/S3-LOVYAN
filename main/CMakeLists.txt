set (main_root ${CMAKE_CURRENT_LIST_DIR})

set(COMPONENT_ADD_INCLUDEDIRS
    ${main_root}/include
    ${main_root}/ui
)
file(GLOB SRCS
    ${main_root}/*.cpp
    ${main_root}/src/*.cpp
    ${main_root}/ui/*.c
    ${main_root}/ui/*.cpp
    ${main_root}/pages/*.cpp
)

set(COMPONENT_SRCS ${SRCS})

# if(IDF_VERSION_MAJOR GREATER_EQUAL 5)
#     set(COMPONENT_REQUIRES nvs_flash efuse esp_lcd driver esp_timer)
# elseif((IDF_VERSION_MAJOR EQUAL 4) AND (IDF_VERSION_MINOR GREATER 3) OR IDF_VERSION_MAJOR GREATER 4)
#     set(COMPONENT_REQUIRES nvs_flash efuse esp_lcd)
# else()
#     set(COMPONENT_REQUIRES nvs_flash efuse)
# endif()


register_component()