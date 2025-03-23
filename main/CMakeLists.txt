set(COMPONENT_SRCS
    "main.cpp"
    "DoorSensingApp.cpp"
    "driver/LightSensor_OPT4003.cpp"
)

set(COMPONENT_ADD_INCLUDEDIRS
    "."
    "driver"
)

register_component()

target_compile_options(${COMPONENT_LIB} PRIVATE 
)