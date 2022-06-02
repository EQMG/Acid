set(_acid_network_headers
		Network/Ftp/Ftp.hpp
		Network/Ftp/FtpDataChannel.hpp
		Network/Ftp/FtpResponse.hpp
		Network/Ftp/FtpResponseDirectory.hpp
		Network/Ftp/FtpResponseListing.hpp
		Network/Http/Http.hpp
		Network/Http/HttpRequest.hpp
		Network/Http/HttpResponse.hpp
		Network/IpAddress.hpp
		Network/Packet.hpp
		Network/Socket.hpp
		Network/SocketSelector.hpp
		Network/Tcp/TcpListener.hpp
		Network/Tcp/TcpSocket.hpp
		Network/Udp/UdpSocket.hpp
		)
set(_acid_network_sources
		Network/Ftp/Ftp.cpp
		Network/Ftp/FtpDataChannel.cpp
		Network/Ftp/FtpResponse.cpp
		Network/Ftp/FtpResponseDirectory.cpp
		Network/Ftp/FtpResponseListing.cpp
		Network/Http/Http.cpp
		Network/Http/HttpRequest.cpp
		Network/Http/HttpResponse.cpp
		Network/IpAddress.cpp
		Network/Packet.cpp
		Network/Socket.cpp
		Network/SocketSelector.cpp
		Network/Tcp/TcpListener.cpp
		Network/Tcp/TcpSocket.cpp
		Network/Udp/UdpSocket.cpp
		)

set(_acid_network_third_party_headers
		)
set(_acid_network_third_party_sources
		)
set(_acid_network_third_party_includes
		)

add_library(Acid.Network
        ${_acid_network_headers}
        ${_acid_network_sources}
        ${_acid_network_third_party_headers}
        ${_acid_network_third_party_sources}
        )

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Acid.Network EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Network/Export.hpp")

target_compile_features(Acid.Network PUBLIC c_std_11 cxx_std_17)
target_include_directories(Acid.Network
		PUBLIC
		# Generated headers
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		# Project source includes
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		# Project third_party includes
		${_acid_network_third_party_includes}
		# Helps the includes find what they need at runtime
		# Although this also allows people to not prefix "Acid" before includes as well
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
		)
target_link_libraries(Acid.Network
		PUBLIC
		Acid.Core
		)
