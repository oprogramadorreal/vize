macro(ADD_GDCM 3rdPartyDir)
	add_subdirectory(${3rdPartyDir}/gdcm 3rd-party/gdcm)
	set(GDCM_INCLUDE_DIR
		"${CMAKE_CURRENT_BINARY_DIR}/3rd-party/gdcm/Source/Common"
		"${3rdPartyDir}/gdcm/Source/Common"
		"${3rdPartyDir}/gdcm/Source/DataStructureAndEncodingDefinition"
		"${3rdPartyDir}/gdcm/Source/MediaStorageAndFileFormat"
		"${3rdPartyDir}/gdcm/Source/MessageExchangeDefinition"
		"${3rdPartyDir}/gdcm/Source/DataDictionary"
		"${3rdPartyDir}/gdcm/Source/InformationObjectDefinition"
	)
endmacro()