set(_acid_network_headers
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/Ftp.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpDataChannel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponse.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseDirectory.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseListing.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/Http.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpRequest.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpResponse.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/IpAddress.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Packet.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Socket.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketSelector.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpListener.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpSocket.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Udp/UdpSocket.hpp
		)
set(_acid_network_sources
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/Ftp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpDataChannel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponse.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseDirectory.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Ftp/FtpResponseListing.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/Http.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpRequest.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Http/HttpResponse.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/IpAddress.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Packet.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Socket.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketSelector.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpListener.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Tcp/TcpSocket.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Network/Udp/UdpSocket.cpp
		)
set(_acid_network_generated_headers
		${CMAKE_CURRENT_BINARY_DIR}/include/NetworkExport.hpp
		)

add_library(Acid.Network
        ${_acid_network_headers}
        ${_acid_network_sources}
		${_acid_network_generated_headers}
        )

include(GenerateExportHeader)
generate_export_header(Acid.Network EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/NetworkExport.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_network_headers})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "" FILES ${_acid_network_sources})
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/include" PREFIX "" FILES ${_acid_network_generated_headers})

set_target_properties(Acid.Network PROPERTIES FOLDER "Acid")
target_compile_features(Acid.Network PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Network
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Network>
		)
target_link_libraries(Acid.Network
		PUBLIC
		Acid.Core
		)

install(TARGETS Acid.Network
		EXPORT AcidNetworkExport
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		)
install(EXPORT AcidNetworkExport
		FILE AcidNetworkConfig.cmake
		#NAMESPACE acid::
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
		)
install_headers_with_dir("Network" "${CMAKE_CURRENT_SOURCE_DIR}" "${_acid_network_headers}")
install_headers_with_dir("Network" "${CMAKE_CURRENT_BINARY_DIR}/include" "${_acid_network_generated_headers}")
