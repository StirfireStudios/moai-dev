
cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )

set ( UNTZ_INCLUDES 
  ${SDL_INCLUDES}
  "${MOAI_ROOT}/3rdparty/untz/include/"
  "${MOAI_ROOT}/3rdparty/untz/src/"
  "${MOAI_ROOT}/3rdparty/untz/src/native/android/" 
)
file ( GLOB UNTZ_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/android/*.cpp"
)
file ( GLOB UNTZ_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/android/*.h"
)
list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/native/android/testRiff.cpp")
list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/native/android/testWave.cpp")

if ( NOT MOAI_VORBIS )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.h")
endif ( NOT MOAI_VORBIS )

if ( NOT MOAI_OPUS )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/OpusAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/OpusAudioSource.h")
endif ( NOT MOAI_OPUS )

if ( NOT MOAI_FLAC )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/FLACAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/FLACAudioSource.h")
endif ( NOT MOAI_FLAC )

add_library ( untz STATIC ${UNTZ_SOURCES} ${UNTZ_HEADERS} )

if (MOAI_VORBIS)
  target_link_libraries ( untz vorbis )
endif (MOAI_VORBIS)

if (MOAI_OPUS})
  target_link_libraries ( untz opus )
endif (MOAI_OPUS)

if (MOAI_FLAC})
  target_link_libraries ( untz flac )
endif (MOAI_FLAC)

target_include_directories ( untz PUBLIC ${UNTZ_INCLUDES} )
