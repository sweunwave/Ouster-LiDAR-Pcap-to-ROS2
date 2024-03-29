#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OusterSDK::ouster_client" for configuration "Release"
set_property(TARGET OusterSDK::ouster_client APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OusterSDK::ouster_client PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libouster_client.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS OusterSDK::ouster_client )
list(APPEND _IMPORT_CHECK_FILES_FOR_OusterSDK::ouster_client "${_IMPORT_PREFIX}/lib/libouster_client.a" )

# Import target "OusterSDK::ouster_pcap" for configuration "Release"
set_property(TARGET OusterSDK::ouster_pcap APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OusterSDK::ouster_pcap PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libouster_pcap.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS OusterSDK::ouster_pcap )
list(APPEND _IMPORT_CHECK_FILES_FOR_OusterSDK::ouster_pcap "${_IMPORT_PREFIX}/lib/libouster_pcap.a" )

# Import target "OusterSDK::ouster_viz" for configuration "Release"
set_property(TARGET OusterSDK::ouster_viz APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OusterSDK::ouster_viz PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libouster_viz.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS OusterSDK::ouster_viz )
list(APPEND _IMPORT_CHECK_FILES_FOR_OusterSDK::ouster_viz "${_IMPORT_PREFIX}/lib/libouster_viz.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
