SOURCES += \
    main.cpp

INCLUDEPATH += \
    /home/u/opencv4/include/opencv4 \
#    /home/u/ur3/urct \
#    /home/u/number/agv \

LIBS += \
 -L/home/u/opencv4/lib \
# -lopencv_aruco \
# -lopencv_bgsegm \
# -lopencv_bioinspired \
 -lopencv_calib3d \
# -lopencv_ccalib \
 -lopencv_core \
# -lopencv_cvv \
# -lopencv_datasets \
# -lopencv_dnn \
# -lopencv_dpm \
# -lopencv_face \
# -lopencv_features2d \
# -lopencv_flann \
#-lopencv_freetype \
# -lopencv_fuzzy \
#-lopencv_hdf \
 -lopencv_highgui \
 -lopencv_imgcodecs \
 -lopencv_imgproc \
# -lopencv_line_descriptor \
# -lopencv_ml \
# -lopencv_objdetect \
# -lopencv_optflow \
#-lopencv_phase_unwrapping \
# -lopencv_photo \
# -lopencv_plot \
# -lopencv_reg \
# -lopencv_rgbd \
# -lopencv_saliency \
#-lopencv_sfm \
# -lopencv_shape \
# -lopencv_stereo \
# -lopencv_stitching \
# -lopencv_structured_light \
# -lopencv_superres \
# -lopencv_surface_matching \
# -lopencv_text \
# -lopencv_tracking \
# -lopencv_videoio \
# -lopencv_video \
# -lopencv_videostab \
#-lopencv_viz \
# -lopencv_xfeatures2d \
# -lopencv_ximgproc \
# -lopencv_xobjdetect \
# -lopencv_xphoto \
-lrealsense2 \
-lglfw \
-lGLU \

CONFIG += c++11 \
