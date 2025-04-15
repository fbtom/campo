set(OpenCVModules
    core
    stitching
    video
    imgproc
    stereo
    photo
    highgui
    objdetect
    imgcodecs
    videoio
    dnn
    calib
    features
    flann
    3d
)

function(target_open_cv target)
    target_include_directories(${target} PRIVATE
        ${OpenCV_SOURCE_DIR}/include
        ${OpenCV_BINARY_DIR}
        ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
        ${OPENCV_MODULE_opencv_modules_LOCATION}/include
    )

    foreach(module ${OpenCVModules})
        target_include_directories(${target} PRIVATE
            ${OPENCV_MODULE_opencv_${module}_LOCATION}/include
        )
        target_link_libraries(${target} PRIVATE opencv_${module})
    endforeach()
endfunction()
